#ifndef SHAREDSPACE_H
#define SHAREDSPACE_H
#include "transport.h"
#include <pthread.h>
#include "sqlhelper.h"
#include <vector>
#include "threadPool.h"
class temprule;
class sharedspace
{
public:
    sharedspace();
    void storeTemp(float **temp);
    int getTemp(int **);
    void SetRect(std::vector<RECTSET> rectset, int len, int mode);
    std::vector<RECT> GetRect(float **temp,WINDOW windows, int Ta);
    int getRectlen();
    void resetSql();
    void getMode();
    float getSerialTemp();
    void setSerialTemp(float temp);
    list<int >getHighAlarm();
    list<int> getPreAlarm();
    list<int> getLinkageAlarm();
    void setWindow(int direction);
    WINDOW getWindow();
    pthread_mutex_t mutex,mutexsql,mutexSerial,mutexurl;
    sqlHelper *sql;
    ClThreadPool *threadpool;
    string getip();
    HTTPURL * url;
private:
    //RECT *rect =nullptr;
    //RECTSET *rectset =nullptr ;
    std::vector <RECTSET> rectset;
    std::vector <RECT> rect;
    int rectsetlen;
    temprule *trule;
	list<string> tableName;
    WINDOW window;
    bool set,setwindow;
    int ID;
    int mode;
    list <int >highalarm;
    list <int> prealarm;
    list <int> linkagealarm;
    float serial_temp;
    string ip;


};

#endif // SHAREDSPACE_H
