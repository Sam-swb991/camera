#include "ipset.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <string>
int ipset::setip(const char *ip)
{
    struct ifreq temp;
    struct sockaddr_in *addr;
    int fd = 0;
    int ret = -1;
    strcpy(temp.ifr_name, "eth0");
    if((fd=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        return -1;
    }
    addr = (struct sockaddr_in *)&(temp.ifr_addr);
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(ip);
    ret = ioctl(fd, SIOCSIFADDR, &temp);
    close(fd);
    if(ret < 0)
        return -1;
    return 0;
}

char *ipset::getip(char *ip_buf)
{
    struct ifreq temp;
    struct sockaddr_in *myaddr;
    int fd = 0;
    int ret = -1;
    strcpy(temp.ifr_name, "eth0");
    if((fd=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        return nullptr;
    }
    ret = ioctl(fd, SIOCGIFADDR, &temp);
    close(fd);
    if(ret < 0)
        return nullptr;
    myaddr = (struct sockaddr_in *)&(temp.ifr_addr);
    strcpy(ip_buf, inet_ntoa(myaddr->sin_addr));
    return ip_buf;
}

int ipset::sed(std::string origin_ip, std::string new_ip)
{

    std::string str = "sed -i 's/"+origin_ip+"/"+new_ip+"/g' /etc/init.d/rcS";
    int ret = system(str.c_str());
    return ret;
}
