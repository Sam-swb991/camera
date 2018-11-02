#include "jsonhelper.h"
#include "common.h"
jsonhelper::jsonhelper()
{

}

jsonhelper::jsonhelper(CJsonObject json)
{
    std::string function;
    bool ret = json.Get("function",function);
    if(ret)
    {
        if(function == "rectset")
        {
            rectlen = json["body"]["rect"].GetArraySize();
            rectset = new RECTSET[rectlen];
            for(int i=0;i<rectlen;i++)
            {
                json["body"]["rect"][static_cast<unsigned int>(i)].Get("name",rectset[i].name);
                json["body"]["rect"][static_cast<unsigned int>(i)].Get("x1",rectset[i].rect.x1);
                json["body"]["rect"][static_cast<unsigned int>(i)].Get("y1",rectset[i].rect.y1);
                json["body"]["rect"][static_cast<unsigned int>(i)].Get("x2",rectset[i].rect.x2);
                json["body"]["rect"][static_cast<unsigned int>(i)].Get("y2",rectset[i].rect.y2);
                json["body"]["rect"][static_cast<unsigned int>(i)].Get("highalarm",rectset[i].highalarm);
                json["body"]["rect"][static_cast<unsigned int>(i)].Get("highvalue",rectset[i].highvalue);
                json["body"]["rect"][static_cast<unsigned int>(i)].Get("lowalarm",rectset[i].lowalarm);
                json["body"]["rect"][static_cast<unsigned int>(i)].Get("lowvalue",rectset[i].lowvalue);
                json["body"]["rect"][static_cast<unsigned int>(i)].Get("rapidtempchangealarm",rectset[i].rapidtempchangealarm);
                json["body"]["rect"][static_cast<unsigned int>(i)].Get("rapidtempchangevalue",rectset[i].rapidtempchangevalue);
                json["body"]["rect"][static_cast<unsigned int>(i)].Get("alarm_level",rectset[i].alarm_level);
            }
            cout<<"rectset:"<<rectset->name<<endl;
        }
        else
            return;
    }
    else
    {
        json.Get("code",code);
        std::cout<<"code is:"<<code<<std::endl;
    }

}
int jsonhelper::getCode()
{
    return code;
}

RECTSET *jsonhelper::getRectset(int *len)
{
    *len = rectlen;
    return rectset;
}

CJsonObject jsonhelper::getJson()
{
    return myjson;
}
void jsonhelper::create_temp(WINDOW windos,RECT *rect,int rectnum,POINT point[5120])
{
    const std::string body = "body";
    myjson.Add("function","temp");
    myjson.AddEmptySubObject(body);
    myjson[body].Add("width",WIDTH);
    myjson[body].Add("height",HEIGHT);
    myjson[body].AddEmptySubObject("window");
    myjson[body]["window"].Add("x1",windos.x1);
    myjson[body]["window"].Add("y1",windos.y1);
    myjson[body]["window"].Add("x2",windos.x2);
    myjson[body]["window"].Add("y2",windos.y2);
    myjson[body].AddEmptySubArray("rect");
    CJsonObject tmprect;
    for(int i =0;i<rectnum;i++)
    {
        tmprect.Clear();
        tmprect.Add("name",rect[i].name);
        tmprect.Add("mode",rect[i].mode);
        tmprect.Add("hightemp",rect[i].tempc.highTemp);
        tmprect.Add("lowtemp",rect[i].tempc.lowTemp);
        tmprect.Add("avgtemp",rect[i].tempc.avgTemp);
        tmprect.Add("alarm-level",rect[i].alarmLevel);
        tmprect.Add("alarmmode",rect[i].alarmMode);
        tmprect.Add("x1",rect[i].transrect.x1);
        tmprect.Add("y1",rect[i].transrect.y1);
        tmprect.Add("x2",rect[i].transrect.x2);
        tmprect.Add("y2",rect[i].transrect.y2);
        myjson[body]["rect"].Add(tmprect);

    }
//    myjson[body].AddEmptySubArray("point");
//    CJsonObject tmppoint;
//    for(int i=0;i<5120;i++)
//    {
//        tmppoint.Clear();
//        tmppoint.Add("isshow",point[i].isShow);
//        tmppoint.Add("value",point[i].value);
//        myjson[body]["point"].Add(tmppoint);
//    }

}
