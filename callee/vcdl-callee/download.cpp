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