#include "sharedspace.h"
#include "temprule.h"
#include <stdio.h>
#include <sstream>
#include <time.h>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <common.h>
#include <pthread.h>
#include <vector>
using namespace std;
/**
 * @brief 构造函数，初始化各种数据
 */
sharedspace::sharedspace()
{
    if(pthread_mutex_init(&mutex, nullptr) != 0)
    {
        perror("mutex init error!");
    }
    if(pthread_mutex_init(&mutexsql, nullptr) != 0)
    {
        perror("mutexsql init error!");
    }
    if(pthread_mutex_init(&mutexSerial, nullptr) != 0)
    {
        perror("mutexserial init error!");
    }
    rectsetlen = 0;
    sql = new sqlHelper();
    sql->clear_table("temperature");
    string sqlstr ="select count(*) from window;";
    pthread_mutex_lock(&mutexsql);
    string num = sql->select_table(sqlstr);
    pthread_mutex_unlock(&mutexsql);
    if(atoi(num.c_str()) == 0)
    {
        window.x1 = 0.143f;
        window.y1 = -0.055f;
        window.x2 = 0.95f;
        window.y2 = 1.088f;
        list<string> tname;
        tname.push_back("ID");
        tname.push_back("x1");
        tname.push_back("x2");
        tname.push_back("y1");
        tname.push_back("y2");
        list<string> value;
        value.push_back("1");
        value.push_back(common::to_string(window.x1));
        value.push_back(common::to_string(window.x2));
        value.push_back(common::to_string(window.y1));
        value.push_back(common::to_string(window.y2));
        pthread_mutex_lock(&mutexsql);
        sql->insert_table("window",tname,value);
        pthread_mutex_unlock(&mutexsql);
    }
    else
    {
        pthread_mutex_lock(&mutexsql);
        window = sql->getWindow();
        pthread_mutex_unlock(&mutexsql);
    }
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
    set = true;
    setwindow = true;
    mode = -1;

}
/**
 * @brief 获取RECT结构体对象
 * @param temp，温度二维数组
 * @param Ta，Ta值
 * @return 返回RECT对象
 */
RECT * sharedspace::GetRect(float **temp,WINDOW windows,int Ta)
{
    cout<<"start get rect!"<<endl;
    if(set)
    {
        //delete this->rectset;
        pthread_mutex_lock(&this->mutexsql);
        this->rectset = sql->getRect(&rectsetlen,false);
        pthread_mutex_unlock(&this->mutexsql);
        set = false;
    }
//    cout<<"rect :"<<rectset[0].id<<" ";
//    cout<<rectset[0].name<<" ";
//    cout<<rectset[0].rect.x1;
//    cout<<" ";
//    cout<<rectset[0].rect.y1;
//    cout<<" ";
//    cout<<rectset[0].rect.x2;
//    cout<<" ";
//    cout<<rectset[0].rect.y2;
//    cout<<" ";
//    cout<<rectset[0].highalarm<<" "<<rectset[0].highvalue<<endl;
    if(rectsetlen != 0)
    {
        TEMP_C *tempc = new TEMP_C[rectsetlen];
        int *alarmmode = new int[rectsetlen];

        trule = new temprule(this->rectset,rectsetlen,windows,temp,this,tempc,alarmmode,Ta);
        highalarm = trule->getHighAlarm();
        prealarm = trule->getPreAlarm();
        linkagealarm = trule->getLinkageAlarm();
        for(int i =0;i<rectsetlen;++i)
        {
            printf("alarmmode is %d\n",alarmmode[i]);
            printf("tempc avg = %f,high = %f,low = %f\n",tempc[i].avgTemp,tempc[i].highTemp,tempc[i].lowTemp);
        }

        rect = new RECT[rectsetlen];
        for(int i =0;i<rectsetlen;++i)
        {
            if(alarmmode[i] == 0)
            {
                rect[i].mode = 0;
            }
            else
            {

                rect[i].mode = 1;

            }

            rect[i].alarmMode = alarmmode[i];

            //cout<<rectset[i].name<<endl;
            rect[i].copy(rectset[i]);


            rect[i].tempc = tempc[i];

        }
        delete []alarmmode;
        delete []tempc;

        delete trule;


    }
    else
        rect = nullptr;
    return rect;

}
/**
 * @brief 设置区域数据
 * @param rectset，区域的结构体对象
 * @param len，区域个数
 * @param mode，模式
 */
void sharedspace::SetRect(RECTSET *rectset,int len,int mode)
{
    if(mode !=GET)
        rectsetlen = len;
    this->mode = mode;
    cout<<"mode is :"<<mode<<"len is :"<<len<<endl;
    if(rectsetlen != 0)
    {

        switch (mode)
        {
        case ADD:
        case MODIFY:{
            cout<<"modify"<<endl;
            list<string> value;
            for(int i=0;i<len;++i)
            {

                value.push_back(common::to_string(rectset[i].id));
                value.push_back(rectset[i].name);
                value.push_back(common::to_string(rectset[i].rect.x1));
                value.push_back(common::to_string(rectset[i].rect.y1));
                value.push_back(common::to_string(rectset[i].rect.x2));
                value.push_back(common::to_string(rectset[i].rect.y2));
                value.push_back(common::to_string(rectset[i].prealarm));
                value.push_back(common::to_string(rectset[i].prevalue));
                value.push_back(common::to_string(rectset[i].highalarm));
                value.push_back(common::to_string(rectset[i].highvalue));
                value.push_back(common::to_string(rectset[i].linkagealarm));
                value.push_back(common::to_string(rectset[i].linkagevalue));
                value.push_back(common::to_string(rectset[i].rapidtempchangealarm));
                value.push_back(common::to_string(rectset[i].rapidtempchangevalue));
                value.push_back(common::to_string(rectset[i].radiance));
                value.push_back(common::to_string(rectset[i].distance));

            }
            if(mode == ADD)
            {
                sql->insert_table("rect",tableName,value);
            }
            else if(mode == MODIFY)
            {
                sql->update_table("rect",tableName,value);
            }
            set = true;
        }break;
        case DEL:{
            for(int i =0 ;i<len;++i)
            {
                string sqlstr = "delete from rect where ID = ";
                sqlstr+= common::to_string(rectset[i].id);
                sqlstr+=";";
                cout<<sqlstr<<endl;
                sql->exec(sqlstr);
            }
            set = true;
        }break;
        case SET:
        case UNSET:{
            string sqlstr;
            for(int i=0;i<len;++i)
            {
                if(mode == SET)
                    sqlstr = "update rect set isset = 1 where ID = ";
                else if(mode == UNSET)
                    sqlstr = "update rect set isset = 0 where ID = ";
                sqlstr+= common::to_string(rectset[i].id);
                sqlstr+=";";
                sql->exec(sqlstr);
                sqlstr.clear();
            }
            set = true;
        }break;
        case GET:break;

        default:break;
        }

    }

}
/**
 * @brief 获取区域个数
 * @return 返回区域个数
 */
int sharedspace::getRectlen()
{
    return rectsetlen;
}
/**
 * @brief 把温度数据存储到数据库
 * @param temp,温度二维数组
 */
void sharedspace::storeTemp(float **temp)
{
    stringstream ss;
    stringstream t;
    char str[256] = { 0 };
    ss<<"'";
    for(int i = 0;i<HEIGHT;++i)
    {
        for(int j=0;j<WIDTH;++j)
        {
            ss<<static_cast<int>(temp[i][j]);
            //cout<<static_cast<int>(temp[i][j])<<" ";
            if(i == HEIGHT-1 && j ==WIDTH-1)
            {
                ss<<"'";
            }
            else
                ss<<",";
        }
        // cout<<endl;
    }
    list<string> name,value;
    name.push_back("tempData");
    name.push_back("time");
    value.push_back(ss.str());

    sprintf(str,"%ld",time(nullptr));
    cout<<"time is:"<<str<<endl;
    value.push_back(str);
    sql->insert_table("temperature",name,value);
    t<<(time(nullptr)-65);
    string delsql ="time < "+ t.str();
    sql->delete_table("temperature",delsql);

}
/**
 * @brief 把数据库读出的温度字符串分割成单个温度字符串
 * @param str，数据库读出温度字符串
 * @param delim，分隔符
 * @return 返回单个温度字符串组成的容器
 */
vector<string> split(const string& str, const string& delim) {
    vector<string> res;
    if("" == str) return res;
    
    char * strs = new char[str.length() + 1] ;
    strcpy(strs, str.c_str());

    char * d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while(p) {
        string s = p;
        res.push_back(s);
        p = strtok(nullptr, d);
    }
    delete []strs;
    delete []d;
    delete p;
    return res;
}
/**
 * @brief 重置数据库
 */
void sharedspace::resetSql()
{
    sql->reset();
}
/**
 * @brief 从数据库读取1分钟前的温度数据
 * @param temp，温度二维数组
 * @return 读取成功返回0,不成功返回-1
 */
int sharedspace::getTemp(int **temp)
{
    cout<<"gettemp"<<endl;
    stringstream ss;
    stringstream t;
    string ret;
    ss<<(time(nullptr)-60);
    string sqlstr = "select tempData from temperature where time < "+ ss.str()+" order by time DESC LIMIT 1 OFFSET 0;";
    ret = sql->select_table(sqlstr);
    if(!ret.empty()&&ret!="error")
    {

        //cout<<ret<<endl;
        vector <string> res = split(ret,",");
        //cout<<"res size is:"<<res.size()<<endl;
        for(int i = 0;i<64;++i)
        {
            for(int j = 0;j<80;++j)
            {
                temp[i][j] = atoi(res[static_cast<unsigned long>(i*80+j)].c_str());
            }
        }

        return 0;
    }
    else
        return -1;
}
WINDOW sharedspace::getWindow()
{
    return window;
}
void sharedspace::setWindow(int direction)
{
    switch (direction) {
    case UP:{
        window.y1 -=0.01f;
        window.y2 -=0.01f;
    }break;
    case DOWN:{
        window.y1 +=0.01f;
        window.y2 +=0.01f;
    }break;
    case LEFT:{
        window.x1 -=0.01f;
        window.x2 -=0.01f;
    }break;
    case RIGHT:{
        window.x1 +=0.01f;
        window.x2 +=0.01f;
    }break;
    default:break;
    }
    list<string> tname;
    tname.push_back("ID");
    tname.push_back("x1");
    tname.push_back("x2");
    tname.push_back("y1");
    tname.push_back("y2");
    list<string> value;
    value.push_back("1");
    value.push_back(common::to_string(window.x1));
    value.push_back(common::to_string(window.x2));
    value.push_back(common::to_string(window.y1));
    value.push_back(common::to_string(window.y2));
    sql->update_table("window",tname,value);
    setwindow = true;

}
list<int> sharedspace::getHighAlarm()
{
    return highalarm;
}
list <int> sharedspace::getPreAlarm()
{
    return prealarm;
}
list<int > sharedspace::getLinkageAlarm()
{
    return linkagealarm;
}

void sharedspace::setSerialTemp(float temp)
{
    serial_temp = temp;
}

float sharedspace::getSerialTemp()
{
    return serial_temp;
}
