#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <sys/socket.h>
#include "sharedspace.h"
#include <arpa/inet.h>
class udpserver
{
public:
    udpserver(sharedspace *ss);
    int getudpfd();
    ssize_t udprecv(unsigned char *, int len);
private:
    int socketfd;
    socklen_t addr_len;
    struct sockaddr_in server_addr;
};

#endif // UDPSERVER_H
