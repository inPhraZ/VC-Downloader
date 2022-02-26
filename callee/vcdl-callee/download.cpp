#include "resource.h"
#include "download.h"

LPDOWNLOADINFO p_dlinfo;

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

static DWORD WINAPI DownloaderThreadProc(LPVOID lpParameter)
{
  size_t len = 0;
  char* url = (char*)malloc(1024);
  char* cookies = (char*)malloc(1024);

  HWND prog = GetDlgItem(p_dlinfo->currDlg, IDC_PROGRESS1);

  SendMessage(prog, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
  SendMessage(prog, PBM_SETPOS, (WPARAM)50, NULL);

  _wfopen_s(&p_dlinfo->fp, p_dlinfo->Path, L"wb");
  if (!p_dlinfo->fp)
    return -1;

  url = (char*)malloc(1024);
  cookies = (char*)malloc(1024);
  ZeroMemory(url, 1024);
  ZeroMemory(cookies, 1024);

  wcstombs_s(&len, url, 1024, p_dlinfo->URL, p_dlinfo->URL_len);
  wcstombs_s(&len, cookies, 1024, p_dlinfo->Cookies, p_dlinfo->Cookies_len);

  p_dlinfo->curl = curl_easy_init();
  if (!p_dlinfo->curl)
    return -1;

  curl_easy_setopt(p_dlinfo->curl, CURLOPT_URL, url);
  curl_easy_setopt(p_dlinfo->curl, CURLOPT_COOKIE, cookies);
  curl_easy_setopt(p_dlinfo->curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(p_dlinfo->curl, CURLOPT_WRITEFUNCTION, download_callback);
  curl_easy_setopt(p_dlinfo->curl, CURLOPT_WRITEDATA, (void*)p_dlinfo);
  p_dlinfo->cres = curl_easy_perform(p_dlinfo->curl);
  fclose(p_dlinfo->fp);
  curl_easy_cleanup(p_dlinfo->curl);

  free(url);
  free(cookies);

  if (p_dlinfo->cres != CURLE_OK)
    return -1;

  return 0;
}

static LRESULT CALLBACK DialogDownloadProgress(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_INITDIALOG:
    if (!CreateThread(0, 0, DownloaderThreadProc, NULL, 0, NULL)) {
      MessageBox(NULL, L"CreateThread Failed", L"ERROR", MB_ICONEXCLAMATION);
      DestroyWindow(hwnd);
      PostQuitMessage(-1);
    }
    break;
  case WM_COMMAND:
    break;
  case WM_CLOSE:
    DestroyWindow(hwnd);
  case WM_DESTROY:
    PostQuitMessage(-1);
    break;
  }
  return 0;
}

static int dialog(LPDOWNLOADINFO dlinfo, int dlg, DLGPROC wndProc)
{
  HWND hwnd;
  p_dlinfo = dlinfo;
  hwnd = CreateDialog(
    dlinfo->hInst,
    MAKEINTRESOURCE(dlg),
    NULL,
    wndProc);

  if (!hwnd)
    return 1;

  p_dlinfo->currDlg = hwnd;
  ShowWindow(hwnd, SW_NORMAL);
  UpdateWindow(hwnd);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
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

int DownloadArchive(LPDOWNLOADINFO dlinfo)
{
  if (!dlinfo)
    return -1;

  if (CreateURL(dlinfo) == -1)
    return -1;

  return dialog(dlinfo, IDD_DOWNLOAD_PROGRESS, (DLGPROC)DialogDownloadProgress);
}