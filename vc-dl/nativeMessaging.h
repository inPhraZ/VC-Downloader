#pragma once

#define     MAX_MSG_LEN     (1024*1024)

// send msg to extension
unsigned int sendToExtension(const char* msg);

// recieve msg from extension
char* recieveFromExtension(unsigned int* len);