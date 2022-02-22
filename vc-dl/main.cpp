#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#include "nativeMessaging.h"
#include "download.h"

// disable IO buffering of stdio and set io in Binary mode
int setupIO(FILE* file);
download_struct *getInfoFromExtension();

char* standard_msg(const char* _msg);

int main()
{
    if (setupIO(stdin) != 0 || setupIO(stdout) != 0)
        exit(EXIT_FAILURE);

    download_struct* dlinfo;
    dlinfo = getInfoFromExtension();
    if (!dlinfo) {
        perror("Allocating memory for dlinfo");
        exit(EXIT_FAILURE);
    }
    download_archive(dlinfo);
    free(dlinfo);
    
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

download_struct* getInfoFromExtension()
{
    download_struct* dlinfo;
    unsigned int title_len, url_len, cookie_len;
    char* title, * url, * cookies;

    title = recieveFromExtension(&title_len);
    sendToExtension(title);
    url = recieveFromExtension(&url_len);
    sendToExtension(url);
    cookies = recieveFromExtension(&cookie_len);

    dlinfo = (download_struct*)malloc(sizeof(download_struct));
    if (!dlinfo)
        return NULL;
    dlinfo->title = standard_msg(title);
    dlinfo->url = standard_msg(url);
    dlinfo->cookies = standard_msg(cookies);

    free(title);
    free(url);
    free(cookies);
    
    return dlinfo;
}

char* standard_msg(const char* _msg)
{
    char* msg = NULL;
    std::string tmp(_msg);

    if (tmp.empty())
        return NULL;

    tmp.erase(remove(tmp.begin(), tmp.end(), '\"'), tmp.end());
    msg = _strdup(tmp.c_str());
    return msg;
}