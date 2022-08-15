#include "ina3221.h"
#include "i2c.h"



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


/***************************************************************************
 * @brief
 *   Initialize the ina3221 sensor (set configuration register and mask register) with predefined values
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] data
 *   Pointer to the data structure.
 ******************************************************************************/
void INA3221_Init(i2cBASE_t *i2c, sensor_config_t *data, ina3221_housekeeping_t *data2)
{
    uint8_t config_temp[2]={0};
    uint8_t mask_temp[2]={0};

    /* Separate the 16bit integers into 8bit arrays */
    config_temp[0] = (uint8_t)(data->ina3221_cfg_setting >> 8);
    config_temp[1] = (uint8_t)data->ina3221_cfg_setting;
    mask_temp[0] = (uint8_t)(data->ina3221_mask_setting >> 8);
    mask_temp[1] = (uint8_t)data->ina3221_mask_setting;

    /* Write values to proper registers */
    INA3221_SendData(i2c,data2->address, INA3221_CFG_REG, config_temp);
    INA3221_SendData(i2c,data2->address, INA3221_MASK_REG, mask_temp);

}


/***************************************************************************
 * @brief
 *   Get shunt voltage (raw data) from shunt voltage register
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
void INA3221_ReadShuntVoltage_Raw(i2cBASE_t *i2c, ina3221_housekeeping_t *data, uint8_t channel)
{
    uint8_t data_temp[2]={0};
    uint16_t data_reg = 0;

    /* Decide which register to read */
    switch (channel)
    {
        case 1:
            INA3221_ReceiveData(i2c, data->address, INA3221_SV_REG1, data_temp);
            break;
        case 2:
            INA3221_ReceiveData(i2c, data->address, INA3221_SV_REG3, data_temp);
            break;
        default:
            break;
    }

    /* Combine 8bit array into 16bit integer */
    data_reg = data_temp[0];
    data_reg = data_reg << 8;
    data_reg = data_reg | data_temp[1];

    /* 2's complement conversion */
    if(data_reg&0x8000)
        data_reg = ~(data_reg - 1);

    /* Right shift 3 bits because the last 3 bits of the register is not used */
    data_reg = data_reg >> 3;

    /* Save the raw data to data structure */
    data->shunt_voltage[channel-1] = data_reg;

}


/***************************************************************************
 * @brief
 *   Get bus voltage (raw data) from bus voltage register
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
void INA3221_ReadBusVoltage_Raw(i2cBASE_t *i2c, ina3221_housekeeping_t *data, uint8_t channel)
{
    uint8_t data_temp[2]={0};
    uint16_t data_reg = 0;

    /* Decide which register to read */
    switch (channel)
    {
        case 1:
            INA3221_ReceiveData(i2c, data->address, INA3221_BV_REG1, data_temp);
            break;
        case 2:
            INA3221_ReceiveData(i2c, data->address, INA3221_BV_REG3, data_temp);
            break;
        default:
            break;
    }

    /* Combine 8bit array into 16bit integer */
    data_reg = data_temp[0];
    data_reg = data_reg << 8;
    data_reg = data_reg | data_temp[1];

    /* 2's complement conversion */
    if(data_reg&0x8000)
        data_reg = ~(data_reg - 1);

    /* Right shift 3 bits because the last 3 bits of the register is not used */
    data_reg = data_reg >> 3;

    /* Save the raw data to data structure */
    data->bus_voltage[channel-1] = data_reg;

}


/***************************************************************************
 * @brief
 *   Convert raw shunt voltage to current
 *
 * @param[in] data
 *   Pointer to the data structure.
 *
 * @param[in] channel
 *   Channel to read
 *
 * @param[in] data2
 *   Shunt resistance for calculation (in mOhm)
 *
 * @return
 *   Returns converted current value (in mA)
 ******************************************************************************/
uint16_t INA3221_ConvToCurrent_mA(ina3221_housekeeping_t *data, uint8_t channel, uint16_t Rshunt)
{
    uint32_t current_temp = data->shunt_voltage[channel-1] * INA3221_SHUNTV_LSB / Rshunt;
    return (uint16_t)current_temp;
}

/***************************************************************************
 * @brief
 *   Convert raw bus voltage to voltage in mV
 *
 * @param[in] data
 *   Pointer to the data structure.
 *
 * @param[in] channel
 *   Channel to read
 *
 * @return
 *   Returns converted voltage value (in mV)
 ******************************************************************************/
uint16_t INA3221_ConvToVoltage_mV(ina3221_housekeeping_t *data, uint8_t channel)
{
    return (data->bus_voltage[channel-1] * INA3221_BUSV_LSB);
}

/***************************************************************************
 * @brief
 *   Convert raw data to power
 *
 * @param[in] data
 *   Pointer to the data structure.
 *
 * @param[in] channel
 *   Channel to read
 *
 * @return
 *   Returns converted power value (in mW)
 ******************************************************************************/
uint16_t INA3221_ConvToPower_mW(ina3221_housekeeping_t *data, uint8_t channel, uint16_t Rshunt)
{
    uint16_t current_temp = INA3221_ConvToCurrent_mA(data, channel, Rshunt);
    uint16_t voltage_temp = INA3221_ConvToVoltage_mV(data, channel);
    uint32_t power_temp = current_temp * voltage_temp / 1000;
    return (uint16_t)power_temp;
}
