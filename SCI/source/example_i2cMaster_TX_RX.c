/** @example example_i2cMaster_TX_RX.c
*   This is an example which contains a sample I2C Master Transmit and 
*   receive functionality. This example can be used to talk to any 
*   I2C Slave at address 0x8 and Baud rate as configured in the HALCoGen 
*   GUI which is by default 100KHz
*
*/

/* 
* Copyright (C) 2009-2015 Texas Instruments Incorporated - http://www.ti.com
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
#include "i2c.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
#define DATA_COUNT  10

#define Master_Address 0x26
#define Slave_Address  0x8

uint8_t TX_Data_Master[10] = { 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19};
uint8_t RX_Data_Master[10] = { 0 };

uint8_t TX_Data_Slave[10] = { 0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29};
uint8_t RX_Data_Slave[10] = { 0 };

/* USER CODE END */

void main(void)
{
/* USER CODE BEGIN (3) */

	int repeat = 0; int delay =0;

///////////////////////////////////////////////////////////////
//        Master Transfer Functionality                      //
///////////////////////////////////////////////////////////////
	/* I2C Init as per GUI
	 *  Mode = Master - Transmitter
	 *  baud rate = 100KHz
	 *  Count = 10
	 *  Bit Count = 8bit
	 */
	i2cInit();

	/* Configure address of Slave to talk to */
	i2cSetSlaveAdd(i2cREG1, Slave_Address);

	/* Set direction to Transmitter */
	/* Note: Optional - It is done in Init */
	i2cSetDirection(i2cREG1, I2C_TRANSMITTER);


	for(repeat = 0; repeat < 2; repeat++)
	{
		/* Configure Data count */
		/* Note: Optional - It is done in Init, unless user want to change */
		i2cSetCount(i2cREG1, DATA_COUNT);

		/* Set mode as Master */
		i2cSetMode(i2cREG1, I2C_MASTER);

		/* Set Stop after programmed Count */
		i2cSetStop(i2cREG1);

		/* Transmit Start Condition */
		i2cSetStart(i2cREG1);

		/* Tranmit DATA_COUNT number of data in Polling mode */
		i2cSend(i2cREG1, DATA_COUNT, TX_Data_Master);

		/* Wait until Bus Busy is cleared */
		while(i2cIsBusBusy(i2cREG1) == true);

		/* Wait until Stop is detected */
		while(i2cIsStopDetected(i2cREG1) == 0);

		/* Clear the Stop condition */
		i2cClearSCD(i2cREG1);

		/* Simple Dealya before starting Next Block */
		/* Depends on how quick the Slave gets ready */
		for(delay=0;delay<1000000;delay++);

	}

	///////////////////////////////////////////////////////////////
	//        Master Receive Functionality                      //
	///////////////////////////////////////////////////////////////

	/* Configure address of Slave to talk to */
	i2cSetSlaveAdd(i2cREG1, Slave_Address);

	/* Set direction to receiver */
	i2cSetDirection(i2cREG1, I2C_RECEIVER);

	for(repeat = 0; repeat < 2; repeat++)
	{
		/* Configure Data count */
		/* Note: Optional - It is done in Init, unless user want to change */
		i2cSetCount(i2cREG1, DATA_COUNT);

		/* Set mode as Master */
		i2cSetMode(i2cREG1, I2C_MASTER);

		/* Set Stop after programmed Count */
		i2cSetStop(i2cREG1);

		/* Transmit Start Condition */
		i2cSetStart(i2cREG1);

		/* Tranmit DATA_COUNT number of data in Polling mode */
		i2cReceive(i2cREG1, DATA_COUNT, RX_Data_Master);

		/* Wait until Bus Busy is cleared */
		while(i2cIsBusBusy(i2cREG1) == true);

		/* Wait until Stop is detected */
		while(i2cIsStopDetected(i2cREG1) == 0);

		/* Clear the Stop condition */
		i2cClearSCD(i2cREG1);

		/* Simple Dealya before starting Next Block */
		/* Depends on how quick the Slave gets ready */
		for(delay=0;delay<1000000;delay++);

	}

	asm(" nop");
	asm(" nop");
	asm(" nop");

	while(1);

/* USER CODE END */
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
