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
    void copy(struct inner_rect& rect)
    {
        name = rect.name;
        transrect = rect.rect;
    }
}RECT;

/**
 * @brief 模式enum
 */
enum mode{ADD,DEL,MODIFY,SET,UNSET,GET};

#endif // TRANSPORT_H
