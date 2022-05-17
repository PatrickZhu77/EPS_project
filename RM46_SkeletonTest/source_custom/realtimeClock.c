
#include "realtimeClock.h"
#include "time.h"
#include "FreeRTOS.h"
#include "os_task.h"

void resetRTC_debug(RTC *clock)
{
    clock->initTime = time(0);
    clock->initTick = (uint32_t)xTaskGetTickCount();
    clock->initOverflow = (uint32_t)xTaskGetTickOverflowCount();
}

void resetRTC(uint32_t t, RTC *clock)
{
    clock->initTime = t;
    clock->initTick = (uint32_t)xTaskGetTickCount();
    clock->initOverflow = (uint32_t)xTaskGetTickOverflowCount();
}


uint32_t getcurrTime(RTC *clock)
{
    uint32_t currTick, currOverflow, t;

    currOverflow = (uint32_t)xTaskGetTickOverflowCount() - clock->initOverflow;
    if(currOverflow == 0)
    {
        currTick = (uint32_t)xTaskGetTickCount() - clock->initTick;
    }
    else
    {
        currTick = 0xFFFFFFFF - clock->initTick + (uint32_t)xTaskGetTickCount();
    }

    t = clock->initTime + currTick/1000 + currOverflow * 0x418937;

    return t;
}


