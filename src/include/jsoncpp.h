#ifndef JSONCPP_H
#define JSONCPP_H
#include "transport.h"
#include "json/json.h"
#include <list>
class jsoncpp
{
public:
    jsoncpp();
    jsoncpp(std::string json);
    std::string getJsonString();
    int getMode();
    int getCode();
    RECTSET *getRectset(int *len);
    std::string toStyledString();
    void create_temp(WINDOW windos, RECT *rect, int rectnum, std::list<int> alarmnum, float **temp);
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
