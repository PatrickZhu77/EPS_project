#ifndef __INA226_H
#define __INA226_H

#include "reg_i2c.h"
#include "flash_data.h"

/**************List of ina226 sensors**************
 *
 *  0: overcurrent protection of watchdog Vout
 *  1: overcurrent protection of buck converter 3v3 Vout
 *  2: overcurrent protection of buck converter 1v2 Vout
 *  3: overcurrent protection of buck converter 5v0 Vout
 *  4: overcurrent protection of 3v3
 *  5: overcurrent protection of 1v2
 *  6: current monitor of pv_3v3
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
 *  25: current monitor of channel 17
 *  26: current monitor of channel 18
 *
 *******************************************/

#define     INA226_CFG_REG         0x00        //Configuration register
#define     INA226_SV_REG          0x01        //Shunt voltage register (shunt resistor here is 0.1 ohm)
#define     INA226_BV_REG          0x02        //Bus voltage register
#define     INA226_PWR_REG         0x03        //Power register
#define     INA226_CUR_REG         0x04        //Current register
#define     INA226_CAL_REG         0x05        //Calibration register
#define     INA226_MASK_REG        0x06        //Mask/Enable register
#define     INA226_AL_REG          0x07        //Alert limit register

#define     INA226_ADDR1    0x47                //SLAVE ADDRESS 1000111 (VS|SCL)
//All 27 addresses need to be defined
//#define     NUM_OF_CHANNELS  18     //There are 18 channels according to schematic of eps.

#define     INA226_CFG_SETTING          0x4527       //configuration register with 16 averages and 1.1ms conversion time
#define     INA226_CAL_SETTING          0x200        //calculated calibration register value: 0.00512/(INA226_CURRENT_LSB(in A) * INA226_SHUNT_RESISTANCE(in Ohm))
#define     INA226_OVERCURRENT_MASK     0x8000       //Mask for overcurrent protection sensor. 0x8000: enable Vshunt overvoltage (overcurrent) alert
#define     INA226_OVERCURRENT_ALERT    0x2710       //Alert value for overcurrent protection sensor. 10A overcurrent limit
#define     INA226_CHANNEL_MASK         0x8001       //Mask for channel sensor. 0x8001: enable Vshunt overvoltage (overcurrent) alert
                                                     //Bit 0 is Alert Latch Enable bit. It is set to Latch mode, which means the Alert pin and Alert Flag bit remains
                                                     //active following a fault until the Mask/Enable Register has been read.
#define     INA226_CHANNEL_ALERT        0x2710       //Alert value for channel sensor. 10A overcurrent limit
#define     INA226_BATTERY_MASK         0x0000       //Mask for battery sensor. Disable alert.
#define     INA226_MONITOR_MASK         0x8000       //Mask for monitor sensor. 0x8000: enable Vshunt overvoltage (overcurrent) alert
#define     INA226_MONITOR_ALERT        0x2710       //Alert value for monitor sensor. 10A overcurrent limit


#define     INA226_SHUNT_RESISTANCE    10      //mOhm. 0.01 Ohm is the value of shunt resistor used

#define     INA226_SHUNTV_LSB      2500      //2500nV (=2.5uV) is the LSB of shunt voltage register
#define     INA226_BUSV_LSB        1250      //1250uV (=1.25mV) is the LSB of bus voltage register
#define     INA226_CURRENT_LSB     1         //1mA is the chosen LSB of the current register
#define     INA226_POWER_LSB       25        //25mW is the calculated LSB of the power register

#define     MASK_REG_ALERT_FLAG_MASK    16

//#define     INA226_err      0xC                 //0001_100. Alert Response slave address

/* Data structure for housekeeping data of ina226 */
typedef struct
{
    uint8_t address;              //i2c address
//    uint16_t mask_reg;
    uint16_t alert_reg;           //alert threshold value (normally in mA)
    uint16_t shunt_voltage;       //shunt voltage (raw data)
    uint16_t bus_voltage;         //bus voltage (raw data, can be converted to voltage in mV)
    uint16_t current;             //current (raw data, can be converted to current in mA)
    uint16_t power;               //power (raw data, can be converted to power in mW)
    uint32_t timestamp_sec;       //sec.
}ina226_housekeeping_t;



void INA226_SendData(i2cBASE_t *i2c,uint8_t addr,uint8_t reg,uint8_t *data);
void INA226_ReceiveData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data);

uint16_t INA226_CurrentToAlert_ShuntVoltage_Raw(uint16_t current_mA, uint16_t cal_reg);
uint16_t INA226_VoltageToAlert_BusVoltage_Raw(uint16_t voltage_mV);

void INA226_Init(i2cBASE_t *i2c, uint16_t Rshunt, sensor_config_t *data2, uint16_t mask, ina226_housekeeping_t *data3);
void INA226_SetMaskReg(i2cBASE_t *i2c, ina226_housekeeping_t *data, uint16_t mask);
void INA226_SetAlertReg(i2cBASE_t *i2c, ina226_housekeeping_t *data, uint16_t alert, uint16_t cal_reg);
uint16_t INA226_ReadMaskReg(i2cBASE_t *i2c, ina226_housekeeping_t *data);

void INA226_ReadShuntVoltage_Raw(i2cBASE_t *i2c, ina226_housekeeping_t *data);
void INA226_ReadBusVoltage_Raw(i2cBASE_t *i2c, ina226_housekeeping_t *data);
void INA226_ReadCurrent_Raw(i2cBASE_t *i2c, ina226_housekeeping_t *data);
void INA226_ReadPower_Raw(i2cBASE_t *i2c, ina226_housekeeping_t *data);

uint16_t INA226_ConvToCurrent_mA(ina226_housekeeping_t *data);
uint16_t INA226_ConvToVoltage_mV(ina226_housekeeping_t *data);
uint16_t INA226_ConvToPower_mW(ina226_housekeeping_t *data);


#endif
