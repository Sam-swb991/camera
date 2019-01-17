#include "temprule.h"
#include "common.h"
#include <cmath>
#include <cstring>
#include <pthread.h>
#include <stdio.h>
#include "httprequest.h"
/**
 * @brief 构造函数，温度策略
 * @param rectset，RECTSET对象，区域信息
 * @param len，区域个数
 * @param temp，温度二维数组
 * @param ss，sharedspace对象
 * @param tempc，需要传出的TEMP_C结构体
 * @param alarmmode，需要传出的alarmmode数组
 * @param Ta，Ta值传入
 */
temprule::temprule(std::vector<RECTSET> rectset, int len, WINDOW windows, float **temp, sharedspace *ss, TEMP_C * tempc, int *alarmmode, int Ta)
{
    cout<<"temprule"<<endl;
    memset(alarmmode,0,sizeof(int)*static_cast<size_t>(len));
    memset(tempc,0,sizeof(TEMP_C)*static_cast<size_t>(len));
    int **ftemp = new int*[64];
    for(int i=0; i<64; ++i)
    {
        ftemp[i] = new int[80];

        memset(ftemp[i],0,80*sizeof(int));
    }
    initWhole();
    int ret = 0;
    pthread_mutex_lock(&ss->mutexsql);
    ret = ss->getTemp(ftemp);
    pthread_mutex_unlock(&ss->mutexsql);
    int num;
    float win_x = windows.x2 -windows.x1;
    float win_y = windows.y2 - windows.y1;
    float win_mid_x = win_x/2+windows.x1;
    pthread_mutex_lock(&ss->mutexSerial);
    float serial_temp = ss->getSerialTemp();
    cout<<"serial temp"<<serial_temp<<endl;
    pthread_mutex_unlock(&ss->mutexSerial);
   // all_temp_selector(temp,serial_temp,Ta);
    int whole_id =-1;
    for(size_t k = 0 ;k<static_cast<size_t>(len);++k)
    {
        if(rectset[k].id!=-1)
        {
            cout<<"rect x"<<rectset[k].rect.x2<<"rect y"<<rectset[k].rect.y2<<endl;
            float rect_x = rectset[k].rect.x2 - rectset[k].rect.x1;
            float rect_mid_x = rect_x/2+rectset[k].rect.x1;
            float real_mid_x = 2*win_mid_x-rect_mid_x;
            float real_end = real_mid_x+(rect_x/2)-windows.x1;
            int start_x = static_cast<int>((real_mid_x-(rect_x/2)-windows.x1)*WIDTH/win_x);
            int start_y = static_cast<int>((rectset[k].rect.y1-windows.y1)*HEIGHT/win_y);
            int end_x = static_cast<int>(ceil(static_cast<double>(real_end*WIDTH/win_x)));
            int end_y = static_cast<int>(ceil(static_cast<double>((rectset[k].rect.y2-windows.y1)*HEIGHT/win_y)));
            cout<<"real_mid_x"<<real_mid_x<<"rect_x"<<rect_x<<"real_end "<<real_end<<endl;
            if(start_x<0)
                start_x = 0;
            if(start_y<0)
                start_y = 0;
            if(end_x>80)
                end_x = 80;
            if(end_y>64)
                end_y = 64;
            cout<<"startx = "<<start_x<<" starty = "<<start_y<<" end_x = "<<end_x<<" end_y = "<<end_y<<endl;
            num=0;
            tempc[k].lowTemp = 1000;
            tempc[k].highTemp = -100;

            for(int i=start_y;i<end_y;++i)
            {
                for(int j=start_x;j<end_x;++j)
                {
                    temp_compensation(temp[i][j],Ta,static_cast<double>(rectset[k].radiance));
                    if(tempc[k].highTemp<temp[i][j])
                        tempc[k].highTemp = temp[i][j];
                    if(tempc[k].lowTemp>temp[i][j])
                        tempc[k].lowTemp = temp[i][j];
                    tempc[k].avgTemp +=temp[i][j];
                    num++;
                    int num_whole = i*80+j;
                    point[num_whole].whole =false;
                    point[num_whole].rectid = rectset[k].id;
                    float delta_temp = temp[i][j] - serial_temp;
                    if(rectset[k].linkagealarm == 1)
                    {

                        if(delta_temp>rectset[k].linkagevalue)
                        {
                            alarmmode[k] |=0x04;
                            linkagealarm.push_back(num_whole);
                        }
                    }
                    if(rectset[k].highalarm == 1)
                    {
                        if(delta_temp>rectset[k].highvalue)
                        {
                            alarmmode[k] |=0x02;
                        }
                        if(delta_temp>rectset[k].highvalue&&delta_temp<=rectset[k].linkagevalue)
                        {
                            highalarm.push_back(num_whole);
                        }
                    }
                    if(rectset[k].prealarm == 1)
                    {
                        if(delta_temp>rectset[k].prevalue)
                        {
                            alarmmode[k] |=0x01;
                        }
                        if(delta_temp>rectset[k].prevalue&&delta_temp<=rectset[k].highvalue)
                        {
                            prealarm.push_back(num_whole);
                        }
                    }
                    if(rectset[k].rapidtempchangealarm == 1)
                    {

                        if(ret ==0)
                        {
                            if(abs(temp[i][j]-ftemp[i][j])>rectset[k].rapidtempchangevalue)
                            {
                                alarmmode[k] |=0x08;
                            }
                        }
                    }
                }
            }
            tempc[k].avgTemp /=num;
            cout<<"avg :"<<tempc[k].avgTemp<<"low :"<<tempc[k].lowTemp<<"high :"<<tempc[k].highTemp<<endl;
            if(alarmmode[k] !=0)
            {
                pthread_mutex_lock(&ss->mutexurl);
                ss->url->alarmmode = alarmmode[k];
                ss->url->com_temp = tempc[k];
                ss->url->camera_id = "hora_188";
                ss->url->ip = ss->getip();
                ss->url->rectname = rectset[k].name;
                ss->url->time = time(nullptr);
                httpRequest * http = new httpRequest(ss->threadpool);
                http->start(ss->url);
                pthread_mutex_unlock(&ss->mutexurl);
            }
        }
        else
        {
            whole_id = (int)k;
        }
    }
    if(whole_id !=-1)
    {
        cout<<"whole set**********************************"<<endl;
        tempc[whole_id].lowTemp = 1000;
        tempc[whole_id].highTemp = -100;
        num =0;
        for(int i=0;i<64;++i)
        {
            for(int j =0;j<80;++j)
            {

                int num_whole = i*80+j;

                if(point[num_whole].whole)
                {

                    if(tempc[whole_id].highTemp<temp[i][j])
                        tempc[whole_id].highTemp = temp[i][j];
                    if(tempc[whole_id].lowTemp>temp[i][j])
                        tempc[whole_id].lowTemp = temp[i][j];
                    tempc[whole_id].avgTemp +=temp[i][j];
                    num++;
                    float delta_temp = temp[i][j] - serial_temp;
                    if(rectset[(size_t)whole_id].linkagealarm == 1)
                    {
                        if(delta_temp>rectset[(size_t)whole_id].linkagevalue)
                        {
                            alarmmode[whole_id] |=0x04;
                            linkagealarm.push_back(num_whole);
                        }
                    }
                    if(rectset[(size_t)whole_id].highalarm == 1)
                    {
                        if(delta_temp>rectset[(size_t)whole_id].highvalue)
                        {
                            alarmmode[whole_id] |=0x02;
                        }
                        if(delta_temp>rectset[(size_t)whole_id].highvalue&&delta_temp<=rectset[(size_t)whole_id].linkagevalue)
                        {
                            highalarm.push_back(num_whole);
                        }
                    }
                    if(rectset[(size_t)whole_id].prealarm == 1)
                    {

                        if(delta_temp>rectset[(size_t)whole_id].prevalue)
                        {
                            alarmmode[whole_id] |=0x01;
                        }
                        if(delta_temp>rectset[(size_t)whole_id].prevalue&&delta_temp<=rectset[(size_t)whole_id].highvalue)
                        {
                            prealarm.push_back(num_whole);
                        }
                    }
                    if(rectset[(size_t)whole_id].rapidtempchangealarm == 1)
                    {

                        if(ret ==0)
                        {
                            if(abs(temp[i][j]-ftemp[i][j])>rectset[(size_t)whole_id].rapidtempchangevalue)
                            {
                                alarmmode[whole_id] |=0x08;
                            }
                        }
                    }
                }
            }
        }
        tempc[whole_id].avgTemp /=num;
        if(alarmmode[whole_id] !=0)
        {
            pthread_mutex_lock(&ss->mutexurl);
            ss->url->alarmmode = alarmmode[whole_id];
            ss->url->com_temp = tempc[whole_id];
            ss->url->camera_id = "hora_188";
            ss->url->ip = ss->getip();
            ss->url->rectname = rectset[(size_t)whole_id].name;
            ss->url->time = time(nullptr);
            cout<<"http ------time ------"<<ss->url->time<<endl;
            httpRequest * http = new httpRequest(ss->threadpool);
            http->start(ss->url);
            pthread_mutex_unlock(&ss->mutexurl);
        }
    }
    for(int i=0; i<64; ++i)
    {
        delete []ftemp[i];
    }
    delete []ftemp;
    cout<<prealarm.size()<<"   "<<highalarm.size()<<"   "<<linkagealarm.size()<<endl;
}
/**
 * @brief 温度补偿
 * @param temp，温度值
 * @param Ta，Ta值
 * @param a，反射系数，范围为0-1之间
 */
void temprule::temp_compensation(float &temp,int Ta,double a)
{
    float T0 = Ta/10 - 273.15f;
    double to = pow(temp,4.09)-(1-a)*pow(T0,4.09);
    if(to>0)
    {
        double ta = to*(1/a);
        double tb = pow(ta,1/4.09);
        temp = static_cast<float>(tb);
    }
}
/**
 * @brief 温度补偿
 * @param temp,温度二维数组
 * @param env_temp,环境温度
 */
void temprule::all_temp_selector(float **temp,float env_temp,int Ta)
{
    float T0 = Ta/10-273.15f;
    for(int i=0;i<64;++i)
    {
        for(int j=0;j<80;++j)
        {
            if(temp[i][j]<(T0+TEMP_DIFFER))
            //if(abs(temp[i][j]-env_temp)<10)
                temp[i][j] = env_temp;
        }
    }
}
/**
 * @brief 初始化每个像素的参数
 */
void temprule::initWhole()
{
    for(int i =0;i<5120;++i)
    {
        point[i].whole = true;
        point[i].rectid = -1;
    }
}
/**
 * @brief 获取高温报警点的坐标
 * @return 返回高温报警的点的坐标(0~5119)
 */
list<int> temprule::getHighAlarm()
{
    return highalarm;
}
/**
 * @brief 获取预警点的坐标
 * @return 返回预警的点的坐标(0~5119)
 */
list<int> temprule::getPreAlarm()
{
    return prealarm;
}
/**
 * @brief 获取联动报警点的坐标
 * @return 返回联动报警的点的坐标(0~5119)
 */
list<int> temprule::getLinkageAlarm()
{
    return linkagealarm;
}
