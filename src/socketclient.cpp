#include "socketclient.h"
#include "myprotocol.h"
#include "calc.h"
#include <cstring>
#include "common.h"
#include <ctime>
#include "jsoncpp.h"
#include <signal.h>
/**
 * @brief 静态数据初始化
 */
int socketclient::clientfd = -1;
bool socketclient::start = false;
float ** socketclient::temp = nullptr;
sharedspace * socketclient::ss = nullptr;
calc *socketclient::c = nullptr;
/**
 * @brief 构造函数，初始化温度二维数组，传入sharedspace对象
 * @param ss，传入sharedspace对象
 */
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
    c= new calc(ss);


}
/**
 * @brief 链接socket服务端
 * @param addr，服务器地址
 * @param port，服务器端口
 * @return 成功返回套接字clientfd，不成功返回-1
 */
int socketclient::connect(const char *addr, int port)
{
    if(start)
        return -2;
    clientfd = client->SocketClientBuilder(addr,port);
    if(clientfd<0)
    {
        start = false;
        return -1;
    }
    else
        start = true;
    return 0;
}
/**
 * @brief socket客户端线程
 * @return 返回固定为null
 */
void *socketclient::clientthread(void *)
{

    WINDOW window;
    // 6 mm camera

    //    POINT point[5120];
    std::vector<RECT> rect;
    int len;
    CJsonObject jsonobject;
    //clock_t start_t,end_t;
    time_t start_t,end_t;
    tempManager *four_t = new tempManager(4);
    cout<<start<<endl;
    //int times = 0;
    bool writetemp = false;
    start_t =time(nullptr);
    while(start)
    {
        if(!c->getDeviceOk())
            break;

        window=ss->getWindow();

        usleep(50000);
        getTemp();
        ss->storeTemp(temp);
        //end_t =clock();
        //cout<<"time 1:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
        pthread_mutex_lock(&ss->mutexsql);
        cout<<"store four temp"<<endl;
        bool isfull = four_t->addTemp(temp);
        pthread_mutex_unlock(&ss->mutexsql);
        //end_t =clock();
        //cout<<"time 2:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
        //        for(int i =0;i<64;++i)
        //        {
        //            for(int j=0;j<80;++j)
        //            {
        //                printf("%d ",temp[i][j]);
        //            }
        //            printf("\n");
        //        }
        if(isfull)
        {
            four_t->getCalcTemp(temp);
            pthread_mutex_lock(&ss->mutex);
            end_t = time(nullptr);
            if((end_t-start_t)>=300)
            {
                writetemp = true;
                start_t = time(nullptr);
            }
            else
                writetemp = false;
            rect = ss->GetRect(temp,window,c->getTa(),writetemp);
            len = ss->getRectlen();
            list <int> highalarm = ss->getHighAlarm();
            list <int> prealarm = ss->getPreAlarm();
            list <int> linkagealarm = ss->getLinkageAlarm();
            pthread_mutex_unlock(&ss->mutex);

            //end_t =clock();
            //cout<<"time 3:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;



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
            //end_t =clock();
            //cout<<"time 4:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;

            jsoncpp *json = new jsoncpp();
            json->create_temp(window,rect,len,linkagealarm,highalarm,prealarm,temp);
            //end_t =clock();
            //cout<<"time 5:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
            // cout<<json->getJsonString()<<endl;

            myProtocol *pro = new myProtocol(0x01,0x03,json->getJsonString());
            unsigned long prolen = pro->Getlength();
            cout<<"data lenth:"<<prolen<<endl;
            long len = send(clientfd,pro->GetData(),prolen,0);
            if (len <=0)
            {
                cout<<"send error!"<<endl;
                start = false;
                delete json;
                delete pro;
                //delete rect;
               // c->close_fd();
                break;
            }
            if(!c->getDeviceOk())
            {
                cout<<"Divice error!"<<endl;
                start = false;
                delete json;
                delete pro;
                //c->close_fd();
                //delete rect;
                break;
            }
            //end_t =clock();
            //cout<<"time 6:"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
            //usleep(200000);

            delete json;
            delete pro;

        }

        usleep(300000);


    }
    cout<<"client end"<<endl;
    close(clientfd);
    delete four_t;
    return nullptr;
}
/**
 * @brief 创建socket客户端线程
 * @return 返回线程id
 */
pthread_t socketclient::startclient()
{
    pthread_t id;
    int ret = pthread_create(&id,nullptr,clientthread,nullptr);

    if(ret) {
        std::cout << "Create client pthread error!" << std::endl;

    }
    return id;
}
/**
 * @brief 获取客户端线程是否还在运行
 * @return 返回true为在运行，false则不在
 */
bool socketclient::isStart()
{
    return start;
}
/**
 * @brief 获取温度数据
 */
void socketclient::getTemp()
{

    c->get_all_temp(temp);

    //        for(int i = 0;i<64;i++)
    //        {
    //            for(int j = 0;j<80;j++)
    //            {
    //                cout<<temp[i][j]<<" ";
    //            }
    //            cout<<endl;
    //        }
    //        cout<<"print end"<<endl;
}
