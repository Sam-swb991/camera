#include "rs485.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <string.h>
#include "spi.h"
#include "common.h"
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
    else
    {
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
    }
    return fd;
}

int Rs485::control(int mode, int *angle)
{
    int ret=0;
    switch (mode) {
    case YUNTAI_STOP:{
        unsigned char buf[7] ={0xff,0x01,0x00,0x00,0x00,0x00,0x01};
        prepare_write_rs485();
        ssize_t res = write(fd,buf,7);
        if(res == 7)
            ret += 0;
        else
            ret += 1;
        usleep(50);
    }break;
    case YUNTAI_LEFT:{
        cout<<"yuntai contrl left"<<endl;
        prepare_write_rs485();
        int temp = control(YUNTAI_STOP,nullptr);
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
        cout<<"yuntai contrl right"<<endl;
        prepare_write_rs485();
        int temp = control(YUNTAI_STOP,nullptr);
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
        int ret = pthread_create(&id,nullptr,yuntaiautothread,(void *)angle);
        if(ret) {
            std::cout << "Create yuntai auto pthread error!" << std::endl;
        }
    }break;
    default:break;
    }
    return ret;
}

void * Rs485::yuntaiautothread(void *angle)
{
    cout<<"yuntai auto start!  "<<ss->yuntai_auto<<endl;
    int yuntaiangle = *(int *)angle;
    if(yuntaiangle == 0)
    {
        yuntaiangle = 2;
    }
    unsigned int yuntai_move_time;
    int yuntai_move_times;
    if(TABLENUMBER == 122)
    {
        yuntai_move_time = 3;
        yuntai_move_times = 4*yuntaiangle;
    }
    else if(TABLENUMBER == 124)
    {
        yuntai_move_time = 5;
        switch (yuntaiangle) {
        case 1:{
            yuntai_move_times = 3;
        };break;
        case 2:{
            yuntai_move_times =5;
        };break;
        case 3:{
            yuntai_move_times = 8;
        };break;
        case 4:{
            yuntai_move_times = 11;
        };break;
        case 5:{
            yuntai_move_times = 13;
        };break;
        case 6:{
            yuntai_move_times = 16;
        };break;
        default:{
            yuntai_move_times = 5;
        }break;
        }

    }
    while(ss->yuntai_auto)
    {
        for(int i =0;i<yuntai_move_times;++i)
        {
            if(ss->yuntai_auto == false)
                break;
            int temp = control(YUNTAI_LEFT,nullptr);
            if(temp>0)
                printf("at least one command send error!");
            usleep(yuntai_move_time*1000000);
            control(YUNTAI_STOP,nullptr);
            if(ss->yuntai_auto == false)
                break;
            usleep(yuntai_stop_time*1000000);
        }
        for(int i =0;i<yuntai_move_times;++i)
        {
            if(ss->yuntai_auto == false)
                break;
            int temp = control(YUNTAI_RIGHT,nullptr);
            if(temp>0)
                printf("at least one command send error!");
            usleep(yuntai_move_time*1000000);
            control(YUNTAI_STOP,nullptr);
            if(ss->yuntai_auto == false)
                break;
            usleep(yuntai_stop_time*1000000);
        }
    }
    return nullptr;
}

void Rs485::close_485()
{
    close(fd);
}
