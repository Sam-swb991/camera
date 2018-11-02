#include "socketclient.h"
#include "myprotocol.h"
#include "calc.h"
#include <cstring>
#include "common.h"
#include <ctime>

int socketclient::clientfd = -1;
bool socketclient::start = true;
int ** socketclient::temp = nullptr;
sharedspace * socketclient::ss = nullptr;
calc *socketclient::c = nullptr;
socketclient::socketclient(sharedspace *ss)
{
    client = new socketHelper();
    this->ss = ss;
    temp = new int*[64];
    for(int i=0; i<64; ++i)
    {
        temp[i] = new int[80];

        memset(temp[i],0,80*sizeof(int));
    }
    c= new calc();

}
int socketclient::connect(char *addr, int port)
{
    clientfd = client->SocketClientBuilder(addr,port);
    if(clientfd<0)
    {
        start = false;
    }
    return clientfd;
}
void *socketclient::clientthread(void *)
{
    WINDOW window;
    POINT point[5120];
    RECT *rect;
    int len;
    CJsonObject jsonobject;
//    clock_t start_t,end_t;
//    int times = 0;
    while(start)
    {
//        start_t =clock();
        memset(&window,0,sizeof (WINDOW));
        usleep(200000);
        getTemp();
//        end_t =clock();
//        cout<<"time 1:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
//        for(int i =0;i<64;++i)
//        {
//            for(int j=0;j<80;++j)
//            {
//                printf("%d ",temp[i][j]);
//            }
//            printf("\n");
//        }
        pthread_mutex_lock(&ss->mutexsql);
        usleep(200000);
        ss->storeTemp(temp);
        pthread_mutex_unlock(&ss->mutexsql);
//        end_t =clock();

//        cout<<"time 2:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
        usleep(200000);
        pthread_mutex_lock(&ss->mutex);
        rect = ss->GetRect(temp);
        len = ss->getRectlen();
        pthread_mutex_unlock(&ss->mutex);
//        end_t =clock();

//        cout<<"time 3:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
//        for(int i =0;i<64;i++)
//        {
//            for(int j=0;j<80;j++)
//            {
//                point[i*80+j].value = temp[i][j];
//                point[i*80+j].isShow = 1;
//            }
//        }
//        end_t =clock();
//        cout<<"time 4:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
        usleep(200000);
        jsonhelper *json =new jsonhelper();
        json->create_temp(window,rect,len,point);
        jsonobject = json->getJson();
//        end_t =clock();
 //       cout<<"time 5:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
        cout<<jsonobject.ToFormattedString()<<endl;
        usleep(200000);
        myProtocol *pro = new myProtocol(0x01,0x03,jsonobject);
        cout<<"data lenth:"<<pro->Getlength()<<endl;
//        end_t =clock();
//        cout<<"time 6:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
        //send(clientfd,pro->GetData(),static_cast<unsigned long>(pro->Getlength()),0);
        delete json;
        delete pro;
        delete rect;
//        if(times++ == 60)
//        {
//            times = 0;
//            ss->resetSql();
//        }


    }
    cout<<"client end"<<endl;
    return nullptr;
}

void socketclient::startclient()
{
    pthread_t id;
    int ret = pthread_create(&id,nullptr,clientthread,nullptr);

    if(ret) {
        std::cout << "Create client pthread error!" << std::endl;

    }
}

void socketclient::getTemp()
{

    c->get_all_temp(temp);

//    for(int i = 0;i<64;i++)
//    {
//        for(int j = 0;j<80;j++)
//        {
//            cout<<temp[i][j]<<" ";
//        }
//        cout<<endl;
//    }
//    cout<<"print end"<<endl;
}
