#include "ina226.h"
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
 *
 ******************************************************************************/
void INA226_SendData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data)
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
    i2cSetCount(i2c, 2+1);

    /* Set mode as Master */
    i2cSetMode(i2c, I2C_MASTER);

    /* Set Stop after programmed Count */
    i2cSetStop(i2c);

    /* Transmit Start Condition */
    i2cSetStart(i2c);

    /* Send the Word Address */
    i2cSendByte(i2c, reg);

    /* Transmit DATA_COUNT number of data in Polling mode */
    i2cSend(i2c,2, data);

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
void INA226_ReceiveData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data)
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
 *   Convert overcurrent alert value (in mA) to raw shunt voltage value
 *
 * @param[in] current_mA
 *   Value to be converted.
 *
 * @return
 *   Returns converted shunt voltage value
 ******************************************************************************/
uint16_t INA226_CurrentToAlert_ShuntVoltage_Raw(uint16_t current_mA, uint16_t cal_reg)
{
    uint32_t shuntV_temp = (current_mA / INA226_CURRENT_LSB) * 2048 / cal_reg;
    return (uint16_t)shuntV_temp;
}

/***************************************************************************
 * @brief
 *   Convert overvoltage alert value (in mV) to raw bus voltage value
 *
 * @param[in] voltage_mV
 *   Value to be converted.
 *
 * @return
 *   Returns converted bus voltage value
 ******************************************************************************/
uint16_t INA226_VoltageToAlert_BusVoltage_Raw(uint16_t voltage_mV)
{
    uint32_t busV_temp = voltage_mV * 1000 / INA226_BUSV_LSB;
    return (uint16_t)busV_temp;
}

/***************************************************************************
 * @brief
 *   Calculate calibration register value according to shunt resistance (in mOhm)
 *
 * @param[in] Rshunt_mOhm
 *   Value used for calculation.
 *
 * @return
 *   Returns calculated register value
 ******************************************************************************/
uint16_t INA226_RshuntToCalReg(uint16_t Rshunt_mOhm)
{
    uint32_t calreg_temp = 512 / (1 * Rshunt_mOhm);         //calculated calibration register value: 0.00512/(INA226_CURRENT_LSB(in A) * INA226_SHUNT_RESISTANCE(in Ohm))
    return (uint16_t)calreg_temp;
}

/***************************************************************************
 * @brief
 *   Initialize the ina226 sensor (set configuration register, calibration, mask register and alert register)
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] data
 *   Pointer to the data structure.
 *
 * @param[in] mask
 *   Value to be used for initializing mask register.
 *
 * @param[in] alert
 *   Value to be used for initializing alert register.
 ******************************************************************************/
void INA226_Init(i2cBASE_t *i2c, uint16_t Rshunt, sensor_config_t *data2, uint16_t mask, ina226_housekeeping_t *data3)
{
    uint8_t config_temp[2]={0};
    uint8_t cal_temp[2]={0};
    uint8_t mask_temp[2]={0};
    uint8_t alert_temp[2]={0};
    uint16_t alert_raw = 0;
    uint16_t cal_calculated = 0;

    /* Calculate the calibration register value according to shunt resistance */
    cal_calculated = INA226_RshuntToCalReg(Rshunt);


    /* Convert alert value in mA to raw shunt voltage value */
    alert_raw = INA226_CurrentToAlert_ShuntVoltage_Raw(data3->alert_reg, cal_calculated);

    /* Separate the 16bit integers into 8bit arrays */
    config_temp[0] = (uint8_t)(data2->ina226_cfg_setting >> 8);
    config_temp[1] = (uint8_t)data2->ina226_cfg_setting;
    cal_temp[0] = (uint8_t)(cal_calculated >> 8);
    cal_temp[1] = (uint8_t)cal_calculated;
    mask_temp[0] = (uint8_t)(mask >> 8);
    mask_temp[1] = (uint8_t)mask;
    alert_temp[0] = (uint8_t)(alert_raw >> 8);
    alert_temp[1] = (uint8_t)alert_raw;


    /* Write values to proper registers */
    INA226_SendData(i2c,data3->address, INA226_CFG_REG, config_temp);
    INA226_SendData(i2c,data3->address, INA226_CAL_REG, cal_temp);
    INA226_SendData(i2c,data3->address, INA226_MASK_REG, mask_temp);
    INA226_SendData(i2c,data3->address, INA226_AL_REG, alert_temp);
}

/***************************************************************************
 * @brief
 *   Change the value of mask register
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] data
 *   Pointer to the data structure.
 *
 * @param[in] mask
 *   Value to be written to mask register.
 *
 ******************************************************************************/
void INA226_SetMaskReg(i2cBASE_t *i2c, ina226_housekeeping_t *data, uint16_t mask)
{
    uint8_t mask_temp[2]={0};

    mask_temp[0] = (uint8_t)(mask >> 8);
    mask_temp[1] = (uint8_t)mask;

    INA226_SendData(i2c,data->address, INA226_MASK_REG, mask_temp);
}

/***************************************************************************
 * @brief
 *   Change the value of alert register
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] data
 *   Pointer to the data structure.
 *
 * @param[in] alert
 *   Value to be written to alert register.
 *
 ******************************************************************************/
void INA226_SetAlertReg(i2cBASE_t *i2c, ina226_housekeeping_t *data, uint16_t alert, uint16_t cal_reg)
{
    uint8_t alert_temp[2]={0};
    uint16_t alert_raw = 0;

    alert_raw = INA226_CurrentToAlert_ShuntVoltage_Raw(alert, cal_reg);

    alert_temp[0] = (uint8_t)(alert_raw >> 8);
    alert_temp[1] = (uint8_t)alert_raw;

    INA226_SendData(i2c,data->address, INA226_AL_REG, alert_temp);
}


/***************************************************************************
 * @brief
 *   Read mask register value
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] data
 *   Pointer to the data structure.
 *
 * @return
 *   Return the register value
 ******************************************************************************/
uint16_t INA226_ReadMaskReg(i2cBASE_t *i2c, ina226_housekeeping_t *data)
{
    uint8_t data_temp[2]={0};

    INA226_ReceiveData(i2c, data->address, INA226_MASK_REG, data_temp);

    return ((data_temp[0]<<8) | data_temp[1]);
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
 ******************************************************************************/
void INA226_ReadShuntVoltage_Raw(i2cBASE_t *i2c, ina226_housekeeping_t *data)
{
    uint8_t data_temp[2]={0};
    uint16_t data_reg = 0;

    /* Get raw data in 8bit array */
    INA226_ReceiveData(i2c, data->address, INA226_SV_REG, data_temp);

    /* Combine 8bit array into 16bit integer */
    data_reg = data_temp[0];
    data_reg = data_reg << 8;
    data_reg = data_reg | data_temp[1];

    /* 2's complement conversion */
//    if(data_reg&0x8000)
//        data_reg = ~(data_reg - 1);

    data->shunt_voltage = data_reg;
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
 ******************************************************************************/
void INA226_ReadBusVoltage_Raw(i2cBASE_t *i2c, ina226_housekeeping_t *data)
{
    uint8_t data_temp[2]={0};
    uint16_t data_reg = 0;

    /* Get raw data in 8bit array */
    INA226_ReceiveData(i2c, data->address, INA226_BV_REG, data_temp);

    /* Combine 8bit array into 16bit integer */
    data_reg = data_temp[0];
    data_reg = data_reg << 8;
    data_reg = data_reg | data_temp[1];

    /* Save the raw data to data structure */
    data->bus_voltage = data_reg;
}

/***************************************************************************
 * @brief
 *   Get current (raw data) from current register
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] data
 *   Pointer to the data structure.
 *
 ******************************************************************************/
void INA226_ReadCurrent_Raw(i2cBASE_t *i2c, ina226_housekeeping_t *data)
{
    uint8_t data_temp[2]={0};
    uint16_t data_reg = 0;

    /* Get raw data in 8bit array */
    INA226_ReceiveData(i2c, data->address, INA226_CUR_REG, data_temp);

    /* Combine 8bit array into 16bit integer */
    data_reg = data_temp[0];
    data_reg = data_reg << 8;
    data_reg = data_reg | data_temp[1];

    /* 2's complement conversion */
    if(data_reg&0x8000)
        data_reg = ~(data_reg - 1);


    /* Save the raw data to data structure */
    data->current = data_reg;
}

/***************************************************************************
 * @brief
 *   Get power (raw data) from power register
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 * @param[in] data
 *   Pointer to the data structure.
 *
 ******************************************************************************/
void INA226_ReadPower_Raw(i2cBASE_t *i2c, ina226_housekeeping_t *data)
{
    uint8_t data_temp[2]={0};
    uint16_t data_reg = 0;

    /* Get raw data in 8bit array */
    INA226_ReceiveData(i2c, data->address, INA226_PWR_REG, data_temp);

    /* Combine 8bit array into 16bit integer */
    data_reg = data_temp[0];
    data_reg = data_reg << 8;
    data_reg = data_reg | data_temp[1];

    /* Save the raw data to data structure */
    data->power = data_reg;
}

/***************************************************************************
 * @brief
 *   Convert raw current to current in mA
 *
 * @param[in] data
 *   Pointer to the data structure.
 *
 * @return
 *   Returns converted current value (in mA)
 ******************************************************************************/
uint16_t INA226_ConvToCurrent_mA(ina226_housekeeping_t *data)
{
    return data->current * INA226_CURRENT_LSB;
}

/***************************************************************************
 * @brief
 *   Convert raw bus voltage to voltage in mV
 *
 * @param[in] data
 *   Pointer to the data structure.
 *
 * @return
 *   Returns converted voltage value (in mV)
 ******************************************************************************/
uint16_t INA226_ConvToVoltage_mV(ina226_housekeeping_t *data)
{
    uint32_t voltage_temp = data->bus_voltage * INA226_BUSV_LSB / 1000;
    return (uint16_t)voltage_temp;
}

/***************************************************************************
 * @brief
 *   Convert raw power to power in mW
 *
 * @param[in] data
 *   Pointer to the data structure.
 *
 * @return
 *   Returns converted power value (in mW)
 ******************************************************************************/
uint16_t INA226_ConvToPower_mW(ina226_housekeeping_t *data)
{
    return data->power * INA226_POWER_LSB;
}

