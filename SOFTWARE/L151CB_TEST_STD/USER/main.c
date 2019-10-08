#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "key.h"
#include "tm1650.h"
#include "usmart.h"
#include "ds3231.h"
//ALIENTEK Mini STM32开发板范例代码3
//串口实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
int main(void)
{	
	u16 t = 0;
    u8 Brightness = 1;
 
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(115200);	 //串口初始化为9600

    TIM3_Int_Init(4999, 3200 - 1);//10Khz的计数频率，计数到5000为500ms  
    usmart_dev.init(72);	//初始化USMART   
	KEY_Init();          	//初始化与按键连接的硬件接口
    TM16_INIT(Brightness);
    DS3231_Init();
	LED_Init();		  	 //初始化与LED连接的硬件接口    
	
    while(1){       
        DS3231_ReadDateTime(1);
        
        t=KEY_Scan(1);		//得到键值
		switch(t)
		{				 
			case KEY0_PRES:
				DateTime.Minute++;
                DS3231_ModifyDateTime(DateTime.Year,DateTime.Month,DateTime.Day,DateTime.Hour,DateTime.Minute,DateTime.Second);
				break;
			case KEY1_PRES:
                DateTime.Minute--;
                DS3231_ModifyDateTime(DateTime.Year,DateTime.Month,DateTime.Day,DateTime.Hour,DateTime.Minute,DateTime.Second);
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
        
//        printf("年月日:");
//        printf("%d,%d,%d    ",DateTime.Year,DateTime.Month,DateTime.Day);
//        printf("时分秒:");
//        printf("%d,%d,%d\r\n",DateTime.Hour,DateTime.Minute,DateTime.Second);
//        delay_ms(1000);
	} 
}


