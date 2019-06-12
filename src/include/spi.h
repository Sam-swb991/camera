#ifndef  __SPI__H
#define  __SPI__H
#ifdef __cplusplus  
extern "C" {
#endif
#include <stdint.h>
#include <sys/types.h>
typedef enum{
	PIN_0 ,
	PIN_1 ,
	PIN_2 ,
	PIN_3 ,
	PIN_4 ,
	PIN_5 ,
	PIN_6 ,
	PIN_7 ,
}PIN_NUM;

typedef enum{
	PIN_INPUT=0,
	PIN_OUTPUT
}PIN_DIR;


typedef enum{
	GPIO_SET_MUX_AS_GPIO=0,
	GPIO_SET_DIR=1,
	GPIO_SET_VAL=3,
	GPIO_GET_VAL=4,
}GPIO_CMD;

typedef struct 
{
	int gpio_num;
	int pin_num;
	int pin_dir;
	int val;
}GPIO_CMD_DATA;
int spi_init();
void cs_change_Sensor();
void cs_change_Eeprom();
void prepare_write_rs485();
void prepare_read_rs485();
void transfer_Sensor_Write(int fd,uint8_t const *tx,size_t tx_len);
void transfer_Sensor_Read(int fd,uint8_t const *tx,uint8_t const *rx,
size_t rx_len);
void transfer_Eeprom(int fd,uint8_t const *tx,uint8_t const *rx);
uint8_t transfer_Eeprom_Read(int fd,uint8_t addr_H,uint8_t addr_L);
void close_spi(int fd);

#ifdef __cplusplus
}
#endif

#endif
