#include "rs485.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <string.h>
sharedspace* Rs485::ss = nullptr;
int Rs485::fd = -1;
Rs485::Rs485(sharedspace *ss)
{
    this->ss = ss;
}
int Rs485::open_485(const char * addr)
{
    fd = open(addr,O_RDWR | O_NOCTTY);
    if(fd <0)
    {
        printf("cannot open %s\r\n",addr);
    }
    struct termios oldstdio;
    tcgetattr( fd, &oldstdio);
    if (cfsetispeed(&oldstdio, B2400) == 0)
    {
        printf("success\n");
    }
    else
    {
        printf("failed\n");
    }

    if(cfsetospeed(&oldstdio, B2400)==0)
    {
        printf("success\n");
    }
    else
    {
        printf("failed\n");
    }
    tcsetattr( fd, TCSANOW, &oldstdio);
    return fd;
}

int Rs485::control(int mode)
{
    int ret=0;
    switch (mode) {
    case YUNTAI_STOP:{
        unsigned char buf[7] ={0xff,0x01,0x00,0x00,0x00,0x00,0x01};
        ssize_t res = write(fd,buf,7);
        if(res == 7)
            ret += 0;
        else
            ret += 1;
        usleep(50);
    }break;
    case YUNTAI_LEFT:{
        int temp = control(YUNTAI_STOP);
        if(temp==0)
            ret += 1;
        unsigned char buf[7] ={0xff,0x01,0x00,0x02,0x20,0x00,0x23};
        ssize_t res = write(fd,buf,7);
        if(res == 7)
            ret += 0;
        else
            ret += 1;
    }break;
    case YUNTAI_RIGHT:{
        int temp = control(YUNTAI_STOP);
        if(temp==0)
            ret +=1;
        unsigned char buf[7] ={0xff,0x01,0x00,0x04,0x20,0x00,0x25};
        ssize_t res = write(fd,buf,7);
        if(res == 7)
            ret += 0;
        else
            ret += 1;
    }break;
    case YUNTAI_AUTO:{
        pthread_t id;
        int ret = pthread_create(&id,nullptr,yuntaiautothread,nullptr);
        if(ret) {
            std::cout << "Create yuntai auto pthread error!" << std::endl;
        }
    }break;
    default:break;
    }
    return ret;
}

void * Rs485::yuntaiautothread(void *)
{
    cout<<"yuntai auto start!"<<endl;
    while(ss->yuntai_auto)
    {
        for(int i =0;i<yuntai_auto_move_times;++i)
        {
            if(ss->yuntai_auto == false)
                break;
            int temp = control(YUNTAI_LEFT);
            if(temp>0)
                printf("at least one command send error!");
            usleep(yuntai_move_time*1000000);
            control(YUNTAI_STOP);
            if(ss->yuntai_auto == false)
                break;
            usleep(yuntai_stop_time*1000000);
        }
        for(int i =0;i<yuntai_auto_move_times;++i)
        {
            if(ss->yuntai_auto == false)
                break;
            int temp = control(YUNTAI_RIGHT);
            if(temp>0)
                printf("at least one command send error!");
            usleep(yuntai_move_time*1000000);
            control(YUNTAI_STOP);
            if(ss->yuntai_auto == false)
                break;
            usleep(yuntai_stop_time*1000000);
        }
    }
    return nullptr;
}
