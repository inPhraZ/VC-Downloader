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
static int ParseCmdLine(LPDOWNLOADINFO dlinfo)
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


// Get path from user to save archive
static int GetPathFromUser(LPDOWNLOADINFO dlinfo)
{
  OPENFILENAME ofn;
  if (!dlinfo)
    return -1;

  dlinfo->Path = (LPWSTR)malloc(MAX_PATH + 1);
  if (!dlinfo->Path)
    return -1;
  ZeroMemory(dlinfo->Path, MAX_PATH + 1);

  dlinfo->FileTitle = (LPWSTR)malloc(MAX_PATH + 1);
  if (!dlinfo->FileTitle)
    return -1;
  ZeroMemory(dlinfo->FileTitle, MAX_PATH + 1);

  ZeroMemory(&ofn, sizeof(OPENFILENAME));

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = NULL;
  ofn.lpstrTitle = szTitle;
  ofn.lpstrFilter = L"Archive Files (*.zip)\0*.zip\0All Files (*.*)\0*.*\0";
  ofn.lpstrFile = dlinfo->Path;
  ofn.lpstrFileTitle = dlinfo->FileTitle;
  ofn.nMaxFileTitle = MAX_PATH;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt = L"zip";
  if (GetSaveFileName(&ofn))
    return 0;

  return -1;
}

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

  if (GetPathFromUser(dlinfo) == -1) {
    DownloadInfoFree(dlinfo);
    exit(EXIT_FAILURE);
  }

  if (DownloadArchive(dlinfo) == -1) {
    MessageBox(NULL, L"Error: Download failed", L"VC Downloader", MB_OK | MB_ICONWARNING);
    DownloadInfoFree(dlinfo);
    return 1;
  }

  MessageBox(NULL, L"Download completed successfully", L"VC Downloader", MB_OK);
  DownloadInfoFree(dlinfo);

  return 0;
}