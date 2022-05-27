
#include "i2c.h"
#include "max6698.h"



/***************************************************************************
 * @brief
 *   Write data to given register with address.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address, in 7 bit format
 *
 * @param[in] reg
 *   Register to be written.
 *
 * @param[in] data
 *   Pointer to the data to be transmitted.
 *
 * @return
 *   Returns 0 if register written, <0 if unable to write to register.
 ******************************************************************************/
void MAX6698_SendData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data)
{
    uint8_t delay=0;

//    while(i2cIsMasterReady(i2c) != true);
    /* Configure address of Slave to talk to */
    i2cSetSlaveAdd(i2c, addr);

    /* Set direction to Transmitter */
    /* Note: Optional - It is done in Init */
    i2cSetDirection(i2c, I2C_TRANSMITTER);

    /* Configure Data count */
    /* Data Count + 1 ( Word Address) */
    i2cSetCount(i2c, 1+1);

    /* Set mode as Master */
    i2cSetMode(i2c, I2C_MASTER);

    /* Set Stop after programmed Count */
    i2cSetStop(i2c);

    /* Transmit Start Condition */
    i2cSetStart(i2c);

    /* Send the Word Address */
    i2cSendByte(i2c, reg);

    /* Transmit DATA_COUNT number of data in Polling mode */
    i2cSend(i2c,1, data);

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(i2c) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(i2c) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(i2c);

    while(i2cIsMasterReady(i2c) != true);
    /* Simple Delay before starting Next Block */
    /* Depends on how quick the Slave gets ready */
    for(delay=0;delay<50;delay++);

}

/***************************************************************************
 * @brief
 *   Read data from given register with address.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address, in 7 bit format.
 *
 * @param[in] reg
 *   Register to be read.
 *
 * @param[out] data
 *   Pointer to the data to be stored.
 *
 * @return
 *   Returns 0 if register written, <0 if unable to write to register.
 ******************************************************************************/
void MAX6698_ReceiveData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data)
{
    uint8_t delay=0;

    //while(i2cIsMasterReady(i2c) != true);

    /* Configure address of Slave to talk to */
    i2cSetSlaveAdd(i2c, addr);

    /* Set direction to Transmitter */
    /* Note: Optional - It is done in Init */
    i2cSetDirection(i2c, I2C_TRANSMITTER);

    /* Configure Data count */
    /* Slave address + Word address write operation before reading */
    i2cSetCount(i2c, 0x1);

    /* Set mode as Master */
    i2cSetMode(i2c, I2C_MASTER);

    /* Set Stop after programmed Count */
    i2cSetStop(i2c);

    /* Transmit Start Condition */
    i2cSetStart(i2c);

    /* Send the Word Address */
    i2cSendByte(i2c, reg);

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(i2c) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(i2c) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(i2c);

    /*****************************************/
    //  Start receving the data From Slave
    /*****************************************/

    /* wait until MST bit gets cleared, this takes
     * few cycles after Bus Busy is cleared */
    while(i2cIsMasterReady(i2c) != true);

    /* Configure address of Slave to talk to */
    i2cSetSlaveAdd(i2c, addr);

    /* Set direction to receiver */
    i2cSetDirection(i2c, I2C_RECEIVER);

    /* Configure Data count */
    /* Note: Optional - It is done in Init, unless user want to change */
    i2cSetCount(i2c, 1);

    /* Set mode as Master */
    i2cSetMode(i2c, I2C_MASTER);

    /* Set Stop after programmed Count */
    i2cSetStop(i2c);

    /* Transmit Start Condition */
    i2cSetStart(i2c);

    /* Tranmit DATA_COUNT number of data in Polling mode */
    i2cReceive(i2c, 1, data);

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(i2c) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(i2c) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(i2c);
    for(delay=0;delay<50;delay++);

}

void MAX6698_GetTM(i2cBASE_t *i2c, uint8_t addr, uint8_t num, uint8_t *data)
{
    //int err = -1;
    uint8_t data_temp[1]={0};           //temp. data from i2c

    MAX6698_ReceiveData(i2c, addr, TM1_REG+(num-1), data_temp);

    *data = data_temp[0];

    //return(err);
}

void MAX6698_SetCfgReg(i2cBASE_t *i2c, uint8_t addr, uint8_t num, uint8_t data)
{
    uint8_t data_temp[1]={data};           //default reg. data

    MAX6698_SendData(i2c,addr, CFG1_REG+(num-1), data_temp);
}


void MAX6698_SetAlertReg(i2cBASE_t *i2c, uint8_t addr, uint8_t num, uint8_t data)
{
    uint8_t data_temp[1]={data};           //default reg. data

    MAX6698_SendData(i2c,addr, CFG1_REG+(num-1), data_temp);
}

void MAX6698_SetOverTReg(i2cBASE_t *i2c, uint8_t addr, uint8_t num, uint8_t data)
{
    uint8_t data_temp[1]={data};           //default reg. data

    MAX6698_SendData(i2c,addr, TM1O_REG+(num-1), data_temp);
}


void MAX6698_Init(i2cBASE_t *i2c, uint8_t addr, max6698_data *data)
{
    MAX6698_SetCfgReg(i2c, addr, 1, data->config_reg[0]);
    MAX6698_SetCfgReg(i2c, addr, 2, data->config_reg[1]);
    MAX6698_SetCfgReg(i2c, addr, 3, data->config_reg[2]);

}
