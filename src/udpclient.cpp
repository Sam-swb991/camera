#include "udpclient.h"
#include "myprotocol.h"
#include "jsoncpp.h"
udpclient::udpclient(sharedspace *ss)
{
    this->ss = ss;
    if((socketfd = socket(PF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("socket");
    }
    printf("socketfd = %d\n",socketfd);
    int i=1;
    socklen_t len = sizeof(i);

    setsockopt(socketfd,SOL_SOCKET,SO_BROADCAST,&i,len);

    string ip = ss->getbroadcast();
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    server_addr.sin_port = htons(11072);
    addr_len=sizeof(server_addr);
}

int udpclient::sendmsg(unsigned char * buf,unsigned long len)
{
    ssize_t ret =  sendto(socketfd,buf,len,0,(struct sockaddr*)&server_addr,addr_len);
    if( ret < 0)
    {
        perror("sendrto");
    }
    return (int)ret;
}

int udpclient::udpsend(HTTPURL *url,string preHash)
{
    jsoncpp * json  = new jsoncpp();
    json->create_udp_data(url,preHash);
    myProtocol * pro = new myProtocol(0x02,0x03,json->getJsonString());
    int ret = sendmsg(pro->GetData(),pro->Getlength());
    return ret;
}
