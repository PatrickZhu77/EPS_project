#include "ina226.h"
#include "i2c.h"


INA226 INA226_data;

void INA226_Init(void)
{
    i2cInit();
    delay_ms(10);
    INA226_SendData(INA226_ADDR1, CFG_REG,0x4127);
    INA226_SendData(INA226_ADDR1, CAL_REG,0x0000);
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
int INA226_SetRegPointer(i2cBASE_t *i2c, uint8_t addr, uint8_t reg)
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
int INA226_SendData(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint16_t data)
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
int INA226_ReadData(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
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
int INA226_GetShunt_Current(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    int err = -1;
    uint16_t temp = 0;
    INA226_SetRegPointer(i2c, addr,CUR_REG);

    err = INA226_ReadData(i2c, addr, &temp);
    if (err < 0)
    {
      return(err);
    }

    if(temp&0x8000)
        temp = ~(temp - 1);
    *data = temp;

    return(err);
}

//read id
int INA226_Get_ID(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    int err = -1;
    uint32_t temp = 0;
    INA226_SetRegPointer(i2c, addr,ID_REG);

    err = INA226_ReadData(i2c, addr, &temp);
    if (err < 0)
    {
      return(err);
    }

    *data = (uint16_t)temp;

    return(err);
}

//read calibration register
int INA226_GET_CAL_REG(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    int err = -1;
    uint32_t temp = 0;
    INA226_SetRegPointer(i2c, addr,CAL_REG);

    err = INA226_ReadData(i2c, addr, &temp);
    if (err < 0)
    {
      return(err);
    }

    *data = (uint16_t)temp;

    return(err);
}

//1.25mV/bit
int INA226_GetVoltage(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    int err = -1;
    uint32_t temp = 0;
    INA226_SetRegPointer(i2c, addr,BV_REG);

    err = INA226_ReadData(i2c, addr, &temp);
    if (err < 0)
    {
      return(err);
    }

    *data = (uint16_t)temp;

    return(err);
}


//2.5uV/bit
int INA226_GetShuntVoltage(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    int err = -1;
    uint16_t temp = 0;
    INA226_SetRegPointer(i2c, addr,SV_REG);

    err = INA226_ReadData(i2c, addr, &temp);
    if (err < 0)
    {
      return(err);
    }

    if(temp&0x8000)
        temp = ~(temp - 1);
    *data = temp;

    return(err);
}


int INA226_Get_Power(i2cBASE_t *i2c, uint8_t addr, uint16_t *data)
{
    int err = -1;
    uint32_t temp = 0;
    INA226_SetRegPointer(i2c, addr,PWR_REG);

    err = INA226_ReadData(i2c, addr, &temp);
    if (err < 0)
    {
      return(err);
    }

    *data = (uint16_t)temp;

    return(err);
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
