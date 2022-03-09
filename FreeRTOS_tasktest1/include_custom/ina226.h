#ifndef __INA226_H
#define __INA226_H

#include "reg_i2c.h"

#define     CFG_REG         0x00        //Configuration register
#define     SV_REG          0x01        //Shunt voltage register (shunt resistor here is 0.1 ohm)
#define     BV_REG          0x02        //Bus voltage register
#define     PWR_REG         0x03        //Power register
#define     CUR_REG         0x04        //Current register
#define     CAL_REG         0x05        //Calibration register
#define     ONFF_REG        0x06        //Mask/Enable register
#define     AL_REG          0x07        //Alert limit register
#define     ID_REG          0XFF        //Die ID register

#define     INA226_ADDR1    0x44//0x8E        //SLAVE ADDRESS 1000111 (VS|SCL)

//#define       INA226_GETALADDR    0x14
typedef struct
{
    uint8_t address;              //i2c address
    uint32_t shunt_voltage;       //uV
    uint32_t bus_voltage;         //mV
    uint16_t calibration;
    uint32_t current;             //mA
    uint32_t power;               //uW
    uint8_t flag;
    uint8_t config_reg;
    uint8_t cal_reg;
}ina226_data;


void INA226_Init(i2cBASE_t *i2c, uint8_t addr, ina226_data *data);
void INA226_SetRegPointer(i2cBASE_t *i2c, uint8_t addr, uint8_t reg);
void INA226_SendData(i2cBASE_t *i2c,uint8_t addr,uint8_t reg,uint8_t *data);
void INA226_ReceiveData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data);



void INA226_GetShuntVoltage(i2cBASE_t *i2c, uint8_t addr, uint32_t *data);
void INA226_GetVoltage(i2cBASE_t *i2c, uint8_t addr, uint32_t *data);
void INA226_SetCalReg(i2cBASE_t *i2c, uint8_t addr,uint16_t *data);
void INA226_GetCurrent(i2cBASE_t *i2c, uint8_t addr, uint32_t *data);
void INA226_GetPower(i2cBASE_t *i2c, uint8_t addr, uint32_t *data);
void INA226_GetID(i2cBASE_t *i2c, uint8_t addr, uint16_t *data);
void INA226_GetCalReg(i2cBASE_t *i2c, uint8_t addr, uint16_t *data);

#endif
