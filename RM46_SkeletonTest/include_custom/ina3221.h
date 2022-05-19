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
#define     WA_REG3         0x0C        //Warning alert limit  3

#define     EN_REG        0x0F        //Mask/Enable register


#define     INA3221_CFG_SETTING     0x0527  //configuration register with 16 averages and 1.1ms conversion time

#define     voltage_mask     0x7FF8     //mask used for both voltage registers (only 14--3 bits are used)

#define     INA3221_ADDR1    0x41//0x8E        //SLAVE ADDRESS 1000111 (VS|SCL)

#define     INA3221_err      0xC                 //0001_100. Alert Response slave address

#define     NUM_OF_INA3221   4          // 4 is default value according to schematic of eps


typedef struct
{
    uint8_t address;              //i2c address
    uint8_t flag;
    uint16_t shunt_voltage[3];       //mV
    uint16_t bus_voltage[3];         //mV
    uint16_t shunt_resistance;       //mOhm
    uint16_t config_reg;
    uint16_t mask_reg;              //0x8: enable channel1 Vshunt overvoltage (overcurrent) alert ???do we need this?
    uint16_t alert_reg[3];
    uint32_t timestamp_sec;
}ina3221_data;


void INA3221_SetRegPointer(i2cBASE_t *i2c, uint8_t addr, uint8_t reg);
void INA3221_SendData(i2cBASE_t *i2c,uint8_t addr,uint8_t reg,uint8_t *data);
void INA3221_ReceiveData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data);


void INA3221_GetShuntVoltage(i2cBASE_t *i2c, uint8_t addr, uint16_t *data, uint8_t channel);
void INA3221_GetVoltage(i2cBASE_t *i2c, uint8_t addr, uint16_t *data, uint8_t channel);
void INA3221_Init(i2cBASE_t *i2c, uint8_t addr, ina3221_data *data);


#endif
