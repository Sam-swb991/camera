#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H
#include "sockethelper.h"
#include "jsonhelper.h"
#include <pthread.h>
#include "sharedspace.h"
#include "calc.h"
class socketclient
{
public:
    socketclient(sharedspace *ss);
    int connect(const char *addr,int port);
    void startclient();
    bool isStart();
private:
    static void *clientthread(void *);
    static int clientfd;
    static bool start;
    socketHelper *client;
    jsonhelper *json;
    static sharedspace *ss;
    static float **temp;
    static void getTemp();
    static calc *c;
};

#endif // SOCKETCLIENT_H
