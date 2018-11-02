#include "eepromctrl.h"
#include <cstring>
#include "common.h"
using namespace common;
eepromCtrl::eepromCtrl(spiCtrl *spictrl)
{
    this->spictrl = spictrl;
    c_data = new E_C_DATA();
}
int eepromCtrl::read_int(uint8_t addr_H, uint8_t addr_L)
{
    uint8_t recv[4];
    int ret;
    for(int i =0;i<4;++i)
    {
        recv[i] = spictrl->spi_eeprom_read(addr_H,addr_L);
        addr_L++;
    }
    memcpy(&ret,recv,4);
    return ret;
}

float eepromCtrl::read_float(uint8_t addr_H, uint8_t addr_L)
{
    uint8_t recv[4];
    float ret;
    for(int i =0;i<4;++i)
    {
        recv[i] = spictrl->spi_eeprom_read(addr_H,addr_L);
        addr_L++;
    }
    memcpy(&ret,recv,4);
    return ret;
}
unsigned short eepromCtrl::read_short(uint8_t addr_H, uint8_t addr_L)
{
    uint8_t recv[2];
    unsigned short ret;
    for(int i =0;i<2;++i)
    {
        recv[i] = spictrl->spi_eeprom_read(addr_H,addr_L);
        addr_L++;
    }
    memcpy(&ret,recv,2);
    return ret;
}
void eepromCtrl::readAll()
{
    uint8_t addr_H = 0x00;
    uint8_t addr_L = 0x00;

    PixCmin = read_float(addr_H,addr_L);
    addr_L = 0x04;
    PixCmax = read_float(addr_H,addr_L);
    addr_L = 0x08;
    gradscale = spictrl->spi_eeprom_read(addr_H,addr_L);
    addr_L = 0x0B;
    TN = read_short(addr_H,addr_L);
    addr_L = 0x0D;
    epsilon = spictrl->spi_eeprom_read(addr_H,addr_L);
    addr_L = 0x22;
    Arraytype = spictrl->spi_eeprom_read(addr_H,addr_L);
    addr_L = 0x26;
    VDDTH1 = read_short(addr_H,addr_L);
    addr_L = 0x28;
    VDDTH2 = read_short(addr_H,addr_L);
    addr_L = 0x34;
    PTAT_gradient = read_float(addr_H,addr_L);
    addr_L = 0x38;
    PTAT_offset = read_float(addr_H,addr_L);
    addr_L = 0x3C;
    PTATTH1 = read_short(addr_H,addr_L);
    addr_L = 0x3E;
    PTATTH2 = read_short(addr_H,addr_L);
    addr_L = 0x4E;
    VddScGrad = spictrl->spi_eeprom_read(addr_H,addr_L);
    addr_L = 0x4F;
    VddScOff = spictrl->spi_eeprom_read(addr_H,addr_L);
    addr_L = 0x54;
    GlobalOff = spictrl->spi_eeprom_read(addr_H,addr_L);
    addr_L = 0x55;
    GlobalGain = read_short(addr_H,addr_L);
    addr_L = 0x74;
    DeviceID = read_int(addr_H,addr_L);
    addr_L = 0x7F;
    NrOfDefPix = spictrl->spi_eeprom_read(addr_H,addr_L);
    DeadPixAdr = new unsigned short(NrOfDefPix);
    DeadPixMask = new uint8_t(NrOfDefPix);
    addr_L = 0x80;
    for(int i = 0 ;i<NrOfDefPix;++i)
    {
        DeadPixAdr[i] = read_short(addr_H,addr_L);
        addr_L +=2;
    }
    addr_L = 0xB0;
    for(int i = 0 ;i<NrOfDefPix;++i)
    {
        DeadPixMask[i] = spictrl->spi_eeprom_read(addr_H,addr_L);
        addr_L ++;
    }
    addr_H = 0x08;
    addr_L = 0x00;
    for(int i = 0;i<1280;++i)
    {
        VddCompGrad[i] = read_short(addr_H,addr_L);
        if(addr_L == 0xFE)
        {
            addr_L = 0x00;
            addr_H++;
        }
        else
            addr_L +=2;
    }
    addr_H = 0x12;
    addr_L = 0x00;
    for(int i = 0;i<1280;++i)
    {
        VddCompOff[i] = read_short(addr_H,addr_L);
        if(addr_L == 0xFE)
        {
            addr_L = 0x00;
            addr_H++;
        }
        else
            addr_L +=2;
    }
    addr_H = 0x1C;
    addr_L = 0x00;
    for(int i = 0;i<5120;++i)
    {
        ThGrad[i] = spictrl->spi_eeprom_read(addr_H,addr_L);
        if(addr_L == 0xFF)
        {
            addr_L = 0x00;
            addr_H++;
        }
        else
            addr_L ++;
    }
    addr_H = 0x30;
    addr_L = 0x00;
    for(int i = 0;i<5120;++i)
    {
        ThOffset[i] = read_short(addr_H,addr_L);
        if(addr_L == 0xFE)
        {
            addr_L = 0x00;
            addr_H++;
        }
        else
            addr_L +=2;
    }
    addr_H = 0x58;
    addr_L = 0x00;
    for(int i = 0;i<5120;++i)
    {
        P[i] = read_short(addr_H,addr_L);
        if(addr_L == 0xFE)
        {
            addr_L = 0x00;
            addr_H++;
        }
        else
            addr_L +=2;
    }

}
E_C_DATA* eepromCtrl::get_common_data()
{

    c_data->PTATTH1 = this->PTATTH1;
    c_data->PTATTH2 = this->PTATTH2;
    c_data->PTAT_gradient = this->PTAT_gradient;
    c_data->PTAT_offset = this->PTAT_offset;
    c_data->VDDTH1 = this->VDDTH1;
    c_data->VDDTH2 = this->VDDTH2;
    c_data->epsilon = this->epsilon;
    c_data->gradscale = this->gradscale;
    c_data->nrOfDefPix = this->NrOfDefPix;
    c_data->GlobalOff = this->GlobalOff;
    c_data->pixCmax = this->PixCmax;
    c_data->pixCmin = this->PixCmin;
    c_data->vddScGrad = this->VddScGrad;
    c_data->vddScOff = this->VddScOff;
    c_data->GlobalGrain = this->GlobalGain;
    c_data->Device = this->DeviceID;
    return c_data;

}

E_M_DATA* eepromCtrl::get_muti_data(int num)
{
    m_data = new E_M_DATA();
    int row = num/80;
    int column = num%80;
    if(row<32)
    {
        m_data->thGrad = static_cast<signed char>(this->ThGrad[num]) ;
        m_data->thOffSet = static_cast<signed short>(this->ThOffset[num]);
        m_data->p = this->P[num];
        int data_num = (row%8)*80 +column;
        m_data->vddCompGrad = static_cast<signed short>(this->VddCompGrad[data_num]);
        m_data->vddCompOff = static_cast<signed short>(this->VddCompOff[data_num]);
    }
    else
    {
        int row_num = row; //95-row
        int data_num = row_num*80+column;
        m_data->thGrad = static_cast<signed char>(this->ThGrad[num]);
        m_data->thOffSet = static_cast<signed short>(this->ThOffset[num]);
        m_data->p = this->P[num];
        data_num = ((row_num%8) +8)*80+column;
        m_data->vddCompGrad = static_cast<signed short>(this->VddCompGrad[data_num]);
        m_data->vddCompOff = static_cast<signed short>(this->VddCompOff[data_num]);
    }
    return m_data;
}
void eepromCtrl::printvalue()
{
    cout<<"**********************eeprom value****************************"<<endl;
    cout<<"PixCmin:"<<PixCmin<<"   PixCmax:"<<PixCmax<<endl;
    cout<<"VDDTH1:"<<VDDTH1<<"    VDDTH2:"<<VDDTH2<<endl;
    cout<<"PTATTH1:"<<PTATTH1<<"    PTATTH2:"<<PTATTH2<<endl;
    cout<<"GlobalGain:";
    common::print_V(&GlobalGain,1,2);
    cout<<"  epsilon:";
    common::print_V(&epsilon,1,1);
    cout<<endl;
    cout<<"PTAT_gradient:"<<PTAT_gradient<<"    PTAT_offset:"<<PTAT_offset<<endl;
    cout<<"VddScGrad:";
    common::print_V(&VddScGrad,1,1);
    cout<<"     VddScOff:";
    common::print_V(&VddScOff,1,1);
    cout<<endl;
    cout<<"NrOfDefPix:";
    common::print_V(&NrOfDefPix,1,1);
    cout<<"   gradScale:";
    common::print_V(&gradscale,1,1);
    cout<<endl;
    cout<<"Divice ID :"<<DeviceID;
    cout<<"  TN :";
    common::print_V(&TN,1,2);
    cout<<endl;
    cout<<"************************************************************"<<endl;
    cout<<"DeadPixAdr:"<<endl;
    common::print_V(DeadPixAdr,NrOfDefPix*2,1);
    cout<<endl;
    cout<<"************************************************************"<<endl;
    cout<<"DeadPixMask:"<<endl;
    common::print_V(DeadPixMask,NrOfDefPix,1);
    cout<<endl;
    cout<<"************************************************************"<<endl;
//    cout<<"VddCompGrad:"<<endl;
//    common::print_V(VddCompGrad,1280,2);
//    cout<<endl;
//    cout<<"************************************************************"<<endl;
//    cout<<"VddCompOff:"<<endl;
//    common::print_V(VddCompOff,1280,2);
//    cout<<endl;
//    cout<<"************************************************************"<<endl;
//    cout<<"ThOffset:"<<endl;
//    common::print_V(ThOffset,5120,2);
//    cout<<endl;
//    cout<<"************************************************************"<<endl;
//    cout<<"P:"<<endl;
//    common::print_V(P,5120,2);
//    cout<<endl;
//    cout<<"************************************************************"<<endl;
//    cout<<"ThGrad:"<<endl;
//    common::print_V(ThGrad,5120,1);
//    cout<<endl;
//    cout<<"************************************************************"<<endl;
}
