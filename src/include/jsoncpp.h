#ifndef JSONCPP_H
#define JSONCPP_H
#include "transport.h"
#include "json/json.h"
#include <list>
class jsoncpp
{
public:
    jsoncpp();
    jsoncpp(Json::Value json);
    jsoncpp(std::string json);
    std::string getJsonString();
    int getMode();
    int getCode();
    void create_code(int code);
    RECTSET *getRectset(int *len);
    std::string toStyledString();
    void create_rect(RECTSET *rectset,int len);
    void create_temp(WINDOW windos, RECT *rect, int rectnum, std::list<int> linkagealarm, std::list<int> highalarm, std::list<int> prealarm, float **temp);
    void create_real_temp(float realtemp);
private:
    Json::Value myjson;
    Json::CharReaderBuilder readbuilder;
    Json::StreamWriterBuilder  writebuilder;
    int rectlen;
    RECTSET *rectset;
    int mode;
    int code;
};

#endif // JSONCPP_H
