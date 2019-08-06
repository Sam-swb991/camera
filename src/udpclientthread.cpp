#include "udpclientthread.h"
sharedspace * udpclientthread::ss = nullptr;
udpclient * udpclientthread::udpc = nullptr;
udpclientthread::udpclientthread(sharedspace *ss)
{
    this->ss = ss;
    udpc = new udpclient(ss);
}


void udpclientthread::start()
{
    pthread_t id;
    int ret = pthread_create(&id,nullptr,thread,nullptr);

    if(ret) {
        std::cout << "Create udp client pthread error!" << std::endl;

    }
    return;
}

void * udpclientthread::thread(void *)
{
    while(1)
    {
        cout<<"wait udp send!"<<endl;
        sem_wait(&ss->mutex_udp);
        if(ss->udpsend->send(ss->isudpsend))
            udpc->udpsend(ss->url,ss->preHash);
    }
}
