#include "myprotocol.h"
#include <cstring>
#include <iostream>
#include "common.h"
/**
 * @brief myProtocol构造函数，按照协议拼接成传输的字符串
 * @param sync，0x01为同步，0x02为异步
 * @param platform，0x01为PC端 ,0x02为移动端,0x03为摄像头内部
 * @param json，传输的json数据，为CJsonObject对象
 */
myProtocol::myProtocol(unsigned char sync,unsigned char platform,CJsonObject json)
{
    isrecv = false;
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
/**
 * @brief myProtocol构造函数，按照协议拼接成传输的字符串
 * @param sync，0x01为同步，0x02为异步
 * @param platform，0x01为PC端 ,0x02为移动端,0x03为摄像头内部
 * @param json，传输的json数据，为string对象
 */
myProtocol::myProtocol(unsigned char sync,unsigned char platform,std::string json)
{
    isrecv = false;
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
/**
 * @brief 析构函数，删除生成的字符串，释放空间
 */
myProtocol::~myProtocol()
{
    if(isrecv)
        delete []json_data;
    else
        delete []data;
}
/**
 * @brief 获取字符串
 * @return 返回unsigned char字符串
 */
unsigned char * myProtocol::GetData()
{
    return data;
}
/**
 * @brief 获取字符串长度
 * @return 返回字符串长度
 */
unsigned long myProtocol::Getlength()
{
    return (json_len+10);
}
/**
 * @brief 获取平台参数
 * @return 返回平台参数
 */
unsigned char myProtocol::GetPlatform()
{
    return platform;
}
/**
 * @brief 获取同步参数
 * @return 返回同步参数
 */
unsigned char myProtocol::GetSync()
{
    return sync;
}
/**
 * @brief 获取json对象
 * @return 返回json对象
 */
jsoncpp *myProtocol::GetJson()
{
    std::string str(json_data);
    cout<<"get json:"<<str<<endl;
    jsonc = new jsoncpp(str);
    return jsonc;
}

char * myProtocol::GetJsondata()
{
    return json_data;
}
/**
 * @brief 获取协议是不是通过校验
 * @return 返回ture为通过，false为不通过
 */
bool myProtocol::getCheck()
{
    return check;
}

/**
 * @brief 构造函数，解析收到的字符串，存储sync,platform，json数据
 * @param recv,收到的字符串
 */
myProtocol::myProtocol(char *recv)
{
    cout<<"start pro"<<endl;
    isrecv =true;
    memcpy(&sync,recv+1,1);
    memcpy(&platform,recv+2,1);
    memcpy(&json_len,recv+4,4);
    cout<<"len is"<<json_len<<endl;
    cout<<"0"<<endl;
    json_data = new char[json_len];
    cout<<"1"<<endl;
    memcpy(json_data,recv+8,static_cast<unsigned long>(json_len));
    unsigned char recv_check;
    memcpy(&recv_check,recv+8+json_len,1);
    cout<<"2"<<endl;
    unsigned char checksum = CheckSum(reinterpret_cast<unsigned char *>(recv),8+json_len);
    if(recv_check != checksum)
    {
        std::cout<<"checksum is wrong"<<std::endl;
        check = false;
    }
    else
        check = true;
    cout<<"end pro"<<endl;
}
/**
 * @brief 校验和算法
 * @param 校验的字符串
 * @param 校验的字符串长度
 * @return 返回校验和，类型为unsigned char
 */
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
