#ifndef MYPROTOCOL_H
#define MYPROTOCOL_H

#include "CJsonObject.hpp"
#include "jsoncpp.h"
using namespace neb;
class myProtocol
{
public:
    myProtocol(unsigned char sync,unsigned char platform,CJsonObject json);
    myProtocol(unsigned char sync,unsigned char platform,std::string json);
    myProtocol(char *);
    ~myProtocol();
    unsigned char GetSync();
    jsoncpp *GetJson();
    char * GetJsondata();
    unsigned char *GetData();
    unsigned long Getlength();
    unsigned char GetPlatform();
    bool getCheck();
private:
    unsigned char CheckSum(void *uBuff, unsigned long uBuffLen);
    const unsigned char HEAD = 0xEF;
    const unsigned char END = 0x1D;
    unsigned char sync,platform;
    size_t json_len;
    unsigned char *data;
    char *json_data;
    CJsonObject *json;
    jsoncpp *jsonc;
    bool check;
    bool isrecv=false;
};

#endif // MYPROTOCOL_H
