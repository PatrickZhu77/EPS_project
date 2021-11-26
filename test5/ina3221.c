#include "ina3221.h"
#include "i2c.h"


ina3221 ina3221_data;

void ina3221_Init(void)
{
    i2cInit();
    delay_ms(10);
    ina3221_SendData(ina3221_ADDR1, CFG_REG,0x7127);
    ina3221_SendData(ina3221_ADDR1, CAL_REG,0x0000);
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
int ina3221_SetRegPointer(i2cBASE_t *i2c, uint8_t addr, uint8_t reg)
{


    int error = 0;


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
    }

    return error;

}



/***************************************************************************//**
 * @brief
 *   Write data to given address.
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
int ina3221_SendData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint16_t data)
{

    int error = 0;
    uint8_t temp = 0;

    if((i2c->STR & (uint32)I2C_BUSBUSY ) == 0)    //Check if the busy bit is currently busy
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

    return error;

}

/***************************************************************************//**
 * @brief
 *   Read data from given address.
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
int ina3221_ReadData(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{

    int error = 0;
    if((i2c->STR & (uint32)I2C_BUSBUSY ) == 0)
    {

        i2cSetDirection(i2c, I2C_TRANSMITTER);
        i2cSetSlaveAdd(i2c, addr);

        i2cSetStart(i2c);

        data = i2cReceiveByte(i2c);
        data<<=8;
        data |= i2cReceiveByte(i2c);

        i2cSetStop(i2c);

      }
      else
      {
          error = -1
      }

      return error;
}
/*
uint8_t ina3221_AlertAddr(i2cBASE_t *i2c)
{
    uint8_t temp;

    i2cSetStart(i2c);

    i2cSendByte(i2c,ina3221_GETALADDR);

    temp = i2cReceiveByte(i2c);

    i2cSetStop(i2c);

    return temp;
}
*/


//read id
int ina3221_Get_ID(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    int err = -1;
    uint32_t temp = 0;
    ina3221_SetRegPointer(i2c, addr,ID_REG);

    err = ina3221_ReadData(i2c, addr, &temp);
    if (err < 0)
    {
      return(err);
    }

    *data = (uint16_t)temp;

    return(err);
}


int ina3221_GetVoltage(i2cBASE_t *i2c, uint8_t addr, uint16_t *data, uint8_t channel)
{
    int err = -1;
    uint32_t temp = 0;
    switch(channel)
    {
        case 1:INA3221_SetRegPointer(addr,C1_BV_REG);break;
        case 2:INA3221_SetRegPointer(addr,C2_BV_REG);break;
        case 3:INA3221_SetRegPointer(addr,C3_BV_REG);break;
        default:break;
    }

    err = ina3221_ReadData(i2c, addr, &temp);
    if (err < 0)
    {
      return(err);
    }

    *data = (uint16_t)temp;

    return(err);
}


//40uV/bit
int ina3221_GetShuntVoltage(i2cBASE_t *i2c, uint8_t addr, uint16_t *data, uint8_t channel)
{
    int err = -1;
    uint16_t temp = 0;

    switch(channel)
    {
        case 1:INA3221_SetRegPointer(addr,C1_SV_REG);break;
        case 2:INA3221_SetRegPointer(addr,C2_SV_REG);break;
        case 3:INA3221_SetRegPointer(addr,C3_SV_REG);break;
        default:break;
    }

    err = ina3221_ReadData(i2c, addr, &temp);
    if (err < 0)
    {
      return(err);
    }

    if(temp&0x8000)
        temp = ~(temp - 1);
    *data = temp;

    return(err);
}

