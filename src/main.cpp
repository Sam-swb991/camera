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
    serialPort *serial = new serialPort(shared);
    if(serial->getIsHave())
        serial->startRead();
    socketServer *ss = new socketServer(12345,shared);
    pthread_t id_s = ss->startServer();
    UNUSED(id_s);
    socketclient *sc = new socketclient(shared);
 New:while(true)
    {
        string addr = "127.0.0.1";
        int ret = sc->connect(addr.c_str(),10300);
        if(ret >0)
            break;
        else
        {
            cout<<"Waiting for server start!!"<<endl;
            sleep(3);
        }
    }
    pthread_t id = sc->startclient();

    pthread_join(id,nullptr);
    goto New;
       // pthread_join(id_s,nullptr);

}

