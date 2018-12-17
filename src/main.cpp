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
#define UNUSED(var) {var++;var--;}
using namespace Json;
int main()
{
    signal(SIGPIPE,SIG_IGN);
    signal(SIGABRT,SIG_IGN);
    sharedspace *shared = new sharedspace();
    serialPort *serial = new serialPort(shared);
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
        //pthread_join(id_s,nullptr);

}

