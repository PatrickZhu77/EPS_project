#ifndef __INA3221_H
#define __INA3221_H

#include "reg_i2c.h"
#include "i2c.h"

#define     CFG_REG         0x00        //Configuration register
#define     SV_REG1         0x01        //Shunt voltage register 1
#define     BV_REG1         0x02        //Bus voltage register   1
#define     SV_REG2         0x03        //Shunt voltage register 2
#define     BV_REG2         0x04        //Bus voltage register   2
#define     SV_REG3         0x05        //Shunt voltage register 3
#define     BV_REG3         0x06        //Bus voltage register   3

#define     CA_REG1         0x07        //Critical alert limit 1
#define     WA_REG1         0x08        //Warning alert limit  1
#define     CA_REG2         0x09        //Critical alert limit 2
#define     WA_REG2         0x0A        //Warning alert limit  2
#define     CA_REG3         0x0B        //Critical alert limit 3
#define     WA_REG4         0x0C        //Warning alert limit  3


#define     voltage_mask     0x7FF8     //mask used for both voltage registers (only 14--3 bits are used)

#define     INA3221_ADDR1    0x44//0x8E        //SLAVE ADDRESS 1000111 (VS|SCL)


typedef struct
{
    uint8_t address;              //i2c address

    uint32_t shunt_voltage[3];       //mV
    uint32_t bus_voltage[3];         //mV
    uint32_t voltage[3];             //mV (shunt voltage + bus voltage)
    uint32_t current[3];             //mA
    uint32_t power[3];               //uW

    uint8_t flag;
    uint16_t config_reg;
}ina3221_data;


void INA3221_SetRegPointer(i2cBASE_t *i2c, uint8_t addr, uint8_t reg);
void INA3221_SendData(i2cBASE_t *i2c,uint8_t addr,uint8_t reg,uint8_t *data);
void INA3221_ReceiveData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data);


void INA3221_GetShuntVoltage(i2cBASE_t *i2c, uint8_t addr, uint32_t *data, uint8_t channel);
void INA3221_GetBusVoltage(i2cBASE_t *i2c, uint8_t addr, uint32_t *data, uint8_t channel);
void INA3221_DoCalculation(i2cBASE_t *i2c, uint8_t addr, ina3221_data *data, uint8_t channel);
void INA3221_Init(i2cBASE_t *i2c, uint8_t addr, ina3221_data *data);


#endif
