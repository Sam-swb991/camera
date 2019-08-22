#include "spictrl.h"
#include "eepromctrl.h"
#include "sensorctrl.h"
#include "calc.h"
#include <cstring>
#include <stdio.h>
#include "CJsonObject.hpp"
#include "socketserver.h"
#include "sharedspace.h"
#include "socketclient.h"
#include "sqlhelper.h"
#include "json/json.h"
#include "serialport.h"
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include "httprequest.h"
#include "md5.h"
#include "common.h"
#include "sendtoarduino.h"
#include <queue>
#include <fcntl.h>
#include <unistd.h>
#include "rs485.h"
#include "udpclientthread.h"
#define UNUSED(var) {var++;var--;}
using namespace Json;
int main()
{
//    string shellcmd = "/mnt/download/down.sh 192.168.0.29 camera fc10deecfd92aec6236395e970489da8";
//    FILE *fp;
//    char buffer[80];
//    fp = popen(shellcmd.c_str(),"r");
//    fgets(buffer,sizeof(buffer),fp);
//    printf("buf is:\n%s\n",buffer);
//    pclose(fp);
    signal(SIGPIPE,SIG_IGN);
    signal(SIGABRT,SIG_IGN);
    printf("VERSION:%s\n",VER);
    sharedspace *shared = new sharedspace();
//    Rs485 * yuntai = new Rs485(shared);
//    int ret =yuntai->open_485("/dev/ttyAMA1");
//    cout<<"ret yuntai :"<<ret<<endl;
//    shared->yuntai_auto = true;
//    yuntai->control(Rs485::YUNTAI_AUTO,nullptr);
    udpclientthread * udpct = new udpclientthread(shared);
    udpct->start();
    serialPort *serial = new serialPort(shared);
    if(serial->getIsHave())
        serial->startRead();
    socketclient *sc = new socketclient(shared);
    socketServer *ss = new socketServer(12345,shared);
    pthread_t id_s = ss->startServer();
    UNUSED(id_s);

    sendtoarduino * arduino = new sendtoarduino(shared);
    string addr = "127.0.0.1";
    while(true)
    {

        int ret = sc->connect(addr.c_str(),10300);
        if(ret ==0)
        {
            sc->startclient();
        }
        else if (ret == -2)
        {
            cout<<"socket client has already started!"<<endl;
        }
        else
        {
            cout<<"Waiting for server start!!"<<endl;

        }
        sleep(1);
        ret = arduino->connect(shared->arduinoIp.c_str(),10001);
        if(ret ==0)
        {
            arduino->start();
        }
        else if(ret == -2)
        {
            cout<<"arduino has already started!"<<endl;
        }
        else
        {
            cout<<"Waiting for arduino start!!"<<endl;

        }
        sleep(1);
    }

 //  pthread_join(id,nullptr);
  //  goto New;
      // pthread_join(id_s,nullptr);

}

