#include "ina226.h"
#include "i2c.h"


//INA226 INA226_data;

void INA226_Init(i2cBASE_t *i2c, uint8_t addr)
{
    //i2cInit();

    INA226_SendData(i2c,addr, CFG_REG,0x4127);
    INA226_SendData(i2c,addr, CAL_REG,0x0000);
}

/***************************************************************************//**
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
void INA226_SetRegPointer(i2cBASE_t *i2c, uint8_t addr, uint8_t reg)
{

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
void INA226_SendData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data)
{
    int delay=0;

    while(i2cIsMasterReady(i2c) != true);
    /* Configure address of Slave to talk to */
    i2cSetSlaveAdd(i2c, addr);

    /* Set direction to Transmitter */
    /* Note: Optional - It is done in Init */
    i2cSetDirection(i2c, I2C_TRANSMITTER);

    /* Configure Data count */
    /* Data Count + 1 ( Word Address) */
    i2cSetCount(i2c, 2 + 1);

    /* Set mode as Master */
    i2cSetMode(i2c, I2C_MASTER);

    /* Set Stop after programmed Count */
    i2cSetStop(i2c);

    /* Transmit Start Condition */
    i2cSetStart(i2c);

    /* Send the Word Address */
    i2cSendByte(i2c, reg);

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
    //for(delay=0;delay<100000;delay++);

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
void INA226_ReceiveData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data)
{
    int delay=0;

    while(i2cIsMasterReady(i2c) == true);

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
    //for(delay=0;delay<100000;delay++);

}

/*
uint8_t INA226_AlertAddr(i2cBASE_t *i2c)
{
    uint8_t temp;

    i2cSetStart(i2c);

    i2cSendByte(i2c,INA226_GETALADDR);

    temp = i2cReceiveByte(i2c);

    i2cSetStop(i2c);

    return temp;
}
*/

//LSB = 2.5uV/bit
void INA226_GetShuntVoltage(i2cBASE_t *i2c, uint8_t addr, double *data)
{
    //int err = -1;
    uint16_t temp = 0;
    //INA226_SetRegPointer(i2c, addr,SV_REG);

    INA226_ReceiveData(i2c, addr, SV_REG, &temp);

    if(temp&0x8000)
        temp = ~(temp - 1);
    *data = temp * 2.5e-6 * 1000;

    //return(err);
}

//LSB = 1.25mV/bit
void INA226_GetVoltage(i2cBASE_t *i2c, uint8_t addr, double *data)
{
    //int err = -1;
    uint16_t temp = 0;
    //INA226_SetRegPointer(i2c, addr,BV_REG);

    INA226_ReceiveData(i2c, addr, BV_REG, &temp);

    *data = temp* 1.25e-3;

    //return(err);
}

// Set value of CAL_REG
// CAL = 0.00512/(current_LSB * Rshunt)
void INA226_SetCalReg(i2cBASE_t *i2c, uint8_t addr,uint16_t *data)
{
    INA226_SendData(i2c,addr, CAL_REG, data);
}


//LSB = 1mA/bit
void INA226_GetCurrent(i2cBASE_t *i2c, uint8_t addr, double *data)
{
    //int err = -1;
    uint16_t temp = 0;
    //INA226_SetRegPointer(i2c, addr,CUR_REG);

    INA226_ReceiveData(i2c, addr, CUR_REG, &temp);

    if(temp&0x8000)
        temp = ~(temp - 1);
    *data = temp;
    *data = temp * 1e-3;
    //return(err);
}


//LSB = 25mW/bit
void INA226_GetPower(i2cBASE_t *i2c, uint8_t addr, double *data)
{
    //int err = -1;
    uint16_t temp = 0;
    //INA226_SetRegPointer(i2c, addr,PWR_REG);

    INA226_ReceiveData(i2c, addr, PWR_REG, &temp);

    *data = temp * 25e-3;

    //return(err);
}

//read id
void INA226_GetID(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    //int err = -1;
    uint16_t temp = 0;
    //INA226_SetRegPointer(i2c, addr,ID_REG);

    INA226_ReceiveData(i2c, addr, ID_REG, &temp);


    *data = (uint16_t)temp;

    //return(err);
}

//read calibration register
void INA226_GetCalReg(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    //int err = -1;
    uint16_t temp = 0;
    //INA226_SetRegPointer(i2c, addr,CAL_REG);

    INA226_ReceiveData(i2c, addr, CAL_REG, &temp);


    *data = (uint16_t)temp;

    //return(err);
}


/*void GetVoltage(float *Voltage)//mV
{
    Voltage[0] = INA226_GetVoltage(INA226_ADDR1)*1.25f;
}


void Get_Shunt_voltage(float *Voltage)//uV
{
    Voltage[0] = (INA226_GetShuntVoltage(INA226_ADDR1)*2.2);
}


void Get_Shunt_Current(float *Current)//mA
{
    Current[0] = (INA226_GetShunt_Current(INA226_ADDR1)* 2.5f);
}


void GetPower()//W
{
    GetVoltage(&INA226_data.voltageVal);            //mV
    Get_Shunt_voltage(&INA226_data.Shunt_voltage);  //uV
    Get_Shunt_Current(&INA226_data.Shunt_Current);  //mA
    INA226_data.powerVal=INA226_data.voltageVal*0.001f * INA226_data.Shunt_Current*0.001f;
}
*/
