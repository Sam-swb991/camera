#include "common.h"
#include <stdio.h>
#include <cmath>
#include "temprule.h"
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

