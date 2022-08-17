
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


#define     NUM_OF_8BIT_IN_CHANNEL_CONFIG_T         1       //real block size will time this value by NUM_OF_CHANNELS (with 8 bit crc for every 8 bit data)
#define     NUM_OF_16BIT_IN_CHANNEL_CONFIG_T        5       //real block size will time this value by NUM_OF_CHANNELS (with 8 bit crc for every 8 bit data)
#define     NUM_OF_32BIT_IN_CHANNEL_CONFIG_T        1       //real block size will time this value by NUM_OF_CHANNELS (with 8 bit crc for every 8 bit data)

#define     NUM_OF_8BIT_IN_SYSTEM_CONFIG_T          8
#define     NUM_OF_16BIT_IN_SYSTEM_CONFIG_T         7+NUM_OF_INA226_OVERCURRENT_PROTECTION*2+NUM_OF_INA226_MONITOR*2+NUM_OF_INA226_BATTERY+NUM_OF_INA226_CHANNEL+NUM_OF_INA3221

#define     NUM_OF_8BIT_IN_SENSOR_CONFIG_T          4
#define     NUM_OF_16BIT_IN_SENSOR_CONFIG_T         7




#endif /* INCLUDE_CUSTOM_DATA_STRUCTURE_CONST_H_ */
