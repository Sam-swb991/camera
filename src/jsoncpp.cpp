#include "jsoncpp.h"
#include "common.h"
#include <memory>
/**
 * @brief 构造函数
 */
jsoncpp::jsoncpp()
{

}
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
            rectset = new RECTSET[rectlen];
            if(function == "rectadd")
            {

                    for(int i=0;i<rectlen;i++)
                    {
                        mode = ADD;
                        rectset[i].id = myjson["body"]["rect"][static_cast<unsigned int>(i)]["ID"].asInt();
                        rectset[i].name = myjson["body"]["rect"][static_cast<unsigned int>(i)]["name"].asString();
                        rectset[i].rect.x1 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["x1"].asFloat();
                        rectset[i].rect.y1 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["y1"].asFloat();
                        rectset[i].rect.x2 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["x2"].asFloat();
                        rectset[i].rect.y2 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["y2"].asFloat();
                        rectset[i].prealarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["prealarm"].asInt();
                        rectset[i].prevalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["prevalue"].asInt();
                        rectset[i].highalarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["highalarm"].asInt();
                        rectset[i].highvalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["highvalue"].asInt();
                        rectset[i].linkagealarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["linkagealarm"].asInt();
                        rectset[i].linkagevalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["linkagevalue"].asInt();
                        rectset[i].rapidtempchangealarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["rapidtempchangealarm"].asInt();
                        rectset[i].rapidtempchangevalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["rapidtempchangevalue"].asInt();
                        rectset[i].radiance = myjson["body"]["rect"][static_cast<unsigned int>(i)]["radiance"].asInt();
                        rectset[i].distance = myjson["body"]["rect"][static_cast<unsigned int>(i)]["distance"].asInt();

                    }
                    cout<<"rectset:"<<rectset->name<<endl;
                }

            else if(function == "rectdel")
            {
                for(int i=0;i<rectlen;i++)
                {
                    mode = DEL;
                    rectset[i].id = myjson["body"]["rect"][static_cast<unsigned int>(i)]["ID"].asInt();
                }

            }
            else if(function == "rectset")
            {
                for(int i=0;i<rectlen;i++)
                {
                    mode = SET;
                    rectset[i].id = myjson["body"]["rect"][static_cast<unsigned int>(i)]["ID"].asInt();
                }
            }
            else if(function == "rectunset")
            {
                for(int i=0;i<rectlen;i++)
                {
                    mode = UNSET;
                    rectset[i].id = myjson["body"]["rect"][static_cast<unsigned int>(i)]["ID"].asInt();
                }
            }
            else if(function == "rectmodify")
            {
                for(int i=0;i<rectlen;i++)
                {
                    mode = MODIFY;
                    rectset[i].id = myjson["body"]["rect"][static_cast<unsigned int>(i)]["ID"].asInt();
                    rectset[i].name = myjson["body"]["rect"][static_cast<unsigned int>(i)]["name"].asString();
                    rectset[i].rect.x1 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["x1"].asFloat();
                    rectset[i].rect.y1 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["y1"].asFloat();
                    rectset[i].rect.x2 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["x2"].asFloat();
                    rectset[i].rect.y2 = myjson["body"]["rect"][static_cast<unsigned int>(i)]["y2"].asFloat();
                    rectset[i].prealarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["prealarm"].asInt();
                    rectset[i].prevalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["prevalue"].asInt();
                    rectset[i].highalarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["highalarm"].asInt();
                    rectset[i].highvalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["highvalue"].asInt();
                    rectset[i].linkagealarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["linkagealarm"].asInt();
                    rectset[i].linkagevalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["linkagevalue"].asInt();
                    rectset[i].rapidtempchangealarm = myjson["body"]["rect"][static_cast<unsigned int>(i)]["rapidtempchangealarm"].asInt();
                    rectset[i].rapidtempchangevalue = myjson["body"]["rect"][static_cast<unsigned int>(i)]["rapidtempchangevalue"].asInt();
                    rectset[i].radiance = myjson["body"]["rect"][static_cast<unsigned int>(i)]["radiance"].asInt();
                    rectset[i].distance = myjson["body"]["rect"][static_cast<unsigned int>(i)]["distance"].asInt();
                }
            }
            else if(function == "getrect")
            {
                mode = GET;
                cout<<"mode inner is :"<<mode<<endl;
            }
            else
                return;
        }
        else
        {
            if(function == "getrect")
            {
                mode = GET;
                cout<<"mode is :"<<mode<<endl;
            }
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
std::string jsoncpp::toStyledString()
{
    return myjson.toStyledString();
}
int jsoncpp::getCode()
{
    return code;
}
int jsoncpp::getMode()
{
    return mode;
}
RECTSET * jsoncpp::getRectset(int *len)
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
void jsoncpp::create_temp(WINDOW windos, RECT *rect, int rectnum,list<int> linkagealarm,list<int> highalarm,list<int> prealarm, float **temp)
{
    myjson["function"] = "temp";
    myjson["body"]["height"] = HEIGHT;
    myjson["body"]["width"] = WIDTH;
    myjson["body"]["window"]["x1"] = (double)windos.x1;
    myjson["body"]["window"]["y1"] = (double)windos.y1;
    myjson["body"]["window"]["x2"] = (double)windos.x2;
    myjson["body"]["window"]["y2"] = (double)windos.y2;
    cout<<"rect num is :"<<rectnum<<endl;
    for(int i =0;i<rectnum;++i)
    {
         myjson["body"]["rect"][i]["name"] = rect[i].name;
         myjson["body"]["rect"][i]["mode"] = rect[i].mode;
         myjson["body"]["rect"][i]["hightemp"] = (double)rect[i].tempc.highTemp;
         myjson["body"]["rect"][i]["lowtemp"] = (double)rect[i].tempc.lowTemp;
         myjson["body"]["rect"][i]["avgtemp"] = (double)rect[i].tempc.avgTemp;
         myjson["body"]["rect"][i]["alarmmode"] = rect[i].alarmMode;
         myjson["body"]["rect"][i]["x1"] = (double)rect[i].transrect.x1;
         myjson["body"]["rect"][i]["y1"] = (double)rect[i].transrect.y1;
         myjson["body"]["rect"][i]["x2"] = (double)rect[i].transrect.x2;
         myjson["body"]["rect"][i]["y2"] = (double)rect[i].transrect.y2;

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
    cout<<"linkagealarm:"<<myjson["body"]["linkagealarm"].toStyledString()<<endl;
    //cout<<"prealarm:"<<myjson["body"]["prealarm"].toStyledString()<<endl;
    //cout<<"highalarm:"<<myjson["body"]["highalarm"].toStyledString()<<endl;
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

void jsoncpp::create_rect(RECTSET *rectset ,int len)
{
    myjson["function"] = "rect";
    for(int i =0;i<len;++i)
    {
         myjson["body"]["rect"][i]["id"] = rectset[i].id;
         myjson["body"]["rect"][i]["name"] = rectset[i].name;
         myjson["body"]["rect"][i]["prealarm"] = rectset[i].prealarm;
         myjson["body"]["rect"][i]["prevalue"] = rectset[i].prevalue;
         myjson["body"]["rect"][i]["highalarm"] = rectset[i].highalarm;
         myjson["body"]["rect"][i]["highvalue"] = rectset[i].highvalue;
         myjson["body"]["rect"][i]["linkagealarm"] = rectset[i].linkagealarm;
         myjson["body"]["rect"][i]["linkagevalue"] = rectset[i].linkagevalue;
         myjson["body"]["rect"][i]["rapidtempchangealarm"] = rectset[i].rapidtempchangealarm;
         myjson["body"]["rect"][i]["rapidtempchangevalue"] = rectset[i].rapidtempchangevalue;
         myjson["body"]["rect"][i]["radiance"] = rectset[i].highalarm;
         myjson["body"]["rect"][i]["distance"] = rectset[i].highvalue;
         myjson["body"]["rect"][i]["isset"] = rectset[i].isset;
         myjson["body"]["rect"][i]["x1"] = (double)rectset[i].rect.x1;
         myjson["body"]["rect"][i]["y1"] = (double)rectset[i].rect.y1;
         myjson["body"]["rect"][i]["x2"] = (double)rectset[i].rect.x2;
         myjson["body"]["rect"][i]["y2"] = (double)rectset[i].rect.y2;

    }

}

void jsoncpp::create_code(int code)
{
    myjson["code"] = code;
}
