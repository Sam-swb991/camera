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
typedef struct inner_rect{
	int id;
    std::string name;
    struct rect rect;
    int highalarm;
    int highvalue;
    int lowalarm;
    int lowvalue;
    int rapidtempchangealarm;
    int rapidtempchangevalue;
    int alarm_level;
	int isset;
}RECTSET;
typedef struct trans_rect{
    std::string name;
    int mode;
    struct commontemp tempc;
    int alarmLevel;
    int alarmMode;
    struct rect transrect;
    void copy(struct inner_rect& rect)
    {
        name = rect.name;
        transrect = rect.rect;
        alarmLevel = rect.alarm_level;
    }
}RECT;

/**
 * @brief 模式enum
 */
enum mode{ADD,DEL,MODIFY,SET,GET};

#endif // TRANSPORT_H
