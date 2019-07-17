#ifndef BLOCKCHAINDATA_H
#define BLOCKCHAINDATA_H
#include <cstring>
#include <iostream>
using namespace std;
class blockchainData
{
public:
    blockchainData();
    void setPrehash(string);
    void setSN(string);
    void setDevName(string);
    void setIP(string);
    void setData(string);
    void setAlarm(int);
    void setSensorStatus(int);
    void setStatus(int);
    void setTime(string);
    void setHash(string);
    string getPrehash();
    string getSN();
    string getDevName();
    string getIP();
    string getData();
    int getAlarm();
    int getSensorstatus();
    int getStatus();
    string getTime();
    string getHash();
private:
    string prehash;
    string SN;
    string devName;
    string IP;
    string Data;
    int Alarm;
    int SensorStatus;
    int Status;
    string time;
    string hash;
};

#endif // BLOCKCHAINDATA_H
