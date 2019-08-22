#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H
#include "sockethelper.h"

#include <pthread.h>
#include "sharedspace.h"
#include "calc.h"
class socketclient
{
public:
    socketclient(sharedspace *ss);
    int connect(const char *addr,int port);
    pthread_t startclient();
    bool isStart();
private:
    static void *clientthread(void *);
    static int clientfd;
    static bool start;
    socketHelper *client;
    static sharedspace *ss;
    static float **temp;
    static void getTemp();
    static calc *c;
};

#endif // SOCKETCLIENT_H
