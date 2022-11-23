/*
 * uart_cmd.h
 *
 *  Created on: Feb 15, 2022
 *      Author: sdamkjar
 */

#ifndef INCLUDE_UART_CMD_H_
#define INCLUDE_UART_CMD_H_


#include "FreeRTOS.h"
#include "os_task.h"
#include "os_timer.h"


unsigned char * uart_tx(uint8_t length,uint8 * cmd2);


#endif /* INCLUDE_UART_CMD_H_ */
