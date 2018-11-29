#include "jsoncpp.h"
#include "common.h"
#include <memory>
/**
 * @brief 构造函数
 */
jsoncpp::jsoncpp()
{

}
//jsoncpp::jsoncpp(std::string json)
//{
//    std::unique_ptr<Json::CharReader> const reader(readbuilder.newCharReader());
//    const char * cjson = json.c_str();
//    std::string error;
//    if(!reader->parse(cjson,cjson+json.size(),&myjson,&error))
//    {
//        std::cout<<"parse error"<<endl;
//        return;
//    }
//    std::string function = myjson["function"].asString();


//}
/**
 * @brief 穿件需要传输的json数据
 * @param windos，窗口大小
 * @param rect，区域的信息
 * @param rectnum，区域的个数
 * @param temp，温度的二维数组
 */
void jsoncpp::create_temp(WINDOW windos, RECT *rect, int rectnum, float **temp)
{
    myjson["function"] = "temp";
    myjson["body"]["height"] = HEIGHT;
    myjson["body"]["width"] = WIDTH;
    myjson["body"]["window"]["x1"] = (double)windos.x1;
    myjson["body"]["window"]["y1"] = (double)windos.y1;
    myjson["body"]["window"]["x2"] = (double)windos.x2;
    myjson["body"]["window"]["y2"] = (double)windos.y2;
    for(int i =0;i<rectnum;++i)
    {
         myjson["body"]["rect"][i]["name"] = rect->name;
         myjson["body"]["rect"][i]["mode"] = rect->mode;
         myjson["body"]["rect"][i]["hightemp"] = (double)rect->tempc.highTemp;
         myjson["body"]["rect"][i]["lowtemp"] = (double)rect->tempc.lowTemp;
         myjson["body"]["rect"][i]["avgtemp"] = (double)rect->tempc.avgTemp;
         myjson["body"]["rect"][i]["alarm-level"] = rect->alarmLevel;
         myjson["body"]["rect"][i]["alarmmode"] = rect->alarmMode;
         myjson["body"]["rect"][i]["x1"] = (double)rect->transrect.x1;
         myjson["body"]["rect"][i]["y1"] = (double)rect->transrect.y1;
         myjson["body"]["rect"][i]["x2"] = (double)rect->transrect.x2;
         myjson["body"]["rect"][i]["y2"] = (double)rect->transrect.y2;

    }
    for(int i=0;i<5120;i++)
    {
        myjson["body"]["point"][i] =static_cast<int>(temp[i/80][i%80]);
    }

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
