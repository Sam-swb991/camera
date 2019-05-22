#include "sendtoarduino.h"
#include "common.h"
#include "myprotocol.h"
int sendtoarduino::fd = -1;
sharedspace * sendtoarduino::ss =nullptr;
int sendtoarduino::sendnum = 0;
sendtoarduino::sendtoarduino(sharedspace *ss)
{
    this->ss =ss;
    socket = new socketHelper();
    this->isstart = false;
}

int sendtoarduino::connect(const char *ip, int port)
{
    if(fd>0)
        return -2;
    cout<<"arduino IP:"<<ip<<endl;
    fd = socket->SocketClientBuilder(ip,port);
    if(fd<0)
        return -1;
    else
        return 0;
}
string sendtoarduino::cat_json(HTTPURL *url)
{
    string json;
    json.clear();
    json = "{\"function\":\"alarm\",\"body\":{\"ip\":"+url->ip+",\"alarmmode\":"
            +common::to_string(url->alarmmode)+",\"sn\":"+url->camera_id+",\"time\":\""+url->time+"\"}}";
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
            pthread_mutex_lock(&ss->mutexarduinoUrl);
            string sendjson =cat_json(ss->arduinoUrl);
            pthread_mutex_unlock(&ss->mutexarduinoUrl);
            myProtocol *pro = new myProtocol(0x01,0x04,sendjson);
            long len = send(fd,pro->GetData(),pro->Getlength(),0);
            if(len<0)
            {
                delete pro;
                cout<<"send to arduino error!"<<endl;
                break;
            }
            else
            {
                cout<<"============send to arduino success!!==========="<<endl;
            }
            if(ss->arduinoUrl->alarmmode ==0)
            {
                if(sendnum >0)
                {
                    cout<<"++++++++++++++++++++++"<<ss->arduinoUrl->alarmmode<<"++++++++++++++"<<endl;
                    cout<<"++++++++++++++++++++++"<<ss->arduinoUrl->alarmmode<<"++++++++++++++"<<endl;
                    ss->needsendtoarduino = false;
                    sendnum =0;
                }
                else
                    sendnum++;
            }

        }
        sleep(1);

    }
    close(fd);
    fd = -1;
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
