#include "sockethelper.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <cstring>
#include <stdio.h>
socketHelper::socketHelper()
{
    client = false;
    server = false;
}

int socketHelper::SocketServerBuilder(int clientNum,int port)
{
    if(client)
    {
        printf("socket is client!\n");
    }
    int serverfd;
    memset(&my_addr,0,sizeof(my_addr));
    my_addr.sin_family=AF_INET;
    my_addr.sin_addr.s_addr=INADDR_ANY;
    my_addr.sin_port=htons(static_cast<unsigned short>(port));
    if((serverfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket");
        return -1;
    }
    if (bind(serverfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))<0)
    {
        perror("bind");
        return -1;
    }
    listen(serverfd,clientNum);
    server = true;
    return serverfd;
}

int socketHelper::ServerGetClient(int fd)
{
    if(client)
    {
        printf("socket is client!\n");
    }
    int clientfd;
    unsigned int num;
    if((clientfd=accept(fd,(struct sockaddr *)&remote_addr,&num))<0)
    {
        perror("accept client_sockfd failed");
        return -1;
    }
    return clientfd;


}

int socketHelper::SocketClientBuilder(char *addr,int port)
{
    if(server)
    {
        printf("socket is server!\n");
        return -1;
    }
    int clientfd;
    memset(&remote_addr,0,sizeof(remote_addr));
    remote_addr.sin_family=AF_INET;
    remote_addr.sin_addr.s_addr=inet_addr(addr);
    remote_addr.sin_port=htons(static_cast<unsigned short>(port));
    if((clientfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("client socket creation failed");
        return -1;
    }
    if(connect(clientfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)
    {
        perror("connect to server failed");
        return -1;
    }
    client = true;
    return clientfd;
}

void socketHelper::SocketSend(int fd,unsigned char *msg,int msgNum)
{
    if(msg ==nullptr&& msg[0] == '\0')
    {
        printf("send NULL\n");
        return;
    }
    long number = send(fd,msg,static_cast<unsigned long>(msgNum),0);
    if(number != msgNum)
    {
        printf("send number is not require number\n");
    }
    return;
}

