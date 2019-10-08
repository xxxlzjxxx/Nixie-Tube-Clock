#include "DS3231.h"
#include "myiic.h" 
#include "delay.h"   
#include "sys.h"  
     
DS3231_DateTime DateTime;
DS3231Alarm1Time Alarm1Time;
DS3231Alarm2Time Alarm2Time;

void DS3231_Init(void)
{
	IIC_Init();
	DS3231_WriteByte(DS3231_CONTROL, 0x04);   //中断允许，闹钟1和2关闭
	DS3231_WriteByte(DS3231_STATUS, 0x00);   //32KHZ输出禁止，闹钟标志位清零
}

u8 BCD2HEX(u8 val)   //BCD转换为Byte
{
	u8 i;
	i = val & 0x0f;
	val >>= 4;
	val &= 0x0f;
	val *= 10;
	i += val;

	return i;
}
u8 HEX2BCD(u8 val)//B码转换为BCD码
{
	u8 i, j, k;
	i = val / 10;
	j = val % 10;
	k = j + (i << 4);
	return k;
}

u8 DS3231_ReadByte(u8 reg_addr)
{
	u8 Ret;

	IIC_Start();
	IIC_Send_Byte(DS3231_ADDRESS);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg_addr);
	IIC_Wait_Ack();

	IIC_Start();
	IIC_Send_Byte(DS3231_ADDRESS + 1);
	IIC_Wait_Ack();

	Ret = IIC_Read_Byte(0);
	IIC_Stop();
	return Ret;
}

void DS3231_WriteByte(u8 reg_addr, u8 reg_dat)
{   
	IIC_Start();
	IIC_Send_Byte(DS3231_ADDRESS);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg_addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg_dat);
	IIC_Wait_Ack();
	IIC_Stop();
}

/*********************读取时间**********************/
void DS3231_ReadDateTime(u8 IsHex)
{
	DateTime.Year = DS3231_ReadByte(DS3231_YEAR); //年
	DateTime.Month = DS3231_ReadByte(DS3231_MONTH); //月
	DateTime.Day = DS3231_ReadByte(DS3231_DAY); //日
	DateTime.Hour = DS3231_ReadByte(DS3231_HOUR); //时
	DateTime.Minute = DS3231_ReadByte(DS3231_MINUTE); //分
	DateTime.Second = DS3231_ReadByte(DS3231_SECOND); //秒

	DateTime.TemperH = DS3231_ReadByte(DS3231_TEMPERATUREH); //温度高字节
	DateTime.TemperL = DS3231_ReadByte(DS3231_TEMPERATUREL); //温度低字节

	DateTime.Week = DS3231_ReadByte(DS3231_WEEK); //星期

	if (IsHex == 1)
	{
		DateTime.Year = BCD2HEX(DateTime.Year); //年
		DateTime.Month = BCD2HEX(DateTime.Month); //月
		DateTime.Day = BCD2HEX(DateTime.Day); //日
		DateTime.Hour = BCD2HEX(DateTime.Hour); //时
		DateTime.Minute = BCD2HEX(DateTime.Minute); //分
		DateTime.Second = BCD2HEX(DateTime.Second); //秒

		DateTime.TemperH = BCD2HEX(DateTime.TemperH); //温度高字节
		DateTime.TemperL = BCD2HEX(DateTime.TemperL); //温度低字节
		
		DateTime.Week = BCD2HEX(DateTime.Week); //星期
	}

}
/***************************************************/

/*******************读取闹铃时间********************/
void DS3231_ReadAlarmTime(u8 IsHex)
{
	Alarm1Time.Second = DS3231_ReadByte(DS3231_ALARM1SECOND); //秒
	Alarm1Time.Minute = DS3231_ReadByte(DS3231_ALARM1MINUTE); //分
	Alarm1Time.Hour = DS3231_ReadByte(DS3231_ALARM1HOUR); //时
	Alarm1Time.Week = DS3231_ReadByte(DS3231_ALARM1WEEK); //星期or日
	if (IsHex == 1)
	{
		Alarm1Time.Second = BCD2HEX(Alarm1Time.Second);
		Alarm1Time.Minute = BCD2HEX(Alarm1Time.Minute);
		Alarm1Time.Hour = BCD2HEX(Alarm1Time.Hour);
		Alarm1Time.Week = BCD2HEX(Alarm1Time.Week);
	}


	Alarm2Time.Minute = DS3231_ReadByte(DS3231_ALARM2MINUTE); //分
	Alarm2Time.Hour = DS3231_ReadByte(DS3231_ALARM2HOUR); //时
	Alarm2Time.Week = DS3231_ReadByte(DS3231_ALARM2WEEK); //星期or日
	if (IsHex == 1)
	{
		Alarm2Time.Minute = BCD2HEX(Alarm2Time.Minute);
		Alarm2Time.Hour = BCD2HEX(Alarm2Time.Hour);
		Alarm2Time.Week = BCD2HEX(Alarm2Time.Week);
	}
}

/****************修改时间,BCD码输入*****************/
void DS3231_ModifyDateTime(u8 yea, u8 mon, u8 da, u8 hou, u8 min, u8 sec)
{
	u8 temp = 0;

	temp = HEX2BCD(yea);
	DS3231_WriteByte(DS3231_YEAR, temp); //修改年

	temp = HEX2BCD(mon);
	DS3231_WriteByte(DS3231_MONTH, temp); //修改月

	temp = HEX2BCD(da);
	DS3231_WriteByte(DS3231_DAY, temp); //修改日

	temp = HEX2BCD(hou);
	DS3231_WriteByte(DS3231_HOUR, temp); //修改时

	temp = HEX2BCD(min);
	DS3231_WriteByte(DS3231_MINUTE, temp); //修改分

	temp = HEX2BCD(sec);
	DS3231_WriteByte(DS3231_SECOND, temp); //修改秒

	 //自动计算星期 写入 1---7  == 星期一>>>星期日(七)
	if( mon == 1 || mon == 2 )
	{
		mon += 12;
		if( yea > 0 )
			yea--;
		else
			yea = 4;
	}
	temp = (1 + (( da + 2 * mon + 3 * (mon + 1) / 5 + yea + yea / 4 ) % 7));
	DS3231_WriteByte(DS3231_WEEK, temp); //修改星期
}
/****************************************************/

u8 DS3231_RunNian(void)//闰年判断
{
	if((DateTime.Year % 4 == 0 && DateTime.Year % 100 != 0) || (DateTime.Year % 100 == 0 && DateTime.Year % 400 == 0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


