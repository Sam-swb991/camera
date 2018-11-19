#ifndef JSONHELPER_H
#define JSONHELPER_H
#include "CJsonObject.hpp"
#include "transport.h"
using namespace neb;
class jsonhelper
{
public:
    jsonhelper();
    jsonhelper(CJsonObject json);
    void create_temp(WINDOW windos, RECT *rect, int rectnum, POINT point[5120]);
	void create_rect(RECTSET *rectset,int len);
    CJsonObject getJson();
    int getCode();
    RECTSET *getRectset(int *len);
private:
    CJsonObject myjson;
    RECTSET *rectset;
    int code;
    int rectlen;
};

#endif // JSONHELPER_H
