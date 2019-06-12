#ifndef RS485_H
#define RS485_H

#include "sharedspace.h"
class Rs485
{
public:
    enum mode{YUNTAI_LEFT,YUNTAI_RIGHT,YUNTAI_STOP,YUNTAI_AUTO};
    Rs485(sharedspace *ss);
    static int control(int mode);
    int open_485(const char * );
private:
    static int fd;
    static const int yuntai_auto_move_times = 7;
    static const int yuntai_move_time = 3;
    static const int yuntai_stop_time = 10;
    static sharedspace *ss;
    static void * yuntaiautothread(void *);
};

#endif // RS485_H
