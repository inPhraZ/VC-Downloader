#include <cstdlib>
#include <cstring>
#include <curl/curl.h>

#include "download.h"

#define		QUESTION_MARK	0x00003F

static void save_stat(download_struct* dlinfo)
{
	FILE* fp = NULL;
	fopen_s(&fp, "stat.txt", "w");
	if (fp) {
		fprintf(fp, "Title: %s\n", dlinfo->title);
		fprintf(fp, "URL: %s\n", dlinfo->url);
		fprintf(fp, "Cookies: %s\n", dlinfo->cookies);
		fprintf(fp, "res: %s\n", curl_easy_strerror(dlinfo->res));
		fclose(fp);
	}
}

static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
	download_struct* dlinfo = (download_struct*)userdata;
	fwrite(ptr, size, nmemb, dlinfo->fp);
	return size * nmemb;
}

static char *create_url(const char *_title, const char *_url)
{
	const char* first = _url;
	const char* last = strchr(_url, QUESTION_MARK);
	const char* pre = "output/";
	const char* post = ".zip?download=zip";

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

	save_stat(dlinfo);

	/*
	dlinfo->fp = NULL;
	fopen_s(&dlinfo->fp, "output.zip", "wb");
	if (!dlinfo->fp)
		return -1;

	dlinfo->curl = curl_easy_init();
	curl_easy_setopt(dlinfo->curl, CURLOPT_URL, dlinfo->url);
	curl_easy_setopt(dlinfo->curl, CURLOPT_COOKIE, dlinfo->cookies);
	curl_easy_setopt(dlinfo->curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(dlinfo->curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(dlinfo->curl, CURLOPT_WRITEDATA, (void *)dlinfo);
	dlinfo->res = curl_easy_perform(dlinfo->curl);
	fclose(dlinfo->fp);
	curl_easy_cleanup(dlinfo->curl);
	*/

	return 0;
}