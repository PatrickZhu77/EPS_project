

#ifndef INCLUDE_CUSTOM_LOW_POWER_MODE_H_
#define INCLUDE_CUSTOM_LOW_POWER_MODE_H_

#include "sys_common.h"
#include "rti.h"
#include "sys_mpu.h"
#include "reg_flash.h"
#include "system.h"
#include "FreeRTOS.h"
#include "os_task.h"
#include "os_timer.h"
#include "os_queue.h"

void enter_doze(void);
void enter_snooze(void);
void post_wakeup(void);

#endif /* INCLUDE_CUSTOM_LOW_POWER_MODE_H_ */
