#ifndef __TM1650_H__
#define __TM1650_H__

#include "sys.h"

//���ýӿڵ� pin
#define TM16_RCC RCC_APB2Periph_GPIOB			//RCC
#define TM16_PORT GPIOB					//PORT
#define TM16_SDA GPIO_Pin_7			//SDA
#define TM16_SCL GPIO_Pin_6			//SCL
#define TM16_ReadSDA() 	GPIO_ReadInputDataBit(TM16_PORT,TM16_SDA)		//��ȡSDA

#define TM16_SDA_H GPIO_SetBits(TM16_PORT,TM16_SDA)
#define TM16_SDA_L GPIO_ResetBits(TM16_PORT,TM16_SDA)
#define TM16_SCL_H GPIO_SetBits(TM16_PORT,TM16_SCL)
#define TM16_SCL_L GPIO_ResetBits(TM16_PORT,TM16_SCL)

#define TM16_delay(x){dtmelay(x);}		//������ʱ����,������ֲ�����޸�Ϊ�Լ�����ʱ����
void TM16_INIT(unsigned char brig);		//��ʼ��TM1650,  ����Ϊ���� 0-7
void TM16_Set_Brig(unsigned char BRIG);
void TM16_Display(unsigned int data,unsigned char di);//��ʾ������dataΪ��ʾ�����ݣ�0-99��di�����Ƿ���ʾС���㣬0Ϊ������ʾС���㣬1Ϊ��һλ��ʾС���㣬2Ϊ�ڶ�λ��ʾС���㣬3Ϊ��һλ�͵ڶ�λ����ʾС����


#endif

