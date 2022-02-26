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

static size_t download_callback(void* ptr, size_t size, size_t nmemb, void* userdate)
{
  LPDOWNLOADINFO dlinfo = (LPDOWNLOADINFO)userdate;
  fwrite(ptr, size, nmemb, dlinfo->fp);
  return size * nmemb;
}

int DownloadArchive(LPDOWNLOADINFO dlinfo)
{
  if (!dlinfo)
    return -1;


  _wfopen_s(&dlinfo->fp, dlinfo->Path, L"wb");
  if (!dlinfo->fp)
    return -1;

  dlinfo->curl = curl_easy_init();
  if (!dlinfo->curl)
    return -1;

  curl_easy_setopt(dlinfo->curl, CURLOPT_URL, dlinfo->URL);
  curl_easy_setopt(dlinfo->curl, CURLOPT_WRITEDATA, (void *)dlinfo);
  curl_easy_setopt(dlinfo->curl, CURLOPT_WRITEFUNCTION, download_callback);
  dlinfo->cres = curl_easy_perform(dlinfo->curl);
  curl_easy_cleanup(dlinfo->curl);
  fclose(dlinfo->fp);

  if (dlinfo->cres != CURLE_OK)
    return 1;

  return 0;
}