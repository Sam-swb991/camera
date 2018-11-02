#include "temprule.h"
#include "common.h"
#include <cmath>
#include <cstring>
#include <pthread.h>
#include <stdio.h>
temprule::temprule(RECTSET *rectset, int len, int **temp, sharedspace *ss, TEMP_C * tempc, int *alarmmode)
{

    memset(alarmmode,0,sizeof(int)*static_cast<size_t>(len));
    memset(tempc,0,sizeof(TEMP_C)*static_cast<size_t>(len));
    int **ftemp = new int*[64];
    for(int i=0; i<64; ++i)
    {
        ftemp[i] = new int[80];

        memset(ftemp[i],0,80*sizeof(int));
    }
    int ret = 0;
    pthread_mutex_lock(&ss->mutexsql);
    ret = ss->getTemp(ftemp);
    pthread_mutex_unlock(&ss->mutexsql);

    int num;

    for(int k = 0 ;k<len;++k)
    {
        int start_x = static_cast<int>(rectset[k].rect.x1*WIDTH);
        int start_y = static_cast<int>(rectset[k].rect.y1*HEIGHT);
        int end_x = static_cast<int>(ceil(static_cast<double>(rectset[k].rect.x2*WIDTH)));
        int end_y = static_cast<int>(ceil(static_cast<double>(rectset[k].rect.y2*HEIGHT)));

        cout<<"startx = "<<start_x<<" starty = "<<start_y<<" end_x = "<<end_x<<" end_y = "<<end_y<<endl;
        num=0;
        tempc[k].lowTemp = 1000;
        tempc[k].highTemp = -100;

        for(int i=start_y;i<=end_y;++i)
        {
            for(int j=start_x;j<=end_x;++j)
            {
                if(tempc[k].highTemp<temp[i][j])
                    tempc[k].highTemp = temp[i][j];
                if(tempc[k].lowTemp>temp[i][j])
                    tempc[k].lowTemp = temp[i][j];
                tempc[k].avgTemp +=temp[i][j];
                num++;
                if(rectset[k].highalarm == 1)
                {
                    if(temp[i][j]>rectset->highvalue)
                    {
                        alarmmode[k] |=0x01;
                    }
                }
                if(rectset[k].lowalarm == 1)
                {
                    if(temp[i][j]<rectset->lowvalue)
                    {
                        alarmmode[k] |=0x02;
                    }
                }
                if(rectset[k].rapidtempchangealarm == 1)
                {

                    if(ret ==0)
                    {
                        if(abs(temp[i][j]-ftemp[i][j])>rectset->rapidtempchangevalue)
                        {
                            alarmmode[k] |=0x04;
                        }
                    }

                }
            }
        }
        tempc[k].avgTemp /=num;
    }
    for(int i=0; i<64; ++i)
    {
        delete []ftemp[i];
    }
    delete []ftemp;
}

//int* temprule::getalarmmode()
//{
//    return alarmmode;
//}

//TEMP_C *temprule::getcommontemp()
//{
//    return tempc;
//}
