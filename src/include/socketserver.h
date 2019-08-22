#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H
#include <sys/epoll.h>
#include "sockethelper.h"
#include <pthread.h>
#include <netinet/in.h>
#include "sharedspace.h"
#include "udpserver.h"
#define MAXLINE 4096

class socketServer
{
public:
    socketServer(int port, sharedspace *ss);
    ~socketServer();
    pthread_t startServer();

private:
    static int setnonblocking(int fd);
    static void * serverthread(void *);
    static int serverfd ,epfd,udpfd;
    static udpserver * udp;
    static bool closeThread;
    socketHelper *server;
    struct epoll_event ev;
    static sharedspace *ss;
};


#endif // SOCKETSERVER_H
