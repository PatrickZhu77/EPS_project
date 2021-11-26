#ifndef __INA3221_H
#define __INA3221_H
 #include "main.h"


#define     CFG_REG_3221             0x00        //
#define     SV_REG_3221             0x01        //������ѹ�� �˴���������Ϊ 0.1ŷ
#define     BV_REG_3221             0x02        //���ߵ�ѹ
#define     PWR_REG_3221         0x03        //��Դ����
#define     CUR_REG_3221        0x04        //����
#define     CAL_REG_3221         0x05        //У׼���趨�����̷�Χ�Լ������͹��ʲ�����
//#define     ONFF_REG         0x06        //���� ʹ�� �������ú�ת��׼������
//#define     AL_REG             0x07        //��������ѡ����������Ƚϵ��޶�ֵ
#define     INA3221_GET_ADDR 0XFF        //����Ψһ��оƬ��ʶ��
#define       INA3221_ADDR1    0x80


//IO����
#define IIC_SCL_H_3221   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET) //SCL H
#define IIC_SCL_L_3221   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET) //SCL H
#define IIC_SDA_H_3221   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET) //SDA_H
#define IIC_SDA_L_3221  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET) //SDA_L
#define READ_SDA_3221     HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)  //����SDA
#define SDA_READ_3221  GPIOB->IDR& GPIO_IDR_IDR7


static void delay_nns_3221(uint16_t D);
void delay_nms_3221(uint16_t ms) ;
    void delay_nus_3221(uint16_t us);
        void INA3221_IIC_Init(void);
            void INA3221_IIC_Start(void);
    void INA3221_IIC_Stop(void);
void INA3221_IIC_Ack(void);
void INA3221_IIC_NAck(void);
uint8_t INA3221_IIC_Wait_Ack(void);
void INA3221_IIC_Send_Byte(uint8_t txd);
uint8_t INA3221_IIC_Read_Byte(unsigned char ack);
void INA3221_Init(void);
    void INA3221_SendData(uint8_t addr,uint8_t reg,uint16_t data);
        void INA3221_SetRegPointer(uint8_t addr,uint8_t reg);
            uint16_t INA3221_ReadData(uint8_t addr);
            uint16_t INA3221_GetVoltage(uint8_t addr,uint8_t channel);
            uint16_t INA3221_GetShuntVoltage(uint8_t addr,uint8_t channel);


 #endif
��������������������������������
��Ȩ����������ΪCSDN���������ֲ���Cypher����ԭ�����£���ѭCC 4.0 BY-SA��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
ԭ�����ӣ�https://blog.csdn.net/weixin_36038705/article/details/106346879
