
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
void MAX6698_Init(i2cBASE_t *i2c, sensor_config_t data, max6698_housekeeping_t *data2)
{
    uint8_t config_temp1[1]={0};
    uint8_t config_temp2[1]={0};
    uint8_t config_temp3[1]={0};

    /* Separate the 16bit integers into 8bit arrays */
    config_temp1[0] = (uint8_t)data.max6698_cfg1_setting;
    config_temp2[0] = (uint8_t)data.max6698_cfg2_setting;
    config_temp3[0] = (uint8_t)data.max6698_cfg3_setting;


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
            MAX6698_ReceiveData(i2c, data->address, TM3_REG, data_temp);
            break;
        default:
            break;
    }

    /* Save the raw data to data structure */
    data->temp[channel-1] = data_temp[0];
}


/***************************************************************************
 * @brief
 *   Convert raw temperature data (reference voltage) to real temperature in C (negative value in 2's complement format).
 *
 * @param[in] data
 *   Pointer to the data structure.
 *
 * @param[in] channel
 *   The number of the channel to be converted
 *
 * @return
 *   Return converted temperature value (in C)
 ******************************************************************************/
int MAX6698_ConvertTemp_C(max6698_housekeeping_t *data, uint8_t channel)
{
    uint32_t data_temp = 0;

    data_temp = MAX6698_REXT * (1000 - (1000*data->temp[channel-1])/200) / ((1000*data->temp[channel-1])/200);      //200 is the maximum value of raw temperature data.

    if(data_temp >= 97140)
    {
        return -20;
    }
    else if(data_temp < 97140 && data_temp >= 91670)
    {
        return -19;
    }
    else if(data_temp < 91670 && data_temp >= 86550)
    {
        return -18;
    }
    else if(data_temp < 86550 && data_temp >= 81740)
    {
        return -17;
    }
    else if(data_temp < 81740 && data_temp >= 77220)
    {
        return -16;
    }
    else if(data_temp < 77220 && data_temp >= 72990)
    {
        return -15;
    }
    else if(data_temp < 72990 && data_temp >= 69010)
    {
        return -14;
    }
    else if(data_temp < 69010 && data_temp >= 65270)
    {
        return -13;
    }
    else if(data_temp < 65270 && data_temp >= 61760)
    {
        return -12;
    }
    else if(data_temp < 61760 && data_temp >= 58450)
    {
        return -11;
    }
    else if(data_temp < 58450 && data_temp >= 55350)
    {
        return -10;
    }
    else if(data_temp < 55350 && data_temp >= 52420)
    {
        return -9;
    }
    else if(data_temp < 52420 && data_temp >= 49670)
    {
        return -8;
    }
    else if(data_temp < 49670 && data_temp >= 47080)
    {
        return -7;
    }
    else if(data_temp < 47080 && data_temp >= 44640)
    {
        return -6;
    }
    else if(data_temp < 44640 && data_temp >= 42340)
    {
        return -5;
    }
    else if(data_temp < 42340 && data_temp >= 40170)
    {
        return -4;
    }
    else if(data_temp < 40170 && data_temp >= 38130)
    {
        return -3;
    }
    else if(data_temp < 38130 && data_temp >= 36200)
    {
        return -2;
    }
    else if(data_temp < 36200 && data_temp >= 34380)
    {
        return -1;
    }
    else if(data_temp < 34380 && data_temp >= 32660)
    {
        return 0;
    }
    else if(data_temp < 32660 && data_temp >= 31040)
    {
        return 1;
    }
    else if(data_temp < 31040 && data_temp >= 29510)
    {
        return 2;
    }
    else if(data_temp < 29510 && data_temp >= 28060)
    {
        return 3;
    }
    else if(data_temp < 28060 && data_temp >= 26690)
    {
        return 4;
    }
    else if(data_temp < 26690 && data_temp >= 25400)
    {
        return 5;
    }
    else if(data_temp < 25400 && data_temp >= 24180)
    {
        return 6;
    }
    else if(data_temp < 24180 && data_temp >= 23020)
    {
        return 7;
    }
    else if(data_temp < 23020 && data_temp >= 21920)
    {
        return 8;
    }
    else if(data_temp < 21920 && data_temp >= 20890)
    {
        return 9;
    }
    else if(data_temp < 20890 && data_temp >= 19900)
    {
        return 10;
    }
    else if(data_temp < 19900 && data_temp >= 18970)
    {
        return 11;
    }
    else if(data_temp < 18970 && data_temp >= 18090)
    {
        return 12;
    }
    else if(data_temp < 18090 && data_temp >= 17260)
    {
        return 13;
    }
    else if(data_temp < 17260 && data_temp >= 16470)
    {
        return 14;
    }
    else if(data_temp < 16470 && data_temp >= 15710)
    {
        return 15;
    }
    else if(data_temp < 15710 && data_temp >= 15000)
    {
        return 16;
    }
    else if(data_temp < 15000 && data_temp >= 14320)
    {
        return 17;
    }
    else if(data_temp < 14320 && data_temp >= 13680)
    {
        return 18;
    }
    else if(data_temp < 13680 && data_temp >= 13070)
    {
        return 19;
    }
    else if(data_temp < 13070 && data_temp >= 12490)
    {
        return 20;
    }
    else if(data_temp < 12490 && data_temp >= 11940)
    {
        return 21;
    }
    else if(data_temp < 11940 && data_temp >= 11420)
    {
        return 22;
    }
    else if(data_temp < 11420 && data_temp >= 10920)
    {
        return 23;
    }
    else if(data_temp < 10920 && data_temp >= 10450)
    {
        return 24;
    }
    else if(data_temp < 10450 && data_temp >= 10000)
    {
        return 25;
    }
    else if(data_temp < 10000 && data_temp >= 9572)
    {
        return 26;
    }
    else if(data_temp < 9572 && data_temp >= 9165)
    {
        return 27;
    }
    else if(data_temp < 9165 && data_temp >= 8777)
    {
        return 28;
    }
    else if(data_temp < 8777 && data_temp >= 8408)
    {
        return 29;
    }
    else if(data_temp < 8408 && data_temp >= 8055)
    {
        return 30;
    }
    else if(data_temp < 8055 && data_temp >= 7721)
    {
        return 31;
    }
    else if(data_temp < 7721 && data_temp >= 7402)
    {
        return 32;
    }
    else if(data_temp < 7402 && data_temp >= 7098)
    {
        return 33;
    }
    else if(data_temp < 7098 && data_temp >= 6808)
    {
        return 34;
    }
    else if(data_temp < 6808 && data_temp >= 6531)
    {
        return 35;
    }
    else if(data_temp < 6531 && data_temp >= 6267)
    {
        return 36;
    }
    else if(data_temp < 6267 && data_temp >= 6015)
    {
        return 37;
    }
    else if(data_temp < 6015 && data_temp >= 5774)
    {
        return 38;
    }
    else if(data_temp < 5774 && data_temp >= 5545)
    {
        return 39;
    }
    else if(data_temp < 5545 && data_temp >= 5325)
    {
        return 40;
    }
    else if(data_temp < 5325 && data_temp >= 5116)
    {
        return 41;
    }
    else if(data_temp < 5116 && data_temp >= 4916)
    {
        return 42;
    }
    else if(data_temp < 4916 && data_temp >= 4725)
    {
        return 43;
    }
    else if(data_temp < 4725 && data_temp >= 4543)
    {
        return 44;
    }
    else if(data_temp < 4543 && data_temp >= 4368)
    {
        return 45;
    }
    else if(data_temp < 4368 && data_temp >= 4201)
    {
        return 46;
    }
    else if(data_temp < 4201 && data_temp >= 4041)
    {
        return 47;
    }
    else if(data_temp < 4041 && data_temp >= 3888)
    {
        return 48;
    }
    else if(data_temp < 3888 && data_temp >= 3742)
    {
        return 49;
    }
    else if(data_temp < 3742 && data_temp >= 3602)
    {
        return 50;
    }
    else if(data_temp < 3602 && data_temp >= 3468)
    {
        return 51;
    }
    else if(data_temp < 3468 && data_temp >= 3340)
    {
        return 52;
    }
    else if(data_temp < 3340 && data_temp >= 3217)
    {
        return 53;
    }
    else if(data_temp < 3217 && data_temp >= 3099)
    {
        return 54;
    }
    else if(data_temp < 3099 && data_temp >= 2987)
    {
        return 55;
    }
    else if(data_temp < 2987 && data_temp >= 2878)
    {
        return 56;
    }
    else if(data_temp < 2878 && data_temp >= 2775)
    {
        return 57;
    }
    else if(data_temp < 2775 && data_temp >= 2675)
    {
        return 58;
    }
    else if(data_temp < 2675 && data_temp >= 2580)
    {
        return 59;
    }
    else if(data_temp < 2580 && data_temp >= 2488)
    {
        return 60;
    }
    else
    {
        return 61;
    }

}
