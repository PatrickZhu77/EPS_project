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


/*    int error = 0;


    if((i2c->STR & (uint32)I2C_BUSBUSY ) == 0)    //Check if the busy bit is currently busy
    {
        i2cSetDirection(i2c, I2C_TRANSMITTER);
        i2cSetSlaveAdd(i2c, addr);

        i2cSetStart(i2c);

        i2cSendByte(i2c, reg);
        i2cSetStop(i2c);

    }
    else
    {

        error = -1;
    }*/

    //return error;
    while(i2cIsMasterReady(i2c) != true);
    i2cSetSlaveAdd(i2c, addr);

    /* Set direction to Transmitter */
    /* Note: Optional - It is done in Init */
    i2cSetDirection(i2c, I2C_TRANSMITTER);

    /* Configure Data count */
    /* Slave address + Word address write operation before reading */
    i2cSetCount(i2c, 1);

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
 *   I2C address, in 8 bit format, where LSB is reserved for R/W bit.
 *
 * @param[in] reg
 *   Register to be written.
 *
 * @param[in] data
 *   Data to be sent.
 *
 * @return
 *   Returns 0 if register written, <0 if unable to write to register.
 ******************************************************************************/
void INA226_SendData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t data)
{

    //int error = 0;
//    uint8_t temp[2] = {0};
//    temp[0] = (uint8_t)(data>>8);
//    temp[1] = (uint8_t)(data&0x00FF);

    int delay =0;

/*    if((i2c->STR & (uint32)I2C_BUSBUSY ) == 0)    //Check if the busy bit is currently busy
    {
        i2cSetDirection(i2c, I2C_TRANSMITTER);
        i2cSetSlaveAdd(i2c, addr);

        i2cSetStart(i2c);

        i2cSendByte(i2c, reg);
        temp = (uint8_t)(data>>8);
        i2cSendByte(i2c, temp);
        temp = (uint8_t)(data&0x00FF);
        i2cSendByte(i2c, temp);

        i2cSetStop(i2c);

    }
    else
    {

        error = -1;
    }
*/
    //while(i2cIsMasterReady(i2c) != true);
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

    /* Tranmit DATA_COUNT number of data in Polling mode */
    i2cSend(i2c, 2, data);

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(i2c) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(i2c) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(i2c);

    /* Simple Dealya before starting Next Block */
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
 *   I2C address, in 8 bit format, where LSB is reserved for R/W bit.
 *
 * @param[in] reg
 *   Register to be read.
 *
 * @param[out] data
 *   Reference to place data read.
 *
 * @return
 *   Returns 0 if register written, <0 if unable to write to register.
 ******************************************************************************/
void INA226_ReadData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint16_t *data)
{

    uint8_t temp[2] = {0};
    uint16_t temp2 = 0;
    int delay =0;
    //int error = 0;
//    if((i2c->STR & (uint32)I2C_BUSBUSY ) == 0)
//    {
//
//        i2cSetDirection(i2c, I2C_TRANSMITTER);
//        i2cSetSlaveAdd(i2c, addr);
//
//        i2cSetStart(i2c);
//
//        data = i2cReceiveByte(i2c);
//        data<<=8;
//        data |= i2cReceiveByte(i2c);
//
//        i2cSetStop(i2c);
//
//      }
//      else
//      {
//          error = -1
//      }
//
//      //return error;

    while(i2cIsMasterReady(i2c) != true);

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
    i2cReceive(i2cREG1, 2, temp);
    temp2 = temp[0];
    temp2 <<=8;
    temp2 |= temp[1];

    *data = temp2;

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(i2c) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(i2c) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(i2c);
    for(delay=0;delay<100000;delay++);

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

//1mA/bit
void INA226_GetShunt_Current(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    //int err = -1;
    uint16_t temp = 0;
    //INA226_SetRegPointer(i2c, addr,CUR_REG);

    INA226_ReadData(i2c, addr, CUR_REG, &temp);

    if(temp&0x8000)
        temp = ~(temp - 1);
    *data = temp;

    //return(err);
}

//read id
void INA226_Get_ID(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    //int err = -1;
    uint16_t temp = 0;
    //INA226_SetRegPointer(i2c, addr,ID_REG);

    INA226_ReadData(i2c, addr, ID_REG, &temp);


    *data = (uint16_t)temp;

    //return(err);
}

//read calibration register
void INA226_GET_CAL_REG(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    //int err = -1;
    uint16_t temp = 0;
    //INA226_SetRegPointer(i2c, addr,CAL_REG);

    INA226_ReadData(i2c, addr, CAL_REG, &temp);


    *data = (uint16_t)temp;

    //return(err);
}

//1.25mV/bit
void INA226_GetVoltage(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    //int err = -1;
    uint16_t temp = 0;
    //INA226_SetRegPointer(i2c, addr,BV_REG);

    INA226_ReadData(i2c, addr, BV_REG, &temp);

    *data = (uint16_t)temp;

    //return(err);
}


//2.5uV/bit
void INA226_GetShuntVoltage(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    //int err = -1;
    uint16_t temp = 0;
    //INA226_SetRegPointer(i2c, addr,SV_REG);

    INA226_ReadData(i2c, addr, SV_REG, &temp);

    if(temp&0x8000)
        temp = ~(temp - 1);
    *data = temp;

    //return(err);
}


void INA226_Get_Power(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    //int err = -1;
    uint16_t temp = 0;
    //INA226_SetRegPointer(i2c, addr,PWR_REG);

    INA226_ReadData(i2c, addr, PWR_REG, &temp);

    *data = (uint16_t)temp;

    //return(err);
}


/*void GetVoltage(float *Voltage)//mV
{
    Voltage[0] = INA226_GetVoltage(INA226_ADDR1)*1.25f;
}


void Get_Shunt_voltage(float *Voltage)//uV
{
    Voltage[0] = (INA226_GetShuntVoltage(INA226_ADDR1)*2.2);//这里原来乘的系数是2.5
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
