#pragma once

typedef struct {
	char* title;
	char* url;
	char* cookies;
} download_struct;

int download_archive(download_struct *dlinfo);