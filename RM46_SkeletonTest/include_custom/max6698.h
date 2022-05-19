/*
 * max6698.h
 *
 *  Created on: Mar 15, 2022
 *      Author: sdamkjar
 */

#ifndef INCLUDE_CUSTOM_MAX6698_H_
#define INCLUDE_CUSTOM_MAX6698_H_

#include "reg_i2c.h"

#define     LC_REG          0x07        //Local temperature register
#define     RM1_REG         0x01        //Channel 1 remote temperature register
#define     RM2_REG         0x02        //Channel 1 remote temperature register
#define     RM3_REG         0x03        //Channel 1 remote temperature register
#define     TM1_REG         0x04        //Thermistor 1 voltage register
#define     TM2_REG         0x05        //Thermistor 2 voltage register
#define     TM3_REG         0x06        //Thermistor 3 voltage register
#define     CFG1_REG        0x41        //Configuration register 1
#define     CFG2_REG        0X42        //Configuration register 2
#define     CFG3_REG        0X43        //Configuration register 3
#define     ST1_REG         0X44        //Status register 1
#define     ST2_REG         0X45        //Status register 2
#define     ST3_REG         0X46        //Status register 3
#define     LCA_REG         0x17        //Local alert high-temperature threshold limit registe
#define     RM1A_REG        0x11        //Channel 1 remote-diode alert high-temperature threshold limit register
#define     RM2A_REG        0x12        //Channel 2 remote-diode alert high-temperature threshold limit register
#define     RM3A_REG        0x13        //Channel 3 remote-diode alert high-temperature threshold limit register
#define     TM1A_REG        0x14        //Thermistor 1 voltage alert high-threshold limit register
#define     TM2A_REG        0x15        //Thermistor 2 voltage alert high-threshold limit register
#define     TM3A_REG        0x16        //Thermistor 3 voltage alert high-threshold limit register
#define     RM1O_REG        0X21        //Channel 1 remote-diode overtemperature threshold limit register
#define     TM1O_REG        0X24        //Thermistor 1 overtemperature threshold limit register
#define     TM2O_REG        0X25        //Thermistor 2 overtemperature threshold limit register
#define     TM3O_REG        0X26        //Thermistor 3 overtemperature threshold limit register

#define     NUM_OF_MAX6698  1

#define     MAX6698_ADDR1   0x38        //SLAVE ADDRESS 1000111 (VS|SCL)

#define     MAX6698_CFG1_SETTING     0x40       //0x40: power-on reset the sensor
#define     MAX6698_CFG2_SETTING     0x38       //0x38: mask 3 thermistors' alert
#define     MAX6698_CFG3_SETTING     0x38       //0x38: mask 3 thermistors' overT


typedef struct
{
    uint8_t address;              //i2c address
    uint8_t flag;
    uint8_t config1_reg;          //0x40: power-on reset the sensor
    uint8_t config2_reg;          //0x38: mask 3 thermistors' alert
    uint8_t config3_reg;          //0x38: mask 3 thermistors' overT
    uint8_t status1_reg;          //3 thermistors' alert status
    uint8_t status2_reg;          //3 thermistors' overT status
    uint8_t alert1_reg;
    uint8_t alert2_reg;
    uint8_t alert3_reg;
    uint8_t over1_reg;
    uint8_t over2_reg;
    uint8_t over3_reg;
}max6698_data;



void MAX6698_SendData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data);
void MAX6698_ReceiveData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data);
void MAX6698_GetTM(i2cBASE_t *i2c, uint8_t addr, uint8_t num, uint8_t *data);
void MAX6698_SetCfgReg(i2cBASE_t *i2c, uint8_t addr, uint8_t num, uint8_t data);
void MAX6698_SetAlertReg(i2cBASE_t *i2c, uint8_t addr, uint8_t num, uint8_t data);
void MAX6698_SetOverTReg(i2cBASE_t *i2c, uint8_t addr, uint8_t num, uint8_t data);
void MAX6698_Init(i2cBASE_t *i2c, uint8_t addr, max6698_data *data);

#endif /* INCLUDE_CUSTOM_MAX6698_H_ */
