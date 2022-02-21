#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#include "nativeMessaging.h"

// disable IO buffering of stdio and set io in Binary mode
int setupIO(FILE* file);

int main()
{
    unsigned int title_len, url_len, cookie_len;
    char* title, * url, * cookies;

    if (setupIO(stdin) != 0 || setupIO(stdout) != 0)
        exit(EXIT_FAILURE);

    title = recieveFromExtension(&title_len);
    sendToExtension(title);
    url = recieveFromExtension(&url_len);
    sendToExtension(url);
    cookies = recieveFromExtension(&cookie_len);

    FILE* fp = NULL;
    fopen_s(&fp, "Data.txt", "w");
    if (fp) {
        fprintf(fp, "Title: %s\n", title);
        fprintf(fp, "URL: %s\n", url);
        fprintf(fp, "Cookies: %s\n", cookies);
        fclose(fp);
    }

    free(title);
    free(url);
    free(cookies);

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