#include "serialport.h"
#include <stdio.h>
#include "common.h"
#include "stdlib.h"
sharedspace * serialPort::ss = nullptr;
int serialPort::fd = -1;
serialPort::serialPort(sharedspace *ss)
{
    this->ss = ss;
    struct termios oldstdio;
    fd = open("/dev/ttyUSB0", O_RDONLY );
    if( -1==fd )
    {
        printf("cannot open /dev/ttyUSB0\r\n");
        return ;
    }
    tcgetattr( fd, &oldstdio);
    cfsetispeed(&oldstdio, B9600);
    tcsetattr( fd, TCSANOW, &oldstdio);

}
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
void serialPort::startRead()
{
    pthread_t id;
    int ret = pthread_create(&id,nullptr,readthread,nullptr);

    if(ret) {
        std::cout << "Create serial pthread error!" << std::endl;

    }
    return ;
}
