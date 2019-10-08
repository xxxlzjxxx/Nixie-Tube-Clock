#include "sys.h"
#ifndef __DS3231_H__
#define __DS3231_H__


#define	DS3231_ADDRESS   0xD0  //������ַ

#define DS3231_SECOND 0x00  //��
#define DS3231_MINUTE 0x01  //��
#define DS3231_HOUR 0x02  //ʱ
#define DS3231_WEEK 0x03  //����
#define DS3231_DAY 0x04  //��
#define DS3231_MONTH 0x05  //��
#define DS3231_YEAR 0x06  //��

//����1
#define DS3231_ALARM1SECOND 0x07    //��
#define DS3231_ALARM1MINUTE 0x08    //��
#define DS3231_ALARM1HOUR   0x09    //ʱ
#define DS3231_ALARM1WEEK   0x0A    //����/��
//����2
#define DS3231_ALARM2MINUTE 0x0b    //��
#define DS3231_ALARM2HOUR   0x0c    //ʱ
#define DS3231_ALARM2WEEK   0x0d    //����/��

#define DS3231_CONTROL      0x0e    //���ƼĴ���
#define DS3231_STATUS       0x0f    //״̬�Ĵ���
#define BSY                 2       //æ
#define OSF                 7       //����ֹͣ��־
#define DS3231_XTAL         0x10    //�����ϻ��Ĵ���
#define DS3231_TEMPERATUREH 0x11    //�¶ȼĴ������ֽ�(8λ)
#define DS3231_TEMPERATUREL 0x12    //�¶ȼĴ������ֽ�(��2λ) 

typedef struct
{
	u8 Year;
	u8 Month;
	u8 Day;
	u8 Week;
	u8 Hour;
	u8 Minute;
	u8 Second;
	u8 TemperH;
	u8 TemperL;
}DS3231_DateTime;

typedef struct
{
	u8 Week;
	u8 Hour;
	u8 Minute;
	u8 Second;
}DS3231Alarm1Time;

typedef struct
{
	u8 Week;
	u8 Hour;
	u8 Minute;
}DS3231Alarm2Time;

extern DS3231_DateTime DateTime;
extern DS3231Alarm1Time Alarm1Time;
extern DS3231Alarm2Time Alarm2Time;

void DS3231_Init(void);
u8 DS3231_ReadByte(u8 reg_addr);
void DS3231_WriteByte(u8 reg_addr, u8 reg_dat);
void DS3231_ReadDateTime(u8 IsHex);
void DS3231_ReadAlarmTime(u8 IsHex);
void DS3231_ModifyDateTime(u8 yea, u8 mon, u8 da, u8 hou, u8 min, u8 sec);
u8 DS3231_RunNian(void);	

#endif

