#ifndef CALC_H
#define CALC_H
#include "eepromctrl.h"
#include "sensorctrl.h"
//int get_one_temp(E_M_DATA* em_data, S_M_DATA* sm_data, S_C_DATA * sc_data);
//#define DEBUG 1
class calc
{
public:
    calc();
    ~calc();
    int get_one_temp(E_M_DATA* em_data,S_M_DATA* sm_data,S_C_DATA * sc_data);
    void get_all_temp(int **);
    void sort_temp(int **);
private:
    double pow_vddscgrad,pow_vddscoff,pow_gradscale;
    float pixC_delta,pixCmin,PTAT_gradient,PTAT_offset;
    float pixC_muti;
    int VDD_delta,PTAT_delta;
    unsigned short PTATTH1,VDDTH1;
    eepromCtrl *ectrl;
    sensorCtrl *sctrl;
    spiCtrl *spi;
    int DeadPixAdr[8];
    void change_sort(int *, int *, size_t len);

};
#endif // CALC_H
