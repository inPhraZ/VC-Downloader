#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>

#include "nativeMessaging.h"

uint32_t sendToExtension(const char* msg)
{
  uint32_t len;
  if (!msg)
    return 0;

  len = strnlen(msg, MAX_MSG_LEN);
  fwrite(&len, sizeof(uint32_t), 1, stdout);
  return fwrite(msg, 1, len, stdout);
}

char* recieveFromExtension(uint32_t* len)
{
  uint32_t sz;
  fread(&sz, sizeof(uint32_t), 1, stdin);
  *len = sz;

  char* msg = (char*)malloc(sz + 1);
  if (!msg) {
    perror("Allocating buffer");
    return NULL;
  }

  memset(msg, 0, sz + 1);
  fread(msg, 1, sz, stdin);
  return msg;
}