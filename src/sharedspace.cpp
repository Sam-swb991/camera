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
#include <fstream>
#include <vector>
#include "ipset.h"
#include "rs485.h"

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
    if(pthread_mutex_init(&mutexurl, nullptr) != 0)
    {
        perror("mutexurl init error!");
    }
    if(pthread_mutex_init(&mutexsendalarm, nullptr) != 0)
    {
        perror("mutexsendalarm init error!");
    }
    if(pthread_mutex_init(&mutexarduinoUrl, nullptr) != 0)
    {
        perror("mutexarduinoUrl init error!");
    }
    rectsetlen = 0;
    sql = new sqlHelper();
    pthread_mutex_lock(&mutexsql);
    sql->clear_table("temperature");
    string sqlstr ="select count(*) from window;";
    string num = sql->select_table(sqlstr);
    string sqlstr1 ="select count(*) from common;";
    string num1 = sql->select_table(sqlstr1);
    pthread_mutex_unlock(&mutexsql);
    if(atoi(num.c_str()) == 0)
    {
        if(TABLENUMBER == 124)
        {
            window.x1 = 0.143f;
            window.y1 = -0.055f;
            window.x2 = 0.95f;
            window.y2 = 1.088f;
        }
        else if(TABLENUMBER == 123)
        {
            window.x1 = -0.02f;
            window.y1 = -0.13f;
            window.x2 = 0.96f;
            window.y2 = 1.23f;
        }
        else if(TABLENUMBER == 122)
        {
            window.x1 = 0;
            window.y1 = -0.15f;
            window.x2 = 0.94f;
            window.y2 = 1.2f;
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
    if(atoi(num1.c_str()) == 0)
    {
        list<string> tname;
        tname.push_back("ID");
        tname.push_back("serialtemp");
        list<string> value;
        value.push_back("1");
        value.push_back("0");
        pthread_mutex_lock(&mutexsql);
        sql->insert_table("common",tname,value);
        pthread_mutex_unlock(&mutexsql);
    }
    else
    {
        pthread_mutex_lock(&mutexsql);
        serial_temp = sql->getSerialTemp();
        pthread_mutex_unlock(&mutexsql);
    }

    pthread_mutex_lock(&mutexsql);
    arduinoIp = sql->getArduinoIp();
    cout<<"arduinoIp:"<<arduinoIp<<endl;
    pthread_mutex_unlock(&mutexsql);

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
    needsendtoarduino = false;
    useserialtemp = false;
    haveserialmodel = false;

    warningtimes = 0;
    mode = -1;
    readSN();
    pthread_mutex_lock(&mutexsql);
    coefficient = sql->getCoefficient();
    cout<<"coefficient:"<<coefficient<<endl;
    pthread_mutex_unlock(&mutexsql);
    pthread_mutex_lock(&mutexsql);
    yuntai_auto = sql->getyuntaiauto();
    cout<<"yuntaiauto:"<<yuntai_auto<<endl;
    pthread_mutex_unlock(&mutexsql);
    char ipbuf[16] = {0};
    char broadcast_buf[16] = {0};
    ip = ipset::getip(ipbuf);
    broadcast = ipset::getbroadcast(broadcast_buf);
    threadpool = new ClThreadPool();
    threadpool->Create(10);
    url = new HTTPURL[1];
    url->camera_id.clear();
    url->camera_id.append(SN);
    url->ip.clear();
    url->ip.append(ip);
    arduinoUrl = new HTTPURL[1];
    arduinoUrl->camera_id.clear();
    arduinoUrl->camera_id.append(SN);
    arduinoUrl->ip.clear();
    arduinoUrl->ip.append(ip);
    sixteen_t = new tempManager(60);
    sqlbc = new sqlblockchain();
    preHash = sqlbc->getLastHash();

}
/**
 * @brief 获取RECT结构体对象
 * @param temp，温度二维数组
 * @param Ta，Ta值
 * @return 返回RECT对象
 */
std::vector<RECT>  sharedspace::GetRect(float **temp,WINDOW windows,int Ta,bool writetemp)
{
    cout<<"start get rect!"<<endl;
    if(set)
    {
        //delete this->rectset;
        pthread_mutex_lock(&this->mutexsql);
        rectset = sql->getRect(&rectsetlen,false);
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

        trule = new temprule(rectset,rectsetlen,windows,temp,this,tempc,alarmmode,Ta,writetemp);
        highalarm = trule->getHighAlarm();
        prealarm = trule->getPreAlarm();
        linkagealarm = trule->getLinkageAlarm();
        for(int i =0;i<rectsetlen;++i)
        {
            printf("alarmmode is %d\n",alarmmode[i]);
            printf("tempc avg = %f,high = %f,low = %f\n",tempc[i].avgTemp,tempc[i].highTemp,tempc[i].lowTemp);
        }
        rect.clear();

        for(int i =0;i<rectsetlen;++i)
        {
            RECT  * _rect = new RECT[1];
            if(alarmmode[i] == 0)
            {
                _rect->mode = 0;
            }
            else
            {

                _rect->mode = 1;

            }

            _rect->alarmMode = alarmmode[i];

            //cout<<rectset[i].name<<endl;
            _rect->name=rectset[(size_t)i].name;
            _rect->transrect = rectset[(size_t)i].rect;


            _rect->tempc = tempc[i];

            rect.push_back(*_rect);
            delete [] _rect;
        }
        delete []alarmmode;
        delete []tempc;

        delete trule;
        cout<<"end get rect"<<endl;

    }
    else
    {

    }
    return rect;

}
/**
 * @brief 设置区域数据
 * @param rectset，区域的结构体对象
 * @param len，区域个数
 * @param mode，模式
 */
void sharedspace::SetRect(std::vector<RECTSET> rectset,int len,int mode)
{
    if(mode <GET)
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
            for(size_t i=0;i<(size_t)len;++i)
            {
                cout<<"radiance:"<<rectset[i].radiance<<endl;
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
                cout<<"add"<<endl;
                pthread_mutex_lock(&mutexsql);
                sql->insert_table("rect",tableName,value);
                pthread_mutex_unlock(&mutexsql);
            }
            else if(mode == MODIFY)
            {
                pthread_mutex_lock(&mutexsql);
                sql->update_table("rect",tableName,value);
                pthread_mutex_unlock(&mutexsql);
            }
            set = true;
        }break;
        case DEL:{
            for(size_t i =0 ;i<(size_t)len;++i)
            {
                string sqlstr = "delete from rect where ID = ";
                sqlstr+= common::to_string(rectset[i].id);
                sqlstr+=";";
                cout<<sqlstr<<endl;
                pthread_mutex_lock(&mutexsql);
                sql->exec(sqlstr);
                pthread_mutex_unlock(&mutexsql);
            }
            set = true;
        }break;
        case SET:
        case UNSET:{
            string sqlstr;
            for(size_t i=0;i<(size_t)len;++i)
            {
                if(mode == SET)
                    sqlstr = "update rect set isset = 1 where ID = ";
                else if(mode == UNSET)
                    sqlstr = "update rect set isset = 0 where ID = ";
                sqlstr+= common::to_string(rectset[i].id);
                sqlstr+=";";
                pthread_mutex_lock(&mutexsql);
                sql->exec(sqlstr);
                pthread_mutex_unlock(&mutexsql);
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
bool sharedspace::storeTemp(float **temp)
{
    return sixteen_t->addTemp(temp);
//    stringstream ss;
//    stringstream t;
//    char str[256] = { 0 };
//    ss<<"'";
//    for(int i = 0;i<HEIGHT;++i)
//    {
//        for(int j=0;j<WIDTH;++j)
//        {
//            ss<<static_cast<int>(temp[i][j]);
//            //cout<<static_cast<int>(temp[i][j])<<" ";
//            if(i == HEIGHT-1 && j ==WIDTH-1)
//            {
//                ss<<"'";
//            }
//            else
//                ss<<",";
//        }
//         //cout<<endl;
//    }
//    cout<<"temp get end"<<endl;
//    list<string> name,value;
//    name.push_back("tempData");
//    name.push_back("time");
//    value.push_back(ss.str());

//    sprintf(str,"%ld",time(nullptr));
//    cout<<"time is:"<<str<<endl;
//    value.push_back(str);
//    sql->insert_table("temperature",name,value);
//    t<<(time(nullptr)-65);
//    string delsql ="time < "+ t.str();
//    sql->delete_table("temperature",delsql);

}

/**
 * @brief 重置数据库
 */
void sharedspace::resetSql()
{
    pthread_mutex_lock(&mutexsql);
    sql->reset();
    pthread_mutex_unlock(&mutexsql);
}
/**
 * @brief 从数据库读取1分钟前的温度数据
 * @param temp，温度二维数组
 * @return 读取成功返回0,不成功返回-1
 */
int sharedspace::getTemp(float **temp)
{
    bool res = sixteen_t->getFirstTemp(temp);
    if(res)
        return 0;
    else
        return -1;
//    cout<<"gettemp"<<endl;
//    stringstream ss;
//    string ret;
//    time_t g_time = time(nullptr)-60;
//    time_t l_time = time(nullptr)-65;
//    string sqlstr = "select tempData from temperature where time < "+ common::to_string(g_time)+
//            " and time > "+common::to_string(l_time)+" order by time DESC LIMIT 1 OFFSET 0;";
//    ret = sql->select_table(sqlstr);
//    if(!ret.empty()&&ret!="error")
//    {

//        //cout<<ret<<endl;
//        vector <string> res = common::split(ret,",");
//        //cout<<"res size is:"<<res.size()<<endl;
//        for(int i = 0;i<64;++i)
//        {
//            for(int j = 0;j<80;++j)
//            {
//                temp[i][j] = atoi(res[static_cast<unsigned long>(i*80+j)].c_str());
//            }
//        }

//        return 0;
//    }
//    else
//        return -1;
}
/**
 * @brief 获取红外像素在可见光中的坐标
 * @return 返回WINDOW结构体
 */
WINDOW sharedspace::getWindow()
{
    return window;
}
/**
 * @brief 设置window坐标，在可见光坐标中移动
 * @param direction,方向参数，可以设置上下左右
 */
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
    pthread_mutex_lock(&mutexsql);
    sql->update_table("window",tname,value);
    pthread_mutex_unlock(&mutexsql);
    setwindow = true;

}
/**
 * @brief 获取高温报警点的坐标
 * @return 返回高温报警点的坐标
 */
list<int> sharedspace::getHighAlarm()
{
    return highalarm;
}
/**
 * @brief 获取预警点334:3的坐标
 * @return 返回预警点的坐标
 */
list <int> sharedspace::getPreAlarm()
{
    return prealarm;
}
/**
 * @brief 获取联动报警点的坐标
 * @return 返回联动报警点的坐标
 */
list<int > sharedspace::getLinkageAlarm()
{
    return linkagealarm;
}
/**
 * @brief 记录串口读出的温度
 * @param temp,读出的串口温度
 */
void sharedspace::setSerialTemp(float temp)
{
    useserialtemp = false;
    list<string> tname;
    tname.push_back("ID");
    tname.push_back("serialtemp");
    list<string> value;
    value.push_back("1");
    value.push_back(common::to_string(temp));
    pthread_mutex_lock(&mutexsql);
    sql->update_table("common",tname,value);
    pthread_mutex_unlock(&mutexsql);
    serial_temp = temp;
    cout<<"set serial time is:"<<temp<<endl;
}

void sharedspace::setArduinoIp(string ip)
{
    list<string> tname;
    tname.push_back("ID");
    tname.push_back("arduinoip");
    list<string> value;
    value.push_back("1");
    value.push_back("\""+ip+"\"");
    pthread_mutex_lock(&mutexsql);
    sql->update_table("common",tname,value);
    pthread_mutex_unlock(&mutexsql);
    arduinoIp.clear();
    arduinoIp.append(ip);
}
/**
 * @brief 获取串口读出的温度
 * @return 返回串口读出的温度
 */
float sharedspace::getSerialTemp()
{
    return serial_temp;
}

string sharedspace::getip()
{
    return  ip;
}

string sharedspace::getbroadcast()
{
    return broadcast;
}
void sharedspace::readSN()
{
    SN = new char[12];
    ifstream sn;
    sn.open("/mnt/sn",ios::in);
    sn.getline(SN,12);
    cout<<SN<<endl;
    if(strcmp(SN,"")==0)
    {
        memcpy(SN,"NO NUMBER",9);
    }
    sn.close();
}

string sharedspace::getSN()
{
    return SN;
}

void sharedspace::setCoefficient(float coefficient)
{
    list<string> tname;
    tname.push_back("ID");
    tname.push_back("coefficient");
    list<string> value;
    value.push_back("1");
    value.push_back(common::to_string(coefficient));
    pthread_mutex_lock(&mutexsql);
    sql->update_table("common",tname,value);
    pthread_mutex_unlock(&mutexsql);
    this->coefficient = coefficient;
}
void sharedspace::setyuntai(bool yuntaiauto, int angle)
{
    int isAuto;
    if(yuntaiauto)
        isAuto = 1;
    else
        isAuto = 0;
    list<string> tname;
    tname.push_back("ID");
    tname.push_back("yuntaiauto");
    tname.push_back("yuntaiangle");
    list<string> value;
    value.push_back("1");
    value.push_back(common::to_string(isAuto));
    value.push_back(common::to_string(angle));
    pthread_mutex_lock(&mutexsql);
    sql->update_table("common",tname,value);
    pthread_mutex_unlock(&mutexsql);
    this->yuntaiangle = angle;
    this->yuntai_auto = yuntaiauto;
}
float sharedspace::getCoefficient()
{

    return coefficient;
}
