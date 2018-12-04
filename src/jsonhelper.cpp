#include "jsonhelper.h"
#include "common.h"
/**
 * @brief 构造函数
 */
jsonhelper::jsonhelper()
{

}
/**
 * @brief 构造函数，解析json对象，储存mode，或者code
 * @param json,传入CJsonObject对象
 */
jsonhelper::jsonhelper(CJsonObject json)
{
    std::string function;
    bool ret = json.Get("function",function);
    if(ret)
    {
        rectlen = json["body"]["rect"].GetArraySize();
        if(rectlen != 0)
        {
        	rectset = new RECTSET[rectlen];
	        if(function == "rectadd")
	        {

	                for(int i=0;i<rectlen;i++)
	                {
                        mode = ADD;
	                	json["body"]["rect"][static_cast<unsigned int>(i)].Get("id",rectset[i].id);
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
	       
			else if(function == "rectdel")
			{
				for(int i=0;i<rectlen;i++)
	            {
                    mode = DEL;
					json["body"]["rect"][static_cast<unsigned int>(i)].Get("id",rectset[i].id);
				}
				
			}
			else if(function == "rectset")
			{
				for(int i=0;i<rectlen;i++)
	            {
                    mode = SET;
					json["body"]["rect"][static_cast<unsigned int>(i)].Get("id",rectset[i].id);
				}
			}
            else if(function == "rectunset")
            {
                for(int i=0;i<rectlen;i++)
                {
                    mode = UNSET;
                    json["body"]["rect"][static_cast<unsigned int>(i)].Get("id",rectset[i].id);
                }
            }
			else if(function == "rectmodify")
			{
				for(int i=0;i<rectlen;i++)
	            {
                    mode = MODIFY;
	               	json["body"]["rect"][static_cast<unsigned int>(i)].Get("id",rectset[i].id);
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
                cout<<"mode inner is :"<<mode<<endl;
            }
        }
   	}

 
    else
    {
        json.Get("code",code);
        std::cout<<"code is:"<<code<<std::endl;
    }

}
/**
 * @brief 获取code
 * @return 返回code
 */
int jsonhelper::getCode()
{
    return code;
}
/**
 * @brief 获取mode
 * @return 返回mode
 */
int jsonhelper::getMode()
{
    return mode;
}
/**
 * @brief 获取区域数据，以及区域个数
 * @param len,区域长度
 * @return 返回区域数据
 */
RECTSET *jsonhelper::getRectset(int *len)
{
    *len = rectlen;
    return rectset;
}
/**
 * @brief 获取json数据
 * @return 返回CJsonObject 对象
 */
CJsonObject jsonhelper::getJson()
{
    return myjson;
}

//void jsonhelper::create_temp(WINDOW windos,RECT *rect,int rectnum,float **temp)
//{
//    const std::string body = "body";
//    myjson.Add("function","temp");
//    myjson.AddEmptySubObject(body);
//    myjson[body].Add("width",WIDTH);
//    myjson[body].Add("height",HEIGHT);
//    myjson[body].AddEmptySubObject("window");
//    myjson[body]["window"].Add("x1",windos.x1);
//    myjson[body]["window"].Add("y1",windos.y1);
//    myjson[body]["window"].Add("x2",windos.x2);
//    myjson[body]["window"].Add("y2",windos.y2);
//    myjson[body].AddEmptySubArray("rect");
//    CJsonObject tmprect;
//    for(int i =0;i<rectnum;i++)
//    {
//        tmprect.Clear();
//        tmprect.Add("name",rect[i].name);
//        tmprect.Add("mode",rect[i].mode);
//        tmprect.Add("hightemp",rect[i].tempc.highTemp);
//        tmprect.Add("lowtemp",rect[i].tempc.lowTemp);
//        tmprect.Add("avgtemp",rect[i].tempc.avgTemp);
//        tmprect.Add("alarm-level",rect[i].alarmLevel);
//        tmprect.Add("alarmmode",rect[i].alarmMode);
//        tmprect.Add("x1",rect[i].transrect.x1);
//        tmprect.Add("y1",rect[i].transrect.y1);
//        tmprect.Add("x2",rect[i].transrect.x2);
//        tmprect.Add("y2",rect[i].transrect.y2);
//        myjson[body]["rect"].Add(tmprect);

//    }
//    myjson[body].AddEmptySubArray("point");
//    //    CJsonObject tmppoint;
//    for(int i=0;i<5120;i++)
//    {
//        myjson[body]["point"].Add(static_cast<int>(temp[i/80][i%80]));
//    }

//}
/**
 * @brief 把RECTSET结构体数据拼接成json
 * @param rectset，RECTSET结构体数据（区域数据）
 * @param len，区域长度
 */
void jsonhelper::create_rect(RECTSET * rectset, int len)
{
	myjson.Add("function","rect");
	myjson.AddEmptySubObject("body");
	myjson["body"].AddEmptySubArray("rect");
	CJsonObject tmprect;
    for(int i =0;i<len;i++)
    {
        tmprect.Clear();
		tmprect.Add("id",rectset[i].id);
        tmprect.Add("name",rectset[i].name);
        tmprect.Add("highalarm",rectset[i].highalarm);
        tmprect.Add("highvalue",rectset[i].highvalue);
        tmprect.Add("lowalarm",rectset[i].lowalarm);
        tmprect.Add("lowvalue",rectset[i].lowvalue);
		tmprect.Add("rapidtempchangealarm",rectset[i].rapidtempchangealarm);
        tmprect.Add("rapidtempchangevalue",rectset[i].rapidtempchangevalue);
        tmprect.Add("alarm_level",rectset[i].alarm_level);
        tmprect.Add("x1",rectset[i].rect.x1);
        tmprect.Add("y1",rectset[i].rect.y1);
        tmprect.Add("x2",rectset[i].rect.x2);
        tmprect.Add("y2",rectset[i].rect.y2);
		tmprect.Add("isset",rectset[i].isset);
        myjson["body"]["rect"].Add(tmprect);

    }
	
}
