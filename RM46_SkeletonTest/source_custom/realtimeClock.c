
#include "realtimeClock.h"
#include "time.h"
#include "FreeRTOS.h"
#include "os_task.h"

/***************************************************************************
 * @brief
 *   reset real-time clock in debugging mode
 *
 * @param[in] clock
 *   Pointer to Real-time clock data structure
 *
 ******************************************************************************/
void resetRTC_debug(RTC_t *clock)
{
    clock->initTime = time(0);
    clock->initTick = (uint32_t)xTaskGetTickCount();
    clock->initOverflow = (uint32_t)xTaskGetTickOverflowCount();
}

/***************************************************************************
 * @brief
 *   reset real-time clock
 *
 * @param[in] clock
 *   Pointer to Real-time clock data structure
 *
 ******************************************************************************/
void resetRTC(uint32_t t, RTC_t *clock)
{
    clock->initTime = t;
    clock->initTick = (uint32_t)xTaskGetTickCount();
    clock->initOverflow = (uint32_t)xTaskGetTickOverflowCount();
}


/***************************************************************************
 * @brief
 *   Calculate current time according to RTC_t and tick counts
 *
 * @param[in] clock
 *   Pointer to Real-time clock data structure
 *
 * @return
 *   Returns current time (in sec)
 *
 ******************************************************************************/
uint32_t getcurrTime(RTC_t *clock)
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


