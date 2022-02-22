#pragma once

#include <curl/curl.h>

typedef struct {
	char* title;
	char* url;
	char* cookies;
	CURL* curl;
	CURLcode res;
} download_struct;

int download_archive(download_struct *dlinfo);