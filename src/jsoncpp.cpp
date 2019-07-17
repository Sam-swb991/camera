#include "jsoncpp.h"
#include "common.h"
#include <memory>
#include <stdio.h>
#include "md5.h"
/**
 * @brief 默认构造函数
 */
jsoncpp::jsoncpp()
{
    direction = -1;
}
/**
 * @brief 构造函数，传入Json::value
 * @param json,Json::value类
 */
jsoncpp::jsoncpp(Json::Value json)
{
    myjson = json;
}
/**
 * @brief 构造函数，传入string,用于解析
 * @param json,json的string数据
 */
jsoncpp::jsoncpp(std::string json)
{
    std::unique_ptr<Json::CharReader> const reader(readbuilder.newCharReader());
    const char * cjson = json.c_str();
    std::string error;
    if(!reader->parse(cjson,cjson+json.size(),&myjson,&error))
    {
        std::cout<<"parse error"<<endl;
        return;
    }
    else
        cout<<"right"<<endl;
    std::string function = myjson["function"].asString();
    if(!function.empty())
    {
        rectlen = static_cast<int>(myjson["body"]["rect"].size());
        cout<<"**********rectlen is :"<<rectlen<<endl;
        if(rectlen != 0)
        {

            if(function == "rectadd")
            {

                for(int i=0;i<rectlen;i++)
                {
                    RECTSET *rect_set = new RECTSET[1];
                    mode = ADD;
                    rect_set->id = myjson["body"]["rect"][static_cast<unsigned int>(i)]["ID"].asInt();
                    rect_set->name.clear();
                    rect_set->name.append(myjson["body"]["rect"][static_cast<unsigned int>(i)]["name"].asString());
                    rect_set->rect.x1 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["x1"].asFloat();
                    rect_set->rect.y1 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["y1"].asFloat();
                    rect_set->rect.x2 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["x2"].asFloat();
                    rect_set->rect.y2 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["y2"].asFloat();
                    rect_set->prealarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["prealarm"].asInt();
                    rect_set->prevalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["prevalue"].asInt();
                    rect_set->highalarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["highalarm"].asInt();
                    rect_set->highvalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["highvalue"].asInt();
                    rect_set->linkagealarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["linkagealarm"].asInt();
                    rect_set->linkagevalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["linkagevalue"].asInt();
                    rect_set->rapidtempchangealarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["rapidtempchangealarm"].asInt();
                    rect_set->rapidtempchangevalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["rapidtempchangevalue"].asInt();
                    rect_set->radiance = myjson["body"]["rect"][static_cast<unsigned int>(i)]["radiance"].asFloat();
                    rect_set->distance = myjson["body"]["rect"][static_cast<unsigned int>(i)]["distance"].asFloat();
                    rectset.push_back(*rect_set);
                    delete [] rect_set;

                }
                cout<<"rectset:"<<rectset[0].name<<endl;
            }

            else if(function == "rectdel")
            {
                for(size_t i=0;i<(size_t)rectlen;i++)
                {
                    RECTSET *rect_set = new RECTSET[1];
                    mode = DEL;
                    rect_set->id = myjson["body"]["rect"][static_cast<unsigned int>(i)]["ID"].asInt();
                    rect_set->name = "";
                    rectset.push_back(*rect_set);
                    delete []rect_set;
                }

            }
            else if(function == "rectset")
            {
                for(size_t i=0;i<(size_t)rectlen;i++)
                {
                    RECTSET *rect_set = new RECTSET[1];
                    mode = SET;
                    rect_set->id = myjson["body"]["rect"][static_cast<unsigned int>(i)]["ID"].asInt();
                    rect_set->name = "";
                    rectset.push_back(*rect_set);
                    delete []rect_set;
                }
            }
            else if(function == "rectunset")
            {
                for(size_t i=0;i<(size_t)rectlen;i++)
                {
                    RECTSET *rect_set = new RECTSET[1];
                    mode = UNSET;
                    rect_set->id = myjson["body"]["rect"][static_cast<unsigned int>(i)]["ID"].asInt();
                    rect_set->name = "";
                    rectset.push_back(*rect_set);
                    delete []rect_set;
                }
            }
            else if(function == "rectmodify")
            {
                for(size_t i=0;i<(size_t)rectlen;i++)
                {

                    RECTSET *rect_set = new RECTSET[1];
                    mode = MODIFY;
                    rect_set->id = myjson["body"]["rect"][static_cast<unsigned int>(i)]["ID"].asInt();
                    rect_set->name.clear();
                    rect_set->name.append(myjson["body"]["rect"][static_cast<unsigned int>(i)]["name"].asString());
                    rect_set->rect.x1 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["x1"].asFloat();
                    rect_set->rect.y1 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["y1"].asFloat();
                    rect_set->rect.x2 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["x2"].asFloat();
                    rect_set->rect.y2 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["y2"].asFloat();
                    rect_set->prealarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["prealarm"].asInt();
                    rect_set->prevalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["prevalue"].asInt();
                    rect_set->highalarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["highalarm"].asInt();
                    rect_set->highvalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["highvalue"].asInt();
                    rect_set->linkagealarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["linkagealarm"].asInt();
                    rect_set->linkagevalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["linkagevalue"].asInt();
                    rect_set->rapidtempchangealarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["rapidtempchangealarm"].asInt();
                    rect_set->rapidtempchangevalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["rapidtempchangevalue"].asInt();
                    rect_set->radiance = myjson["body"]["rect"][static_cast<unsigned int>(i)]["radiance"].asFloat();
                    rect_set->distance = myjson["body"]["rect"][static_cast<unsigned int>(i)]["distance"].asFloat();
                    rectset.push_back(*rect_set);
                    delete [] rect_set;


                }
            }
            else
                return;
        }

        if(function == "getrect")
        {
            mode = GET;
            cout<<"mode is :"<<mode<<endl;
        }
        else if(function == "getrealtemp")
        {
            mode = GETRTEMP;
        }
        else if(function == "rectmove")
        {
            direction = myjson["body"]["direction"].asInt();
            mode = MOVE;
        }
        else if(function == "getwindow")
        {
            mode = GETWIN;
        }
        else if(function == "ipset")
        {
            ip=myjson["body"]["ip"].asString();
            cout<<"ip is:"<<ip<<endl;
            mode = IPSET;
        }
        else if(function == "recovery")
        {
            mode = RECOVERY;
        }
        else if(function == "reboot")
        {
            mode = REBOOT;
        }
        else if(function == "update")
        {
            mode = UPDATE;
            int updatemode =myjson["body"]["updatemode"].asInt();
            if(updatemode == 1)
            {
                shellcmd = "/mnt/download/down.sh ";
                ip=myjson["body"]["ip"].asString();
                shellcmd +=ip +" ";
                int file = myjson["body"]["file"].asInt();
                if(file == 1)
                shellcmd +="camera ";
                else if(file == 2)
                {
                    shellcmd +="IRIPC ";
                }
                string md5 = myjson["body"]["md5"].asString();
                shellcmd +=md5;
                cout<<"shellcmd:"<<shellcmd<<endl;
            }
        }
        else if(function == "getver")
        {
            mode = GETVER;
        }
        else if(function == "settime")
        {
            mode = SETTIME;
            string time = myjson["body"]["time"].asString();
            shellcmd.clear();
            shellcmd = "sh /mnt/time/set.sh "+time;
            cout<<"shellcmd:"<<shellcmd<<endl;
        }
        else if(function == "getsn")
        {
            mode = GETSN;
        }
        else if(function == "setatemp")
        {
            mode = SETATEMP;
            atempmode = myjson["body"]["mode"].asInt();
            if(atempmode == 1)
            {
                ambienttemp = myjson["body"]["atemp"].asFloat();
            }
        }
        else if(function == "setarduinoip")
        {
            mode = SETARDUINOIP;
            arduinoip = myjson["body"]["ip"].asString();
        }
        else if(function == "setcoefficient")
        {
            mode = SETCOEFFICIENT;
            coefficient = myjson["body"]["coefficient"].asFloat();
        }
        else if(function =="getcoefficient")
        {
            mode = GETCOEFFICIENT;
        }
        else if(function == "checksd")
        {
            mode = CHECKSD;
        }
        else if(function == "yuntaicontrl")
        {
            mode = YUNTAICONTRL;
            yuntaimode = myjson["body"]["mode"].asInt();
            if(yuntaimode == 3)
            {
                angle = myjson["body"]["angle"].asInt();
            }

        }
        else if(function == "yuntaistate")
        {
            mode = YUNTAISTATE;
        }


    }


    else
    {
        code = myjson["code"].asInt();
        std::cout<<"code is:"<<code<<std::endl;
    }
    cout<<"end json"<<endl;
    delete cjson;


}
/**
 * @brief 获取格式化的json数据
 * @return 返回json的string字符串
 */
jsoncpp::jsoncpp(string json,blockchainData *data)
{
    std::unique_ptr<Json::CharReader> const reader(readbuilder.newCharReader());
    const char * cjson = json.c_str();
    std::string error;
    if(!reader->parse(cjson,cjson+json.size(),&myjson,&error))
    {
        std::cout<<"parse error"<<endl;
        return;
    }
    else
        cout<<"right"<<endl;
    //preHash,SN,devName,IP,data,Alarm,SensorStatus,Status,time,Hash

    data->setPrehash(myjson["preHash"].asString());
    data->setSN(myjson["SN"].asString());
    data->setDevName(myjson["devName"].asString());
    data->setIP(myjson["IP"].asString());
    data->setData(myjson["Data"].asString());
    data->setAlarm(myjson["Alarm"].asInt());
    data->setSensorStatus(myjson["Sensorstatus"].asInt());
    data->setStatus(myjson["Status"].asInt());
    data->setTime(myjson["Time"].asString());
    preHash = myjson["Hash"].asString();
    data->setHash(preHash);
}
std::string jsoncpp::toStyledString()
{
    return myjson.toStyledString();
}
/**
 * @brief 获取code
 * @return 返回code
 */
int jsoncpp::getCode()
{
    return code;
}
/**
 * @brief 获取mode
 * @return 返回mode
 */
int jsoncpp::getMode()
{
    return mode;
}
/**
 * @brief 获取direction
 * @return 返回directionpthread_mutex_lock(&ss->mutexSerial);
 */
float jsoncpp::getatemp()
{
    return ambienttemp;
}

float jsoncpp::getcoefficient()
{
    return coefficient;
}
int jsoncpp::getDirection()
{
    return direction;
}
int jsoncpp::getatempmode()
{
    return atempmode;
}

int jsoncpp::getyuntaimode()
{
    return yuntaimode;
}
string jsoncpp::getip()
{
    cout<<"get ip:"<<ip<<endl;
    return ip;
}

string jsoncpp::getarduinoip()
{
    return arduinoip;
}

int jsoncpp::getyuntaiangle()
{
    return angle;
}

string jsoncpp::getpreHash()
{
    return preHash;
}
/**
 * @brief 获取RECTSET结构体
 * @param len,获取的RECTSET结构体个数
 * @return 返回RECTSET的容器
 */
std::vector<RECTSET> jsoncpp::getRectset(int *len)
{
    cout<<"jsoncpp getrect"<<endl;
    *len = rectlen;
    return rectset;
}
/**
 * @brief 穿件需要传输的json数据
 * @param windos，窗口大小
 * @param rect，区域的信息
 * @param rectnum，区域的个数
 * @param temp，温度的二维数组
 */
void jsoncpp::create_temp(WINDOW windos, std::vector<RECT> rect, int rectnum,list<int> linkagealarm,list<int> highalarm,list<int> prealarm, float **temp)
{
    myjson["function"] = "temp";
    myjson["body"]["height"] = HEIGHT;
    myjson["body"]["width"] = WIDTH;
    myjson["body"]["window"]["x1"] = (double)windos.x1;
    myjson["body"]["window"]["y1"] = (double)windos.y1;
    myjson["body"]["window"]["x2"] = (double)windos.x2;
    myjson["body"]["window"]["y2"] = (double)windos.y2;
    cout<<"rect num is :"<<rectnum<<endl;
    for(size_t i =0;i<(size_t)rectnum;++i)
    {
        myjson["body"]["rect"][(int)i]["name"] = rect[i].name;
        myjson["body"]["rect"][(int)i]["mode"] = rect[i].mode;
        myjson["body"]["rect"][(int)i]["hightemp"] = (double)rect[i].tempc.highTemp;
        myjson["body"]["rect"][(int)i]["lowtemp"] = (double)rect[i].tempc.lowTemp;
        myjson["body"]["rect"][(int)i]["avgtemp"] = (double)rect[i].tempc.avgTemp;
        myjson["body"]["rect"][(int)i]["alarmmode"] = rect[i].alarmMode;
        myjson["body"]["rect"][(int)i]["x1"] = (double)rect[i].transrect.x1;
        myjson["body"]["rect"][(int)i]["y1"] = (double)rect[i].transrect.y1;
        myjson["body"]["rect"][(int)i]["x2"] = (double)rect[i].transrect.x2;
        myjson["body"]["rect"][(int)i]["y2"] = (double)rect[i].transrect.y2;

    }
    for(int i=0;i<5120;i++)
    {
        myjson["body"]["point"][i] =static_cast<int>(temp[i/80][i%80]);
    }
    int num = (int)highalarm.size();
    list<int>::iterator j= highalarm.begin();
    for(int i =0;i<num;++i)
    {
        myjson["body"]["highalarm"][i] = *j;

        ++j;
    }
    num = (int)prealarm.size();
    j = prealarm.begin();
    for(int i =0;i<num;++i)
    {
        myjson["body"]["prealarm"][i] = *j;

        ++j;
    }
    j = linkagealarm.begin();
    num = (int)linkagealarm.size();
    for(int i =0;i<num;++i)
    {
        myjson["body"]["linkagealarm"][i] = *j;

        ++j;
    }
//    cout<<"linkagealarm:"<<myjson["body"]["linkagealarm"].toStyledString()<<endl;
   // cout<<"prealarm:"<<myjson["body"]["prealarm"].toStyledString()<<endl;
   // cout<<"highalarm:"<<myjson["body"]["highalarm"].toStyledString()<<endl;
  // cout<<"rect:"<<myjson["body"]["rect"].toStyledString()<<endl;
//    cout<<"point:"<<myjson["body"]["point"].toStyledString()<<endl;
    cout<<"window:"<<myjson["body"]["window"].toStyledString()<<endl;
}
/**
 * @brief 把json转化为字符串
 * @return 返回转化的字符串
 */
string jsoncpp::getJsonString()
{
    writebuilder.settings_["indentation"] = "";
    string str = Json::writeString(writebuilder, myjson);
    return str;
}
/**
 * @brief 通过RECTSET结构体,封装成需要发送的json对象
 * @param rectset,RECTSET结构体的容器
 * @param len，RECTSET结构体个数
 */
void jsoncpp::create_rect(std::vector<RECTSET> rectset ,int len)
{
    myjson["function"] = "rect";
    for(size_t i =0;i<(size_t)len;++i)
    {
        myjson["body"]["rect"][(int)i]["id"] = rectset[i].id;
        myjson["body"]["rect"][(int)i]["name"] = rectset[i].name;
        myjson["body"]["rect"][(int)i]["prealarm"] = rectset[i].prealarm;
        myjson["body"]["rect"][(int)i]["prevalue"] = rectset[i].prevalue;
        myjson["body"]["rect"][(int)i]["highalarm"] = rectset[i].highalarm;
        myjson["body"]["rect"][(int)i]["highvalue"] = rectset[i].highvalue;
        myjson["body"]["rect"][(int)i]["linkagealarm"] = rectset[i].linkagealarm;
        myjson["body"]["rect"][(int)i]["linkagevalue"] = rectset[i].linkagevalue;
        myjson["body"]["rect"][(int)i]["rapidtempchangealarm"] = rectset[i].rapidtempchangealarm;
        myjson["body"]["rect"][(int)i]["rapidtempchangevalue"] = rectset[i].rapidtempchangevalue;
        myjson["body"]["rect"][(int)i]["radiance"] = (double)rectset[i].radiance;
        myjson["body"]["rect"][(int)i]["distance"] = (double)rectset[i].distance;
        myjson["body"]["rect"][(int)i]["isset"] = rectset[i].isset;
        myjson["body"]["rect"][(int)i]["x1"] = (double)rectset[i].rect.x1;
        myjson["body"]["rect"][(int)i]["y1"] = (double)rectset[i].rect.y1;
        myjson["body"]["rect"][(int)i]["x2"] = (double)rectset[i].rect.x2;
        myjson["body"]["rect"][(int)i]["y2"] = (double)rectset[i].rect.y2;
    }
}
/**
 * @brief 通过串口读出的问题数据，封装成需要发送的json对象
 * @param realtemp,串口读出的温度
 */
void jsoncpp::create_udp_data(HTTPURL * url,string prehash)
{
    myjson["preHash"] = prehash;
    myjson["SN"] = url->camera_id;
    myjson["devName"] = "TempSensor";
    myjson["IP"] = url->ip;
    myjson["Data"] = common::to_string(url->com_temp.highTemp);
    myjson["Alarm"] = url->alarmmode;
    myjson["Sensor_status"] = 0;
    myjson["Status"] = 1;
    myjson["Time"] = url->time;
    string jsonstr = this->getJsonString();
    unsigned char hash_str[16]={0};
    string_md5((unsigned char *)jsonstr.c_str(),(int)jsonstr.length(),hash_str);
    myjson["Hash"] = common::charToHexString((const char *)hash_str,16);

}
void jsoncpp::create_real_temp(float realtemp)
{
    myjson["function"] = "realtemp";
    myjson["body"]["temp"] = (double)realtemp;
}
/**
 * @brief 通过code数据，封装成需要发送的json对象
 * @param code,code数据
 */
void jsoncpp::create_code(int code)
{
    myjson["code"] = code;
}
/**
 * @brief 通过WINDOW结构体，封装成需要发送的json对象
 * @param window,WINDOW结构体,即红外窗口在可见光中的坐标
 */
void jsoncpp::create_window(WINDOW window)
{
    myjson["function"] = "window";
    myjson["body"]["x1"] = (double)window.x1;
    myjson["body"]["y1"] = (double)window.y1;
    myjson["body"]["x2"] = (double)window.x2;
    myjson["body"]["y2"] = (double)window.y2;
}
void jsoncpp::create_atemp(bool havemodel)
{
    myjson["function"] = "setatemp";
    int model;
    if(havemodel)
        model = 1;
    else
        model = 0;
    myjson["body"]["havemodel"] = model;

}

void jsoncpp::create_ver()
{
    myjson["ver"] = VER;
}
int jsoncpp::execshellcmd()
{
    FILE *fp;
    char buffer[80];
    memset(buffer,0,80);
    fp = popen(shellcmd.c_str(),"r");
    fgets(buffer,sizeof(buffer),fp);
    printf("cmdbuf:%s\n",buffer);
    pclose(fp);
    if(strcmp(buffer,"ok\n") == 0)
    {
        return 0;
    }
    else if(strcmp(buffer,"fail\n") == 0)
    {
        return -1;
    }
    else if(strcmp(buffer,"error\n") == 0)
    {
        return -2;
    }
    return -3;
}

int jsoncpp::settime()
{
    FILE *fp;
    char buffer[80];
    memset(buffer,0,80);
    fp = popen(shellcmd.c_str(),"r");
    fgets(buffer,sizeof(buffer),fp);
    printf("cmdbuf:%s\n",buffer);
    pclose(fp);
    return 0;
}

void jsoncpp::create_SN(string SN)
{
    myjson["SN"] = SN;
}

void jsoncpp::create_coefficient(float coefficient)
{
    myjson["function"] = "getcoefficient";
    myjson["body"]["coefficient"] = (double)coefficient;
}
