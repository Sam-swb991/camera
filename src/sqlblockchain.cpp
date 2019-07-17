#include "sqlblockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "common.h"
using namespace std;
sqlblockchain::sqlblockchain()
{
    open();
    create_table();
}


int sqlblockchain::open()
{

    int res = sqlite3_open("/mnt/sd_card/blockchain.db",&db);
    if(res != SQLITE_OK)
    {
        fprintf(stderr,"open err:%s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    else
    {
        cout<<"open ok!"<<endl;
        return 0;
    }
}

void sqlblockchain::create_table()
{
    string sql = "create table if not exists chainData(ID integer primary key autoincrement,\
                    preHash varchar(20)  not null,SN varchar(15) not null,devName varchar(10) not null,\
                    IP varchar(20) not null ,data varchar(10) not null default 0,Alarm integer not null ,\
                    SensorStatus integer not null, Status integer not null ,time TEXT not null ,Hash varchar(20) not null);";
    char *errorMsg;
    int res = sqlite3_exec(db,sql.c_str(),nullptr,nullptr,&errorMsg);
    if(res != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errorMsg);
        sqlite3_free(errorMsg);
    }
    else
    {
        cout<<"create chainData OK!"<<endl;
        sqlite3_free(errorMsg);
        delete errorMsg;
    }
}

int sqlblockchain::insert_block_chain(blockchainData *data)
{
    string sql = "insert into chainData(preHash,SN,devName,IP,data,Alarm,SensorStatus,Status,time,Hash)values('"+
            data->getPrehash()+"','"+data->getSN()+"','"+data->getDevName()+"','"+data->getIP()+"','"+
            data->getData()+"',"+common::to_string(data->getAlarm())+","+common::to_string(data->getSensorstatus())+","+
            common::to_string(data->getStatus())+",'"+data->getTime()+"','"+data->getHash()+"');";
    char *errorMsg;
    cout<<sql<<endl;
    int res = sqlite3_exec(db,sql.c_str(),nullptr,nullptr,&errorMsg);
    if(res != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errorMsg);
        sqlite3_free(errorMsg);
    }
    else
    {
        cout<<"insert chainData OK!"<<endl;
        sqlite3_free(errorMsg);
        delete errorMsg;
    }
    return res;
}

string sqlblockchain::getLastHash()
{
    string strsql = "select Hash from chainData order by ID desc limit 1";
    char * errmsg;
    char** pResult;
    int nCol;
    int nRow;
    int res = sqlite3_get_table(db,strsql.c_str(),&pResult,&nRow,&nCol,&errmsg);
    string Hash = "FFFFFFFFFFFFFFFF";
    if (res != SQLITE_OK)
    {
        printf("sqlite3_get_table error\n");
        fprintf(stderr,"get table error %s\n",sqlite3_errmsg(db));
        sqlite3_free(errmsg);
        return Hash;
    }
    int nIndex = nCol;
    for(int j=0;j<nCol;j++)
    {
         if(strcmp(pResult[j],"Hash")==0)
         {
            Hash = pResult[nIndex];
         }
         nIndex ++;
    }
    sqlite3_free_table(pResult);
    sqlite3_free(errmsg);
    return Hash;
}
