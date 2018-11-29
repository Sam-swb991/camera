#include "spictrl.h"
#include <stdio.h>
/**
 * @brief 构造函数，spi初始化
 */
spiCtrl::spiCtrl()
{
    fd = spi_init();
    if(fd<0)
    {
        cout<<"can not open spi device!"<<endl;
    }

}
/**
 * @brief 关闭spi
 */
void spiCtrl::close()
{
    close_spi(fd);
}
/**
 * @brief spi红外传感器写入数据
 * @param cmd，写入命令
 * @param value，写入值
 */
void spiCtrl::spi_sensor_write(uint8_t cmd,uint8_t value)
{
    cs_change_Sensor();
    uint8_t send[2];
    send[0] = cmd;
    send[1] = value;
    transfer_Sensor_Write(fd,send,2);
}
/**
 * @brief spi红外传感器读取数据
 * @param cmd，读取命令
 * @param data，读取的字符串
 */
void spiCtrl::spi_sensor_read_data(uint8_t cmd,uint8_t *data)
{
    cs_change_Sensor();
    transfer_Sensor_Read(fd,&cmd,data,1282);
}
/**
 * @brief spi红外传感器读取数据（1个字符）
 * @param cmd，读取命令
 * @return 返回读取的字符
 */
uint8_t spiCtrl::spi_sensor_read_byte(uint8_t cmd)
{
    cs_change_Sensor();
    uint8_t ret = 0;
    transfer_Sensor_Read(fd,&cmd,&ret,1);
    //printf("ret is :%02x\n",ret);
    return ret;
}
/**
 * @brief spi红外传感器内置EEPROM读取
 * @param addr_H，地址的高字节
 * @param addr_L，地址的低字节
 * @return 返回读取设置地址的字节
 */
uint8_t spiCtrl::spi_eeprom_read(uint8_t addr_H,uint8_t addr_L)
{
    cs_change_Eeprom();
    return transfer_Eeprom_Read(fd,addr_H,addr_L);
}


