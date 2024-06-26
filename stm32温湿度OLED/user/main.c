#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "OLED.h"
#include "LED.h"
#include "hc.h"
#include "Servo.h"
#include "mlx90614.h"
#include "dht11.h"
#include "HEADER.h"
uint32_t length;
float leng;

u8 temp,humi;
int main(void)
{
	
	 uint32_t bufe[5];
	OLED_Init();
  Hcsr04Init();
// OLED_ShowString(1,1,"length:");
// OLED_ShowString(2,9,"cm");
	OLED_ShowString(3,1,"wendu:");
//	OLED_ShowString(4,1,"shi:");
	 
 DHT11_Init();
while(1)
	{
		
		leng =Hcsr04GetLength();
		DHT11_Read_Data(&temp,&humi);
	    bufe[0]=temp;
		
		bufe[1]=humi;
		bufe[2]=45;
		length = leng*100;
		OLED_ShowNum(2,4,length/1000,1);//bai
		OLED_ShowNum(2,5,length%1000/100,1);//shi
		OLED_ShowString(2,6,":");
		OLED_ShowNum(2,7,length%100/10,1);//
		OLED_ShowNum(2,8,length%10/1,1);
		
		OLED_ShowNum(3,7,bufe[0],2);
		OLED_ShowString(4,9,"%");
		OLED_ShowNum(4,7,bufe[1],2);
		OLED_ShowString(4,9,"%");
		if(bufe[0]< 50)
		{
			Header_ON();
			OLED_ShowString(3,10,"++");	
		}
		else 
		{
			Header_OFF();
			OLED_ShowString(3,10,"  ");
		}
  }
}
