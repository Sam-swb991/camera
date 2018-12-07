#ifndef SHAREDSPACE_H
#define SHAREDSPACE_H
#include "transport.h"
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
    RECT * GetRect(float **temp,WINDOW windows, int Ta);
    int getRectlen();
    void resetSql();
    void getMode();
    float getSerialTemp();
    void setSerialTemp(float temp);
    list<int >getHighAlarm();
    list<int> getPreAlarm();
    list<int> getLinkageAlarm();
    pthread_mutex_t mutex,mutexsql,mutexSerial;
    sqlHelper *sql;
private:
    RECT *rect =nullptr;
    RECTSET *rectset =nullptr ;
    int rectsetlen;
    temprule *trule;
	list<string> tableName;
	bool set;
    int mode;
    list <int >highalarm;
    list <int> prealarm;
    list <int> linkagealarm;
    float serial_temp;
};

#endif // SHAREDSPACE_H
