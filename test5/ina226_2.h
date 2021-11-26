#ifndef _INA226_H
#define _INA226_H



#define     CFG_REG             0x00        //
#define     SV_REG             0x01        //������ѹ�� �˴���������Ϊ 0.1ŷ
#define     BV_REG             0x02        //���ߵ�ѹ
#define     PWR_REG         0x03        //��Դ����
#define     CUR_REG         0x04        //����
#define     CAL_REG         0x05        //У׼���趨�����̷�Χ�Լ������͹��ʲ�����
//#define     ONFF_REG         0x06        //���� ʹ�� �������ú�ת��׼������
//#define     AL_REG             0x07        //��������ѡ����������Ƚϵ��޶�ֵ
#define     INA226_GET_ADDR 0XFF        //����Ψһ��оƬ��ʶ��
#define       INA226_ADDR1    0x80



//IO����
#define IIC_SCL_H   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET) //SCL H
#define IIC_SCL_L   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET) //SCL H
#define IIC_SDA_H    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET) //SDA_H
#define IIC_SDA_L      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET) //SDA_L
#define READ_SDA      HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)  //����SDA
#define SDA_READ   GPIOB->IDR& GPIO_IDR_IDR11


typedef struct
{
    float voltageVal;            //mV
    float Shunt_voltage;        //uV
    float Shunt_Current;        //mA
    float powerVal;
}INA226;

static void delay_nns(uint16_t D);
void delay_nms(uint16_t ms);
void delay_nus(uint16_t us);
void INA226_IIC_Init(void);
void INA226_IIC_Start(void);
void INA226_IIC_Stop(void);
void INA226_IIC_Ack(void);
void INA226_IIC_NAck(void);
uint8_t INA226_IIC_Wait_Ack(void);
void INA226_IIC_Send_Byte(uint8_t txd);
uint8_t INA226_IIC_Read_Byte(unsigned char ack);
void INA226_Init(void);
void INA226_SendData(uint8_t addr,uint8_t reg,uint16_t data);
void INA226_SetRegPointer(uint8_t addr,uint8_t reg);
uint16_t INA226_ReadData(uint8_t addr);
long INA226_GetShunt_Current(uint8_t addr);
uint16_t  INA226_Get_ID(uint8_t addr);
uint16_t INA226_GET_CAL_REG(uint8_t addr);
uint32_t INA226_GetVoltage(uint8_t addr);
uint32_t INA226_GetShuntVoltage(uint8_t addr);
uint16_t INA226_Get_Power(uint8_t addr);
void GetVoltage(float *Voltage);
void Get_Shunt_voltage(float *Voltage);
void Get_Shunt_Current(float *Current);
void GetPower();
extern INA226 INA226_data;
float GetV(void);
#endif

