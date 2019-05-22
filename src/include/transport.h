#ifndef TRANSPORT_H
#define TRANSPORT_H
#include <iostream>
typedef struct rect{
    float x1;
    float y1;
    float x2;
    float y2;
}WINDOW;
typedef struct commontemp{
    float highTemp;
    float lowTemp;
    float avgTemp;
}TEMP_C;
typedef struct point_whole{
    bool whole;
    int rectid;
}POINT_WHOLE;
typedef struct inner_rect{
	int id;
    std::string name;
    struct rect rect;
    int prealarm;
    int prevalue;
    int highalarm;
    int highvalue;
    int linkagealarm;
    int linkagevalue;
    int rapidtempchangealarm;
    int rapidtempchangevalue;
    float radiance;
    float distance;
	int isset;
}RECTSET;
typedef struct trans_rect{
    std::string name;
    int mode;
    struct commontemp tempc;
    int alarmMode;
    struct rect transrect;

}RECT;
typedef struct http_url{
    std::string rectname;
    int alarmmode;
    TEMP_C com_temp;
    std::string ip;
    std::string camera_id;
    std::string time;
    time_t dtime;
}HTTPURL;

typedef struct warning{
    int id;
    int times;
}WARN;
/**
 * @brief 模式enum
 */
enum mode{ADD,DEL,MODIFY,SET,UNSET,GET,GETRTEMP,
          MOVE,GETWIN,IPSET,RECOVERY,REBOOT,UPDATE,
          GETVER,SETTIME,GETSN,SETATEMP,SETARDUINOIP,SETCOEFFICIENT,
          GETCOEFFICIENT,CHECKSD};
enum direction{UP,DOWN,LEFT,RIGHT};

#endif // TRANSPORT_H
