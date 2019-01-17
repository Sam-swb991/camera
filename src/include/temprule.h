#ifndef TEMPRULE_H
#define TEMPRULE_H

#include "transport.h"
#include "sharedspace.h"
#include <vector>
class temprule
{
public:
    temprule(std::vector<RECTSET> rectset, int len, WINDOW windows, float **temp, sharedspace *ss, TEMP_C *tempc, int *alarmmode, int Ta);
    list<int> getHighAlarm();
    list<int> getPreAlarm();
    list<int> getLinkageAlarm();
private:
    void temp_compensation(float &temp, int Ta, double a);
    void all_temp_selector(float **temp, float env_temp, int Ta);
    int len;
    void initWhole();
    POINT_WHOLE point[5120];
    const int TEMP_DIFFER = 3;
    list<int> linkagealarm;
    list<int> highalarm;
    list<int> prealarm;

};

#endif // TEMPRULE_H
