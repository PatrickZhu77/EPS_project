#include "INA3221.h"

static void delay_nns_3221(uint16_t D)  //30����ns  �����ֲ�Ҫ�õ�IIC��HS����ģʽ
{
    while(--D);
}

void delay_nms_3221(uint16_t ms)  //����
{
    uint16_t i;
    uint32_t M = 0;//720W
    for(i = 0;i < ms; i++)
    for(M=12000;M > 0;M--);
}

void delay_nus_3221(uint16_t us)//΢��
{
    uint16_t i;
    uint16_t M = 0;//720W
    for(i = 0;i < us; i++)
    for(M=72;M > 0;M--);
}
/****************IIC***************************/
void INA3221_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();   //ʹ��GPIOBʱ��

     //PC11,12��ʼ������
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;//PB6SCL PB7SDA
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    IIC_SDA_H_3221;
    IIC_SCL_H_3221;
        delay_nms_3221(5);
}

void INA3221_IIC_Start(void)
{
    IIC_SDA_H_3221;
    IIC_SCL_H_3221;
    delay_nns_3221(5);
     IIC_SDA_L_3221;//START:when CLK is high,DATA change form high to low
    delay_nns_3221(5);
    IIC_SCL_L_3221;//ǯסI2C���ߣ�׼�����ͻ��������
    delay_nns_3221(5);
}

void INA3221_IIC_Stop(void)
{
    IIC_SDA_L_3221;//STOP:when CLK is high DATA change form low to high
     delay_nns_3221(5);
    IIC_SCL_H_3221;
        delay_nns_3221(5);
    IIC_SDA_H_3221;//����I2C���߽����ź�
    delay_nns_3221(5);
}

void INA3221_IIC_Ack(void)
{
    IIC_SDA_L_3221;
    delay_nns_3221(5);
    IIC_SCL_H_3221;
    delay_nns_3221(5);
    IIC_SCL_L_3221;
        delay_nns_3221(5);
    IIC_SDA_H_3221;
}

void INA3221_IIC_NAck(void)
{
    IIC_SDA_H_3221;
    delay_nns_3221(5);
    IIC_SCL_H_3221;
delay_nns_3221(5);
    IIC_SCL_L_3221;
    delay_nns_3221(5);
    IIC_SDA_L_3221;
}

uint8_t INA3221_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    //SDA_IN();      //SDA����Ϊ����
    IIC_SDA_H_3221;
    delay_nns_3221(5);
    IIC_SCL_H_3221;
    delay_nns_3221(5);

    while(READ_SDA_3221)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            INA3221_IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_L_3221;//ʱ�����0
    return 0;
}

void INA3221_IIC_Send_Byte(uint8_t txd)
{
   IIC_SCL_L_3221;//����ʱ�ӿ�ʼ���ݴ���
   for(uint8_t i = 0;i < 8;i++)
    {
        if(txd&0x80)
                    IIC_SDA_H_3221;
                else
                    IIC_SDA_L_3221;
        txd<<=1;

        IIC_SCL_H_3221;
        delay_nns_3221(5);
        IIC_SCL_L_3221;
        delay_nns_3221(5);
    }
    //    IIC_SDA_H_3221;
    //    IIC_SCL_H_3221;            //��ȥ��wait_ackʱ��ʱ�����
    //    delay_nns_3221(5);
        //IIC_SCL_L_3221;
}

uint8_t INA3221_IIC_Read_Byte(unsigned char ack)
{
      uint8_t TData=0,i;
    for(i=0;i<8;i++)
    {
        IIC_SCL_H_3221;
        delay_nns_3221(5);
        TData=TData<<1;
        //if(GPIOB->IDR& GPIO_IDR_IDR7)        //�ж�SDA�Ƿ�Ϊ��
            if(SDA_READ_3221)
        {
            TData|=0x01;
        }
        IIC_SCL_L_3221;
        delay_nns_3221(5);
    }
        if(!ack)
            INA3221_IIC_NAck();
        else
            INA3221_IIC_Ack();
//    IIC_SCL_L_3221;
//    delay_nns_3221(5);
//    IIC_SDA_L_3221;
//    delay_nns_3221(5);                //ACK
//    IIC_SCL_H_3221;
//    delay_nns_3221(5);
//    IIC_SCL_L_3221;
//    delay_nns_3221(5);
//    IIC_SDA_H_3221;
    return TData;
}

/**********************IIC_END*************************/

void INA3221_Init(void)
{
    INA3221_IIC_Init();
    //HAL_Delay(10);

    INA3221_SendData(0x82 ,CFG_REG_3221,0x8000);//�����λ
    INA3221_SendData(0x84 ,CFG_REG_3221,0x8000);
    HAL_Delay(10);
    INA3221_SendData(0x82 ,CFG_REG_3221,0x7127);//7127
    INA3221_SendData(0x84 ,CFG_REG_3221,0x7127);
//    HAL_Delay(2);
//    INA3221_SendData(0x82, 0x01, 0x00);
//    INA3221_SendData(0x82, 0x03, 0x00);
//    INA3221_SendData(0x82, 0x05, 0x00);
//    INA3221_SendData(0x84, 0x01, 0x00);
//    INA3221_SendData(0x84, 0x03, 0x00);
//    INA3221_SendData(0x84, 0x05, 0x00);
//
    }

void INA3221_SendData(uint8_t addr,uint8_t reg,uint16_t data)
{
    uint8_t temp = 0;
    INA3221_IIC_Start();
    INA3221_IIC_Send_Byte(addr);
    INA3221_IIC_Wait_Ack();

    INA3221_IIC_Send_Byte(reg);
    INA3221_IIC_Wait_Ack();

    temp = (uint8_t)(data>>8);
    INA3221_IIC_Send_Byte(temp);
    INA3221_IIC_Wait_Ack();

    temp = (uint8_t)(data&0x00FF);
    INA3221_IIC_Send_Byte(temp);
    INA3221_IIC_Wait_Ack();

    INA3221_IIC_Stop();
}

void INA3221_SetRegPointer(uint8_t addr,uint8_t reg)
{
    INA3221_IIC_Start();

    INA3221_IIC_Send_Byte(addr);
    INA3221_IIC_Wait_Ack();

    INA3221_IIC_Send_Byte(reg);
    INA3221_IIC_Wait_Ack();

    INA3221_IIC_Stop();
}

uint16_t INA3221_ReadData(uint8_t addr)
{
    uint16_t temp=0;
    INA3221_IIC_Start();

    INA3221_IIC_Send_Byte(addr+1);
    INA3221_IIC_Wait_Ack();

    temp = INA3221_IIC_Read_Byte(1);
    temp<<=8;
    temp |= INA3221_IIC_Read_Byte(0);

    INA3221_IIC_Stop();
    return temp;
}

uint16_t INA3221_GetVoltage(uint8_t addr,uint8_t channel)
{
    uint32_t temp=0;
    switch(channel)
    {
        case 1:INA3221_SetRegPointer(addr,0x02);break;
        case 2:INA3221_SetRegPointer(addr,0x04);break;
        case 3:INA3221_SetRegPointer(addr,0x06);break;
        default:break;
    }
    temp = INA3221_ReadData(addr);//printf("\r\n temp = %d",temp);
    if(temp&0x8000)    temp = ~(temp - 1);
    return (uint16_t)temp;
//    return (u16)temp;
}

//40uV/bit
uint16_t INA3221_GetShuntVoltage(uint8_t addr,uint8_t channel)
{
    uint32_t temp=0;
    switch(channel)
    {
        case 1:INA3221_SetRegPointer(addr,0x01);break;
        case 2:INA3221_SetRegPointer(addr,0x03);break;
        case 3:INA3221_SetRegPointer(addr,0x05);break;
        default:break;
    }
    temp = INA3221_ReadData(addr);
    if(temp&0x8000)    temp = ~(temp - 1);
    return (uint16_t)temp;
}

//1mA/bit
//long INA226_GetShunt_Current(u8 addr)
//{
//    long temp=0;
//    INA226_SetRegPointer(addr,CUR_REG);
//    temp = INA226_ReadData(addr);
//    if(temp&0x8000)
//    {
//        temp = ~(temp - 1);
//        temp = (u16)temp - 2 * (u16)temp;
//    }
//    return temp;
//}

