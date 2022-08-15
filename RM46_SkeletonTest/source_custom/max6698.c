
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

/***************************************************************************
 * @brief
 *   Initialize the max6698 sensor (set 3 configuration registers) with predefined values
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] data
 *   Pointer to the data structure.
 ******************************************************************************/
void MAX6698_Init(i2cBASE_t *i2c, sensor_config_t *data, max6698_housekeeping_t *data2)
{
    uint8_t config_temp1[1]={0};
    uint8_t config_temp2[1]={0};
    uint8_t config_temp3[1]={0};

    /* Separate the 16bit integers into 8bit arrays */
    config_temp1[0] = (uint8_t)data->max6698_cfg1_setting;
    config_temp2[0] = (uint8_t)data->max6698_cfg2_setting;
    config_temp3[0] = (uint8_t)data->max6698_cfg3_setting;


    MAX6698_SendData(i2c, data2->address, CFG1_REG, config_temp1);
    MAX6698_SendData(i2c, data2->address, CFG2_REG, config_temp2);
    MAX6698_SendData(i2c, data2->address, CFG3_REG, config_temp3);

}

/***************************************************************************
 * @brief
 *   Get temperature (raw data) from thermistor register
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] data
 *   Pointer to the data structure.
 *
 * @param[in] channel
 *   Channel to read
 *
 ******************************************************************************/
void MAX6698_ReadTemp_Raw(i2cBASE_t *i2c, max6698_housekeeping_t *data, uint8_t channel)
{
    uint8_t data_temp[1]={0};

    /* Decide which register to read */
    switch (channel)
    {
        case 1:
            MAX6698_ReceiveData(i2c, data->address, TM1_REG, data_temp);
            break;
        case 2:
            MAX6698_ReceiveData(i2c, data->address, TM2_REG, data_temp);
            break;
        default:
            break;
    }

    /* Save the raw data to data structure */
    data->temp[channel-1] = data_temp[0];
}


void MAX6698_ConvertTemp_C(max6698_housekeeping_t *data)
{

}
