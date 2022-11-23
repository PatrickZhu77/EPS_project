
#include <stdint.h>

#ifndef INCLUDE_CUSTOM_REALTIMECLOCK_H_
#define INCLUDE_CUSTOM_REALTIMECLOCK_H_

typedef struct
{
    uint16_t initTime_ms;          //ms.  microsecond when reseting RTC_t
    uint32_t initTime_s;          //sec. time when reseting RTC_t
    uint32_t initTick;            //tick count when reseting RTC_t
    uint32_t initOverflow;        //tick overflow when reseting RTC_t
}RTC_t;

void resetRTC_debug(RTC_t * time);
void resetRTC(uint32_t t, RTC_t *clock);

uint32_t getcurrTime_sec(RTC_t * time);
uint16_t getcurrTime_ms(RTC_t *clock);

uint32_t getrunTime_sec(RTC_t *clock);


#endif /* INCLUDE_CUSTOM_REALTIMECLOCK_H_ */
