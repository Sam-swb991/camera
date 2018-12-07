#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "sharedspace.h"
class serialPort
{
public:
    serialPort(sharedspace *ss);
    void startRead();
private:
    static int fd;
    static void *readthread(void *);
    static sharedspace *ss;
};

#endif // SERIALPORT_H
