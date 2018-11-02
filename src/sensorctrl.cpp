#include "sensorctrl.h"
#include <cstring>
#include "common.h"
sensorCtrl::sensorCtrl(spiCtrl* spictrl)
{
    this->spictrl = spictrl;

}

void sensorCtrl::init_sensor()
{

    spictrl->spi_sensor_write(0x01,0x01);//wake up PTAT BOLCK 0
    spictrl->spi_sensor_write(0x03,0x1c);
    spictrl->spi_sensor_write(0x04,0x05);
    spictrl->spi_sensor_write(0x05,0x05);
    spictrl->spi_sensor_write(0x06,0x15);
    spictrl->spi_sensor_write(0x07,0x0c);
    spictrl->spi_sensor_write(0x08,0x0c);
    spictrl->spi_sensor_write(0x01,0x09);//start sensor
    usleep(30000);//delay 30ms
    spictrl->spi_sensor_read_byte(0x02);
}
//void sensorCtrl::read_pixel_select_BLOCK(int BLOCK,uint8_t *data,unsigned short *pixel)
//{
//    for(int i = 0;i<8;i++)
//    {
//        memcpy(pixel+5040-BLOCK*640-80*i,data+2+160*i,160);
//    }
//}
void sensorCtrl::read_pixel_select_BLOCK(int BLOCK,uint8_t *data,unsigned short *pixel)
{
    for(int i = 0;i<8;++i)
    {
        memcpy(pixel+2560+BLOCK*640+80*i,data+2+160*i,160);
    }
}
void sensorCtrl::change_BLOCK(uint8_t BLOCK,uint8_t VDD)
{
    uint8_t value = ((0x09|VDD<<2)|BLOCK<<4)&0xff; //BLOCK|start|VDD
    spictrl->spi_sensor_write(0x01,value);//start BLOCK 1
    usleep(30000);//delay 30ms
    spictrl->spi_sensor_read_byte(0x02);

}
void sensorCtrl::read_pixel_PTAT()
{
    //********************change BLOCK0*********************************
    change_BLOCK(0x00,0x00);
    //***********************BLOCK0********************************
    uint8_t data[1282]={0};
    spictrl->spi_sensor_read_data(0x0A,data);//PTAT BLOCK 0
    memcpy(PTAT,data,2); //copy PTAT[0]
    memcpy(pixel,data+2,1280);//copy top 640 pixel

    memset(data,0,1282);
    spictrl->spi_sensor_read_data(0x0B,data);
    memcpy(PTAT+1,data,2); //copy PTAT[1]
    read_pixel_select_BLOCK(0,data,pixel); //copy bottom 640 pixel
    //***************************************************************
    //*****************change BLOCK 1********************************
    change_BLOCK(0x01,0x00);
    //*****************************************************************

    //***********************BLOCK 1**********************************
    memset(data,0,1282);
    spictrl->spi_sensor_read_data(0x0A,data); //PTAT BLOCK 1
    memcpy(PTAT+2,data,2); //copy PTAT[2]
    memcpy(pixel+640,data+2,1280); //copy top 640+640 pixel

    memset(data,0,1282);
    spictrl->spi_sensor_read_data(0x0B,data);
    memcpy(PTAT+3,data,2); //copy PTAT[3]
    read_pixel_select_BLOCK(1,data,pixel); //copy bottom 640+640 pixel
    //*****************************************************************
    //********************change BLOCK 2********************************
    change_BLOCK(0x02,0x00);
    //****************************************************************

    //***********************BLOCK 2**********************************
    memset(data,0,1282);
    spictrl->spi_sensor_read_data(0x0A,data); //PTAT BLOCK 1
    memcpy(PTAT+4,data,2); //copy PTAT[4]
    memcpy(pixel+640*2,data+2,1280); //copy top 640+640*2 pixel

    memset(data,0,1282);
    spictrl->spi_sensor_read_data(0x0B,data);
    memcpy(PTAT+5,data,2); //copy PTAT[5]
    read_pixel_select_BLOCK(2,data,pixel); //copy bottom 640+640*2 pixel
    //***********************************************************************

    //*********************change BLOCK 3**********************************
    change_BLOCK(0x03,0x00);
    //********************************************************************

    //**************************BLOCK 3************************************
    memset(data,0,1282);
    spictrl->spi_sensor_read_data(0x0A,data); //PTAT BLOCK 1
    memcpy(PTAT+6,data,2); //copy PTAT[6]
    memcpy(pixel+640*3,data+2,1280); //copy top 640+640*3 pixel

    memset(data,0,1282);
    spictrl->spi_sensor_read_data(0x0B,data);
    memcpy(PTAT+7,data,2); //copy PTAT[7]
    read_pixel_select_BLOCK(3,data,pixel); //copy bottom 640+640*3 pixel
    //************************************************************************

    common::change_edian(pixel,5120);
    common::change_edian(PTAT,8);
}
void sensorCtrl::read_VDD()
{
    uint8_t data[1282] = {0};
    change_BLOCK(0x00,0x01); //VDD 0x01
    spictrl->spi_sensor_read_data(0x0A,data);
    memcpy(VDD,data,2); //copy VDD[0]
    memset(data,0,2);
    spictrl->spi_sensor_read_data(0x0B,data);
    memcpy(VDD+1,data,2); // copy VDD[1]

    change_BLOCK(0x01,0x01); //BLOCK 1 VDD 0x01
    memset(data,0,2);
    spictrl->spi_sensor_read_data(0x0A,data);
    memcpy(VDD+2,data,2); //copy VDD[2]
    memset(data,0,2);
    spictrl->spi_sensor_read_data(0x0B,data);
    memcpy(VDD+3,data,2);  //copy VDD[3]

    change_BLOCK(0x02,0x01);
    memset(data,0,2);
    spictrl->spi_sensor_read_data(0x0A,data);
    memcpy(VDD+4,data,2); //copy VDD[4]
    memset(data,0,2);
    spictrl->spi_sensor_read_data(0x0B,data);
    memcpy(VDD+5,data,2); // copy VDD[5]

    change_BLOCK(0x03,0x01);
    memset(data,0,2);
    spictrl->spi_sensor_read_data(0x0A,data);
    memcpy(VDD+6,data,2); //copy VDD[6]
    memset(data,0,2);
    spictrl->spi_sensor_read_data(0x0B,data);
    memcpy(VDD+7,data,2); // copy VDD[7]
    common::change_edian(VDD,8);

}

void sensorCtrl::read_electricaloffset()
{
    spictrl->spi_sensor_write(0x01,0x0B);//start 0x09 |BLIND 0x01<<1
    usleep(30000);//delay 30ms
    spictrl->spi_sensor_read_byte(0x02);


    uint8_t data[1282] = {0};
    spictrl->spi_sensor_read_data(0x0A,data);
    memcpy(electrical_offset,data+2,1280); //copy top 640 ele_offset

    memset(data,0,1282);
    spictrl->spi_sensor_read_data(0x0B,data);
    for(int i = 0;i<8;++i)
    {
        //memcpy(electrical_offset+1200-80*i,data+2+160*i,160); //copy bottom 640 ele_offset
        memcpy(electrical_offset+640+80*i,data+2+160*i,160);
    }
    common::change_edian(electrical_offset,1280);

}

void sensorCtrl::read_all()
{
    read_pixel_PTAT();
    read_VDD();
    read_electricaloffset();
}
S_M_DATA* sensorCtrl::get_muti_data(int num)
{
    m_data = new S_M_DATA();
    int row = num/80;
    int column = num%80;
    int data_num;
    if(row<32)
    {
        data_num = (row%8)*80+column;

    }
    else
    {
        data_num = (((row)%8) +8)*80+column;//95-row
    }
    m_data->ele_offset = this->electrical_offset[data_num];
    m_data->pixel = this->pixel[num];
    return m_data;
}
S_C_DATA* sensorCtrl::get_common_data()
{
    c_data = new S_C_DATA();
    int PTAT_total=0,VDD_total=0;
    for(int i = 0;i<8;++i)
    {
        PTAT_total +=this->PTAT[i];
        VDD_total +=this->VDD[i];
    }
    c_data->PTAT_av = static_cast<unsigned short>(PTAT_total/8);
    c_data->VDD_av = static_cast<unsigned short>(VDD_total/8);
    return c_data;
}
void sensorCtrl::printvalue()
{
    cout<<"*****************************sensor value******************************"<<endl;
    cout<<"PTAT:";
    common::print_V(PTAT,8,2);
    cout<<endl;
    cout<<"VDD:";
    common::print_V(VDD,8,2);
    cout<<endl;
    cout<<"**********************************************************************"<<endl;
//    cout<<"Pixel:";
//    common::print_V(pixel,5120,2);
//    cout<<endl;
//    cout<<"**********************************************************************"<<endl;
//    cout<<"electrical_offset:";
//    common::print_V(electrical_offset,1280,2);
//    cout<<endl;
//    cout<<"**********************************************************************"<<endl;
}
