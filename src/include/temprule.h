#ifndef TEMPRULE_H
#define TEMPRULE_H

#include "transport.h"
#include "sharedspace.h"
class temprule
{
public:
    temprule(RECTSET *rectset, int len, int **temp, sharedspace *ss, TEMP_C *tempc, int *alarmmode);

private:

    int len;

};

#endif // TEMPRULE_H
