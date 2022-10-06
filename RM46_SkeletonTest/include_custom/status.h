/*
 * status.h
 *
 *  Created on: Sep 08, 2022
 *      Author: zqqqq
 */

#ifndef INCLUDE_CUSTOM_STATUS_H_
#define INCLUDE_CUSTOM_STATUS_H_

typedef struct
{
    uint8_t num_of_reset;               //number of the reset time
    uint8_t config_ver;                 //version of the current configuration setting
    uint16_t runtime_ms;                //ms. millisecond of system runtime
    uint16_t current_time_ms;           //ms. millisecond of current time
    uint32_t runtime_s;                 //s. second of system runtime
    uint32_t current_time_s;            //s. second of current time
}system_status_t;


#endif /* INCLUDE_CUSTOM_STATUS_H_ */
