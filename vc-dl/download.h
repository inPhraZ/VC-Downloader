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

int download_archive(download_struct *dlinfo);