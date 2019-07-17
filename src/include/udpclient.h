#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#include <sys/socket.h>
#include "sharedspace.h"
#include <arpa/inet.h>
#include "blockchaindata.h"
class udpclient
{
public:
    udpclient(sharedspace *ss);
    int udpsend(HTTPURL *url, string preHash);
private:
    int sendmsg(unsigned char *buf, unsigned long len);
    int socketfd;
    socklen_t addr_len;
    struct sockaddr_in server_addr;
    sharedspace *ss;
};

#endif // UDPCLIENT_H
