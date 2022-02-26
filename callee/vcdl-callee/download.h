#pragma once

#include "framework.h"

typedef struct {
  LPWSTR  Path;
  LPWSTR  Title;
  LPWSTR  URL;
  LPWSTR  Cookies;
  SIZE_T  Title_len;
  SIZE_T  URL_len;
  SIZE_T  Cookies_len;
  FILE*   fp;
  CURL*   curl;
  CURLcode  cres;
} DOWNLOADINFO, *LPDOWNLOADINFO;

LPDOWNLOADINFO    DownloadInfoAlloc();

int DownloadArchive(LPDOWNLOADINFO dlinfo);

#define DownloadInfoFree(dlinfo)    \
do {                                \
  free(dlinfo->Path);               \
  free(dlinfo->Title);              \
  free(dlinfo->URL);                \
  free(dlinfo->Cookies);            \
  free(dlinfo);                     \
  dlinfo = NULL;                    \
} while(0)