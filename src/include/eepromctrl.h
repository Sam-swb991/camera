#ifndef EEPROMCTRL_H
#define EEPROMCTRL_H
#include "spictrl.h"
union u{
    float f;
    uint8_t byte[4];
};
typedef struct erom_muti_data{
    signed char thGrad;
    short vddCompGrad;
    short vddCompOff;
    short thOffSet;
    unsigned short p;
}E_M_DATA;
typedef struct erom_common_data{
    float pixCmin;
    float pixCmax;
    float PTAT_gradient;
    float PTAT_offset;
    uint8_t gradscale;
    uint8_t epsilon;
    uint8_t vddScGrad;
    uint8_t vddScOff;
    uint8_t nrOfDefPix;
    uint8_t GlobalOff;
    unsigned short VDDTH1;
    unsigned short VDDTH2;
    unsigned short PTATTH1;
    unsigned short PTATTH2;
    unsigned short GlobalGrain;
    int Device;
}E_C_DATA;
class eepromCtrl
{
public:
    eepromCtrl(spiCtrl *spictrl);
    void readAll();
    void printvalue();
    E_C_DATA* get_common_data();
    E_M_DATA* get_muti_data(int num);
private:
    E_C_DATA *c_data;
    E_M_DATA *m_data;
    float read_float(uint8_t addr_H,uint8_t addr_L);
    int read_int(uint8_t addr_H,uint8_t addr_L);
    unsigned short read_short(uint8_t addr_H,uint8_t addr_L);
    spiCtrl *spictrl;
    float PixCmin,PixCmax,PTAT_gradient,PTAT_offset;
    uint8_t gradscale,Arraytype,epsilon,VddScGrad,VddScOff,GlobalOff,NrOfDefPix,*DeadPixMask,ThGrad[5120];
    uint8_t MBIT,BIAS,CLK,BPA,PU,MBIT_user,BIAS_user,CLK_user,BPA_user,PU_user;
    unsigned short VDDTH1,VDDTH2,PTATTH1,PTATTH2,GlobalGain,TN;
    unsigned short *DeadPixAdr,VddCompGrad[1280],VddCompOff[1280],ThOffset[5120],P[5120];
    int DeviceID;
};

#endif // EEPROMCTRL_H
