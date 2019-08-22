#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "spi.h"
#define SPI_PATH "/dev/spidev2.0"
#define GPIO_PATH "/dev/htxjgpio"
/**
 * @brief 控制gpio的结构体，静态变量
 */
static GPIO_CMD_DATA cmd_data,cmd_data_rs485;
/**
 * @brief gpio文件描述符，静态变量
 */
static int G_fd;
static int Rs_fd;
/**
 * @brief gpio的初始化
 * @param cmd_data，设置gpio需要传入的结构体
 * @return 初始化成功返回文件描述符fd,不成功返回-1
 */
int gpio_init(GPIO_CMD_DATA *cmd_data,GPIO_CMD_DATA *cmd_data_rs485)
{
    int ret;
    Rs_fd = open(GPIO_PATH,0);
    if (Rs_fd<0)
    {
        printf("Open htxjgpio dev error!\n");
        return -1;
    }
    else
        printf("open Rs485 fd:%d",Rs_fd);
    //**********************Rs485****************************
    cmd_data_rs485->gpio_num = 12;
    cmd_data_rs485->pin_num = 5;
    cmd_data_rs485->pin_dir = PIN_OUTPUT;
    cmd_data_rs485->val = 1;
    ret = ioctl(Rs_fd, GPIO_SET_MUX_AS_GPIO, cmd_data_rs485);
    if(-1 == ret)
    {
        printf("set mux failed!\n");
        return -1;
    }
    ret = ioctl(Rs_fd, GPIO_SET_DIR, cmd_data_rs485);
    if(-1 == ret)
    {
        printf("set dir failed!\n");
        return -1;
    }
    ret = ioctl(Rs_fd, GPIO_SET_VAL, cmd_data_rs485);
    if(-1 == ret)
    {
        printf("set high failed!");
        return -1;
    }
    //************************spi******************************
    int fd = open(GPIO_PATH,0);
    if (fd<0)
    {
        printf("Open htxjgpio dev error!\n");
        return -1;
    }
    else
        printf("open spi_cs fd:%d",fd);
    cmd_data->gpio_num = 1;
    cmd_data->pin_num = 5;
    cmd_data->pin_dir = PIN_OUTPUT;
    cmd_data->val = 0;

    ret = ioctl(fd, GPIO_SET_MUX_AS_GPIO, cmd_data);
    if(-1 == ret)
    {
        printf("set mux failed!\n");
        return -1;
    }
    ret = ioctl(fd, GPIO_SET_DIR, cmd_data);
    if(-1 == ret)
    {
        printf("set dir failed!\n");
        return -1;
    }
    ret = ioctl(fd, GPIO_SET_VAL, cmd_data);
    if(-1 == ret)
    {
        printf("set high failed!");
        return -1;
    }
    return fd;
}
/**
 * @brief 把gpio拉高
 * @param fd，文件描述符
 * @param cmd_data，设置gpio需要传入的结构体
 */
void setGPIOH(int fd,GPIO_CMD_DATA *cmd_data)
{
    cmd_data->val = 1;
    int ret = ioctl(fd, GPIO_SET_VAL, cmd_data);
    if(-1 == ret)
        printf("set high failed!");
}
/**
 * @brief 把gpio拉低
 * @param fd，文件描述符
 * @param cmd_data，设置gpio需要传入的结构体
 */
void setGPIOL(int fd,GPIO_CMD_DATA *cmd_data)
{
    cmd_data->val = 0;
    int ret = ioctl(fd, GPIO_SET_VAL, cmd_data);
    if(-1 == ret)
        printf("set low failed!");
}

void prepare_write_rs485()
{
    setGPIOH(G_fd,&cmd_data_rs485);
}

void prepare_read_rs485()
{
    setGPIOL(G_fd,&cmd_data_rs485);
}
/**
 * @brief 强制把gpio拉低，为了读取红外传感器的数据
 */
void cs_change_Sensor()
{
    if(cmd_data.val == 0)
        return;
    else
        cmd_data.val = 0;
    int ret = ioctl(G_fd, GPIO_SET_VAL, &cmd_data);
    usleep(1);
    if(-1 == ret)
        printf("set cs to sensor failed!");

}
/**
 * @brief 强制把gpio拉高，为了读取红外传感器内置EEPROM的数据
 */
void cs_change_Eeprom()
{
    if(cmd_data.val == 1)
        return;
    else
        cmd_data.val = 1;
    int ret = ioctl(G_fd, GPIO_SET_VAL, &cmd_data);
    usleep(1);
    if(-1 == ret)
        printf("set cs to eeprom failed!");
}
/**
 * @brief 打印错误信息并终止程序
 * @param s,错误字符串
 */
static void pabort(const char *s)
{
    perror(s);
    abort();
}
/**
 * @brief 以16进制格式打印字符串
 * @param head,打印的字符串的头
 * @param hex，打印的16进制字符串主体
 * @param len，打印的16进制字符串长度
 */
void printHEX(char * head,uint8_t const *hex,size_t len)
{
#ifdef LOG
    int i;
    printf("%s",head);
    for(i=0;i<len;i++)
    {
        printf(" %02x",hex[i]);
    }
    printf("\n");
#endif
}
/**
 * @brief spi通信
 * @param fd，文件描述符
 * @param num，为1只需发送，不需要接收，为2则都需要
 * @param type，为1是sensor模式，为2是eeprom模式
 * @param tx，发送的数据
 * @param rx，接收的数据
 * @param tx_len，发送的数据长度
 * @param rx_len，接收的数据长度
 */
void spi_transfer(int fd,int num,int type,uint8_t const *tx,
                  uint8_t const *rx,size_t tx_len,size_t rx_len)
{
    int ret;
    if(type ==1)
    {
        if(num == 1)
        {
            struct spi_ioc_transfer xfer;
            memset(&xfer,'\0',sizeof(xfer));
            xfer.tx_buf = (unsigned long)tx;
            xfer.len = tx_len;
            setGPIOH(G_fd,&cmd_data);
            ret = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
            if(ret < 0 )
                printf("spi transfer 1 failed!");
            setGPIOL(G_fd,&cmd_data); // pull low
        }
        else if(num == 2)
        {
            struct spi_ioc_transfer xfer[2];
            memset(xfer,'\0',sizeof(xfer));
            xfer[0].tx_buf = (unsigned long)tx;
            xfer[0].len = tx_len;
            xfer[1].rx_buf = (unsigned long)rx;
            xfer[1].len = rx_len;
            setGPIOH(G_fd,&cmd_data); //pull high

            ret = ioctl(fd, SPI_IOC_MESSAGE(2), &xfer);
            if(ret < 0 )
                printf("spi transfer 2 failed!");
            setGPIOL(G_fd,&cmd_data); // pull low
        }
    }
    else if(type == 2)
    {
        struct spi_ioc_transfer xfer[2];
        memset(xfer,'\0',sizeof(xfer));
        xfer[0].tx_buf = (unsigned long)tx;
        xfer[0].len = tx_len;
        xfer[1].rx_buf = (unsigned long)rx;
        xfer[1].len = rx_len;
        setGPIOL(G_fd,&cmd_data); //pull low

        ret = ioctl(fd, SPI_IOC_MESSAGE(2), &xfer);
        if(ret < 0 )
            printf("spi transfer 2 failed!");
        setGPIOH(G_fd,&cmd_data); // pull high
    }

}
/**
 * @brief spi初始化
 * @return 初始化成功返回文件描述符fd,失败返回-1
 */
int spi_init()
{
    int ret;
    G_fd = gpio_init(&cmd_data,&cmd_data_rs485);
    if(G_fd<0)
        pabort("gpio init failed");
    static uint32_t mode = 0;
    static uint8_t bits = 8;
    static uint32_t speed = 5000000;
    int fd = open(SPI_PATH,O_RDWR);
    if (fd < 0)
        pabort("can't open device");

    ret = ioctl(fd, SPI_IOC_WR_MODE32, &mode);
    if (ret == -1)
        pabort("can't set spi mode");

    ret = ioctl(fd, SPI_IOC_RD_MODE32, &mode);
    if (ret == -1)
        pabort("can't get spi mode");
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't get bits per word");
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't set max speed hz");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't get max speed hz");

    printf("spi mode: 0x%x\n", mode);
    printf("bits per word: %d\n", bits);
    printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
    return fd;

}
/**
 * @brief spi sensor模式下写数据
 * @param fd，文件描述符
 * @param tx，需要写的数据
 * @param tx_len，需要写的数据长度
 */
void transfer_Sensor_Write(int fd,uint8_t const *tx,size_t tx_len)
{
    //   printHEX("send is:",tx,tx_len);
    spi_transfer(fd,1,1,tx,NULL,tx_len,0);
}
/**
 * @brief spi sensor模式下读数据
 * @param fd，文件描述符
 * @param tx，发送的数据
 * @param rx，接收的数据
 * @param rx_len，接收的数据长度
 */
void transfer_Sensor_Read(int fd,uint8_t const *tx,uint8_t const *rx,
                          size_t rx_len)
{
    //	printHEX("send is:",tx,1);
    spi_transfer(fd,2,1,tx,rx,1,rx_len);
    //	printHEX("recv is:",rx,rx_len);
}
/**
 * @brief EEPROM模式下spi通信
 * @param fd,文件描述符
 * @param tx，发送的数据
 * @param rx，接收的数据
 */
void transfer_Eeprom(int fd,uint8_t const *tx,uint8_t const *rx)
{
    //   printHEX("send is:",tx,3);
    spi_transfer(fd,2,2,tx,rx,3,1);
    //  printHEX("recv is:",rx,1);
}
/**
 * @brief EEPROM模式下读数据
 * @param fd，文件描述符
 * @param addr_H，传输的地址高字节
 * @param addr_L，传输的地址低字节
 * @return 返回此地址的16进制字符
 */
uint8_t transfer_Eeprom_Read(int fd,uint8_t addr_H,uint8_t addr_L)
{
    uint8_t send[3];
    uint8_t recv = 0;
    send[0] = 0x03;
    send[1] = addr_H;
    send[2] = addr_L;
    transfer_Eeprom(fd,send,&recv);
    return recv;
}
/**
 * @brief 关闭spi，关闭gpio
 * @param fd,文件描述符
 */
void close_spi(int fd)
{
    close(G_fd);
    close(Rs_fd);
    close(fd);
}
#ifdef __cplusplus
}
#endif
