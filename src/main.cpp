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

int main()
{
    sqlHelper *sqlite = new sqlHelper();
    list<string > name,value;
    name.push_back("tempData");
    value.push_back("\'123,123\'");
    sqlite->insert_table("temperature",name,value);
    value.clear();
    value = sqlite->select_table("select * from temperature where Id = 1;");
    for(list<string>::iterator i =value.begin();i!=value.end();i++)
    {
        cout<<*i<<endl;
    }
    sharedspace *shared = new sharedspace();
    socketServer *ss = new socketServer(12345,shared);
    ss->startServer();
    socketclient *sc = new socketclient(shared);
    sc->startclient();
    while(true)
    {
        int ch = getchar();
        if(ch == 'q')
            break;
    }

	return 0;
}

