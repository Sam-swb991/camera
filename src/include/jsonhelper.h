#ifndef JSONHELPER_H
#define JSONHELPER_H
#include "CJsonObject.hpp"
#include "transport.h"
#include "jsoncpp.h"
using namespace neb;
class jsonhelper
{
public:
    jsonhelper();
    jsonhelper(CJsonObject json);
    //void create_temp(WINDOW windos, RECT *rect, int rectnum, float **temp);
	void create_rect(RECTSET *rectset,int len);
    CJsonObject getJson();
    int getCode();
    RECTSET *getRectset(int *len);
    int getMode();
private:
    CJsonObject myjson;
    RECTSET *rectset;
    int code;
    int rectlen;
    int mode;

};

#endif // JSONHELPER_H
