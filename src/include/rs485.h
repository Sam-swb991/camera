#ifndef RS485_H
#define RS485_H

#include "sharedspace.h"


class Rs485
{
public:
    enum mode{YUNTAI_LEFT,YUNTAI_RIGHT,YUNTAI_STOP,YUNTAI_AUTO,YUNTAI_AUTO_STOP,YUNTAI_MOVE_PRESET};
    Rs485(sharedspace *ss);
    static int control(int mode,int * angle);
    int open_485(const char * );
    void close_485();
private:
    static int fd;
    static const int yuntai_stop_time = 10;
    static sharedspace *ss;
    static void * yuntaiautothread(void *);
};

#endif // RS485_H
