#include "socketclient.h"
#include "myprotocol.h"
#include "calc.h"
#include <cstring>
#include "common.h"
#include <ctime>
#include "jsoncpp.h"
int socketclient::clientfd = -1;
bool socketclient::start = true;
float ** socketclient::temp = nullptr;
sharedspace * socketclient::ss = nullptr;
calc *socketclient::c = nullptr;
socketclient::socketclient(sharedspace *ss)
{
    client = new socketHelper();
    this->ss = ss;
    temp = new float*[64];
    for(int i=0; i<64; ++i)
    {
        temp[i] = new float[80];

        memset(temp[i],0,80*sizeof(int));
    }
    c= new calc();

}
int socketclient::connect(const char *addr, int port)
{
    clientfd = client->SocketClientBuilder(addr,port);
    if(clientfd<0)
    {
        start = false;
    }
    else
        start = true;
    return clientfd;
}
void *socketclient::clientthread(void *)
{
    WINDOW window;
    window.x1 = 0.08f;
    window.y1 = -0.16f;
    window.x2 = 0.88f;
    window.y2 = 0.98f;
    //    POINT point[5120];
    RECT *rect;
    int len;
    CJsonObject jsonobject;
    clock_t start_t,end_t;
    //   int times = 0;
    while(start)
    {
        start_t =clock();

        usleep(50000);
        getTemp();
        end_t =clock();
        cout<<"time 1:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
        //        for(int i =0;i<64;++i)
        //        {
        //            for(int j=0;j<80;++j)
        //            {
        //                printf("%d ",temp[i][j]);
        //            }
        //            printf("\n");
        //        }

        pthread_mutex_lock(&ss->mutex);
        rect = ss->GetRect(temp,c->getTa());
        len = ss->getRectlen();
        pthread_mutex_unlock(&ss->mutex);

        end_t =clock();
        cout<<"time 2:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;



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
        end_t =clock();
        cout<<"time 4:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;

//        jsonhelper *json =new jsonhelper();
//        json->create_temp(window,rect,len,temp);
//        jsonobject = json->getJson();
        jsoncpp *json = new jsoncpp();
        json->create_temp(window,rect,len,temp);
        end_t =clock();
        cout<<"time 5:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
        //    cout<<jsonobject.ToFormattedString()<<endl;

        myProtocol *pro = new myProtocol(0x01,0x03,json->getJson_data());
        unsigned long prolen = pro->Getlength();
        cout<<"data lenth:"<<prolen<<endl;
        long len = send(clientfd,pro->GetData(),prolen,0);
        if (len <=0)
        {
            cout<<"send error!"<<endl;
            start = false;
            delete json;
            delete pro;
            delete rect;
            break;
        }
        end_t =clock();
        cout<<"time 6:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
        //usleep(200000);
        delete json;
        delete pro;
        delete rect;
        //        if(++times == 10)
        //        {
        //            cout<<"***times is :"<<times<<endl;
        //            times = 0;
        //            pthread_mutex_lock(&ss->mutexsql);
        //            ss->resetSql();
        //            pthread_mutex_unlock(&ss->mutexsql);
        //        }
        pthread_mutex_lock(&ss->mutexsql);
        usleep(50000);
        ss->storeTemp(temp);
        pthread_mutex_unlock(&ss->mutexsql);
        end_t =clock();
        cout<<"time 7:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;

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
bool socketclient::isStart()
{
    return start;
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
