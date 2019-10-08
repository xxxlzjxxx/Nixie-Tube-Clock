#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "key.h"
#include "tm1650.h"
#include "usmart.h"
#include "ds3231.h"
//ALIENTEK Mini STM32�����巶������3
//����ʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
int main(void)
{	
	u16 t = 0;
    u8 Brightness = 1;
 
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(115200);	 //���ڳ�ʼ��Ϊ9600

    TIM3_Int_Init(4999, 3200 - 1);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
    usmart_dev.init(72);	//��ʼ��USMART   
	KEY_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�
    TM16_INIT(Brightness);
    DS3231_Init();
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�    
	
    while(1){       
        DS3231_ReadDateTime(1);
        
        t=KEY_Scan(1);		//�õ���ֵ
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
        
//        printf("������:");
//        printf("%d,%d,%d    ",DateTime.Year,DateTime.Month,DateTime.Day);
//        printf("ʱ����:");
//        printf("%d,%d,%d\r\n",DateTime.Hour,DateTime.Minute,DateTime.Second);
//        delay_ms(1000);
	} 
}


