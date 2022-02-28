#include "merge.h"

int ExtractArchive(LPDOWNLOADINFO dlinfo)
{
  LPSTARTUPINFO  startup_info;
  LPPROCESS_INFORMATION proc_info;
  LPWSTR pathToDir;
  LPWSTR lpCmdLine;
  BOOL res;

  if (!dlinfo)
    return -1;

  startup_info = (LPSTARTUPINFO)malloc(sizeof(STARTUPINFO));
  if (!startup_info) {
    return -1;
  }
  proc_info = (LPPROCESS_INFORMATION)malloc(sizeof(PROCESS_INFORMATION));
  if (!proc_info) {
    free(startup_info);
    return -1;
  }

  ZeroMemory(startup_info, sizeof(STARTUPINFOA));
  ZeroMemory(proc_info, sizeof(PROCESS_INFORMATION));

  lpCmdLine = (LPWSTR)malloc(MAX_PATH * 3);
  if (!lpCmdLine) {
    free(startup_info);
    free(proc_info);
    return -1;
  }

  pathToDir = _wcsdup(dlinfo->Path);
  PathRemoveExtension(pathToDir);
  wsprintf(lpCmdLine, L"7za.exe e \"%s\" -o\"%s\" -y", dlinfo->Path, pathToDir);

  WCHAR curr[MAX_PATH + 1] = { 0 };
  DWORD s;
  GetCurrentDirectory(MAX_PATH, curr);
  MessageBox(NULL, curr, L"current", MB_OK);
  MessageBox(NULL, lpCmdLine, L"arguments", MB_OK);
  res = CreateProcess(L"7za.exe", lpCmdLine,
    NULL, NULL, FALSE, 0, NULL, NULL,
    startup_info, proc_info);

  free(lpCmdLine);
  free(pathToDir);
  free(proc_info);
  free(startup_info);

  if (res == 0)
    return -1;

  return 0;
}