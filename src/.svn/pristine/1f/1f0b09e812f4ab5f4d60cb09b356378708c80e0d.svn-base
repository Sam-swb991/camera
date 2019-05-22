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
                  "y1 float not null,y2 float not null,prealarm integer not null,prevalue integer not null,highalarm integer not null,"\
                  "highvalue integer not null,linkagealarm integer not null,linkagevalue integer not null,"\
                  "rapidtempchangealarm integer not null,rapidtempchangevalue integer not null,"\
                  "radiance float not null,distance float not null,isset integer default 0);";
    string sql2 = "create table if not exists window(ID integer primary key,x1 float not null,x2 float not null,y1 float not null,y2 float not null);";
    string sql3 = "create table if not exists common(ID integer primary key,serialtemp float not null,arduinoip varchar(20) not null default '192.168.0.101',coefficient float not null default 1.0);";
    char *errorMsg;
    char *errorMsg1;
    char *errorMsg2;
    char *errorMsg3;
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
    res = sqlite3_exec(db,sql2.c_str(),nullptr,nullptr,&errorMsg2);
    if(res != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errorMsg2);
        sqlite3_free(errorMsg2);
    }
    else
    {
        cout<<"create window OK!"<<endl;
        sqlite3_free(errorMsg2);
        delete errorMsg2;
    }
    res = sqlite3_exec(db,sql3.c_str(),nullptr,nullptr,&errorMsg3);
    if(res != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errorMsg3);
        sqlite3_free(errorMsg3);
    }
    else
    {
        cout<<"create window OK!"<<endl;
        sqlite3_free(errorMsg3);
        delete errorMsg3;
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
    cout<<sql<<endl;
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
    cout<<sql<<endl;
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
std::vector<RECTSET>  sqlHelper::getRect(int *nRow,bool isset)
{
    char * errmsg;
    string strSql;
    std::vector <RECTSET> rectset;
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
        return rectset;
    }




    int nIndex = nCol;
    /**
      * @brief　pResult
      * 第n列的名称，存放于pResult[n]
      * 第n行第m列的数据，存放于pResult [(n+1) * nCol + m]
      */
    for(int i=0;i<*nRow;i++)
    {
        RECTSET *rect_set = new RECTSET[1];
        //memset(&rect_set,0,sizeof(RECTSET));
        for(int j=0;j<nCol;j++)
        {

            if(strcmp(pResult[j],"ID")==0)
            {
                rect_set->id = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"name")==0)
            {
                rect_set->name = std::string(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"x1")==0)
            {
                rect_set->rect.x1 = (float)atof(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"y1")==0)
            {
                rect_set->rect.y1 = (float)atof(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"x2")==0)
            {
                rect_set->rect.x2 = (float)atof(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"y2")==0)
            {
                rect_set->rect.y2 = (float)atof(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"prealarm")==0)
            {
                rect_set->prealarm = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"prevalue")==0)
            {
                rect_set->prevalue = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"highalarm")==0)
            {
                rect_set->highalarm = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"highvalue")==0)
            {
                rect_set->highvalue = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"linkagealarm")==0)
            {
                rect_set->linkagealarm = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"linkagevalue")==0)
            {
                rect_set->linkagevalue = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"rapidtempchangealarm")==0)
            {
                rect_set->rapidtempchangealarm = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"rapidtempchangevalue")==0)
            {
                rect_set->rapidtempchangevalue = atoi(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"radiance")==0)
            {
                rect_set->radiance= (float)atof(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"distance")==0)
            {
                rect_set->distance= (float)atof(pResult[nIndex]);
            }
            else if(strcmp(pResult[j],"isset")==0)
            {
                rect_set->isset = atoi(pResult[nIndex]);
            }

            ++nIndex;
        }
        rectset.push_back(*rect_set);
        delete []rect_set;
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
 * @brief 查询数据库，获取window表信息
 * @return 返回WINDOW结构体
 */
WINDOW sqlHelper::getWindow()
{
    string strsql = "select * from window where ID = 1";
    char * errmsg;
    char** pResult;
    int nCol;
    int nRow;
    int res = sqlite3_get_table(db,strsql.c_str(),&pResult,&nRow,&nCol,&errmsg);
    WINDOW window;
    memset(&window,0,sizeof(window));
    if (res != SQLITE_OK)
    {
        printf("sqlite3_get_table error\n");
        fprintf(stderr,"get table error %s\n",sqlite3_errmsg(db));
        sqlite3_free(errmsg);
        return window;
    }
    int nIndex = nCol;
    for(int j=0;j<nCol;j++)
    {
         if(strcmp(pResult[j],"x1")==0)
         {
             window.x1 = (float)atof(pResult[nIndex]);
         }
         else if(strcmp(pResult[j],"x2")==0)
         {
             window.x2 = (float)atof(pResult[nIndex]);
         }
         else if(strcmp(pResult[j],"y1")==0)
         {
             window.y1 = (float)atof(pResult[nIndex]);
         }
         else if(strcmp(pResult[j],"y2")==0)
         {
             window.y2 = (float)atof(pResult[nIndex]);
         }
         nIndex ++;
    }
    sqlite3_free_table(pResult);
    sqlite3_free(errmsg);
    return window;
}

float sqlHelper::getSerialTemp()
{
    string strsql = "select serialtemp from common where ID = 1";
    char * errmsg;
    char** pResult;
    int nCol;
    int nRow;
    int res = sqlite3_get_table(db,strsql.c_str(),&pResult,&nRow,&nCol,&errmsg);
    float serialtemp = 0;
    if (res != SQLITE_OK)
    {
        printf("sqlite3_get_table error\n");
        fprintf(stderr,"get table error %s\n",sqlite3_errmsg(db));
        sqlite3_free(errmsg);
        return serialtemp;
    }
    int nIndex = nCol;
    for(int j=0;j<nCol;j++)
    {
         if(strcmp(pResult[j],"serialtemp")==0)
         {
            serialtemp = (float)atof(pResult[nIndex]);
         }
         nIndex ++;
    }
    sqlite3_free_table(pResult);
    sqlite3_free(errmsg);
    return serialtemp;
}

string sqlHelper::getArduinoIp()
{
    string strsql = "select arduinoip from common where ID = 1";
    char * errmsg;
    char** pResult;
    int nCol;
    int nRow;
    int res = sqlite3_get_table(db,strsql.c_str(),&pResult,&nRow,&nCol,&errmsg);
    string ip="192.168.0.101";
    if (res != SQLITE_OK)
    {
        printf("sqlite3_get_table error\n");
        fprintf(stderr,"get table error %s\n",sqlite3_errmsg(db));
        sqlite3_free(errmsg);
        return ip;
    }
    int nIndex = nCol;
    for(int j=0;j<nCol;j++)
    {
         if(strcmp(pResult[j],"arduinoip")==0)
         {
            ip = pResult[nIndex];
         }
         nIndex ++;
    }
    sqlite3_free_table(pResult);
    sqlite3_free(errmsg);
    return ip;
}

float sqlHelper::getCoefficient()
{
    string strsql = "select arduinoip from common where ID = 1";
    char * errmsg;
    char** pResult;
    int nCol;
    int nRow;
    int res = sqlite3_get_table(db,strsql.c_str(),&pResult,&nRow,&nCol,&errmsg);
    float coefficient=1.0;
    if (res != SQLITE_OK)
    {
        printf("sqlite3_get_table error\n");
        fprintf(stderr,"get table error %s\n",sqlite3_errmsg(db));
        sqlite3_free(errmsg);
        return coefficient;
    }
    int nIndex = nCol;
    for(int j=0;j<nCol;j++)
    {
         if(strcmp(pResult[j],"coefficient")==0)
         {
            coefficient = (float)atof(pResult[nIndex]);
         }
         nIndex ++;
    }
    sqlite3_free_table(pResult);
    sqlite3_free(errmsg);
    return coefficient;
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

void sqlHelper::recovery(bool clear_window)
{
    if(clear_window)
        clear_table("window");
    clear_table("rect");
    clear_table("temperature");
    list<string> tableName,value;
    tableName.push_back("ID");
    tableName.push_back("name");
    tableName.push_back("x1");
    tableName.push_back("y1");
    tableName.push_back("x2");
    tableName.push_back("y2");
    tableName.push_back("prealarm");
    tableName.push_back("prevalue");
    tableName.push_back("highalarm");
    tableName.push_back("highvalue");
    tableName.push_back("linkagealarm");
    tableName.push_back("linkagevalue");
    tableName.push_back("rapidtempchangealarm");
    tableName.push_back("rapidtempchangevalue");
    tableName.push_back("radiance");
    tableName.push_back("distance");
    tableName.push_back("isset");
    value.push_back("-1");
    value.push_back("'未设置区域'");
    value.push_back("0");
    value.push_back("0");
    value.push_back("0");
    value.push_back("0");
    value.push_back("1");
    value.push_back("65");
    value.push_back("1");
    value.push_back("80");
    value.push_back("1");
    value.push_back("110");
    value.push_back("0");
    value.push_back("0");
    value.push_back("1");
    value.push_back("25");
    value.push_back("1");
    insert_table("rect",tableName,value);

}
