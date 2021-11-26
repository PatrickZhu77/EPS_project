/*
 * myiic.h
 *
 *  Created on: Apr 2, 2021
 *      Author: zqqqq
 */

myiic.h

#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"  

//IO方向设置
//CRH,CRL 端口输入输出模式寄存器。CRH为8-15引脚，CRL为0-7引脚
#define SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}//PC11引脚为上拉/下拉输入
#define SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}//PC11为推挽输出

//IO操作函数    
#define IIC_SCL    PCout(12) //SCL
#define IIC_SDA    PCout(11) //SDA    
#define READ_SDA   PCin(11)  //输入SDA

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口                
void IIC_Start(void);                //发送IIC开始信号
void IIC_Stop(void);                  //发送IIC停止信号
void IIC_Send_Byte(u8 txd);            //IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void);                 //IIC等待ACK信号
void IIC_Ack(void);                    //IIC发送ACK信号
void IIC_NAck(void);                //IIC不发送ACK信号

//一下两个函数实际并未被定义
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);      
#endif

.c文件

#include "myiic.h"
#include "delay.h"


//初始化IIC
//硬件I2C必须开漏输出
//模拟I2C则为推挽输出（方便输出高低电平？）
void IIC_Init(void)
{                         
    GPIO_InitTypeDef GPIO_InitStructure;
    //RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟
    RCC_APB2PeriphClockCmd(    RCC_APB2Periph_GPIOC, ENABLE );    
       
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
 
    IIC_SCL=1;//PCout(12)
    IIC_SDA=1;//PCout(11)

}
//产生IIC起始信号
//SCL为高时，SDA拉低，准备开始传数据。（记得再拉低SCL）
//SCL为高时，SDA拉高，结束传输数据。
//传输数据时，SCL为高时数据有效
void IIC_Start(void)
{
    SDA_OUT();     //sda线输出
    IIC_SDA=1;            
    IIC_SCL=1;
    delay_us(4);
     IIC_SDA=0;//START:when CLK is high,DATA change form high to low
    delay_us(4);
    IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
}      
//产生IIC停止信号
void IIC_Stop(void)
{
    SDA_OUT();//sda线输出
    IIC_SCL=0;
    IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
     delay_us(4);
    IIC_SCL=1;
    IIC_SDA=1;//发送I2C总线结束信号
    delay_us(4);                                   
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    SDA_IN();      //SDA设置为输入  
    IIC_SDA=1;delay_us(1);       
    IIC_SCL=1;delay_us(1);     //设置数据有效
    while(READ_SDA)//READ_SDA为0时表示接收到应答
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL=0;//时钟输出0        
    return 0;  
}
//产生ACK应答
void IIC_Ack(void)
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=0;
    delay_us(2);
    IIC_SCL=1;
    delay_us(2);
    IIC_SCL=0;
}
//不产生ACK应答            
void IIC_NAck(void)
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=1;
    delay_us(2);
    IIC_SCL=1;
    delay_us(2);
    IIC_SCL=0;
}                                          
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答              
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
    SDA_OUT();         
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;//先发高位
        txd<<=1;       
        delay_us(2);   //对TEA5767这三个延时都是必须的
        IIC_SCL=1;
        delay_us(2);
        IIC_SCL=0;    
        delay_us(2);
    }    
}         
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
    {
        IIC_SCL=0;
        delay_us(2);
        IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
        delay_us(1);
    }                    
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

