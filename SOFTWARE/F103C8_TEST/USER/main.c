#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "key.h"
#include "tm1650.h"
#include "usmart.h"
#include "ds3231.h"

int main(void){	
	u16 t = 0;
    u8 Brightness = 1;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	    	 //延时函数初始化	  
    uart_init(115200);	 	//串口初始化为9600
	LED_Init();		  	 	//初始化与LED连接的硬件接口
    TIM3_Int_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms  
    usmart_dev.init(72);	//初始化USMART   
	KEY_Init();          	//初始化与按键连接的硬件接口
    TM16_INIT(2);
    DS3231_Init();

	while(1){ 
        DS3231_ReadDateTime(1);
        
        t=KEY_Scan(1);		//得到键值
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
                Brightness++;
                if(Brightness == 8){
                    Brightness = 0;
                }
                TM16_Set_Brig(Brightness);
				break;
			default:
				break;	
		} 
        
        t = DateTime.Hour * 100 + DateTime.Minute;
        TM16_Display(t, 3);
	}		 
}
