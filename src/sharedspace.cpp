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
    if(pthread_mutex_init(&mutexsql, nullptr) != 0)
    {
        perror("mutexmq init error!");
    }
    rectsetlen = 0;
    sql = new sqlHelper();
    sql->clear_table("temperature");
	tableName.push_back("ID");
	tableName.push_back("name");
	tableName.push_back("x1");
	tableName.push_back("y1");
	tableName.push_back("x2");
	tableName.push_back("y2");
	tableName.push_back("highalarm");
	tableName.push_back("highvalue");
	tableName.push_back("lowalarm");
	tableName.push_back("lowvalue");
	tableName.push_back("rapidtempchangealarm");
	tableName.push_back("rapidtempchangevalue");
	tableName.push_back("alarm_level");
	set = true;
	

}

RECT * sharedspace::GetRect(float **temp,int Ta)
{
    cout<<"start get rect!"<<endl;
	if(set)
	{
		delete this->rectset;
		this->rectset = sql->getRect(&rectsetlen);
		set = false;
		
	}
	cout<<"rect :"<<rectset[0].id<<" "<<rectset[0].name<<" "<<rectset[0].rect.x1<<" "<<rectset[0].rect.y1<<" "<<
	rectset[0].rect.x2<<" "<<rectset[0].rect.y2<" "<<rectset[0].highalarm<<" "<<rectset[0].highvalue<<endl;
    if(rectsetlen != 0)
    {
        TEMP_C *tempc = new TEMP_C[rectsetlen];
        int *alarmmode = new int[rectsetlen];
        trule = new temprule(this->rectset,rectsetlen,temp,this,tempc,alarmmode,Ta);
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
void sharedspace::SetRect(RECTSET *rectset,int len)
{
    rectsetlen = len;
    if(rectsetlen != 0)
    {

        delete this->rectset;
		int mode = rectset[0].mode;
		switch (mode)
		{
			case ADD:
			case MODIFY:{
				list<string> value;
				for(int i=0;i<len;++i)
				{
					
					value.push_back(common::to_string(rectset[i].id));
					value.push_back(rectset[i].name);
					value.push_back(common::to_string(rectset[i].rect.x1));
					value.push_back(common::to_string(rectset[i].rect.y1));
					value.push_back(common::to_string(rectset[i].rect.x2));
					value.push_back(common::to_string(rectset[i].rect.y2));
					value.push_back(common::to_string(rectset[i].highalarm));
					value.push_back(common::to_string(rectset[i].highvalue));
					value.push_back(common::to_string(rectset[i].lowalarm));
					value.push_back(common::to_string(rectset[i].lowvalue));
					value.push_back(common::to_string(rectset[i].rapidtempchangealarm));
					value.push_back(common::to_string(rectset[i].rapidtempchangevalue));
					value.push_back(common::to_string(rectset[i].alarm_level));
					
				}
				if(mode == ADD)
				{
					sql->insert_table("rect",tableName,value)
				}
				else if(mode == MODIFY)
				{
					sql->update_table("rect",tableName,value);
				}
			}break;
			case DEL:{
				string sqlstr = "delete from rect where ID = ";
				sqlstr+= common::to_string(rectset[i].id);
				sqlstr+=";";
				sql->exec(sqlstr);
			}break;
			case SET:{
				string sqlstr = "update rect set isset = 1 where ID = ";
				sqlstr+= common::to_string(rectset[i].id);
				sqlstr+=";";
				sql->exec(sqlstr);
				set = true;
			}break;
			
			default:break;	
		}
		
    }
        
        

    


}

int sharedspace::getRectlen()
{
    return rectsetlen;
}

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
            cout<<static_cast<int>(temp[i][j])<<" ";
            if(i == HEIGHT-1 && j ==WIDTH-1)
            {
                ss<<"'";
            }
            else
                ss<<",";
            //usleep(1);
        }
        cout<<endl;
    }
    list<string> name,value;
    name.push_back("tempData");
    name.push_back("time");
    value.push_back(ss.str());

    sprintf(str,"%ld",time(nullptr));
    cout<<"time is:"<<str<<endl;
    value.push_back(str);
    sql->insert_table("temperature",name,value);
    t<<(time(nullptr)-70);
    string delsql ="time < "+ t.str();
    sql->delete_table("temperature",delsql);

}

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
void sharedspace::resetSql()
{
    sql->release();
}
int sharedspace::getTemp(int **temp)
{
    cout<<"gettemp"<<endl;
    stringstream ss;
    stringstream t;
    list <string> ret;
    ss<<(time(nullptr)-60);
    string sqlstr = "select tempData from temperature where time < "+ ss.str()+" order by time DESC LIMIT 1 OFFSET 0;";
    ret = sql->select_table(sqlstr);
    if(!ret.empty()&&*ret.begin()!="error")
    {
        string str = *ret.begin();
        vector <string> res = split(str,",");
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
