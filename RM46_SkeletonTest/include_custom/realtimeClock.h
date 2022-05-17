/*
 * realtimeClock.h
 *
 *  Created on: May 17, 2022
 *      Author: sdamkjar
 */
#include <stdint.h>

#ifndef INCLUDE_CUSTOM_REALTIMECLOCK_H_
#define INCLUDE_CUSTOM_REALTIMECLOCK_H_

typedef struct
{
    uint32_t initTime;
    uint32_t initTick;
    uint32_t initOverflow;
}RTC;

void resetRTC_debug(RTC * time);
void resetRTC(uint32_t t, RTC *clock);
uint32_t getcurrTime(RTC * time);

#endif /* INCLUDE_CUSTOM_REALTIMECLOCK_H_ */
