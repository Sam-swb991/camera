#ifndef JSONCPP_H
#define JSONCPP_H
#include "transport.h"
#include "json/json.h"
class jsoncpp
{
public:
    jsoncpp();
   // jsoncpp(std::string json);
    std::string getJsonString();
    void create_temp(WINDOW windos, RECT *rect, int rectnum, float **temp);
private:
    Json::Value myjson;
    Json::CharReaderBuilder readbuilder;
    Json::StreamWriterBuilder  writebuilder;
};

#endif // JSONCPP_H
