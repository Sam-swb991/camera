#include "jsoncpp.h"
#include "common.h"
#include <memory>
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
                    rect_set->name = myjson["body"]["rect"][static_cast<unsigned int>(i)]["name"].asString();
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
                    rect_set->name = myjson["body"]["rect"][static_cast<unsigned int>(i)]["name"].asString();
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
            ip = myjson["body"]["ip"].asString();
            mode = IPSET;
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
 * @return 返回direction
 */
int jsoncpp::getDirection()
{
    return direction;
}

string jsoncpp::getip()
{
    return ip;
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
