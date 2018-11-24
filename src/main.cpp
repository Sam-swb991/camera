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
using namespace Json;
int main()
{


//    Json::Value test;
//    test["id"] = 1;
//    test["body"]["rect"][0] = 1;
//    test["body"]["rect"][1] = 2;
//    cout<<test.toStyledString();
    //    int ** temp = new int*[64];
    //    for(int i=0; i<64; ++i)
    //    {
    //        temp[i] = new int[80];

    //        memset(temp[i],0,80*sizeof(int));
    //    }
    //   calc *c = new calc();

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
    sc->startclient();
    while(true)
    {
        //        c->get_all_temp(temp);
        //        for(int i = 0;i<64;i++)
        //        {
        //            for(int j = 0;j<80;j++)
        //            {
        //                cout<<temp[i][j]<<" ";
        //            }
        //            cout<<endl;
        //        }
        if(sc->isStart())
        {
            sleep(1);
        }
        else
            goto New;
        int ch =getchar();
        if(ch =='q')
            break;
    }
    delete shared;
    delete ss;
    delete sc;
    return 0;
}

