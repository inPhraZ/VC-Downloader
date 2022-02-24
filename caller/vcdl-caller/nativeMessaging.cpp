#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "nativeMessaging.h"

size_t sendToExtension(const char* msg)
{
  size_t len;
  if (!msg)
    return 0;

  len = strnlen(msg, MAX_MSG_LEN);
  fwrite(&len, sizeof(size_t), 1, stdout);
  return fwrite(msg, 1, len, stdout);
}

char* recieveFromExtension(size_t* len)
{
  size_t sz;
  fread(&sz, sizeof(size_t), 1, stdin);
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