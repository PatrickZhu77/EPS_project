
#define IIC_SCL_H   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET) //SCL H
#define IIC_SCL_L   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET) //SCL H
#define IIC_SDA_H    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET) //SDA_H
#define IIC_SDA_L      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET) //SDA_L
#define READ_SDA      HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11)  //输入SDA

void IIC_Init(void)
    {
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOC时钟

    //PC11,12初始化设置
    GPIO_Initure.Pin=GPIO_PIN_11|GPIO_PIN_12;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;     //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);

        GPIO_Initure.Pin = GPIO_PIN_10;
        GPIO_Initure.Mode=GPIO_MODE_INPUT;
    GPIO_Initure.Pull=GPIO_NOPULL;
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;     //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);

    IIC_SDA_H;
    IIC_SCL_H;
        delay_nms(5);
}
View Code
记得初始化时把SDA和SCL拉高

 

IIC开始：

void IIC_Start(void)
{
    IIC_SDA_H;
    IIC_SCL_H;
    delay_nns(5);
     IIC_SDA_L;//START:when CLK is high,DATA change form high to low
    delay_nns(5);
    IIC_SCL_L;//钳住I2C总线，准备发送或接收数据
    delay_nns(5);
}
 

IIC结束：

void IIC_Stop(void)
{
    IIC_SDA_L;//STOP:when CLK is high DATA change form low to high
     delay_nns(5);
    IIC_SCL_H;
        delay_nns(5);
    IIC_SDA_H;//发送I2C总线结束信号
    delay_nns(5);
}
 

发送ACK：

void IIC_Ack(void)
{
    IIC_SDA_L;
    delay_nns(5);
    IIC_SCL_H;
    delay_nns(5);
    IIC_SCL_L;
    delay_nns(5);
    IIC_SDA_H;
}
 

不发送NACK：

void IIC_NAck(void)
{
    IIC_SDA_H;
    delay_nns(5);
    IIC_SCL_H;
delay_nns(5);
    IIC_SCL_L;
    delay_nns(5);
    IIC_SDA_L;
}
 

等待ACK：

uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    //SDA_IN();      //SDA设置为输入
    IIC_SDA_H;
    delay_nns(5);
    IIC_SCL_H;
    delay_nns(5);

    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_L;//时钟输出0
    return 0;
}
 

发送字节：

void IIC_Send_Byte(uint8_t txd)
{
   IIC_SCL_L;//拉低时钟开始数据传输
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
    //    IIC_SCL_H;            //当去掉wait_ack时的时候添加
    //    delay_nns(5);
        //IIC_SCL_L;
}
 

读取字节：

uint8_t IIC_Read_Byte(unsigned char ack)
{
      uint8_t TData=0,i;
    for(i=0;i<8;i++)
    {
        IIC_SCL_H;
        delay_nns(5);
        TData=TData<<1;
        //if(GPIOB->IDR& GPIO_IDR_IDR7)        //判断SDA是否为高
            if(SDA_READ)
        {
            TData|=0x01;
        }
        IIC_SCL_L;
        delay_nns(5);
    }
        if(!ack)
            IIC_NAck();
        else
            IIC_Ack();
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
