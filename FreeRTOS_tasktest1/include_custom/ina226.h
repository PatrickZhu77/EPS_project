#ifndef __INA226_H
#define __INA226_H

#include "reg_i2c.h"

#define     CFG_REG         0x00        //Configuration register
#define     SV_REG          0x01        //Shunt voltage register (shunt resistor here is 0.1 ohm)
#define     BV_REG          0x02        //Bus voltage register
#define     PWR_REG         0x03        //Power register
#define     CUR_REG         0x04        //Current register
#define     CAL_REG         0x05        //Calibration register
#define     MASK_REG        0x06        //Mask/Enable register
#define     AL_REG          0x07        //Alert limit register
#define     ID_REG          0XFF        //Die ID register

#define     INA226_ADDR1    0x41//0x8E        //SLAVE ADDRESS 1000111 (VS|SCL)
#define     INA226_err      0xC         //0001_100. Alert Response slave address

#define     BOL             0x2000      //Mask/Enable register enable Bus Voltage Over-Voltage alert

typedef struct
{
    uint8_t address;              //i2c address
    uint8_t flag;
    uint16_t shunt_voltage;       //2.5uV LSB
    uint16_t bus_voltage;         //1.25mV
    uint16_t shunt_resistance;    //mOhm
    uint16_t config_reg;
    uint16_t cal_reg;
}ina226_data;

/**************List of ina226 sensors**************
 *
 *  0: overcurrent protection of watchdog Vout
 *  1: overcurrent protection of buck converter 3v3 Vout
 *  2: overcurrent protection of buck converter 1v2 Vout
 *  3: overcurrent protection of buck converter 5v0 Vout
 *  4: current monitor of pv_3v3
 *  5: overcurrent protection of 3v3
 *  6: overcurrent protection of 1v2
 *  7: current monitor 1 on battery board
 *  8: current monitor 2 on battery board
 *  9: current monitor of channel 1
 *  10: current monitor of channel 2
 *  11: current monitor of channel 3
 *  12: current monitor of channel 4
 *  13: current monitor of channel 5
 *  14: current monitor of channel 6
 *  15: current monitor of channel 7
 *  16: current monitor of channel 8
 *  17: current monitor of channel 9
 *  18: current monitor of channel 10
 *  19: current monitor of channel 11
 *  20: current monitor of channel 12
 *  21: current monitor of channel 13
 *  22: current monitor of channel 14
 *  23: current monitor of channel 15
 *  24: current monitor of channel 16
 *  25: store data of the error monitor
 *
 *
 *******************************************/

void INA226_Init(i2cBASE_t *i2c, uint8_t addr, ina226_data *data);
void INA226_SetRegPointer(i2cBASE_t *i2c, uint8_t addr, uint8_t reg);
void INA226_SendData(i2cBASE_t *i2c,uint8_t addr,uint8_t reg,uint8_t *data);
void INA226_ReceiveData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data);



void INA226_GetShuntVoltage(i2cBASE_t *i2c, uint8_t addr, uint16_t *data);
void INA226_GetVoltage(i2cBASE_t *i2c, uint8_t addr, uint16_t *data);
void INA226_SetCalReg(i2cBASE_t *i2c, uint8_t addr,uint16_t *data);
//void INA226_GetCurrent(i2cBASE_t *i2c, uint8_t addr, uint32_t *data);
//void INA226_GetPower(i2cBASE_t *i2c, uint8_t addr, uint32_t *data);
void INA226_GetID(i2cBASE_t *i2c, uint8_t addr, uint16_t *data);
void INA226_GetCalReg(i2cBASE_t *i2c, uint8_t addr, uint16_t *data);

#endif
