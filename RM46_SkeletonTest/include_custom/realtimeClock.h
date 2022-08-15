
#include <stdint.h>

#ifndef INCLUDE_CUSTOM_REALTIMECLOCK_H_
#define INCLUDE_CUSTOM_REALTIMECLOCK_H_

typedef struct
{
    uint32_t initTime;          //sec. time when reseting RTC_t
    uint32_t initTick;          //tick count when reseting RTC_t
    uint32_t initOverflow;      //tick overflow when reseting RTC_t
}RTC_t;

void resetRTC_debug(RTC_t * time);
void resetRTC(uint32_t t, RTC_t *clock);

uint32_t getcurrTime(RTC_t * time);

#endif /* INCLUDE_CUSTOM_REALTIMECLOCK_H_ */
