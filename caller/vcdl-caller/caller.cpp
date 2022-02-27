#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <fcntl.h>
#include <io.h>
#include <Windows.h>
#include <processthreadsapi.h>

#include "nativeMessaging.h"

// disable IO buffering of stdio and set io in Binary mode
int setupIO(FILE* file);

int main()
{
  uint32_t tlen, ulen, clen, total;
  char* title, * url, * cookies;
  LPSTR lpCommandLine = NULL;

  if (setupIO(stdin) != 0 || setupIO(stdout) != 0)
    exit(EXIT_FAILURE);

  title = recieveFromExtension(&tlen);
  sendToExtension(title);
  url = recieveFromExtension(&ulen);
  sendToExtension(url);
  cookies = recieveFromExtension(&clen);

  total = tlen + ulen + clen + 3;
  lpCommandLine = (CHAR*)malloc(total);
  if (!lpCommandLine) {
    perror("malloc");
    free(title);
    free(url);
    free(cookies);
    exit(EXIT_FAILURE);
  }
  memset(lpCommandLine, 0, total);
  sprintf_s(lpCommandLine, total, "%s %s %s", title, url, cookies);

  free(title);
  free(url);
  free(cookies);

  LPSTARTUPINFOA  startup_info;
  LPPROCESS_INFORMATION proc_info;

  startup_info = (LPSTARTUPINFOA)malloc(sizeof(STARTUPINFOA));
  if (!startup_info) {
    perror("STARTUPINFO");
    free(lpCommandLine);
    exit(EXIT_FAILURE);
  }
  proc_info  = (LPPROCESS_INFORMATION)malloc(sizeof(PROCESS_INFORMATION));
  if (!proc_info) {
    perror("PROCESS_INFORMATION");
    free(lpCommandLine);
    free(startup_info);
    exit(EXIT_FAILURE);
  }

  ZeroMemory(startup_info, sizeof(STARTUPINFOA));
  ZeroMemory(proc_info, sizeof(PROCESS_INFORMATION));

  BOOL res = CreateProcessA("vcdl-callee.exe", lpCommandLine, 
    NULL, NULL, FALSE, 0, NULL, NULL,
    startup_info, proc_info);

  free(lpCommandLine);

  return 0;
}

int setupIO(FILE* file)
{
  if (_setmode(_fileno(file), _O_BINARY) == -1) {
    perror("Cannot set on binary mode");
    return -1;
  }
  if (setvbuf(file, NULL, _IONBF, 0) == -1) {
    perror("Cannot unset buffering IO");
    return -1;
  }
  return 0;
}