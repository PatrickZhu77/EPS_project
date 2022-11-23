
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

    //    clock->initTime_s = time(0);      //delete


//    clock->initTick = (uint32_t)xTaskGetTickCount();
//    clock->initOverflow = (uint32_t)xTaskGetTickOverflowCount();
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
//    clock->initTime_s = t;
//    clock->initTick = (uint32_t)xTaskGetTickCount();
//    clock->initOverflow = (uint32_t)xTaskGetTickOverflowCount();
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
uint32_t getcurrTime_sec(RTC_t *clock)
{
    uint32_t currTick, currOverflow, t = 0;

//    currOverflow = (uint32_t)xTaskGetTickOverflowCount() - clock->initOverflow;
//    if(currOverflow == 0)
//    {
//        currTick = (uint32_t)xTaskGetTickCount() - clock->initTick;
//    }
//    else
//    {
//        currTick = 0xFFFFFFFF - clock->initTick + (uint32_t)xTaskGetTickCount();
//    }
//
//    t = clock->initTime_s + currTick/1000 + currOverflow * 0xFFFFFFFF;     //tick counter is 32bit, so each overflow indicates a 0xFFFFFFFF                  //0x418937

    return t;
}

/***************************************************************************
 * @brief
 *   Calculate current microsecond according to RTC_t and tick counts
 *
 * @param[in] clock
 *   Pointer to Real-time clock data structure
 *
 * @return
 *   Returns current microsecond
 *
 ******************************************************************************/
uint16_t getcurrTime_ms(RTC_t *clock)
{
    uint32_t currTick, currOverflow, t = 0;

//    currOverflow = (uint32_t)xTaskGetTickOverflowCount() - clock->initOverflow;
//    if(currOverflow == 0)
//    {
//        currTick = (uint32_t)xTaskGetTickCount() - clock->initTick;
//    }
//    else
//    {
//        currTick = 0xFFFFFFFF - clock->initTick + (uint32_t)xTaskGetTickCount();
//    }
//
//    t = (uint16_t)(currTick%1000);

    return t;
}


uint32_t getrunTime_sec(RTC_t *clock)
{
    uint32_t currTick, currOverflow, t = 0;

//    currOverflow = (uint32_t)xTaskGetTickOverflowCount() - clock->initOverflow;
//    if(currOverflow == 0)
//    {
//        currTick = (uint32_t)xTaskGetTickCount() - clock->initTick;
//    }
//    else
//    {
//        currTick = 0xFFFFFFFF - clock->initTick + (uint32_t)xTaskGetTickCount();
//    }
//
//    t = currTick/1000 + currOverflow * 0xFFFFFFFF;

    return t;
}

