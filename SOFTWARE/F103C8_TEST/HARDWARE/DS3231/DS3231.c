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
	DS3231_WriteByte(DS3231_CONTROL, 0x04);   //�ж���������1��2�ر�
	DS3231_WriteByte(DS3231_STATUS, 0x00);   //32KHZ�����ֹ�����ӱ�־λ����
}

u8 BCD2HEX(u8 val)   //BCDת��ΪByte
{
	u8 i;
	i = val & 0x0f;
	val >>= 4;
	val &= 0x0f;
	val *= 10;
	i += val;

	return i;
}
u8 HEX2BCD(u8 val)//B��ת��ΪBCD��
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

/*********************��ȡʱ��**********************/
void DS3231_ReadDateTime(u8 IsHex)
{
	DateTime.Year = DS3231_ReadByte(DS3231_YEAR); //��
	DateTime.Month = DS3231_ReadByte(DS3231_MONTH); //��
	DateTime.Day = DS3231_ReadByte(DS3231_DAY); //��
	DateTime.Hour = DS3231_ReadByte(DS3231_HOUR); //ʱ
	DateTime.Minute = DS3231_ReadByte(DS3231_MINUTE); //��
	DateTime.Second = DS3231_ReadByte(DS3231_SECOND); //��

	DateTime.TemperH = DS3231_ReadByte(DS3231_TEMPERATUREH); //�¶ȸ��ֽ�
	DateTime.TemperL = DS3231_ReadByte(DS3231_TEMPERATUREL); //�¶ȵ��ֽ�

	DateTime.Week = DS3231_ReadByte(DS3231_WEEK); //����

	if (IsHex == 1)
	{
		DateTime.Year = BCD2HEX(DateTime.Year); //��
		DateTime.Month = BCD2HEX(DateTime.Month); //��
		DateTime.Day = BCD2HEX(DateTime.Day); //��
		DateTime.Hour = BCD2HEX(DateTime.Hour); //ʱ
		DateTime.Minute = BCD2HEX(DateTime.Minute); //��
		DateTime.Second = BCD2HEX(DateTime.Second); //��

		DateTime.TemperH = BCD2HEX(DateTime.TemperH); //�¶ȸ��ֽ�
		DateTime.TemperL = BCD2HEX(DateTime.TemperL); //�¶ȵ��ֽ�
		
		DateTime.Week = BCD2HEX(DateTime.Week); //����
	}

}
/***************************************************/

/*******************��ȡ����ʱ��********************/
void DS3231_ReadAlarmTime(u8 IsHex)
{
	Alarm1Time.Second = DS3231_ReadByte(DS3231_ALARM1SECOND); //��
	Alarm1Time.Minute = DS3231_ReadByte(DS3231_ALARM1MINUTE); //��
	Alarm1Time.Hour = DS3231_ReadByte(DS3231_ALARM1HOUR); //ʱ
	Alarm1Time.Week = DS3231_ReadByte(DS3231_ALARM1WEEK); //����or��
	if (IsHex == 1)
	{
		Alarm1Time.Second = BCD2HEX(Alarm1Time.Second);
		Alarm1Time.Minute = BCD2HEX(Alarm1Time.Minute);
		Alarm1Time.Hour = BCD2HEX(Alarm1Time.Hour);
		Alarm1Time.Week = BCD2HEX(Alarm1Time.Week);
	}


	Alarm2Time.Minute = DS3231_ReadByte(DS3231_ALARM2MINUTE); //��
	Alarm2Time.Hour = DS3231_ReadByte(DS3231_ALARM2HOUR); //ʱ
	Alarm2Time.Week = DS3231_ReadByte(DS3231_ALARM2WEEK); //����or��
	if (IsHex == 1)
	{
		Alarm2Time.Minute = BCD2HEX(Alarm2Time.Minute);
		Alarm2Time.Hour = BCD2HEX(Alarm2Time.Hour);
		Alarm2Time.Week = BCD2HEX(Alarm2Time.Week);
	}
}

/****************�޸�ʱ��,BCD������*****************/
void DS3231_ModifyDateTime(u8 yea, u8 mon, u8 da, u8 hou, u8 min, u8 sec)
{
	u8 temp = 0;

	temp = HEX2BCD(yea);
	DS3231_WriteByte(DS3231_YEAR, temp); //�޸���

	temp = HEX2BCD(mon);
	DS3231_WriteByte(DS3231_MONTH, temp); //�޸���

	temp = HEX2BCD(da);
	DS3231_WriteByte(DS3231_DAY, temp); //�޸���

	temp = HEX2BCD(hou);
	DS3231_WriteByte(DS3231_HOUR, temp); //�޸�ʱ

	temp = HEX2BCD(min);
	DS3231_WriteByte(DS3231_MINUTE, temp); //�޸ķ�

	temp = HEX2BCD(sec);
	DS3231_WriteByte(DS3231_SECOND, temp); //�޸���

	 //�Զ��������� д�� 1---7  == ����һ>>>������(��)
	if( mon == 1 || mon == 2 )
	{
		mon += 12;
		if( yea > 0 )
			yea--;
		else
			yea = 4;
	}
	temp = (1 + (( da + 2 * mon + 3 * (mon + 1) / 5 + yea + yea / 4 ) % 7));
	DS3231_WriteByte(DS3231_WEEK, temp); //�޸�����
}
/****************************************************/

u8 DS3231_RunNian(void)//�����ж�
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


