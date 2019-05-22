#include "sockethelper.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
/**
 * @brief 构造函数，初始化
 */
socketHelper::socketHelper()
{
    client = false;
    server = false;
}
/**
 * @brief 创建socket服务端
 * @param clientNum，监听的客户端最大数
 * @param port，服务端端口
 * @return 创建成功返回服务端套接字serverfd，失败返回-1
 */
int socketHelper::SocketServerBuilder(int clientNum,int port)
{
    if(client)
    {
        printf("socket is client!\n");
    }
    int serverfd;
    int flag = 1;
    int len = sizeof (int);
    memset(&my_addr,0,sizeof(my_addr));
    my_addr.sin_family=AF_INET;
    my_addr.sin_addr.s_addr=INADDR_ANY;
    my_addr.sin_port=htons(static_cast<unsigned short>(port));
    if((serverfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket");
        return -1;
    }
    /**
      * @brief 设置地址端口复用
      */
    if( setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &flag, (unsigned int)len) == -1)
    {
        perror("setsocketopt");
        return -1;
    }
    /**
      * @brief 绑定地址和端口
      */
    if (bind(serverfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))<0)
    {
        perror("bind");
        return -1;
    }
    /**
     * @brief 设置监听最大客户端
     */
    listen(serverfd,clientNum);
    server = true;
    return serverfd;
}
/**
 * @brief 获取连接到服务端的客户端的套接字
 * @param fd，服务端的套接字
 * @return 返回客户端的套接字
 */
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
/**
 * @brief 创建socket客户端
 * @param addr，连接的服务端地址
 * @param port，连接的服务端端口
 * @return 成功返回客户端套接字，不成功返回-1
 */
int socketHelper::SocketClientBuilder(const char *addr,int port)
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
        close(clientfd);
        return -1;
    }
    client = true;
    return clientfd;
}
/**
 * @brief socket发送数据
 * @param fd，套接字
 * @param msg，发送的字符串
 * @param msgNum，发送的字符串长度
 */
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


