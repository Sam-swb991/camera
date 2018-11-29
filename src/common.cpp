#include "common.h"
#include <stdio.h>
#include <cmath>
#include "temprule.h"
#include <sstream>
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
