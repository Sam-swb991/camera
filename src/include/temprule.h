#ifndef TEMPRULE_H
#define TEMPRULE_H

#include "transport.h"
#include "sharedspace.h"
class temprule
{
public:
    temprule(RECTSET *rectset, int len, WINDOW windows, float **temp, sharedspace *ss, TEMP_C *tempc, int *alarmmode, int Ta);
    list<int> getAlarmnum();
private:
    void temp_compensation(float &temp, int Ta, double a);
    void all_temp_selector(float **temp,float env_temp);
    int len;
    const int TEMP_DIFFER = 3;
    list<int> alarmnum;

};

#endif // TEMPRULE_H
