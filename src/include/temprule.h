#ifndef TEMPRULE_H
#define TEMPRULE_H

#include "transport.h"
#include "sharedspace.h"
class temprule
{
public:
    temprule(RECTSET *rectset,int len,int **temp,sharedspace *ss);
    int *getalarmmode();
    TEMP_C *getcommontemp();
private:
    TEMP_C *tempc;
    int *alarmmode;
    int len;
    sharedspace *ss;

};

#endif // TEMPRULE_H
