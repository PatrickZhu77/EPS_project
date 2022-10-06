
#ifndef INCLUDE_CUSTOM_DATA_STRUCTURE_CONST_H_
#define INCLUDE_CUSTOM_DATA_STRUCTURE_CONST_H_

#define     NUM_OF_INA226                           27      // 27 is the default value according to schematic of eps
#define     NUM_OF_INA226_OVERCURRENT_PROTECTION    6       // 6 is the default value according to schematic of eps
#define     NUM_OF_INA226_MONITOR                   1       // 1 is the default value according to schematic of eps
#define     NUM_OF_INA226_BATTERY                   2       // 2 is the default value according to schematic of battery board
#define     NUM_OF_INA226_CHANNEL                   18      // 18 is the default value according to schematic of eps
#define     NUM_OF_CHANNELS                         18      //There are 18 channels according to schematic of eps.

#define     NUM_OF_INA3221   4          // 4 is default value according to schematic of eps

#define     NUM_OF_MAX6698  1           // 1 is default value according to schematic of battery board


#define     NUM_OF_SOFTWARE_VERSION         0
#define     NUM_OF_CONFIG_VERSION           0

#define     NUM_OF_16BIT_IN_SYSTEM_CONFIG_T         9+NUM_OF_INA226_OVERCURRENT_PROTECTION*2+NUM_OF_INA226_MONITOR*2+NUM_OF_INA226_BATTERY+NUM_OF_INA226_CHANNEL+NUM_OF_INA3221
#define     NUM_OF_32BIT_IN_SYSTEM_CONFIG_T         8

#define     NUM_OF_8BIT_IN_SENSOR_CONFIG_T          4
#define     NUM_OF_16BIT_IN_SENSOR_CONFIG_T         7

#define     NUM_OF_8BIT_IN_CHANNEL_CONFIG_T         1
#define     NUM_OF_16BIT_IN_CHANNEL_CONFIG_T        6
#define     NUM_OF_32BIT_IN_CHANNEL_CONFIG_T        1


#define     WDT_COUNTER_OBC         0x3C             //sec. 0x3C: 60 sec
#define     WDT_COUNTER_GROUND      0x93A80          //sec. 0x93A80: 1 week

#endif /* INCLUDE_CUSTOM_DATA_STRUCTURE_CONST_H_ */
