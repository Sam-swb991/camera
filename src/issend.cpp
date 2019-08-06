#include "issend.h"
#include <iostream>
using namespace std;
issend::issend(long time)
{
    this->last.time = time;
    this->last.ack = false;
    this->last.ctrl = false;
    this->last.alarmmode = 0;
}

bool issend::send(ISSEND now)
{
    bool ret;
    cout<<"alarmmode :"<<now.alarmmode<<endl;
    if(now.alarmmode!=0)
        if(now.alarmmode!=last.alarmmode)
            ret = true;
        else
            ret = false;
    else
        ret = false;
    cout<<"udp alarmmode: "<<ret<<endl;
    ret = ret||(now.time-last.time>deltatime);
    cout<<"udp time: "<<ret<<endl;
    if(now.ack)
        ret = true;
    cout<<"udp ack: "<<ret<<endl;
    if(now.ctrl)
        ret = false;
    cout<<"udp ctrl: "<<ret<<endl;
    last.ack = now.ack;
    last.alarmmode = now.alarmmode;
    last.ctrl = now.ctrl;
    if(ret == true)
        last.time = now.time;
    cout<<"udp issend: "<<ret<<endl;
    return ret;
}
