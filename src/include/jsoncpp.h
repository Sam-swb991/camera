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
    int getDirection();
    std::string getip();
    void create_code(int code);
    std::vector<RECTSET> getRectset(int *len);
    std::string toStyledString();
    void create_rect(std::vector<RECTSET> rectset, int len);
    void create_temp(WINDOW window, std::vector<RECT> rect, int rectnum, std::list<int> linkagealarm, std::list<int> highalarm, std::list<int> prealarm, float **temp);
    void create_real_temp(float realtemp);
    void create_window(WINDOW window);
    void create_ver();
    int execshellcmd();
private:
    Json::Value myjson;
    Json::CharReaderBuilder readbuilder;
    Json::StreamWriterBuilder  writebuilder;
    int rectlen;
    int direction;
    std::string ip;
    std::string shellcmd;
    std::vector<RECTSET> rectset;
    int mode;
    int code;
};

#endif // JSONCPP_H
