#ifndef JSONCPP_H
#define JSONCPP_H
#include "transport.h"
#include "json/json.h"
class jsoncpp
{
public:
    jsoncpp();
    std::string getJson_data();
    void create_temp(WINDOW windos, RECT *rect, int rectnum, float **temp);
private:
    Json::Value myjson;
    Json::StreamWriterBuilder  builder;
};

#endif // JSONCPP_H
