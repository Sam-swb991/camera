#ifndef TEMPRULE_H
#define TEMPRULE_H

#include "transport.h"
#include "sharedspace.h"
class temprule
{
public:
    temprule(RECTSET *rectset, int len, float **temp, sharedspace *ss, TEMP_C *tempc, int *alarmmode,int Ta);

private:
    void temp_compensation(float &temp, int Ta, double a);
    int len;

};

#endif // TEMPRULE_H
