#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#define     MAX_MSG_FROM_NATIVE     (1024*1024)

// disable IO buffering of stdio and set io in Binary mode
int setupIO(FILE* file);

// send msg to extension
unsigned int sendToExtension(const char* msg);

// recieve msg from extension
char* recieveFromExtension(unsigned int* len);

int main()
{
    if (setupIO(stdin) != 0 || setupIO(stdout) != 0)
        exit(EXIT_FAILURE);

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

unsigned int sendToExtension(const char* msg)
{
    unsigned int len;
    if (!msg)
        return 0;

    len = strnlen(msg, MAX_MSG_FROM_NATIVE);
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