#ifndef UDPCLIENTTHREAD_H
#define UDPCLIENTTHREAD_H

#include "udpclient.h"
#include "sharedspace.h"
class udpclientthread
{
public:
    udpclientthread(sharedspace *ss);
    void start();
private:
    static udpclient * udpc;
    static sharedspace *ss;
    static void * thread(void *);
};

#endif // UDPCLIENTTHREAD_H
