#include "sendtoarduino.h"
#include "common.h"
#include "myprotocol.h"
int sendtoarduino::fd = -1;
sharedspace * sendtoarduino::ss =nullptr;
sendtoarduino::sendtoarduino(sharedspace *ss)
{
    this->ss =ss;
    socket = new socketHelper();
    this->isstart = false;


}

bool sendtoarduino::connect(const char *ip, int port)
{
    if(fd>0)
        return true;
    fd = socket->SocketClientBuilder(ip,port);
    if(fd<0)
        return false;
    else
        return true;
}
string sendtoarduino::cat_json(HTTPURL *url)
{
    string json;
    json.clear();
    json = "{\"function\":\"alarm\",\"body\":{\"ip\":"+url->ip+",\"alarmmode\":"
            +common::to_string(url->alarmmode)+",\"sn\":"+url->camera_id+",\"time\":"+url->time+"}}";
    cout<<"json:"<<json<<endl;
    return json;
}
void * sendtoarduino::thread(void *)
{
    while(true)
    {

        if(ss->needsendtoarduino)
        {
            cout<<"send to arduino"<<endl;
            //pthread_mutex_unlock(&ss->mutexsendalarm);
            pthread_mutex_lock(&ss->mutexurl);
            string sendjson =cat_json(ss->url);
            pthread_mutex_unlock(&ss->mutexurl);
            myProtocol *pro = new myProtocol(0x01,0x04,sendjson);
            long len = send(fd,pro->GetData(),pro->Getlength(),0);
            if(len<0)
            {
                delete pro;
                cout<<"send to arduino error!"<<endl;
                break;
            }
            ss->needsendtoarduino = false;
        }
        else
            sleep(1);

    }
    close(fd);
    return nullptr;
}

pthread_t sendtoarduino::start()
{
    pthread_t id;
    int ret = pthread_create(&id,nullptr,thread,nullptr);

    if(ret) {
        std::cout << "Create pthread error!" << std::endl;

    }
    else
    {
        cout<<"start success!!"<<endl;
        isstart = true;
    }
    return id;
}
