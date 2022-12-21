
#ifndef INCLUDE_CUSTOM_CHANNEL_H_
#define INCLUDE_CUSTOM_CHANNEL_H_

#include "stdint.h"
#include "gio.h"
#include "het.h"
#include "ina226.h"
#include "battery.h"
#include "flash_data.h"
#include "realtimeClock.h"
#include "FreeRTOS.h"
#include "os_task.h"

//#define     NUM_OF_CHANNELS  18     //There are 18 channels according to schematic of eps.

#define     LOWEST_CH   0
#define     LOW_CH      1
#define     MID_CH      2
#define     HIGH_CH     3
#define     HIGHEST_CH  4

#define     MINIMUM_TIME_INTERVAL_AVERAGE       200     //ms
#define     NUM_OF_T_INTERVAL           3

#define     OVERCURRENT_THRESHOLD_INCREMENT     100     //mA

#define     CHANNEL_RECOVER_FROM_CRITICAL_MODE      10  //times of the period of channel task (100ms by default), so it is 1000ms

#define     CHANNEL_MAXIMUM_VOLTAGE             10000    //mV (for testing only)
#define     CHANNEL_MAXIMUM_CURRENT             620
#define     CHANNEL_MAXIMUM_CURRENT_INCREMENT   30

#define     SYSTEM_MODE_CRITICAL    0
#define     SYSTEM_MODE_SAFE        1
#define     SYSTEM_MODE_FULL        2

typedef struct
{
    uint8_t num;                   //# of channel
    uint8_t priority;              //priority of channel
    uint8_t mode;                  //mode that indicating whether the channel should be on or off
    uint8_t sw;                    //switch of channel. 0:off, 1:on
    uint8_t resume;                //Resume statement. It is set to 0 when initialized.
                                   //When resume is set to 1, switch on the channel in next period
    uint16_t current;              //mA
    uint16_t voltage;              //mV
    uint32_t group_mask;           //group mask channels. 1 bit for each channel. If grouped with a channel, that bit is set to 1
    uint32_t t_lastTrip;           //in tick. Tick time of last trip
    uint32_t t_interval[NUM_OF_T_INTERVAL];        //in tick. Time intervals to the last trip
}channel_data_t;


/******************List of EN pins for channels*********************/
static gioPORT_t * CH[18] = {
                                 hetPORT1,      //channel 1 register
                                 hetPORT1,
                                 hetPORT1,
                                 hetPORT1,
                                 hetPORT1,
                                 hetPORT1,
                                 hetPORT1,
                                 hetPORT1,
                                 hetPORT1,
                                 hetPORT1,
                                 gioPORTA,
                                 gioPORTA,
                                 gioPORTA,
                                 gioPORTA,
                                 gioPORTA,
                                 hetPORT1,
                                 gioPORTB,
                                 gioPORTB
                            };
static uint8_t CH_num[18] = {
                                 19,        //channel 1 register bit number
                                 0,
                                 7,
                                 13,
                                 15,
                                 24,
                                 3,
                                 2,
                                 1,
                                 5,
                                 2,
                                 7,
                                 1,
                                 5,
                                 0,
                                 11,
                                 2,
                                 3
                            };

void channel_on(channel_data_t *Fchannel, channel_data_t *channel);
void channel_off(channel_data_t *Fchannel, channel_data_t *channel);

void channel_set_group_mask(channel_data_t *Fchannel, channel_data_t *channel1, channel_data_t *channel2);

void channel_check_mode(channel_data_t *Fchannel, uint8_t sys_mode);
void channel_read_rawdata_and_convert(channel_data_t *Fchannel, ina226_housekeeping_t *Fdata);

void channel_check_batteryV_then_update_switch(channel_data_t *Fchannel, battery_data_t *Fdata1, system_config_t *Fdata2);
void channel_check_batteryI_then_update_switch(channel_data_t *Fchannel, battery_data_t *Fdata1, system_config_t *Fdata2);
void channel_check_chanV_then_update_switch(channel_data_t *Fchannel, system_config_t *Fdata);
void channel_resume(channel_data_t *Fchannel);

void channel_check_overcurrent_then_config_and_resume(channel_data_t *Fchannel, ina226_housekeeping_t *Fdata, system_config_t *data, sensor_config_t data2, i2cBASE_t *i2c);

#endif /* INCLUDE_CUSTOM_CHANNEL_H_ */
