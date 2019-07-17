#include "udpserver.h"
#include <stdio.h>

#include <cstring>
udpserver::udpserver(sharedspace *ss)
{
    if((socketfd = socket(PF_INET,SOCK_DGRAM,0)) < 0)
    {
        printf("udp socket error!\n");
    }

    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    string ip = ss->getbroadcast();
    cout<<"broadcast ip:"<<ip<<endl;
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    server_addr.sin_port = htons(11072);
    addr_len=sizeof(server_addr);
    if(bind(socketfd,(struct sockaddr*)&server_addr,addr_len) < 0)
    {
        printf("udp bind error!\n");
    }
}

ssize_t udpserver::udprecv(unsigned char *buf,int len)
{
    ssize_t recv_len = recvfrom(socketfd,buf,(ssize_t)len,0,(struct sockaddr*)&server_addr,&addr_len);
    printf("recv udp data from %s\n",inet_ntoa(server_addr.sin_addr));
    return recv_len;
}

int udpserver::getudpfd()
{
    return socketfd;
}
