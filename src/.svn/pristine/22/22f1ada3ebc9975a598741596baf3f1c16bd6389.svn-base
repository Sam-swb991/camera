#include "httprequest.h"
#include "common.h"
#include <sstream>
#include "threadPool.h"
#include "jsoncpp.h"
#include "myprotocol.h"
#include "sockethelper.h"
const string httpRequest::URL_HRAD = "http://www.horaintel.com/hora/alarm/data";
//const string httpRequest::URL_HRAD = "http://192.168.0.88:8080/wx/data";
httpRequest::httpRequest(ClThreadPool * threadpool)
{
    this->threadpool = threadpool;
}
size_t httpRequest::req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
{
    //cout << "----->reply" << endl;
    string *str = (string*)stream;
    //cout << *str << endl;
    (*str).append((char*)ptr, size*nmemb);
    return size * nmemb;
}

CURLcode httpRequest::curl_get_req(const std::string &url, std::string &response)
{
    // init curl
    CURL *curl = curl_easy_init();
    struct curl_slist *head = NULL;
    // res code
    CURLcode res;
    if (curl)
    {
        // set params
        //wstring wurl = common::s2ws(url);
        head = curl_slist_append(head, "Content-Type:application/x-www-form-urlencoded;charset=UTF-8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, head);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        //curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
        //curl_easy_setopt(curl, CURLOPT_NOBODY,1);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3); // set transport and time out time
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
        // start req
        res = curl_easy_perform(curl);

    }
    // release curl
    curl_slist_free_all(head);
    curl_easy_cleanup(curl);
    return res;
}

CURLcode httpRequest::curl_post_req(const string &url, const string &postParams, string &response)
{
    // init curl
    CURL *curl = curl_easy_init();
    struct curl_slist *head = NULL;

    // res code
    CURLcode res;
    if (curl)
    {
        // set params
        head = curl_slist_append(head, "Content-Type:application/x-www-form-urlencoded;charset=UTF-8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, head);
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post req
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str()); // params
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
       // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
       // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
       // curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
        // start req
        res = curl_easy_perform(curl);

    }
    // release curl
    curl_slist_free_all(head);
    curl_easy_cleanup(curl);
    return res;
}

int httpRequest::curl_req(const string &url,string &response,int MODE)
{
    CURLcode res ;
    switch (MODE) {
        case GET:{
            res = curl_get_req(url,response);
        }break;
        case POST:{
            vector<string> split = common::split(url,"?");
            cout<<split[0]<<endl;
            if(split.size() == 1)
                res = curl_post_req(split[0],"",response);
            else if(split.size() == 2)
                res = curl_post_req(split[0],split[1],response);
            else
            {
                cout<<"wrong!!"<<endl;
            }
        }break;
        default:{
            res = CURL_LAST;
        }break;

    }
    if (res != CURLE_OK)
    {
        printf("res is:%d\n",res);
        return -1;
    }
    else
        return 0;
}

string httpRequest::cat_url(HTTPURL *url)
{
    string URL;
    URL.clear();
    URL =  URL_HRAD;
    URL+="?rectname="+url->rectname;
    URL+="&alarmmode="+common::to_string(url->alarmmode);
    URL+="&hightemp="+common::to_string(url->com_temp.highTemp);
    URL+="&avgtemp="+common::to_string(url->com_temp.avgTemp);
    URL+="&lowtemp="+common::to_string(url->com_temp.lowTemp);
    URL+="&ip="+url->ip;
    URL+="&time="+common::to_string(url->dtime);
    URL+="&cameraid="+url->camera_id;
    cout<<"request url:"<<URL<<endl;
    return URL;
}

void * httpRequest::httpsend(void * url)
{
    HTTPURL *sendURL = (HTTPURL *)url;
    string sendurl = cat_url(sendURL);
    string response;
    int res = curl_req(sendurl,response,POST);
    if(res == 0)
    {

        cout<<"response:"<<response<<endl;
    }
    else
        cout<<"send alarm error!"<<endl;

    return nullptr;
}

void httpRequest::start(HTTPURL *url)
{
    int ret = threadpool->Add(httpsend,(void *)url,sizeof(HTTPURL));

    if(ret) {
        std::cout << "Create http pthread error!" << std::endl;

    }
    return ;
}
