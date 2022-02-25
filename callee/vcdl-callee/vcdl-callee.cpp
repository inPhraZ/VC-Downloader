#include <iostream>
#include <fstream>
#include <codecvt>
#include <locale>

#include "vcdl-callee.h"

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text

// parse command line arguments to extract (Title, URL, Cookies)
int ParseCmdLine(LPDOWNLOADINFO dlinfo);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // Initialize szTitle
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

  LPDOWNLOADINFO dlinfo = DownloadInfoAlloc();
  if (!dlinfo)
    exit(EXIT_FAILURE);

  if (ParseCmdLine(dlinfo) == -1) {
    DownloadInfoFree(dlinfo);
    exit(EXIT_FAILURE);
  }

  DownloadInfoFree(dlinfo);

  return 0;
}

int ParseCmdLine(LPDOWNLOADINFO dlinfo)
{
  if (!dlinfo)
    return -1;

  int argc;
  LPWSTR* argv;
  argv = CommandLineToArgvW(GetCommandLine(), &argc);

  // Title
  dlinfo->Title_len = wcslen(argv[0]);
  dlinfo->Title = _wcsdup(argv[0]);

  // URL
  dlinfo->URL_len = wcslen(argv[1]);
  dlinfo->URL = _wcsdup(argv[1]);

  // Cookies
  dlinfo->Cookies_len = wcslen(argv[2]);
  dlinfo->Cookies = _wcsdup(argv[2]);

  LocalFree(argv);

  if (!dlinfo->Title ||
    !dlinfo->URL ||
    !dlinfo->Cookies)
    return -1;

  return 0;
}