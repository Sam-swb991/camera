#include "eepromctrl.h"
#include <cstring>
#include "common.h"
using namespace common;
/**
 * @brief eeprom初始化
 * @param spictrl，传入spiCtrl对象
 */
eepromCtrl::eepromCtrl(spiCtrl *spictrl)
{
    this->spictrl = spictrl;
    c_data = new E_C_DATA();
}
/**
 * @brief 读取某个地址的开始的连续4个字节形成的int类型数据
 * @param addr_H，地址的高字节
 * @param addr_L，地址的低字节
 * @return 返回读出的int类型数据
 */
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
/**
 * @brief 读取某个地址的开始的连续4个字节形成的float类型数据
 * @param addr_H，地址的高字节
 * @param addr_L，地址的低字节
 * @return 返回读出的float类型数据
 */
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
/**
 * @brief 读取某个地址的开始的连续2个字节形成的unsigned short类型数据
 * @param addr_H，地址的高字节
 * @param addr_L，地址的低字节
 * @return 返回读出的unsigned short类型数据
 */
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
/**
 * @brief 读取eeprom所有存储的数据
 */
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
    addr_L = 0x1A;
    MBIT = spictrl->spi_eeprom_read(addr_H,addr_L);
    addr_L = 0x1B;
    BIAS = spictrl->spi_eeprom_read(addr_H,addr_L);
    addr_L = 0x1C;
    CLK = spictrl->spi_eeprom_read(addr_H,addr_L);
    addr_L = 0x1D;
    BPA = spictrl->spi_eeprom_read(addr_H,addr_L);
    addr_L = 0x1E;
    PU = spictrl->spi_eeprom_read(addr_H,addr_L);
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
    addr_L = 0x60;
    MBIT_user = spictrl->spi_eeprom_read(addr_H,addr_L);
    addr_L = 0x61;
    BIAS_user = spictrl->spi_eeprom_read(addr_H,addr_L);
    addr_L = 0x62;
    CLK_user = spictrl->spi_eeprom_read(addr_H,addr_L);
    addr_L = 0x63;
    BPA_user = spictrl->spi_eeprom_read(addr_H,addr_L);
    addr_L = 0x64;
    PU_user = spictrl->spi_eeprom_read(addr_H,addr_L);
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
/**
 * @brief 把获取的eeprom数据转储为E_C_DATA结构体数据
 * @return 返回E_C_DATA结构体
 */
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
/**
 * @brief 把获取的eeprom数据转储为E_M_DATA结构体数据
 * @return 返回E_M_DATA结构体
 */
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
        int row_num = row;
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
/**
 * @brief 打印获取的EEPROM数据
 */
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
    cout<<"Divice ID :";
    common::print_V(&DeviceID,4,1);

    cout<<"  TN :";
    common::print_V(&TN,1,2);
    cout<<endl;
    cout<<"**************************************************************"<<endl;
    cout<<"MBIT:";
    common::print_V(&MBIT,1,1);
    cout<<" BIAS:";
    common::print_V(&BIAS,1,1);
    cout<<" CLK:";
    common::print_V(&CLK,1,1);
    cout<<" BPA:";
    common::print_V(&BPA,1,1);
    cout<<" PU:";
    common::print_V(&PU,1,1);
    cout<<endl;
    cout<<"**************************************************************"<<endl;
    cout<<"MBIT:";
    common::print_V(&MBIT_user,1,1);
    cout<<" BIAS:";
    common::print_V(&BIAS_user,1,1);
    cout<<" CLK:";
    common::print_V(&CLK_user,1,1);
    cout<<" BPA:";
    common::print_V(&BPA_user,1,1);
    cout<<" PU:";
    common::print_V(&PU_user,1,1);
    cout<<endl;
    cout<<"************************************************************"<<endl;
//    cout<<"DeadPixAdr:"<<endl;
//    common::print_V(DeadPixAdr,NrOfDefPix*2,1);
//    cout<<endl;
//    cout<<"************************************************************"<<endl;
//    cout<<"DeadPixMask:"<<endl;
//    common::print_V(DeadPixMask,NrOfDefPix,1);
//    cout<<endl;
//    cout<<"************************************************************"<<endl;
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
