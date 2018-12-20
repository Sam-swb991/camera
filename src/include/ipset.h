#ifndef IPSET_H
#define IPSET_H
#include <string>

namespace  ipset{


    int setip(const char *ip);
    char * getip(char *buf);
    int sed(std::string origin_ip,std::string new_ip);


};

#endif // IPSET_H
