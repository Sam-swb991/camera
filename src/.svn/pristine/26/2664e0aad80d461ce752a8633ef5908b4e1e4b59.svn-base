#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <iostream>
#include <string>
#include "curl/curl.h"
#include "transport.h"
#include <pthread.h>
#include "threadPool.h"
using namespace std;
class httpRequest
{
public:
    httpRequest(ClThreadPool * threadpool);

    void start(HTTPURL *url);
    enum HTTPMODE{GET,POST};
private:
    static size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream);
    static CURLcode curl_get_req(const string &url, string &response);
    static CURLcode curl_post_req(const string &url, const string &postParams, string &response);
    static string cat_url(HTTPURL *url);
    const static string URL_HRAD;
    static void * httpsend(void *);
    static int curl_req(const string &url,string &response,int MODE);
    ClThreadPool * threadpool;

};

#endif // HTTPREQUEST_H
