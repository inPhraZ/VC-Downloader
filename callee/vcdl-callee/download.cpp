#include "download.h"

LPDOWNLOADINFO  DownloadInfoAlloc()
{
  LPDOWNLOADINFO dlinfo = NULL;
  dlinfo = (LPDOWNLOADINFO)malloc(sizeof(DOWNLOADINFO));
  if (!dlinfo)
    return NULL;
  ZeroMemory(dlinfo, sizeof(DOWNLOADINFO));
  return dlinfo;
}

static int CreateURL(LPDOWNLOADINFO dlinfo)
{
  LPWSTR first = NULL;
  LPWSTR last = NULL;
  LPWSTR tmp = NULL;
  SIZE_T len = 0;

  if (!dlinfo)
    return -1;

  if (!dlinfo->URL)
    return -1;

  len = wcslen(dlinfo->URL);
  tmp = _wcsdup(dlinfo->URL);
  last = wcschr(tmp, L'?');
  
  wsprintf(last, L"output/%s?download=zip", dlinfo->FileTitle);
  free(dlinfo->URL);
  dlinfo->URL = tmp;

  return 0;
}

static size_t download_callback(void* ptr, size_t size, size_t nmemb, void* userdate)
{
  LPDOWNLOADINFO dlinfo = (LPDOWNLOADINFO)userdate;
  fwrite(ptr, size, nmemb, dlinfo->fp);
  return size * nmemb;
}

int DownloadArchive(LPDOWNLOADINFO dlinfo)
{
  size_t len = 0;
  char* url = (char*)malloc(1024);
  char* cookies = (char*)malloc(1024);

  if (!dlinfo)
    return -1;

  if (CreateURL(dlinfo) == -1)
    return -1;

  _wfopen_s(&dlinfo->fp, dlinfo->Path, L"wb");
  if (!dlinfo->fp)
    return -1;

  dlinfo->curl = curl_easy_init();
  if (!dlinfo->curl)
    return -1;

  url = (char*)malloc(1024);
  cookies = (char*)malloc(1024);
  ZeroMemory(url, 1024);
  ZeroMemory(cookies, 1024);

  wcstombs_s(&len, url, 1024, dlinfo->URL, dlinfo->URL_len);
  wcstombs_s(&len, cookies, 1024, dlinfo->Cookies, dlinfo->Cookies_len);

  curl_easy_setopt(dlinfo->curl, CURLOPT_URL, url);
  curl_easy_setopt(dlinfo->curl, CURLOPT_COOKIE, cookies);
  curl_easy_setopt(dlinfo->curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(dlinfo->curl, CURLOPT_WRITEFUNCTION, download_callback);
  curl_easy_setopt(dlinfo->curl, CURLOPT_WRITEDATA, (void*)dlinfo);
  dlinfo->cres = curl_easy_perform(dlinfo->curl);
  fclose(dlinfo->fp);
  curl_easy_cleanup(dlinfo->curl);

  if (dlinfo->cres != CURLE_OK)
    return -1;

  return 0;
}