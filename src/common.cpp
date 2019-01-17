#include "common.h"
#include <stdio.h>
#include <cmath>
#include "temprule.h"
#include <sstream>
#include <cstdlib>
#include <memory>
/**
 * @brief 按格式打印字符串
 * @param data,需要打印的字符串
 * @param len，需要到打印的长度
 * @param type，为1则把每个字符打印为16进制，为2则转化为unsigned short类型数据再打印
 */
void common::print_V(void *data,int len,int type)
{
    switch (type) {
        case 1:
        {
            unsigned char * c_data = static_cast<unsigned char *>(data);
            for(int i = 0;i<len;i++)
                printf(" %02x",c_data[i]);
        }
        break;
        case 2:
        {
            unsigned short * s_data = static_cast<unsigned short *>(data);
            int tmp[5120] = {0};
            int j =0;
            for(int i = 0;i<len;i++)
            {
                if(s_data[i] == 0)
                {
                   tmp[j] = i;
                   j++;
                }
                printf(" %d",s_data[i]);
            }
            if(j!=0)
            {
                printf("\n\n\nblank value addr is:\n");
                for(int i=0;i<j;i++)
                {
                    printf("%d ",tmp[i]);
                }
                printf("\n");
            }
        }
        break;
        default:break;
    }
}
/**
 * @brief 转换大小端
 * @param data,字符串
 * @param len，字符串长度
 */
void common::change_edian(void *data,int len)
{
    unsigned char *tmp = static_cast<unsigned char *>(data);
    unsigned char t;
    for(int i = 0 ;i<len*2;i+=2)
    {
        t = tmp[i];
        tmp[i] = tmp[i+1];
        tmp[i+1] = t;
    }
}
/**
 * @brief int转string
 * @param num,int数据
 * @return 返回转换后的字符串
 */
string common::to_string(int num)
{
	stringstream ss;
	ss<<num;
	return ss.str();
}
/**
 * @brief float转string
 * @param num,float数据
 * @return 返回转换后的字符串
 */
string common::to_string(float num)
{
	stringstream ss;
	ss<<num;
	return ss.str();
}
/**
 * @brief long转string
 * @param num,long数据
 * @return 返回转换后的字符串
 */
string common::to_string(long num)
{
    stringstream ss;
    ss<<num;
    return ss.str();
}
/**
 * @brief 把数据库读出的温度字符串分割成单个温度字符串
 * @param str，数据库读出温度字符串
 * @param delim，分隔符
 * @return 返回单个温度字符串组成的容器
 */
vector<string> common::split(const string& str, const string& delim) {
    vector<string> res;
    if("" == str) return res;

    char * strs = new char[str.length() + 1] ;
    strcpy(strs, str.c_str());

    char * d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while(p) {
        string s = p;
        res.push_back(s);
        p = strtok(nullptr, d);
    }
    delete []strs;
    delete []d;
    delete p;
    return res;
}

std::wstring common::s2ws(const std::string& str) {
  if (str.empty()) {
    return L"";
  }
  unsigned len = str.size() + 1;
  setlocale(LC_CTYPE, "en_US.UTF-8");
  std::unique_ptr<wchar_t[]> p(new wchar_t[len]);
  mbstowcs(p.get(), str.c_str(), len);
  std::wstring w_str(p.get());
  return w_str;
}

std::string common::ws2s(const std::wstring& w_str) {
    if (w_str.empty()) {
      return "";
    }
    unsigned len = w_str.size() * 4 + 1;
    setlocale(LC_CTYPE, "en_US.UTF-8");
    std::unique_ptr<char[]> p(new char[len]);
    wcstombs(p.get(), w_str.c_str(), len);
    std::string str(p.get());
    return str;
}

string common::charToHexString(const char* aInStr,int len)
{
    const char HEX[] = "0123456789ABCDEF";

    std::string lStr;
    for(int i=0; i < len; i++)
    {
        char lTemp = aInStr[i];
        lStr.push_back(HEX[lTemp/16]);
        lStr.push_back(HEX[lTemp%16]);
    }
    return lStr;
}
