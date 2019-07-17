#ifndef SHAREDSPACE_H
#define SHAREDSPACE_H
#include "transport.h"
#include <pthread.h>
#include "sqlhelper.h"
#include <vector>
#include "threadPool.h"
#include "tempmanager.h"
#include "sqlblockchain.h"
class temprule;
class sharedspace
{
public:
    sharedspace();
    bool storeTemp(float **temp);
    int getTemp(float **);
    void SetRect(std::vector<RECTSET> rectset, int len, int mode);
    std::vector<RECT> GetRect(float **temp, WINDOW windows, int Ta, bool writetemp);
    int getRectlen();
    void resetSql();
    void getMode();
    float getSerialTemp();
    float getCoefficient();
    void setSerialTemp(float temp);
    void setArduinoIp(string ip);
    void setCoefficient(float);
    void setyuntai(bool yuntaiauto,int angle);
    list<int >getHighAlarm();
    list<int> getPreAlarm();
    list<int> getLinkageAlarm();
    void setWindow(int direction);
    WINDOW getWindow();
    int getyuntaiangle();
    pthread_mutex_t mutex,mutexsql,mutexSerial,mutexurl,mutexarduinoUrl,mutexsendalarm;
    sqlHelper *sql;
    sqlblockchain *sqlbc;
    ClThreadPool *threadpool;
    string getip();
    string getbroadcast();
    HTTPURL * url,*arduinoUrl;
    string getSN();
    int warningtimes;
    bool needsendtoarduino;
    bool useserialtemp;
    bool haveserialmodel;
    string arduinoIp;
    bool yuntai_auto;
    string preHash;
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
    int yuntaiangle;
    char *SN;
    list <int >highalarm;
    list <int> prealarm;
    list <int> linkagealarm;
    float serial_temp;
    void readSN();
    string ip,broadcast;
    tempManager *sixteen_t;


};

#endif // SHAREDSPACE_H
