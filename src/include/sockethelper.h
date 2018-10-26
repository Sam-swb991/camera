#ifndef SOCKETHELPER_H
#define SOCKETHELPER_H
#include <netinet/in.h>

class socketHelper
{
public:
    socketHelper();
    int SocketServerBuilder(int clientNum, int port);
    int ServerGetClient(int Serverfd);
    int SocketClientBuilder(char *addr, int port);
    void SocketSend(int fd,unsigned char *,int);
private:
    struct sockaddr_in my_addr,remote_addr;
    bool server,client;
};

#endif // SOCKETHELPER_H
