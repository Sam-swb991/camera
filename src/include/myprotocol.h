#ifndef MYPROTOCOL_H
#define MYPROTOCOL_H

#include "CJsonObject.hpp"
using namespace neb;
class myProtocol
{
public:
    myProtocol(unsigned char sync,unsigned char platform,CJsonObject json);
    myProtocol(char *);
    unsigned char GetSync();
    CJsonObject* GetJson();
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
    bool check;
};

#endif // MYPROTOCOL_H
