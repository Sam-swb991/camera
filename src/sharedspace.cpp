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
using namespace std;
sharedspace::sharedspace()
{
    if(pthread_mutex_init(&mutex, nullptr) != 0)
    {
        perror("mutex init error!");
    }
    if(pthread_mutex_init(&mutexmq, nullptr) != 0)
    {
        perror("mutexmq init error!");
    }
    rectsetlen = 0;
    sql = new sqlHelper();
}

RECT * sharedspace::GetRect(int **temp)
{

    temprule *trule = new temprule(this->rectset,rectsetlen,temp,this);
    int *alarmmode  = trule->getalarmmode();

    TEMP_C *tempc = trule->getcommontemp();
    if(rectsetlen == 0)
    {
        return nullptr;
    }
    rect = new RECT[rectsetlen];
    for(int i =0;i<rectsetlen;i++)
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
        rect[i].copy(rectset[i]);
        rect[i].tempc = tempc[i];
    }
    return rect;
}
void sharedspace::SetRect(RECTSET *rectset,int len)
{

    this->rectset = rectset;
    rectsetlen = len;


}

int sharedspace::getRectlen()
{
    return rectsetlen;
}

void sharedspace::storeTemp(int **temp)
{
    stringstream ss;
    stringstream t;
    char str[256] = { 0 };
    ss<<"'";
    for(int i = 0;i<HEIGHT;i++)
    {
        for(int j=0;j<WIDTH;j++)
        {
            ss<<temp[i][j];
            if(i == HEIGHT-1 && j ==WIDTH-1)
            {
                ss<<"'";
            }
            else
                ss<<",";
            //usleep(1);
        }
    }
    list<string> name,value;
    name.push_back("tempData");
    name.push_back("time");
    value.push_back(ss.str());

    sprintf(str,"%ld",time(nullptr));
    //cout<<"time is:"<<str<<endl;
    value.push_back(str);
    sql->insert_table("temperature",name,value);
    t<<(time(nullptr)-70);
    string delsql ="time < "+ t.str();
    sql->delete_table("temperature",delsql);
}

vector<string> split(const string& str, const string& delim) {
    vector<string> res;
    if("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char * strs = new char[str.length() + 1] ; //不要忘了
    strcpy(strs, str.c_str());

    char * d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while(p) {
        string s = p; //分割得到的字符串转换为string类型
        res.push_back(s); //存入结果数组
        p = strtok(nullptr, d);
    }
    return res;
}

int sharedspace::getTemp(int **temp)
{
    stringstream ss;
    stringstream t;
    list <string >ret;
    ss<<(time(nullptr)-60);
    string sqlstr = "select tempData from temperature where time < "+ ss.str()+"order by time DESC LIMIT 1 OFFSET 0;";
    ret = sql->select_table(sqlstr);
    if(!ret.empty()&&*ret.begin()!="error")
    {
        string str = *ret.begin();
        vector <string> res = split(str,",");
        for(int i = 0;i<64;i++)
        {
            for(int j = 0;j<80;j++)
            {
                temp[i][j] = atoi(res[static_cast<unsigned long>(i*80+j)].c_str());
            }
        }

        return 0;
    }
    else
        return -1;
}
