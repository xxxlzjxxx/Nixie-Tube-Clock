#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "key.h"
#include "tm1650.h"
#include "usmart.h"
#include "ds3231.h"
#include "sht30.h"

int main(void)
{	
	u16 t = 0;
    u8 Brightness = 1;
    u8 DisplayStatus = 0;    //数码管当前显示状态，0-时间，1-温度，2-湿度
 
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(115200);	 //串口初始化为9600

    TIM3_Int_Init(4999, 3200 - 1);//10Khz的计数频率，计数到5000为500ms  
    usmart_dev.init(32);	//初始化USMART   
	printf(">>system reset \r\n");
	KEY_Init();          	//初始化与按键连接的硬件接口
    TM16_INIT(Brightness);
    DS3231_Init();
    SHT_Init();
	LED_Init();		  	 //初始化与LED连接的硬件接口 

	DS3231_ModifyDateTime(2022,7,7,19,01,01);
	
    while(1){       
        DS3231_ReadDateTime(1);
        SHT_GetValue();//获取温湿度数据
        
        t=KEY_Scan(0);		//得到键值
		switch(t)
		{				 
			case KEY0_PRES:
				DateTime.Minute++;
                if(DateTime.Minute == 60){
                    DateTime.Minute = 0;
                }
                DS3231_ModifyDateTime(DateTime.Year,DateTime.Month,DateTime.Day,DateTime.Hour,DateTime.Minute,DateTime.Second);
				break;
			case KEY1_PRES:
                DateTime.Minute--;
                DS3231_ModifyDateTime(DateTime.Year,DateTime.Month,DateTime.Day,DateTime.Hour,DateTime.Minute,DateTime.Second);
                if(DateTime.Minute == 0){
                    DateTime.Minute = 60;
                }
                break;
            case WKUP_PRES:
                DisplayStatus++;
                if(DisplayStatus == 3){
                    DisplayStatus = 0;
                }
                // Brightness++;
                // if(Brightness == 8){
                //     Brightness = 0;
                // }
                // TM16_Set_Brig(Brightness);
				break;
			default:
				break;	
		} 
        //根据DisplayStatus决定当前显示内容
        switch (DisplayStatus)
        {
        case 0:
            t = DateTime.Hour * 100 + DateTime.Minute;
            break;
        case 1:
            t = (int)Tem_Value;
            break;
        case 2:
            t = (int)RH_Value;
            break;

        default:
            break;
        }
        //根据DateTime.Hour决定当前显示亮度
        switch (DateTime.Hour)
        {
        case 7:
            TM16_Set_Brig(1);
            break;
        case 19:
            TM16_Set_Brig(4);
            break;

        default:
            break;
        }

        TM16_Display(t, 3);
        
	} 
}


