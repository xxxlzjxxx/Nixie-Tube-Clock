#include "sys.h"
#ifndef __DS3231_H__
#define __DS3231_H__


#define	DS3231_ADDRESS   0xD0  //器件地址

#define DS3231_SECOND 0x00  //秒
#define DS3231_MINUTE 0x01  //分
#define DS3231_HOUR 0x02  //时
#define DS3231_WEEK 0x03  //星期
#define DS3231_DAY 0x04  //日
#define DS3231_MONTH 0x05  //月
#define DS3231_YEAR 0x06  //年

//闹铃1
#define DS3231_ALARM1SECOND 0x07    //秒
#define DS3231_ALARM1MINUTE 0x08    //分
#define DS3231_ALARM1HOUR   0x09    //时
#define DS3231_ALARM1WEEK   0x0A    //星期/日
//闹铃2
#define DS3231_ALARM2MINUTE 0x0b    //分
#define DS3231_ALARM2HOUR   0x0c    //时
#define DS3231_ALARM2WEEK   0x0d    //星期/日

#define DS3231_CONTROL      0x0e    //控制寄存器
#define DS3231_STATUS       0x0f    //状态寄存器
#define BSY                 2       //忙
#define OSF                 7       //振荡器停止标志
#define DS3231_XTAL         0x10    //晶体老化寄存器
#define DS3231_TEMPERATUREH 0x11    //温度寄存器高字节(8位)
#define DS3231_TEMPERATUREL 0x12    //温度寄存器低字节(高2位) 

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

