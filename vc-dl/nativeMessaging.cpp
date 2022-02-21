#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "nativeMessaging.h"

unsigned int sendToExtension(const char* msg)
{
    unsigned int len;
    if (!msg)
        return 0;

    len = strnlen(msg, MAX_MSG_LEN);
    fwrite(&len, sizeof(unsigned int), 1, stdout);
    return fwrite(msg, 1, len, stdout);
}

char* recieveFromExtension(unsigned int* len)
{
    unsigned int sz;
    fread(&sz, sizeof(unsigned int), 1, stdin);
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