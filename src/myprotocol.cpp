#include "myprotocol.h"
#include <cstring>
#include <iostream>
#include "common.h"
myProtocol::myProtocol(unsigned char sync,unsigned char platform,CJsonObject json)
{
    unsigned char unuse = 0x00;
    json_len = json.ToString().size();
    std::cout<<"json len is:"<<json_len<<std::endl;
    data = new unsigned char[json_len+10];
    memcpy(data,&HEAD,1);
    memcpy(data+1,&sync,1);
    memcpy(data+2,&platform,1);
    memcpy(data+3,&unuse,1);
    memcpy(data+4,&json_len,4);
    memcpy(data+8,json.ToString().c_str(),static_cast<unsigned long>(json_len));
    unsigned char checksum = CheckSum(data,8+json_len);
    memcpy(data+8+json_len,&checksum,1);
    memcpy(data+9+json_len,&END,1);
}
myProtocol::myProtocol(unsigned char sync,unsigned char platform,std::string json)
{
    unsigned char unuse = 0x00;
    json_len = json.size();
    std::cout<<"json len is:"<<json_len<<std::endl;
    data = new unsigned char[json_len+10];
    memcpy(data,&HEAD,1);
    memcpy(data+1,&sync,1);
    memcpy(data+2,&platform,1);
    memcpy(data+3,&unuse,1);
    memcpy(data+4,&json_len,4);
    memcpy(data+8,json.c_str(),static_cast<unsigned long>(json_len));
    unsigned char checksum = CheckSum(data,8+json_len);
    memcpy(data+8+json_len,&checksum,1);
    memcpy(data+9+json_len,&END,1);

}
myProtocol::~myProtocol()
{
    delete []data;
    //delete json_data;
    //delete json;

}
unsigned char * myProtocol::GetData()
{
    return data;
}
unsigned long myProtocol::Getlength()
{
    return (json_len+10);
}
unsigned char myProtocol::GetPlatform()
{
    return platform;
}
unsigned char myProtocol::GetSync()
{
    return sync;
}
CJsonObject* myProtocol::GetJson()
{
    std::string str(json_data);
    cout<<str<<endl;
    json = new CJsonObject(str);
    return json;
}
bool myProtocol::getCheck()
{
    return check;
}
myProtocol::myProtocol(char *recv)
{
    memcpy(&sync,recv+1,1);
    memcpy(&platform,recv+2,1);
    memcpy(&json_len,recv+4,4);
    json_data = new char[json_len];
    memcpy(json_data,recv+8,static_cast<unsigned long>(json_len));
    unsigned char recv_check;
    memcpy(&recv_check,recv+8+json_len,1);
    unsigned char checksum = CheckSum(reinterpret_cast<unsigned char *>(recv),8+json_len);
    if(recv_check != checksum)
    {
        std::cout<<"checksum is wrong"<<std::endl;
        check = false;
    }
    else
        check = true;
}
unsigned char myProtocol::CheckSum(void *uBuff, unsigned long uBuffLen)
{
    unsigned char uSum=0;
    const unsigned char * buff = static_cast<unsigned char *>(uBuff);
    for(unsigned long i=0;i<uBuffLen;i++)
    {
        uSum += buff[i];
    }
    uSum = (~uSum) + 1;
    return uSum;
}
