#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H
#include <sys/epoll.h>
#include "sockethelper.h"
#include <pthread.h>
#include <netinet/in.h>
#include "sharedspace.h"

#define MAXLINE 4096

class socketServer
{
public:
    socketServer(int port, sharedspace *ss);
    void startServer();

private:
    int setnonblocking(int fd);
    static void * serverthread(void *);
    static int serverfd ,epfd;

    static bool closeThread;
    socketHelper *server;
    struct epoll_event ev;
    static sharedspace *ss;
};


#endif // SOCKETSERVER_H
