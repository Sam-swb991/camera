#ifndef JSONCPP_H
#define JSONCPP_H
#include "transport.h"
#include "json/json.h"
#include <list>
#include "rs485.h"
#include "blockchaindata.h"
class jsoncpp
{
public:
    jsoncpp();
    jsoncpp(Json::Value json);
    jsoncpp(std::string json);
    jsoncpp(std::string json,blockchainData *data);
    std::string getJsonString();
    int getMode();
    int getCode();
    int getDirection();
    int getatempmode();
    int getyuntaimode();
    string getpreHash();
    float getatemp();
    float getcoefficient();
    int getyuntaiangle();
    std::string getip();
    std::string getarduinoip();
    void create_code(int code);
    std::vector<RECTSET> getRectset(int *len);
    std::string toStyledString();
    void create_rect(std::vector<RECTSET> rectset, int len);
    void create_temp(WINDOW window, std::vector<RECT> rect, int rectnum, std::list<int> linkagealarm, std::list<int> highalarm, std::list<int> prealarm, float **temp);
    void create_real_temp(float realtemp);
    void create_window(WINDOW window);
    void create_ver();
    void create_SN(std::string SN);
//    void create_alarm(HTTPURL *url);
    void create_atemp(bool havemodel);
    void create_coefficient(float coefficient);
    void create_udp_data(HTTPURL *,string prehash);
    int execshellcmd();
    int settime();
private:
    Json::Value myjson;
    Json::CharReaderBuilder readbuilder;
    Json::StreamWriterBuilder  writebuilder;
    int rectlen;
    int direction;
    std::string ip,arduinoip;
    std::string shellcmd;
    std::vector<RECTSET> rectset;
    std::string preHash;
    int angle;
    int mode;
    int code;
    int atempmode;
    int yuntaimode;
    float ambienttemp;
    float coefficient;
};

#endif // JSONCPP_H
