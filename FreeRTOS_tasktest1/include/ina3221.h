#ifndef __INA3221_H
#define __INA3221_H

#include "reg_i2c.h"

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





#define     PWR_REG         0x03        //Power register
#define     CUR_REG         0x04        //Current register
#define     CAL_REG         0x05        //Calibration register
#define     ONFF_REG        0x06        //Mask/Enable register
#define     AL_REG          0x07        //Alert limit register
#define     ID_REG          0XFF        //Die ID register

#define     INA3221_ADDR1    0x44//0x8E        //SLAVE ADDRESS 1000111 (VS|SCL)


void INA3221_Init(i2cBASE_t *i2c, uint8_t addr);
void INA3221_SetRegPointer(i2cBASE_t *i2c, uint8_t addr, uint8_t reg);
void INA3221_SendData(i2cBASE_t *i2c,uint8_t addr,uint8_t reg,uint8_t *data);
void INA3221_ReceiveData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data);


void INA3221_GetShuntVoltage(i2cBASE_t *i2c, uint8_t addr, double *data, uint8_t channel);
void INA3221_GetVoltage(i2cBASE_t *i2c, uint8_t addr, double *data, uint8_t channel);
/*void INA3221_SetCalReg(i2cBASE_t *i2c, uint8_t addr,uint16_t *data);
void INA3221_GetShuntVoltage(i2cBASE_t *i2c, uint8_t addr, double *data);
void INA3221_Get_Power(i2cBASE_t *i2c, uint8_t addr, double *data);
void INA3221_Get_ID(i2cBASE_t *i2c, uint8_t addr, uint16_t *data);
void INA3221_GET_CAL_REG(i2cBASE_t *i2c, uint8_t addr, uint16_t *data);
*/
#endif
