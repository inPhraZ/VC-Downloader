#pragma once

#include <curl/curl.h>

typedef struct {
	char* title;
	char* url;
	char* cookies;
	char* filename;
	FILE* fp;
	CURL* curl;
	CURLcode res;
} download_struct;

void free_download_struct(download_struct* dlinfo);

#define free_dlinfo(dlinfo)		\
do {							\
	free(dlinfo->title);		\
	free(dlinfo->url);			\
	free(dlinfo->cookies);		\
	free(dlinfo->filename);		\
} while (0)
