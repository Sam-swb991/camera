#ifndef SHAREDSPACE_H
#define SHAREDSPACE_H
#include "transport.h"
#include "myqueue.h"
#include <pthread.h>
#include "sqlhelper.h"
class sharedspace
{
public:
    sharedspace();
    void storeTemp(int **temp);
    int getTemp(int **);
    void SetRect(RECTSET *rectset,int len);
    RECT * GetRect(int **temp);
    int getRectlen();
    pthread_mutex_t mutex,mutexmq;
private:
    RECT *rect =nullptr;
    RECTSET *rectset =nullptr ;
    int rectsetlen;
    sqlHelper *sql;
};

#endif // SHAREDSPACE_H
