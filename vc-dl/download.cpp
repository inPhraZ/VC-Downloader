#include <cstdlib>
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

int download_archive(download_struct *dlinfo)
{
	if (!dlinfo)
		return -1;
	if (!dlinfo->title || !dlinfo->url)
		return -1;

	char* url = create_url(dlinfo->title, dlinfo->url);
	free(dlinfo->url);
	dlinfo->url = url;

	dlinfo->curl = curl_easy_init();
	curl_easy_setopt(dlinfo->curl, CURLOPT_URL, "https://example.com");
	dlinfo->res = curl_easy_perform(dlinfo->curl);
	curl_easy_cleanup(dlinfo->curl);

	return 0;
}