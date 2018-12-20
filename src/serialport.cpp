#include "serialport.h"
#include <stdio.h>
#include "common.h"
#include "stdlib.h"
/**
 * @brief 静态变量初始化
 */
sharedspace * serialPort::ss = nullptr;
int serialPort::fd = -1;
/**
 * @brief 构造函数，传入sharedspace类，初始化串口
 * @param ss,传入的sharedspace类
 */
serialPort::serialPort(sharedspace *ss)
{
    this->ss = ss;
    struct termios oldstdio;
    fd = open("/dev/ttyUSB0", O_RDONLY );
    if( -1==fd )
    {
        printf("cannot open /dev/ttyUSB0\r\n");
        is_have = false;
        return ;
    }
    else
        is_have = true;
    tcgetattr( fd, &oldstdio);
    cfsetispeed(&oldstdio, B9600);
    tcsetattr( fd, TCSANOW, &oldstdio);

}
/**
 * @brief 从读取串口温度的线程
 * @return 固定为NULL
 */
void *serialPort::readthread(void *)
{
    char buf[20]={0};
    while(1)
    {
        memset(buf,0,20);

        tcflush( fd, TCIFLUSH );

        read(fd,buf,20);
        string str(buf);        
        string spiltstr = str.substr(3,str.size()-3);
        float temp = (double)atof(spiltstr.c_str());
        cout<<"temp is :"<<temp<<endl;
        pthread_mutex_lock(&ss->mutexSerial);
        ss->setSerialTemp(temp);
        pthread_mutex_unlock(&ss->mutexSerial);
        sleep(1);

    }
}
/**
 * @brief 开始串口读取温度的线程
 */
void serialPort::startRead()
{
    pthread_t id;
    int ret = pthread_create(&id,nullptr,readthread,nullptr);

    if(ret) {
        std::cout << "Create serial pthread error!" << std::endl;

    }
    return ;
}
/**
 * @brief 获取是否有串口设备
 * @return 返回ture为有串口设备，false为没有
 */
bool serialPort::getIsHave()
{
    return is_have;
}
