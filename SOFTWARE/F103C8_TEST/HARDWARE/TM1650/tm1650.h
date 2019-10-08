#ifndef __TM1650_H__
#define __TM1650_H__

#include "sys.h"

//设置接口的 pin
#define TM16_RCC RCC_APB2Periph_GPIOB			//RCC
#define TM16_PORT GPIOB					//PORT
#define TM16_SDA GPIO_Pin_7			//SDA
#define TM16_SCL GPIO_Pin_6			//SCL
#define TM16_ReadSDA() 	GPIO_ReadInputDataBit(TM16_PORT,TM16_SDA)		//读取SDA

#define TM16_SDA_H GPIO_SetBits(TM16_PORT,TM16_SDA)
#define TM16_SDA_L GPIO_ResetBits(TM16_PORT,TM16_SDA)
#define TM16_SCL_H GPIO_SetBits(TM16_PORT,TM16_SCL)
#define TM16_SCL_L GPIO_ResetBits(TM16_PORT,TM16_SCL)

#define TM16_delay(x){dtmelay(x);}		//设置延时函数,方便移植，可修改为自己的延时函数
void TM16_INIT(unsigned char brig);		//初始化TM1650,  参数为亮度 0-7
void TM16_Set_Brig(unsigned char BRIG);
void TM16_Display(unsigned int data,unsigned char di);//显示函数，data为显示的数据，0-99，di设置是否显示小数点，0为都不显示小数点，1为第一位显示小数点，2为第二位显示小数点，3为第一位和第二位都显示小数点


#endif

