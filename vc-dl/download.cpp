#include <cstring>
#include <curl/curl.h>

#include "download.h"

#define		QUESTION_MARK	0x00003F

static char *create_url(const char *_title, const char *_url)
{
	const char* first = _url;
	const char* last = strchr(_url, QUESTION_MARK);
	const char* pre = "output/";
	const char* post = "?download=zip";

	int len = (int)(last - first);
	int pre_len = strlen(pre);
	int title_len = strlen(_title);
	int post_len = strlen(post);
	int url_len = len + pre_len + title_len + post_len + 1;

	char* url = (char*)malloc(url_len);
	if (!url) {
		perror("inner error: create_url");
		return NULL;
	}
	memset(url, 0, url_len);
	strncpy_s(url, url_len, _url, len);
	strncat_s(url, url_len, pre, pre_len);
	strncat_s(url, url_len, _title, title_len);
	strncat_s(url, url_len, post, post_len);

	return url;
}

int download_archive(const char* _title, const char* _url, const char* _cookies)
{
	if (!_title || !_url)
		return -1;

	char* url = create_url(_title, _url);
	printf("%s\n", url);
	free(url);

	return 0;
}