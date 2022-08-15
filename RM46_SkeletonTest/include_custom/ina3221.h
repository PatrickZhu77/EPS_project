#ifndef __INA3221_H
#define __INA3221_H

#include "reg_i2c.h"
#include "i2c.h"
#include "flash_data.h"


#define     INA3221_CFG_REG         0x00        //Configuration register

#define     INA3221_SV_REG1         0x01        //Shunt voltage register 1
#define     INA3221_BV_REG1         0x02        //Bus voltage register   1
#define     INA3221_SV_REG2         0x03        //Shunt voltage register 2
#define     INA3221_BV_REG2         0x04        //Bus voltage register   2
#define     INA3221_SV_REG3         0x05        //Shunt voltage register 3
#define     INA3221_BV_REG3         0x06        //Bus voltage register   3
#define     INA3221_CA_REG1         0x07        //Critical alert limit 1
#define     INA3221_WA_REG1         0x08        //Warning alert limit  1
#define     INA3221_CA_REG2         0x09        //Critical alert limit 2
#define     INA3221_WA_REG2         0x0A        //Warning alert limit  2
#define     INA3221_CA_REG3         0x0B        //Critical alert limit 3
#define     INA3221_WA_REG3         0x0C        //Warning alert limit  3

#define     INA3221_MASK_REG        0x0F        //Mask/Enable register


#define     INA3221_CFG_SETTING     0x0527      //configuration register with 16 averages and 1.1ms conversion time
#define     INA3221_MASK_SETTING    0x0002      //Mask register initialization

#define     INA3221_ADDR1    0x41        //SLAVE ADDRESS 100 0001 (VS)
#define     INA3221_ADDR2    0x40        //SLAVE ADDRESS 100 0000 (GND)
#define     INA3221_ADDR3    0x42        //SLAVE ADDRESS 100 0010 (SDA)
#define     INA3221_ADDR4    0x43        //SLAVE ADDRESS 100 0011 (SCL)

//#define     NUM_OF_INA3221   4          // 4 is default value according to schematic of eps

#define     INA3221_SHUNT_RESISTANCE    10      //mOhm. 0.01 Ohm is the value of shunt resistor used

#define     INA3221_SHUNTV_LSB      40      //40uV is the LSB of shunt voltage register
#define     INA3221_BUSV_LSB        8       //8mV is the LSB of bus voltage register

//#define     INA3221_err      0xC                 //0001_100. Alert Response slave address

/* Data structure for housekeeping data of ina3221 */
typedef struct
{
    uint8_t address;                 //i2c address
    uint16_t shunt_voltage[2];       //shunt voltage (raw data, can be converted to current in mA)
    uint16_t bus_voltage[2];         //bus voltage (raw data, can be converted to voltage in mV)
    uint32_t timestamp_sec;          //sec.
}ina3221_housekeeping_t;


void INA3221_SendData(i2cBASE_t *i2c,uint8_t addr,uint8_t reg,uint8_t *data);
void INA3221_ReceiveData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data);

void INA3221_Init(i2cBASE_t *i2c, sensor_config_t *data2, ina3221_housekeeping_t *data3);
void INA3221_ReadShuntVoltage_Raw(i2cBASE_t *i2c, ina3221_housekeeping_t *data, uint8_t channel);
void INA3221_ReadBusVoltage_Raw(i2cBASE_t *i2c, ina3221_housekeeping_t *data, uint8_t channel);

uint16_t INA3221_ConvToCurrent_mA(ina3221_housekeeping_t *data, uint8_t channel, uint16_t Rshunt);
uint16_t INA3221_ConvToVoltage_mV(ina3221_housekeeping_t *data, uint8_t channel);
uint16_t INA3221_ConvToPower_mW(ina3221_housekeeping_t *data, uint8_t channel, uint16_t Rshunt);


#endif
