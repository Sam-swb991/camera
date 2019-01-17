#include "recovery.h"
#include <cstring>
#include <string>
#include <iostream>
#include <stdlib.h>
recovery::recovery(sharedspace *ss)
{
    this->ss =ss;
    recover_sql();
    int ret_cp = cp_default_file("/mnt/default/network.sh.default","/mnt/config/network.sh");
    if(ret_cp==-1||ret_cp == 127)
    {
        ret = -1;
    }
    else
    {
        ret_cp = cp_default_file("/mnt/default/config.xml.default","/mnt/config/config.xml");
        if(ret_cp==-1||ret_cp == 127)
        {
            ret = -1;
        }
        else
        {
            ret_cp = cp_default_file("/mnt/default/param.ini.default","/mnt/config/param.ini");
            if(ret_cp==-1||ret_cp == 127)
            {
                ret = -1;
            }
            else
            {
                ret_cp = cp_default_file("/mnt/default/alarmio.dat.default","/mnt/config/alarmio.dat");

                if(ret_cp==-1||ret_cp == 127)
                {
                    ret = -1;
                }
                else
                    ret = 0;
            }
        }
     }
}


int recovery::cp_default_file(const char * src,const char* dst)
{
    char cpbuf[100] = {0};
    std::string cp_head = "cp -rf ";
    memcpy(cpbuf,cp_head.c_str(),cp_head.size());
    strcat(cpbuf,src);
    strcat(cpbuf," ");
    strcat(cpbuf,dst);
    std::cout<<"cp:"<<cpbuf<<std::endl;
    int ret = system(cpbuf);
    return  ret;
}

void recovery::recover_sql()
{
    ss->sql->recovery(false);
}

int recovery::getret()
{
    return  ret;
}
