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
    float get_one_temp(E_M_DATA* em_data, S_M_DATA* sm_data);
    void get_all_temp(float **);
    void sort_temp(float **);
    int getTa();
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
    int Ta,VDD_av,PTAT_av;
    void change_sort(void *, void *, size_t len);

};
#endif // CALC_H
