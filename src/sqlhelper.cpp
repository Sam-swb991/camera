#include "sqlhelper.h"
#include <stdio.h>
#include <sstream>
#include <cstring>
#include <stdlib.h>
/**
 * @brief 构造函数，数据库初始化
 */
sqlHelper::sqlHelper()
{
    open();
    create_table();
}
/**
 * @brief 析构函数，关闭数据库
 */
sqlHelper::~sqlHelper()
{
    sqlite3_close(db);
}
/**
 * @brief 打开或者创建一个数据库
 * @return 成功返回0,失败返回-1
 */
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
/**
 * @brief 重置数据库
 */
void sqlHelper::reset()
{
    sqlite3_close(db);
    open();
}
/**
 * @brief 创建数据库表
 */
void sqlHelper::create_table()
{
    string sql = "create table if not exists temperature(ID integer primary key autoincrement,tempData text not null,time integer);";
    string sql1 = "CREATE TABLE if not exists rect(ID integer primary key ,name varchar(30) not null,x1 float not null,x2 float not null,"\
                  "y1 float not null,y2 float not null ,highalarm integer not null,highvalue integer not null,lowalarm integer not null,"\
                  "lowvalue integer not null,rapidtempchangealarm integer not null,rapidtempchangevalue integer not null,"\
                  "alarm_level integer not null,isset integer default 0);";
    char *errorMsg;
    char *errorMsg1;
    int res = sqlite3_exec(db,sql.c_str(),nullptr,nullptr,&errorMsg);
    if(res != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errorMsg);
        sqlite3_free(errorMsg);
    }
    else
    {
        cout<<"create temperature OK!"<<endl;
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
        cout<<"create rect OK!"<<endl;
        sqlite3_free(errorMsg1);
        delete errorMsg1;
    }

}
/**
 * @brief 向表里插入数据,temperature表专用
 * @param table，表名
 * @param name，表字段名的list
 * @param value，跟字段名对应的值
 */
void sqlHelper::insert_table(string table,list<string> name,list<string> value)
{
    char *errorMsg ;
    int len =(int)name.size();
    int j=1;
    string sql = "insert into "+table+"(";
    for(list<string>::iterator i =name.begin();;j++)
    {
        sql+=*i;
        /**
          * @brief 每个字段名后要加逗号，最后一个不用加
          * @example insert into temperature (ID,name,x1,x2) values(1,'123',0.11,0.88);
          */
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
/**
 * @brief 更新表数据，rect表专用
 * @param table，表名
 * @param name，表字段名的list
 * @param value，跟字段名对应的值
 */
void sqlHelper::update_table(string table, list < string > name, list < string > value)
{
    char *errorMsg ;
    int len =(int)name.size();
    int k=2;
    string sql = "update "+table+" set ";
    list<string>::iterator i =name.begin();
    list<string>::iterator j =value.begin();
/**
  * @brief list首个元素为ID，不需要更新，ID为判断条件，跳过
  */
    i++;
    j++;
    for(;;k++)
    {
        sql+=*i;
        sql+="=";
        sql+=*j;
        /**
          * @brief 每个字段设置后要加逗号，最后一个不用加
          * @example update rect set name = '123',x1=0.12 where ID = 2;
          */
        if(k<len)
        {
            sql+=",";
            i++;
            j++;
        }
        else
            break;
    }
    sql+=" where ID = ";
    sql+=*value.begin();
    sql+=";";
    //cout<<sql<<endl;
    int res = sqlite3_exec(db,sql.c_str(),nullptr,nullptr,&errorMsg);
    if(res != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errorMsg);

    }
    sqlite3_free(errorMsg);
    delete errorMsg;
}
/**
 * @brief 查询表，获取温度数据专用
 * @param sql，sql语句
 * @return 返回查询出的温度数据字符串
 */
string sqlHelper::select_table(string sql)
{
    cout<<"select"<<endl;
    sqlite3_stmt *stmt;
    string ret;
    int res = sqlite3_prepare_v2(db,sql.c_str(),static_cast<int>(sql.size()),&stmt,nullptr);
    if(res != SQLITE_OK)
    {
        printf("sqlite3_prepare_v2 error\n");
        fprintf(stderr,"prepare error %s\n",sqlite3_errmsg(db));

        return "error";
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
                    ret =(const char *)sqlite3_column_text(stmt,i);
                }
                else if(vtype == SQLITE_INTEGER)
                {
                    stringstream ss;
                    ss<<sqlite3_column_int(stmt,i);
                    ret = ss.str();

                }
            }
            break;
        }
        else if(res == SQLITE_DONE)
        {
            printf("Select Finished.\n");
            sqlite3_finalize(stmt);
            return "";
        }
        else
        {
            printf("Failed to SELECT.\n");

            sqlite3_finalize(stmt);

            return "error";
        }
    }

    sqlite3_finalize(stmt);
    //sqlite3_free(stmt);

    return ret;
}
/**
 * @brief 查询数据库，获取rect表信息
 * @param nRow，rect表查询出的个数
 * @param isset，是否需要查询出所有区域信息，true为所有，false为只需要isset为1的区域数据
 * @return 返回RECTSET结构体
 */
RECTSET * sqlHelper::getRect(int *nRow,bool isset)
{
    char * errmsg;
    string strSql;
    if(isset)
        strSql = "select * from rect;";
    else
        strSql = "select * from rect where isset = 1;";
    char** pResult;
    int nCol;
    int res = sqlite3_get_table(db,strSql.c_str(),&pResult,nRow,&nCol,&errmsg);
    if (res != SQLITE_OK)
    {
        printf("sqlite3_get_table error\n");
        fprintf(stderr,"get table error %s\n",sqlite3_errmsg(db));
        sqlite3_free(errmsg);
        return nullptr;
    }


    RECTSET * rectset = new RECTSET[*nRow];

    int nIndex = nCol;
    /**
      * @brief　pResult
      * 第n列的名称，存放于pResult[n]
      * 第n行第m列的数据，存放于pResult [(n+1) * nCol + m]
      */
    for(int i=0;i<*nRow;i++)
    {
        for(int j=0;j<nCol;j++)
        {

            if(strcmp(pResult[j],"ID")==0)
            {
                rectset[i].id = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"name")==0)
            {
                rectset[i].name = pResult[nIndex];
            }
            else if(strcmp(pResult[j],"x1")==0)
            {
                rectset[i].rect.x1 = (float)atof(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"y1")==0)
            {
                rectset[i].rect.y1 = (float)atof(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"x2")==0)
            {
                rectset[i].rect.x2 = (float)atof(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"y2")==0)
            {
                rectset[i].rect.y2 = (float)atof(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"highalarm")==0)
            {
                rectset[i].highalarm = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"highvalue")==0)
            {
                rectset[i].highvalue = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"lowalarm")==0)
            {
                rectset[i].lowalarm = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"lowvalue")==0)
            {
                rectset[i].lowvalue = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"rapidtempchangealarm")==0)
            {
                rectset[i].rapidtempchangealarm = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"rapidtempchangevalue")==0)
            {
                rectset[i].rapidtempchangevalue = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"alarm_level")==0)
            {
                rectset[i].alarm_level = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"isset")==0)
            {
                rectset[i].isset = atoi(pResult[nIndex]);
            }
            ++nIndex;
        }
    }
    sqlite3_free_table(pResult);
    sqlite3_free(errmsg);
    return rectset;

}
/**
 * @brief 删除表数据
 * @param table,表名
 * @param arg，条件语句
 */
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
/**
 * @brief 清空表数据
 * @param table，表名
 */
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
/**
 * @brief 执行sql语句
 * @param sql，需要执行的sql语句
 */
void sqlHelper::exec(string sql)
{
    char *errorMsg;
    int res = sqlite3_exec(db,sql.c_str(),nullptr,nullptr,&errorMsg);
    if(res != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errorMsg);
        sqlite3_free(errorMsg);
    }
    else
    {
        cout<<"exec OK!"<<endl;
        sqlite3_free(errorMsg);
        delete errorMsg;
    }

}
