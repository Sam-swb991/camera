#ifndef SHAREDSPACE_H
#define SHAREDSPACE_H
#include "transport.h"
#include <pthread.h>
#include "sqlhelper.h"
#include <vector>
#include "threadPool.h"
#include "tempmanager.h"
class temprule;
class sharedspace
{
public:
    sharedspace();
    bool storeTemp(float **temp);
    int getTemp(float **);
    void SetRect(std::vector<RECTSET> rectset, int len, int mode);
    std::vector<RECT> GetRect(float **temp,WINDOW windows, int Ta);
    int getRectlen();
    void resetSql();
    void getMode();
    float getSerialTemp();
    float getCoefficient();
    void setSerialTemp(float temp);
    void setArduinoIp(string ip);
    void setCoefficient(float);
    list<int >getHighAlarm();
    list<int> getPreAlarm();
    list<int> getLinkageAlarm();
    void setWindow(int direction);
    WINDOW getWindow();
    pthread_mutex_t mutex,mutexsql,mutexSerial,mutexurl,mutexarduinoUrl,mutexsendalarm;
    sqlHelper *sql;
    ClThreadPool *threadpool;
    string getip();
    HTTPURL * url,*arduinoUrl;
    string getSN();
    int warningtimes;
    bool needsendtoarduino;
    bool useserialtemp;
    bool haveserialmodel;
    string arduinoIp;
    std::vector<WARN> regionwarning;
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
    float coefficient;
    int ID;
    int mode;
    char *SN;
    list <int >highalarm;
    list <int> prealarm;
    list <int> linkagealarm;
    float serial_temp;
    void readSN();
    string ip;
    tempManager *sixteen_t;


};

#endif // SHAREDSPACE_H
