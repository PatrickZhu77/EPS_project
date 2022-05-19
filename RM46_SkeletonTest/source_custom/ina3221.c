#include "ina3221.h"
#include "i2c.h"


/*****************************************************************************
 * @brief
 *   Set pointer to a certain register.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] addr
 *   I2C address, in 8 bit format, where LSB is reserved for R/W bit.
 *
 * @param[in] reg
 *   Register to point.
 *
 * @return
 *   Returns 0 if register written, <0 if unable to write to register.
 ******************************************************************************/
void INA3221_SetRegPointer(i2cBASE_t *i2c, uint8_t addr, uint8_t reg)
{
    uint8_t delay=0;

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

    for(delay=0;delay<50;delay++);

}



/***************************************************************************//**
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
void INA3221_SendData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data)
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
    //// Start sending the data to Slave
    /*****************************************/

    /* wait until MST bit gets cleared, this takes
     * few cycles after Bus Busy is cleared */
    while(i2cIsMasterReady(i2c) != true);
    /* Configure address of Slave to talk to */
    i2cSetSlaveAdd(i2c, addr);

    /* Set direction to Transmitter */
    /* Note: Optional - It is done in Init */
    i2cSetDirection(i2c, I2C_TRANSMITTER);

    /* Configure Data count */
    /* Data Count + 1 ( Word Address) */
    i2cSetCount(i2c, 2);

    /* Set mode as Master */
    i2cSetMode(i2c, I2C_MASTER);

    /* Set Stop after programmed Count */
    i2cSetStop(i2c);

    /* Transmit Start Condition */
    i2cSetStart(i2c);

    /* Transmit DATA_COUNT number of data in Polling mode */
    i2cSend(i2c, 2, data);

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(i2c) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(i2c) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(i2c);

    /* Simple Delay before starting Next Block */
    /* Depends on how quick the Slave gets ready */
    for(delay=0;delay<50;delay++);

}

/***************************************************************************//**
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
void INA3221_ReceiveData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data)
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
    //// Start receving the data From Slave
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
    i2cSetCount(i2c, 2);

    /* Set mode as Master */
    i2cSetMode(i2c, I2C_MASTER);

    /* Set Stop after programmed Count */
    i2cSetStop(i2c);

    /* Transmit Start Condition */
    i2cSetStart(i2c);

    /* Tranmit DATA_COUNT number of data in Polling mode */
    i2cReceive(i2c, 2, data);

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(i2c) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(i2c) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(i2c);
    for(delay=0;delay<50;delay++);

}


//LSB = 40uV/bit
void INA3221_GetShuntVoltage(i2cBASE_t *i2c, uint8_t addr, uint16_t *data, uint8_t channel)
{
    uint8_t data_temp[2]={0};           //temp. data from i2c
    uint16_t data_reg = 0;              //data in the register

    switch (channel)
    {
        case 1:
            INA3221_ReceiveData(i2c, addr, SV_REG1, data_temp);
            break;
        case 2:
            INA3221_ReceiveData(i2c, addr, SV_REG2, data_temp);
            break;
        case 3:
            INA3221_ReceiveData(i2c, addr, SV_REG3, data_temp);
            break;
        default:
            break;
    }

    data_reg = data_temp[0];
    data_reg = data_reg << 8;
    data_reg = data_reg | data_temp[1];

    if(data_reg&0x8000)
        data_reg = ~(data_reg - 1);

    data_reg = data_reg & voltage_mask;
    data_reg = data_reg >> 3;

    *data = data_reg;

}


//LSB = 8mV/bit
void INA3221_GetBusVoltage(i2cBASE_t *i2c, uint8_t addr, uint16_t *data, uint8_t channel)
{
    uint8_t data_temp[2]={0};           //temp. data from i2c
    uint16_t data_reg = 0;              //data in the register

    switch (channel)
    {
        case 1:
            INA3221_ReceiveData(i2c, addr, BV_REG1, data_temp);
            break;
        case 2:
            INA3221_ReceiveData(i2c, addr, BV_REG2, data_temp);
            break;
        case 3:
            INA3221_ReceiveData(i2c, addr, BV_REG3, data_temp);
            break;
        default:
            break;
    }

    data_reg = data_temp[0];
    data_reg = data_reg << 8;
    data_reg = data_reg | data_temp[1];

    if(data_reg&0x8000)
        data_reg = ~(data_reg - 1);

    data_reg = data_reg & voltage_mask;
    data_reg = data_reg >> 3;

    *data = data_reg;

}


//INA3221 INA3221_data;
void INA3221_Init(i2cBASE_t *i2c, uint8_t addr, ina3221_data *data)
{
    uint8_t config_temp[2]={0};
    config_temp[0] = (uint8_t)(data->config_reg >> 8);
    config_temp[1] = (uint8_t)data->config_reg;

    INA3221_SendData(i2c,addr, CFG_REG, config_temp);

}

void INA3221_GetAlertInfo(i2cBASE_t *i2c, ina3221_data *data)
{
    uint8_t temp_addr[1] = {0};

    /*****************************************/
    // Receive the alert sensor i2c address
    /*****************************************/
    uint8_t delay = 0;
    while(i2cIsMasterReady(i2c) != true);

    /* Configure address of Slave to talk to */
    i2cSetSlaveAdd(i2c, INA3221_err);

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
    i2cReceive(i2c, 1, temp_addr);

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(i2c) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(i2c) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(i2c);
    for(delay=0;delay<50;delay++);



    /*****************************************/
    // Read from alert sensor
    /*****************************************/
    data->address = temp_addr[0];

    INA3221_GetShuntVoltage(i2c,data->address,&data->shunt_voltage[0],1);
    INA3221_GetVoltage(i2c,data->address,&data->bus_voltage[0],1);
//    INA226_GetCalReg(i2c, data->address, &data->calibration);
//    INA226_GetCurrent(i2c, data->address, &data->current);
//    INA226_GetPower(i2c, data->address, &data->power);

}


