#include "tempmanager.h"
#include <string.h>
#include <algorithm>
#include <numeric>
tempManager::tempManager(unsigned long num)
{
    this->tempMaxNum = num;
}
bool tempManager::isFull()
{
    if(tempunion.size() == tempMaxNum)
        return true;
    else
        return false;
}
bool tempManager::addTemp(float ** temp)
{
    unique_ptr <MyTemp> tmp(new MyTemp(temp));
    if(tempunion.size()<tempMaxNum)
        tempunion.push_back(move(tmp));
    else if(tempunion.size() == tempMaxNum)
    {
        tempunion[0].reset(nullptr);
        tempunion[0].release();      
        tempunion.erase(tempunion.begin());
        tempunion.push_back(move(tmp));
    }
    else
    {
        signed long num = static_cast<long>(tempunion.size()-tempMaxNum+1);
        for(signed long i =0;i<num;i++)
        {
            tempunion[0].reset(nullptr);
            tempunion[0].release();
            tempunion.erase(tempunion.begin());
        }
        tempunion.push_back(move(tmp));
    }
    return isFull();
}

bool tempManager::getFirstTemp(float ** temp)
{
    if(!isFull())
        return false;
    for(int i=0;i<64;++i)
    {
        memcpy(temp[i],tempunion[0]->temp[i],sizeof(float)*80);
    }
    return true;
}

void tempManager::getCalcTemp(float **temp)
{
    if(!isFull())
        return;
    for(int i =0;i<64;++i)
    {
        for(int j=0;j<80;++j)
        {
            vector <float> tmp;
            for(unsigned long k=0;k<tempMaxNum;++k)
            {
                tmp.push_back(tempunion[k]->temp[i][j]);
            }
            tmp.erase(max_element(tmp.begin(),tmp.end()));
            temp[i][j] = accumulate(tmp.begin(),tmp.end(),0.0f)/tmp.size();

        }
    }
}

void tempManager::setMaxNum(unsigned long num)
{
    this->tempMaxNum = num;
}
