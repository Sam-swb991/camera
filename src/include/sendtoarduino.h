#ifndef SENDTOARDUINO_H
#define SENDTOARDUINO_H
#include "sharedspace.h"
#include <pthread.h>
#include "transport.h"
#include "sockethelper.h"
class sendtoarduino
{
public:
    sendtoarduino(sharedspace *ss);
    bool connect(const char * ip,int port);
    pthread_t start();
    bool isstart;
private:
    static sharedspace *ss;
    static string cat_json(HTTPURL*);
    static int fd;
    static void *thread(void *);
    socketHelper * socket;
};

#endif // SENDTOARDUINO_H
