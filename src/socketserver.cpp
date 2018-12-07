#include "socketserver.h"
#include <stdio.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include "common.h"
#include "myprotocol.h"
#include "transport.h"
#include <signal.h>
/**
 * @brief 初始化静态数据
 */
sharedspace *socketServer::ss = nullptr;
int socketServer::serverfd = -1,socketServer::epfd=-1;
bool socketServer::closeThread = false;
/**
 * @brief 构造函数，创建socket服务端，epoll模型初始化
 * @param port，服务端端口
 * @param ss，sharedspace对象
 */
socketServer::socketServer(int port,sharedspace *ss)
{
    server = new socketHelper();
    serverfd = server->SocketServerBuilder(10,port);
    epfd = epoll_create(256);
    if(epfd < 0)
    {
        perror("epoll_create");
        return;
    }
    ev.data.fd = serverfd;
    ev.events = EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,serverfd,&ev);
    this->ss = ss;

}
/**
 * @brief 析构函数，关闭套接字，
 *        关闭epoll文件描述符，关闭线程
 */
socketServer::~socketServer()
{
    close(serverfd);
    close(epfd);
    closeThread = false;

}
/**
 * @brief 设置非阻塞模式
 * @param fd，文件描述符
 * @return 成功返回0,不成功返回-1
 */
int socketServer::setnonblocking(int fd)
{
    int opts;
    opts=fcntl(fd,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        return -1;
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(fd,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        return -1;
    }
    return 0;
}
/**
 * @brief socket服务端线程
 * @return 返回固定为null
 */
void * socketServer::serverthread(void *)
{
    int nfds,n;
    struct epoll_event ev,events[20];
    struct sockaddr_in peer;
    socklen_t socketlen = sizeof(peer);
    int rsock;
    unsigned long len;
    int rectlen;
    unsigned char sync,platform;
    bool check = true;
    int mode =-1;
    for(;;)
    {
        if(closeThread)
            break;

        memset(&peer,0,sizeof(peer));
        nfds=epoll_wait(epfd,events,20,1);
        //cout<<"waiting :"<<nfds<<endl;
        for(int i=0; i < nfds; ++i)
        {
            cout<<"fd is :"<<events[i].data.fd<<endl;
            if(events[i].data.fd == serverfd)
            {
                int clientfd = accept(serverfd,(struct sockaddr*)&peer,&socketlen);
                if(clientfd > 0)
                {
                    printf("get a new client:%s:%d\n",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port));
                    ev.data.fd = clientfd;
                    ev.events = EPOLLIN|EPOLLET;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&ev);
                }
            }
            else if(events[i].events&EPOLLIN)
            {
                char line[MAXLINE];
                memset(line,0,MAXLINE);
                rsock = events[i].data.fd;
                if ( rsock < 0)
                    continue;
                if ( (n = recv(rsock, line, sizeof(line), 0)) < 0)
                {
                    
                    if (errno == ECONNRESET)
                    {
                        close(rsock);
                        events[i].data.fd = -1;
                    }
                    else
                        std::cout<<"readline error"<<std::endl;
                }
                else if (n == 0) 
                {
                    std::cout<<"close socket!"<<std::endl;
                    close(rsock);
                    events[i].data.fd = -1;
                }
                else
                {
                    //**************************
                    //line to use
                    cout<<"recv data is:"<<endl;
                    common::print_V(line,n,1);
                    cout<<endl;
                    myProtocol *pro = new myProtocol(line);
                    check = pro->getCheck();
                    if(check)
                    {
                        cout<<"check"<<endl;
                        sync = pro->GetSync();
                        platform = pro->GetPlatform();
                        std::cout<<"sync :";
                        common::print_V(&sync,1,1);
                        std::cout<<" platform :";
                        common::print_V(&platform,1,1);
                        std::cout<<std::endl;
                        jsoncpp *json = pro->GetJson();
                        std::cout<<json->toStyledString()<<std::endl;
                        RECTSET *rect = json->getRectset(&rectlen);
                        mode = json->getMode();
                        cout<<"mode is "<<mode<<endl;
                        pthread_mutex_lock(&ss->mutex);
                        ss->SetRect(rect,rectlen,mode);
                        pthread_mutex_unlock(&ss->mutex);
                        //****************************                       
                       // delete json;

                    }

                    ev.data.fd=rsock;
                    ev.events=EPOLLOUT|EPOLLET;
                    epoll_ctl(epfd,EPOLL_CTL_MOD,rsock,&ev);
                   // delete pro;
               //     cout<<"2"<<endl;
                }
            }
            else if(events[i].events&EPOLLOUT)
            {
                cout<<"send start"<<endl;
                unsigned char *sendmsg;
				int rectsetlen;
                jsoncpp *json =new jsoncpp();
                if(check)
                {
                	if(mode == GET)
                	{
                        pthread_mutex_lock(&ss->mutexsql);
                        RECTSET * rectset = ss->sql->getRect(&rectsetlen,true);
                        pthread_mutex_unlock(&ss->mutexsql);
                        json->create_rect(rectset,rectsetlen);
                        delete rectset;

					}
					else
                        json->create_code(100);
                }
                else
                    json->create_code(110);
                //std::cout<<json.ToFormattedString()<<std::endl<<json.ToString().size()<<std::endl;;
                myProtocol *pro = new myProtocol(0x02,0x01,json->toStyledString());
                sendmsg = pro->GetData();
                len = pro->Getlength();
                cout<<"send data is:"<<endl;
                common::print_V(sendmsg,(int)len,1);
                std::cout<<std::endl;
                rsock = events[i].data.fd;
                setnonblocking(rsock);
                ssize_t tmp = send(rsock, sendmsg, len, 0);
                if(tmp <0)
                {
                    cout<<"server send error!"<<endl;
                }
                ev.data.fd=rsock;
                ev.events = EPOLLIN|EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_MOD,rsock,&ev);
                delete sendmsg;
                delete json;
               // delete pro;


            }
        }
    }
    return nullptr;
}
/**
 * @brief 创建服务端线程
 * @return 返回线程id
 */
pthread_t socketServer::startServer()
{
    pthread_t id;
    int ret = pthread_create(&id,nullptr,serverthread,nullptr);

    if(ret) {
        std::cout << "Create server pthread error!" << std::endl;

    }
    return id;
}
