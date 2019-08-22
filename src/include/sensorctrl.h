#ifndef SENSORCTRL_H
#define SENSORCTRL_H

#include <spictrl.h>
typedef struct sensor_muti_data{
    unsigned short ele_offset;
    unsigned short pixel;
}S_M_DATA;
typedef struct sensor_common_data{
    unsigned short PTAT_av;
    unsigned short VDD_av;
}S_C_DATA;
class sensorCtrl
{
public:
    sensorCtrl(spiCtrl *spictrl);
    void init_sensor();
    void read_pixel_PTAT();
    void read_electricaloffset();
    void read_VDD();
    void read_all();
    void printvalue();
    S_M_DATA* get_muti_data(int num);
    S_C_DATA* get_common_data();
private:
    S_M_DATA *m_data;
    S_C_DATA *c_data;
    spiCtrl *spictrl;
    unsigned short PTAT[8];
    unsigned short VDD[8];
    unsigned short pixel[5120];
    unsigned short electrical_offset[1280];
    void read_pixel_select_BLOCK(int BLOCK,uint8_t *data,unsigned short *pixel);
    void change_BLOCK(uint8_t BLOCK, uint8_t VDD);
};

#endif // SENSORCTRL_H
