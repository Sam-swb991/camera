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
#include <time.h>
#include <pthread.h>
using namespace Json;
int main()
{
    sharedspace *shared = new sharedspace();
    socketServer *ss = new socketServer(12345,shared);
    ss->startServer();
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


}

