#pragma once

#include "framework.h"

typedef struct {
  LPWSTR  Title;
  LPWSTR  URL;
  LPWSTR  Cookies;
  SIZE_T  Title_len;
  SIZE_T  URL_len;
  SIZE_T  Cookies_len;
} DOWNLOADINFO, *LPDOWNLOADINFO;

LPDOWNLOADINFO    DownloadInfoAlloc();

#define DownloadInfoFree(dlinfo)    \
do {                                \
  free(dlinfo->Title);              \
  free(dlinfo->URL);                \
  free(dlinfo->Cookies);            \
  free(dlinfo);                     \
  dlinfo = NULL;                    \
} while(0)