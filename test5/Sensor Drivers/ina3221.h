#ifndef __INA226_H
#define __INA226_H


#define     CFG_REG         0x00        //Configuration register
#define     C1_SV_REG       0x01        //Channel 1 Shunt voltage register (shunt resistor here is 0.1 ohm)
#define     C1_BV_REG       0x02        //Channel 1 Bus voltage register
#define     C2_SV_REG       0x03        //Channel 2 Shunt voltage register (shunt resistor here is 0.1 ohm)
#define     C2_BV_REG       0x04        //Channel 2 Bus voltage register
#define     C3_SV_REG       0x05        //Channel 3 Shunt voltage register (shunt resistor here is 0.1 ohm)
#define     C3_BV_REG       0x06        //Channel 3 Bus voltage register
#define     C1_CAL_REG      0x07        //Channel 1 Critical Alert limit register
#define     C1_WAL_REG      0x08        //Channel 1 Warning Alert limit register
#define     C2_CAL_REG      0x09        //Channel 2 Critical Alert limit register
#define     C2_WAL_REG      0x0A        //Channel 2 Warning Alert limit register
#define     C3_CAL_REG      0x0B        //Channel 3 Critical Alert limit register
#define     C3_WAL_REG      0x0C        //Channel 3 Warning Alert limit register
#define     ONFF_REG        0x0F        //Mask/Enable register
#define     ID_REG 0XFF                 //Die ID register

#define     INA3221_ADDR1    0x80        //SLAVE ADDRESS 1000000 (GND)

typedef struct
{
    float voltageVal;           //mV
    float Shunt_voltage;        //uV
    float Shunt_Current;        //mA
    float powerVal;
}INA226;


void INA226_Init(void);
int INA226_SetRegPointer(i2cBASE_t *i2c, uint8_t addr, uint8_t reg);
int INA226_SendData(i2cBASE_t *i2c,uint8_t addr,uint8_t reg,uint16_t data)
int INA226_ReadData(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)

int INA226_Get_ID(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
int ina3221_GetVoltage(i2cBASE_t *i2c, uint8_t addr, uint16_t *data, uint8_t channel);
int INA226_GetShuntVoltage(i2cBASE_t *i2c, uint8_t addr, uint16_t *data, uint8_t channel);


