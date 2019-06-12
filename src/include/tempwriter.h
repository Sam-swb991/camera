#ifndef TEMPWRITER_H
#define TEMPWRITER_H

#include <fstream>
#include "transport.h"
using namespace std;
class tempwriter
{
public:
    enum MODE{common,alarm};
    tempwriter(int mode);
    void write(TEMP_C temp_c, string rectname, int ptat, int alarmmode);
    void close();
private:
    ofstream file;
    bool is_open;
    string fpath = "/root/temp.txt";
    string sd_path = "/mnt/sd_card/temp.txt";
    string alarm_path = "/mnt/sd_card/alarm.txt";
    int mode;
};

#endif // TEMPWRITER_H
