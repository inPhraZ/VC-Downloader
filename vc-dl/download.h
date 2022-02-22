#pragma once

#include <curl/curl.h>

typedef struct {
	char* title;
	char* url;
	char* cookies;
	char* filename;
	char* dlpath;
	FILE* fp;
	CURL* curl;
	CURLcode res;
} download_struct;

int download_archive(download_struct* dlinfo);

#define free_dlinfo(dlinfo)		\
do {							\
	free(dlinfo->title);		\
	free(dlinfo->url);			\
	free(dlinfo->cookies);		\
	free(dlinfo->filename);		\
	free(dlinfo->dlpath);		\
	dlinfo = NULL;				\
} while (0)