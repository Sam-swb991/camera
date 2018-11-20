#ifndef SHAREDSPACE_H
#define SHAREDSPACE_H
#include "transport.h"
#include "myqueue.h"
#include <pthread.h>
#include "sqlhelper.h"
class temprule;
class sharedspace
{
public:
    sharedspace();
    void storeTemp(float **temp);
    int getTemp(int **);
    void SetRect(RECTSET *rectset, int len, int mode);
    RECT * GetRect(float **temp, int Ta);
    int getRectlen();
    void resetSql();
    void getMode();
    pthread_mutex_t mutex,mutexsql;
    sqlHelper *sql;
private:
    RECT *rect =nullptr;
    RECTSET *rectset =nullptr ;
    int rectsetlen;
    temprule *trule;
	list<string> tableName;
	bool set;
    int mode;
};

#endif // SHAREDSPACE_H
