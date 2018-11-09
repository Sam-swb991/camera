#include "sqlhelper.h"
#include <stdio.h>
#include <sstream>
sqlHelper::sqlHelper()
{
    open();
    create_table();
}
sqlHelper::~sqlHelper()
{
    sqlite3_close(db);
}
int sqlHelper::open()
{

    int res = sqlite3_open("data.db",&db);
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
void sqlHelper::release()
{
    sqlite3_close(db);
    open();
}
void sqlHelper::create_table()
{
    string sql = "create table if not exists temperature(ID integer primary key autoincrement,tempData text not null,time integer);";
    char *errorMsg;
    int res = sqlite3_exec(db,sql.c_str(),nullptr,nullptr,&errorMsg);
    if(res != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errorMsg);
        sqlite3_free(errorMsg);
    }
    else
    {
        cout<<"create OK!"<<endl;
        delete errorMsg;
    }

}
void sqlHelper::insert_table(string table,list<string> name,list<string> value)
{
    char *errorMsg ;
    int len =(int)name.size();
    int j=1;
    string sql = "insert into "+table+"(";
    for(list<string>::iterator i =name.begin();;j++)
    {
        sql+=*i;

        if(j<len)
        {
            sql+=",";
            i++;
        }
        else
            break;

    }
    sql +=")values(";
    j=1;
    for(list<string>::iterator i =value.begin();;j++)
    {
        sql+=*i;
        if(j<len)
        {
            sql+=",";
            i++;
        }
        else
            break;

    }
    sql+=");";
    //cout<<sql<<endl;
    int res = sqlite3_exec(db,sql.c_str(),nullptr,nullptr,&errorMsg);
    if(res != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errorMsg);

    }
    sqlite3_free(errorMsg);
    delete errorMsg;
}

list<string> sqlHelper::select_table(string sql)
{
    cout<<"select"<<endl;
    sqlite3_stmt *stmt;
    ret.clear();
    int res = sqlite3_prepare_v2(db,sql.c_str(),static_cast<int>(sql.size()),&stmt,nullptr);
    if(res != SQLITE_OK)
    {
        printf("sqlite3_prepare_v2 error\n");
        fprintf(stderr,"prepare error %s\n",sqlite3_errmsg(db));
        ret.push_back("error");
        return ret;
    }
    int count = sqlite3_column_count(stmt);

    cout<<"count is :"<<count<<endl;
   while(true)
   {
        res = sqlite3_step(stmt);
        if(res == SQLITE_ROW)
        {
            for(int i = 0;i<count;i++)
            {
                int vtype = sqlite3_column_type(stmt,i);
                cout<<"type is :"<<vtype<<endl;
                if(vtype == SQLITE_TEXT)
                {
                    ret.push_back(string((const char *)sqlite3_column_text(stmt,i)));
                }
                else if(vtype == SQLITE_INTEGER)
                {
                    stringstream ss;
                    ss<<sqlite3_column_int(stmt,i);
                    string temp = ss.str();
                    ret.push_back(temp);
                }
            }
            break;
        }
        else if(res == SQLITE_DONE)
        {
            printf("Select Finished.\n");
            ret.clear();
            sqlite3_finalize(stmt);
            return ret;
        }
        else
        {
            printf("Failed to SELECT.\n");

            sqlite3_finalize(stmt);
            ret.push_back("error");
            return ret;
        }
    }
    cout<<"ret :"<<ret.size()<<endl;
    sqlite3_finalize(stmt);
    //sqlite3_free(stmt);

    return ret;
}
void sqlHelper::delete_table(string table,string arg)
{
    char *errorMsg;
    string sql = "delete from "+table+" where "+arg+";";
    int res = sqlite3_exec(db,sql.c_str(),nullptr,nullptr,&errorMsg);
    if(res != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errorMsg);
        sqlite3_free(errorMsg);
    }
    else
    {
        cout<<"delete OK!"<<endl;
        sqlite3_free(errorMsg);
        delete errorMsg;
    }
}
void sqlHelper::clear_table(string table)
{
    char *errorMsg;
    char *errorMsg1;
    string sql = "delete from "+table+" ;";
    string sql1 = "delete from sqlite_sequence where name = '"+table+"';";
    int res = sqlite3_exec(db,sql.c_str(),nullptr,nullptr,&errorMsg);
    if(res != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errorMsg);
        sqlite3_free(errorMsg);
    }
    else
    {
        cout<<"delete table OK!"<<endl;
        sqlite3_free(errorMsg);
        delete errorMsg;
    }
    res = sqlite3_exec(db,sql1.c_str(),nullptr,nullptr,&errorMsg1);
    if(res != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errorMsg1);
        sqlite3_free(errorMsg1);
    }
    else
    {
        cout<<"set sequence OK!"<<endl;
        sqlite3_free(errorMsg1);
        delete errorMsg1;
    }
}
