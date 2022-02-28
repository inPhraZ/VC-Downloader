#include "resource.h"
#include "download.h"

static LPDOWNLOADINFO p_dlinfo;

LPDOWNLOADINFO  DownloadInfoAlloc()
{
  LPDOWNLOADINFO dlinfo = NULL;
  dlinfo = (LPDOWNLOADINFO)malloc(sizeof(DOWNLOADINFO));
  if (!dlinfo)
    return NULL;
  ZeroMemory(dlinfo, sizeof(DOWNLOADINFO));
  return dlinfo;
}

static char* wide_to_char(LPWSTR src)
{
  size_t convertedChars = 0;
  size_t origSize = 0;
  size_t newSize = 0;
  char* dest = NULL;

  if (!src)
    return NULL;

  origSize = wcslen(src) + 1;
  newSize = origSize * 2;
  dest = (char*)malloc(newSize);
  if (!dest) {
    MessageBoxA(NULL, strerror(errno), "Error", MB_ICONEXCLAMATION);
    SendMessage(p_dlinfo->currDlg, WM_DESTROY, 0, 0);
  }
  wcstombs_s(&convertedChars, dest, newSize, src, _TRUNCATE);
}

static int progress_callback(void* clientp,
  curl_off_t dltotal,
  curl_off_t dlnow,
  curl_off_t ultotal,
  curl_off_t ulnow)
{
  UINT prev = SendDlgItemMessage(p_dlinfo->currDlg, IDC_PROGRESS1, PBM_GETPOS, 0, 0);
  if (dltotal > 0) {
    WCHAR   prcntStr[5] = { 0 };
    WPARAM  prcnt = (WPARAM)((dlnow * 100) / dltotal);
    if (prev != prcnt) {
      wsprintf(prcntStr, L"%d%%", prcnt);
      SendDlgItemMessage(p_dlinfo->currDlg, IDC_PROGRESS1, PBM_SETPOS, prcnt, 0);
      SetDlgItemText(p_dlinfo->currDlg, IDC_PERCENT, prcntStr);
    }
  }
  return 0;
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
  char* url;
  char* cookies;

  WCHAR text[100] = { 0 };
  wsprintf(text, L"Downloading (%s) ...", p_dlinfo->Title);
  SendDlgItemMessage(p_dlinfo->currDlg, IDC_PROGRESS1, PBM_SETPOS, 0, 0);
  SendDlgItemMessage(p_dlinfo->currDlg, IDC_PROGRESS1, PBM_SETBARCOLOR, 0, 0x00228B22);
  SetDlgItemText(p_dlinfo->currDlg, IDC_STATIC, text);
  SetDlgItemText(p_dlinfo->currDlg, IDC_PERCENT, L"0%");

  url = wide_to_char(p_dlinfo->URL);
  if (!url)
    SendMessage(p_dlinfo->currDlg, WM_DESTROY, 0, 0);

  cookies = wide_to_char(p_dlinfo->Cookies);
  if (!cookies) {
    free(url);
    SendMessage(p_dlinfo->currDlg, WM_DESTROY, 0, 0);
  }
  
  _wfopen_s(&p_dlinfo->fp, p_dlinfo->Path, L"wb");
  if (!p_dlinfo->fp) {
    MessageBox(NULL, _wcserror(errno), L"VC Downloader", MB_ICONEXCLAMATION);
    SendMessage(p_dlinfo->currDlg, WM_DESTROY, 0, 0);
  }

  p_dlinfo->curl = curl_easy_init();
  if (!p_dlinfo->curl) {
    fclose(p_dlinfo->fp);
    SendMessage(p_dlinfo->currDlg, WM_DESTROY, 0, 0);
  }
  
  curl_easy_setopt(p_dlinfo->curl, CURLOPT_URL, url);
  curl_easy_setopt(p_dlinfo->curl, CURLOPT_COOKIE, cookies);
  curl_easy_setopt(p_dlinfo->curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(p_dlinfo->curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
  curl_easy_setopt(p_dlinfo->curl, CURLOPT_NOPROGRESS, 0L);
  curl_easy_setopt(p_dlinfo->curl, CURLOPT_WRITEFUNCTION, download_callback);
  curl_easy_setopt(p_dlinfo->curl, CURLOPT_WRITEDATA, (void*)p_dlinfo);
  p_dlinfo->cres = curl_easy_perform(p_dlinfo->curl);
  fclose(p_dlinfo->fp);
  curl_easy_cleanup(p_dlinfo->curl);

  free(url);
  free(cookies);

  if (p_dlinfo->cres == CURLE_OK) {
    WCHAR msg[100] = { 0 };
    wsprintf(msg, L"%s Downloaded successfully", p_dlinfo->Title);
    MessageBox(NULL, msg, L"VC Downloader", MB_OK);
  }
  else {
    WCHAR errmsg[100] = { 0 };
    size_t pReturnValue;
    mbstowcs_s(&pReturnValue, errmsg, 100, curl_easy_strerror(p_dlinfo->cres), 100);
    MessageBox(NULL, errmsg, L"VC Downloader", MB_ICONEXCLAMATION);
  }

  SendMessage(p_dlinfo->currDlg, WM_DESTROY, 0, 0);

  return 0;
}

static LRESULT CALLBACK DialogDownloadProgress(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_INITDIALOG:
  {
    HICON hIcon;
    hIcon = (HICON)LoadImageW(GetModuleHandleW(NULL),
      MAKEINTRESOURCEW(IDI_SMALL),
      IMAGE_ICON,
      GetSystemMetrics(SM_CXSMICON),
      GetSystemMetrics(SM_CYSMICON),
      0);
    if (hIcon)
      SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

    if (!CreateThread(0, 0, DownloaderThreadProc, NULL, 0, NULL)) {
      MessageBox(NULL, L"Download thread failed", L"ERROR", MB_ICONEXCLAMATION);
      PostQuitMessage(-1);
    }
  }
    break;
  case WM_COMMAND:
    break;
  case WM_CLOSE:
    if (MessageBox(hwnd, L"Are you sure?", L"VC Downloader", MB_OKCANCEL) == IDOK)
      PostQuitMessage(0);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
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