#ifndef TEMPMANAGER_H
#define TEMPMANAGER_H
#include <vector>
#include "mytemp.h"
#include <memory>
using namespace std;
class tempManager
{
public:
    tempManager(unsigned long num);
    bool addTemp(float **);
    bool getFirstTemp(float **);
    void getCalcTemp(float **);
    void setMaxNum(unsigned long);
private:
    unsigned long tempMaxNum;
    vector <unique_ptr <MyTemp>> tempunion;
    bool isFull();
};

#endif // TEMPMANAGER_H
