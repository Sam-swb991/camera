#include "spictrl.h"
#include <stdio.h>
spiCtrl::spiCtrl()
{
    fd = spi_init();
    if(fd<0)
    {
        cout<<"can not open spi device!"<<endl;
    }

}
void spiCtrl::close()
{
    close_spi(fd);
}
void spiCtrl::spi_sensor_write(uint8_t cmd,uint8_t value)
{
    cs_change_Sensor();
    uint8_t send[2];
    send[0] = cmd;
    send[1] = value;
    transfer_Sensor_Write(fd,send,2);
}

void spiCtrl::spi_sensor_read_data(uint8_t cmd,uint8_t *data)
{
    cs_change_Sensor();
    transfer_Sensor_Read(fd,&cmd,data,1282);
}

uint8_t spiCtrl::spi_sensor_read_byte(uint8_t cmd)
{
    cs_change_Sensor();
    uint8_t ret = 0;
    transfer_Sensor_Read(fd,&cmd,&ret,1);
    //printf("ret is :%02x\n",ret);
    return ret;
}

uint8_t spiCtrl::spi_eeprom_read(uint8_t addr_H,uint8_t addr_L)
{
    cs_change_Eeprom();
    return transfer_Eeprom_Read(fd,addr_H,addr_L);
}


