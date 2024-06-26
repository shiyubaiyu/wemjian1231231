#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "exti.h"
#include "key.h"
#include "oled.h"
#include "adc.h"

 /**
************************************************************
* @file         
* @brief        
* @author    xiaoming
* @date  		 2022.2.12
* @version   当前为软件V1.4.1 兼容硬件V1.4 和V1.5的板子
* @note      视频资料链接：https://space.bilibili.com/434192043
*            相关配套元件: https://shop415231378.taobao.com/?spm=a1z10.1-c.0.0.13e8fc79Mq3TsN
*                          https://item.taobao.com/item.htm?spm=a230r.1.14.11.7f004ab4M63n8v&id=668903824337&ns=1&abbucket=9#detail
			 联  系 作 者：QQ:1930299709
*            感谢正点原子团队，本工程参考了正点原子例程
*			 			 

*						 更改说明：
						 V1.4.1更改UsartPrintf函数，UsartPrintf函数后面添加了UsartPrintfBuf数组的清除
*
***********************************************************/

int g_USART1_FLAG =0;
int g_USART3_FLAG =0;

extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值	
int Mode = 3;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
u8 string[10] = {0};	//定义在前面
float pitch =1.2;
int time = 0;
int Distance = 0;
int t = 0;

 int main(void)
 {	

	u16 adcx;
	float temp;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	    //延时函数初始化	  
	uart_init(115200);
	uart3_init(115200);
	LED_Init();		  	//初始化与LED连接的硬件接口
	TB6612_GPIO_Init();
  EXTIX_Init();
	TCRT5000_Init();
  OLED_Init();
  OLED_Clear();
	SR04_GPIO_Init(); //超声波PA0
	Adc_Init();		  		//ADC初始化
	 
	TIM3_PWM_Init(999,1439);   //舵机PWM 
	 
	TIM1_PWM_Init(1999,359); //电机的PWM 
	TIM_SetCompare1(TIM1,500);	
	TIM_SetCompare4(TIM1,500);
	
	TIM2_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
	t=0;
	while(1)
	{
		

			//OLED 显示ADC测量
		adcx=Get_Adc_Average(ADC_Channel_4,10);
		temp=(float)adcx*(3.3/4096);
		sprintf((char *)string,"U:%.2f   ",(temp*5));
		OLED_ShowString(12,0,string,16);		
		
		//OLED显示距离功能
		sprintf((char *)string,"D:%d      ",SR04_Distance());
		OLED_ShowString(12,3,string,16);
		
		//OLED显示模式功能
		sprintf((char *)string,"Mode:%d   ",Mode);
		OLED_ShowString(12,6,string,16);
		//串口输出
		UsartPrintf(USART3,"Mode:%d\r\n",Mode);
		UsartPrintf(USART3,"U:%.2f\r\n",(temp*5));
		UsartPrintf(USART3,"D:%d \r\n",SR04_Distance());
		while(Mode == 0)
		{
			//停止模式
			TIM_SetCompare1(TIM3,80);	 //舵机向前 使超声波朝前方 
			delay_ms(200);
			AIN1 =0;
			AIN2  =0;
			BIN1 =0;
			BIN2 =0;
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			Mode=3;
//			Forward();
		}	
		while(Mode==2)
		{
		  AIN1 =0;
			AIN2  =0;
			BIN1 =0;
			BIN2 =0;
				delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
				delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
				delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
				delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
				delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			Mode=3;
		}
	while (Mode == 3) 
		{
	// ????
		
	TIM_SetCompare1(TIM3, 80);	// ???????????
	delay_ms(200);
		
	while (SR04_Distance() >= 25 && Mode == 3) {
	// ??????
	if(t==0)
		{
			delay_ms(10000);delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			delay_ms(10000);
			t++;
		}
	if (HW_2 == 1 && HW_1 == 0 && HW_3 == 0 && HW_4 == 1) {
		Forward();
		delay_ms(5);
	} else if (HW_2 == 1 && HW_1 == 0 && HW_3 == 1 && HW_4 == 1) {
		Forward();
		delay_ms(5);
	} else if (HW_2 == 0 && HW_1 == 1 && HW_3 == 1 && HW_4 == 1) {
		Rightward();
		delay_ms(5);
	} else if (HW_2 == 1 && HW_1 == 0 && HW_3 == 1 && HW_4 == 1) {
		Forward();
		delay_ms(5);
	} else if (HW_2 == 1 && HW_1 == 1 && HW_3 == 0 && HW_4 == 1) {
		Forward();
		delay_ms(5);
	} else if (HW_2 ==1 && HW_1 == 1 && HW_3 == 1 && HW_4 == 0) {
		Leftward();
		delay_ms(5);
	} else if (HW_2 == 1 && HW_1 == 1 && HW_3 == 0 && HW_4 == 1) {
		Forward();
		delay_ms(5);
	} else if (HW_2 == 1 && HW_1 == 1 && HW_3 == 1 && HW_4 == 1) {
		Forward();
		delay_ms(5);}
	 else if (HW_2 == 0 && HW_1 == 0 && HW_3 == 0 && HW_4 == 0) {
		Forward();
		delay_ms(5);}
} 
	while(SR04_Distance() < 25)
	{
	
	    AIN1 =0;
			AIN2  =0;
			BIN1 =0;
			BIN2 =0;
	}
}
	}}

//		if(Mode == 1)
//		{
//			//定距离跟随
//			if(SR04_Distance()>20)
//			{
//				Forward();
//				delay_ms(50);
//			}
//			if(SR04_Distance()<15)
//			{
//				Backward();
//				delay_ms(50);
//			}
//			AIN1 =0;
//			AIN2 =0;
//			BIN1 =0;
//			BIN2 =0;		
//		}

//		if(Mode == 2)
//		{
//		  //蓝牙遥控
//			if(g_USART3_FLAG == 5)//前
//			{
//				LED =~LED;
//				AIN1 =0;
//				AIN2 =0;
//				BIN1 =0;
//				BIN2 =0;					
//			}
//			if(g_USART3_FLAG == 1)//前
//			{
//				LED =~LED;
//				Forward();
//				delay_ms(100);
//     					
//			}
//			if(g_USART3_FLAG == 2)//后
//			{
//				LED =~LED;
//				Backward();
//				delay_ms(100);
//				
//			}
//			if(g_USART3_FLAG == 3)//右
//			{
//				LED =~LED;
//				Rightward();
//				delay_ms(100);	
//					
//			}
//			if(g_USART3_FLAG == 4)//左
//			{
//				LED =~LED;
//				Leftward();
//				delay_ms(100);
//      
//			}
//					
//		}
//		if(Mode == 4)
//		{
//			
//			//红外对管循迹
//			if(HW_1 == 0 && HW_2 == 0 && HW_3 == 0 && HW_4 == 0)
//			{
//				Forward();
//				delay_ms(50);
//			}
//			if(HW_1 == 0 && HW_2 == 1 && HW_3 == 0 && HW_4 == 0)
//			{
//				Rightward();
//				delay_ms(50);
//			}
//						if(HW_1 == 1 && HW_2 == 0 && HW_3 == 0 && HW_4 == 0)
//			{ 
//				Rightward();
//				delay_ms(50);
//			}
//			if(HW_1 == 1 && HW_2 == 1 && HW_3 == 0 && HW_4 == 0)
//			{
//				Rightward();
//				delay_ms(50);
//			}
//			if(HW_1 == 0 && HW_2 == 0 && HW_3 == 1 && HW_4 == 0)
//			{
//				Leftward();
//				delay_ms(50);
//			}
//			if(HW_1 == 0 && HW_2 == 0 && HW_3 == 0 && HW_4 == 1)
//			{
//				Leftward();
//				delay_ms(50);
//			}
//			if(HW_1 == 0 && HW_2 == 0 && HW_3 == 1 && HW_4 == 1)
//			{
//				Leftward();
//				delay_ms(50);
//			}
//		}
//	}
//}}

