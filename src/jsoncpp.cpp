#include "jsoncpp.h"
#include "common.h"
jsoncpp::jsoncpp()
{

}

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

string jsoncpp::getJson_data()
{
    string str = Json::writeString(builder, myjson);
    return str;
}
