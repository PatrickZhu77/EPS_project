#include "ina226_2.h"


INA226 INA226_data;

static void delay_nns(uint16_t D)  //30����ns  �����ֲ�Ҫ�õ�IIC��HS����ģʽ
{
    while(--D);
}

void delay_nms(uint16_t ms)  //����
{
    uint16_t i;
    uint32_t M = 0;//720W
    for(i = 0;i < ms; i++)
    for(M=12000;M > 0;M--);
}

void delay_nus(uint16_t us)//΢��
{
    uint16_t i;
    uint16_t M = 0;//720W
    for(i = 0;i < us; i++)
    for(M=72;M > 0;M--);
}

/****************IIC***************************/
void INA226_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();   //ʹ��GPIOBʱ��

     //PC11,12��ʼ������
    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11;//PB10SCL PB11SDA
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    IIC_SDA_H;
    IIC_SCL_H;
        delay_nms(5);
}

void INA226_IIC_Start(void)
{
    IIC_SDA_H;
    IIC_SCL_H;
    delay_nns(5);
     IIC_SDA_L;//START:when CLK is high,DATA change form high to low
    delay_nns(5);
    IIC_SCL_L;//ǯסI2C���ߣ�׼�����ͻ��������
    delay_nns(5);
}

void INA226_IIC_Stop(void)
{
    IIC_SDA_L;//STOP:when CLK is high DATA change form low to high
     delay_nns(5);
    IIC_SCL_H;
        delay_nns(5);
    IIC_SDA_H;//����I2C���߽����ź�
    delay_nns(5);
}

void INA226_IIC_Ack(void)
{
    IIC_SDA_L;
    delay_nns(5);
    IIC_SCL_H;
    delay_nns(5);
    IIC_SCL_L;
        delay_nns(5);
    IIC_SDA_H;
}

void INA226_IIC_NAck(void)
{
    IIC_SDA_H;
    delay_nns(5);
    IIC_SCL_H;
delay_nns(5);
    IIC_SCL_L;
    delay_nns(5);
    IIC_SDA_L;
}

uint8_t INA226_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    //SDA_IN();      //SDA����Ϊ����
    IIC_SDA_H;
    delay_nns(5);
    IIC_SCL_H;
    delay_nns(5);

    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            INA226_IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_L;//ʱ�����0
    return 0;
}

void INA226_IIC_Send_Byte(uint8_t txd)
{
   IIC_SCL_L;//����ʱ�ӿ�ʼ���ݴ���
   for(uint8_t i = 0;i < 8;i++)
    {
        if(txd&0x80)
                    IIC_SDA_H;
                else
                    IIC_SDA_L;
        txd<<=1;

        IIC_SCL_H;
        delay_nns(5);
        IIC_SCL_L;
        delay_nns(5);
    }
    //    IIC_SDA_H;
    //    IIC_SCL_H;            //��ȥ��wait_ackʱ��ʱ�����
    //    delay_nns(5);
        //IIC_SCL_L;
}

uint8_t INA226_IIC_Read_Byte(unsigned char ack)
{
      uint8_t TData=0,i;
    for(i=0;i<8;i++)
    {
        IIC_SCL_H;
        delay_nns(5);
        TData=TData<<1;
        //if(GPIOB->IDR& GPIO_IDR_IDR7)        //�ж�SDA�Ƿ�Ϊ��
            if(SDA_READ)
        {
            TData|=0x01;
        }
        IIC_SCL_L;
        delay_nns(5);
    }
        if(!ack)
            INA226_IIC_NAck();
        else
            INA226_IIC_Ack();
//    IIC_SCL_L;
//    delay_nns(5);
//    IIC_SDA_L;
//    delay_nns(5);                //ACK
//    IIC_SCL_H;
//    delay_nns(5);
//    IIC_SCL_L;
//    delay_nns(5);
//    IIC_SDA_H;
    return TData;
}

/**********************IIC_END*************************/

void INA226_Init(void)
{
    INA226_IIC_Init();
    HAL_Delay(10);
    //    INA226_SendData(INA226_ADDR1,CFG_REG,0xC000);

//    HAL_Delay(10);
    //uint16_t cfg_reg_value = 0x4000|(3<<9)|(4<<6)|(4<<3)|7;
//    INA226_SendData(INA226_ADDR1,CFG_REG,0x484f);
//    INA226_SendData(INA226_ADDR1,CAL_REG,0x0012);
    INA226_SendData(INA226_ADDR1,0x00,0x4727);
    INA226_SendData(INA226_ADDR1,0x05,0x800);//A00
}

void INA226_SendData(uint8_t addr,uint8_t reg,uint16_t data)
{
    uint8_t temp = 0;
    INA226_IIC_Start();
    INA226_IIC_Send_Byte(addr);
    INA226_IIC_Wait_Ack();

    INA226_IIC_Send_Byte(reg);
    INA226_IIC_Wait_Ack();

    temp = (uint8_t)(data>>8);
    INA226_IIC_Send_Byte(temp);
    INA226_IIC_Wait_Ack();

    temp = (uint8_t)(data&0x00FF);
    INA226_IIC_Send_Byte(temp);
    INA226_IIC_Wait_Ack();

    INA226_IIC_Stop();
}


void INA226_SetRegPointer(uint8_t addr,uint8_t reg)
{
    INA226_IIC_Start();

    INA226_IIC_Send_Byte(addr);
    INA226_IIC_Wait_Ack();

    INA226_IIC_Send_Byte(reg);
    INA226_IIC_Wait_Ack();

    INA226_IIC_Stop();
}


uint16_t INA226_ReadData(uint8_t addr)
{
    uint16_t temp=0;
    INA226_IIC_Start();

    INA226_IIC_Send_Byte(addr+1);
    INA226_IIC_Wait_Ack();

    temp = INA226_IIC_Read_Byte(1);
    temp<<=8;
    temp |= INA226_IIC_Read_Byte(0);

    INA226_IIC_Stop();
    return temp;
}


long INA226_GetShunt_Current(uint8_t addr)
{
    long temp=0;
    INA226_SetRegPointer(addr,CUR_REG);
    temp = INA226_ReadData(addr);
    if(temp&0x8000)
    {
        temp = ~(temp - 1);
            temp = (uint16_t)temp - 2 * (uint16_t)temp;
    }
        return temp;
}

//��ȡ id
uint16_t  INA226_Get_ID(uint8_t addr)
{
    uint32_t temp=0;
    INA226_SetRegPointer(addr,INA226_GET_ADDR);
    temp = INA226_ReadData(addr);
    return (uint16_t)temp;
}

//��ȡУ׼ֵ
uint16_t INA226_GET_CAL_REG(uint8_t addr)
{
    uint32_t temp=0;
    INA226_SetRegPointer(addr,CAL_REG);
    temp = INA226_ReadData(addr);
    return (uint16_t)temp;
}

//1.25mV/bit
uint32_t INA226_GetVoltage(uint8_t addr)
{
    uint32_t temp=0;
    INA226_SetRegPointer(addr,BV_REG);
    temp = INA226_ReadData(addr);
    return (uint32_t)temp;
}



//2.5uV/bit,�о����ֵ�ǲⲻ׼�ģ���������ĳ�2.2��
uint32_t INA226_GetShuntVoltage(uint8_t addr)
{
    uint32_t temp=0;
    INA226_SetRegPointer(addr,SV_REG);
    temp = INA226_ReadData(addr);
    if(temp&0x8000)    temp = ~(temp - 1);
    return (uint32_t)temp;
}


uint16_t INA226_Get_Power(uint8_t addr)
{
    int16_t temp=0;
    INA226_SetRegPointer(addr,PWR_REG);
    temp = INA226_ReadData(addr);
    return (uint16_t)temp;
}


void GetVoltage(float *Voltage)//mV
{
    Voltage[0] = INA226_GetVoltage(INA226_ADDR1)*1.25f;
    printf("ina226 v = %.2f\r\n", Voltage[0]);
}


void Get_Shunt_voltage(float *Voltage)//uV
{
    Voltage[0] = (INA226_GetShuntVoltage(INA226_ADDR1)*2.2);//����ԭ���˵�ϵ����2.5

}


void Get_Shunt_Current(float *Current)//mA
{
    Current[0] = (INA226_GetShunt_Current(INA226_ADDR1)* 2.5f);
}


void GetPower()//W
{
    GetVoltage(&INA226_data.voltageVal);            //mV
    Get_Shunt_voltage(&INA226_data.Shunt_voltage);    //uV
    Get_Shunt_Current(&INA226_data.Shunt_Current);    //mA
    INA226_data.powerVal=INA226_data.voltageVal*0.001f * INA226_data.Shunt_Current*0.001f;
}
uint16_t read_dat;
float read_vol;
float GetV(void)
{
    //��ѹ����
    INA226_IIC_Start();
    INA226_IIC_Send_Byte(0x80);
    INA226_IIC_Wait_Ack();
  INA226_IIC_Send_Byte(0x00);
  INA226_IIC_Wait_Ack();
     INA226_IIC_Send_Byte(0x47);
  INA226_IIC_Wait_Ack();
     INA226_IIC_Send_Byte(0x27);
  INA226_IIC_Wait_Ack();
  INA226_IIC_Stop();
    HAL_Delay(1);

  INA226_IIC_Start();
    INA226_IIC_Send_Byte(0x81);
    INA226_IIC_Wait_Ack();
    read_dat = INA226_IIC_Read_Byte(1)<<8;
    //IIC_Wait_Ack();
    read_dat = read_dat + INA226_IIC_Read_Byte(0);
    read_vol = 0.00125*read_dat;
    INA226_IIC_Stop();
    return read_vol;
}

