#ifndef SPICTRL_H
#define SPICTRL_H
extern "C"
{
#include "spi.h"
}
#include <iostream>
#include <unistd.h>
using namespace std;
class spiCtrl
{
public:

    spiCtrl();
    void close();
    uint8_t spi_sensor_read_byte(uint8_t cmd);
    void spi_sensor_read_data(uint8_t cmd,uint8_t *data);
    void spi_sensor_write(uint8_t cmd,uint8_t value);
    uint8_t spi_eeprom_read(uint8_t addr_H,uint8_t addr_L);

private:
    int fd ;


};

#endif // SPICTRL_H
