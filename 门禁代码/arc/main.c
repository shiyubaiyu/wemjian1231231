
#define AT24C02_ADDRESS		0xA0
#include <reg52.h>
#include "main.h"
#include "rc522.h"
#include<string.h>
sbit PWM=P3^6;	                                                                            
#define  PWMmax 	22	 //������Ƕ�
#define  PWMmin 	 7	 //�����С�Ƕ�
sbit P1_0 = 0x90;
sbit P1_1 = 0x91;
sbit P1_2 = 0x92;
sbit P1_3 = 0x93;
sbit P1_4 = 0x94;
sbit P1_5 = 0x95;
sbit P1_6 = 0x96;
sbit P1_7 = 0x97;
sbit P3_0 = 0xB0;
sbit P3_1 = 0xB1;
sbit P3_2 = 0xB2;
sbit P3_3 = 0xB3;
sbit P3_4 = 0xB4;
sbit P3_5 = 0xB5;
sbit P3_6 = 0xB6;
sbit P3_7 = 0xB7;
 unsigned char KeyNum;
unsigned int Password,cishu;
unsigned int Num;
unsigned int fangshi;
unsigned int t;
unsigned int z=60;
sbit SG_PWM=P3^6;	
unsigned int p=0;					     //�ƴ�  ����ֵΪ0
unsigned char PWM_p=0;     
	
u8 UID[5],Temp[4];
u16 count = PWMmin, time=0;  //count���ƶ���Ƕȣ�7����С�Ƕȣ�22�����Ƕ�
u8 UI0[5]={0xAE,0x45,0xD6,0x1D};	   //	�˴�Ϊ���ſ�����Ϣ,UI0��UI1    //����
u8 UI1[5]={0x50,0xFD,0xDF,0x12};  //�׿���Ϣ

void Rc522Test1(void);

 void Delay(unsigned int xms)
{
	unsigned char i, j;
	while(xms--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
	}
}

void init0()
{
	TMOD = 0x21; 	// ��ʽһ
	TH0 = 0xFF;
	TL0 = 0xA4;
	TR0 = 1;       // ������ʱ��0
	EA = 1;		   // �������ж�
	ET0 = 1;       // ��ʱ��0�����ж�
	TMOD = 0x21;		// T1����Ϊ8λ�Զ���װ�ض�ʱ��			
    SCON = 0x40;		// ���ڹ�����ģʽ1��8λUART�����ʿɱ䣬�ҽ�ֹ����
    TH1 = 0xE8;			// ��Ƭ��С����V1.3�����2400�������Ҳ����ʣ�����Ϊ11.0592MHz������ĳ�0xE8,12M�ĳ�0xE8;
    TL1 = TH1;			// �ӱ�ʱ�Ķ�ʱ������ֵ��
    PCON = 0x80;		// ����Ϊ�����ʼӱ�
// �����ж�
	ES = 1;		    // �������ж�
	TR1 = 1;		    // ��ʱ��1������
	EX0=1;//�����ⲿ�ж�0
	IT0=1;				//�ⲿ�ж�0Ϊ���ش�����ʽ
	EX1=1;//�����ⲿ�ж�1
	IT1=1;//�ⲿ�ж�1Ϊ���ش�����ʽ
	PX1=1;//�ⲿ�ж�һλ=Ϊ������
    PcdReset();				//��λRC522
}
void int0_isr(void) interrupt 0 using 0
{   
	fangshi=0;    //���뷽ʽ0��д����ģʽ

}
void int1_isr(void) interrupt 2 using 1//�жϳ���ͨ��WiFiģ�����
{
fangshi=2;	
}

void T0_inter() interrupt 1
{
		static unsigned int T0Count;
	TH0 = 0XFF; 
	TL0 = 0XA4;	//���¸���ʱ����ֵ
 
			 
	if(fangshi==0||fangshi==3) 
     	{ 	T0Count++;	
		if(T0Count>=8000)
	{
		T0Count=0;
	    z--;	
		   if(z==0)
		   { P3_7=1;//��������
		   	t=3; //��������
				 P3_1=0; //�̵���
			
       
			 
		   }
     	}	
	  }
			time ++ ; //������ÿ��ת����ʱ�侫��
	if (time < count)	
	{
		PWM = 1;
	}
	else
	{
		PWM = 0;
	}
	if (time >= 200)
	{
		time = 0;
	}//ʹ�����time0-x��x<14��ʱ��ת����ʱ��x-200
}

void main(void)
{ 		 	LCD_Init();		//LCD��������ʼ��
		Num=AT24C02_ReadByte(0);//ȡAT24C02�Ĵ������ݵĵͰ�λ
		Num|=AT24C02_ReadByte(1)<<8;//ȡAT24C02�Ĵ������ݵĸ߰�λ
		t=3; //��������Ϊ3
		fangshi=0; //���뷽ʽ4������״̬
		while(1)
	{ 
		 init0(); 
	Rc522Test1();
    	
		while(fangshi==0)
		{ 
    
		KeyNum=MatrixKey();
		 LCD_ShowNum(2,6,t,1);
		 LCD_ShowString(1,1,"Password:"); 
		  Rc522Test1();
		if(KeyNum)
		{P3_7=0;
		Delay(25);
		P3_7=1;
			if(KeyNum<=10)	
			{
				if(cishu<4)	
				{
					Password*=10;			
					Password+=KeyNum%10;	
					cishu++;
				}
				LCD_ShowNum(2,1,Password,4);
				Rc522Test1();
			}
			if(KeyNum==11)
			{
				if(Password==Num)
				{fangshi=2;	
				}
				else			
				{
					LCD_ShowString(1,14,"ERR");	
					Password=0;	
					cishu=0;	
					LCD_ShowNum(2,1,Password,4);
					t--;
					LCD_ShowNum(2,6,t,1);
					if(t==0)
					{	 
		      	
					fangshi=3;
					}
				}
			}
			if(KeyNum==12)	
			{
				Password=0;		
			cishu=0;	
				LCD_ShowNum(2,1,Password,4);
			}
		}
	}
  while(fangshi==1)
	{  	
		KeyNum=MatrixKey();
		LCD_ShowString(1,1,"Write Password:");
		if(KeyNum)
		{	 P3_7=0;
		Delay(25);
		P3_7=1;
			if(KeyNum<=10)
			{
				if(cishu<4)	
				{
					Password*=10;				
					Password+=KeyNum%10;		
					cishu++;
				}
				LCD_ShowNum(2,1,Password,4);	
			}
			if(KeyNum==11)	
			{   
			 LCD_Init();
		  Num=Password;
				AT24C02_WriteByte(0,Num%256);
			Delay(5);
			AT24C02_WriteByte(1,Num/256);
			Delay(5);
					Password=0;		
					cishu=0;		
				  fangshi=0;
				  LCD_ShowString(1,14,"ok");
				  Delay(2000);
				   LCD_Init();	

			}
			if(KeyNum==12)	//如果S12按键按下，取消
			{	
				Password=0;		//密码清零
				cishu=0;		//计次清零
				LCD_ShowNum(2,1,Password,4);	//更新显示
			}
			
		}
	}
	while(fangshi==2)
	{
		P3_0=1;P3_7=1;
		Delay(100);
		P3_7=1;	P3_7=0;
		Delay(100);
		P3_7=1;P3_7=0;
		Delay(100);
		P3_7=1;	

		Rc522Test1();
	                LCD_Init();
					LCD_ShowString(1,14,"OK ");
					Password=0;	
				
					 count = 14;						  //���ת��
					delay1ms(1500);			
					count=PWMmin;						   //���ת��ȥ
					delay1ms(1000);
				
					 LCD_Init();
                 P3_0=0;   	
	                 t=3;
		   			  
					    fangshi=0;
	
	
	}

      while(fangshi==3)
	{	P3_7=0;
		P3_1=1;
		Rc522Test1();
		LCD_ShowNum(2,10,z,2);
    	LCD_ShowNum(2,6,t,1);
	}
while(fangshi==4)	
	
   {
		 t=3;
			z=60;  
     PCON=0X01;
     

   }
	}
}


void delay1ms(unsigned int i)  
{
    unsigned char a,b,c;
	for(;i>0;i--)
 	   for(c=1;c>0;c--)
    	    for(b=142;b>0;b--)
        	    for(a=2;a>0;a--);
}

void delay10us(unsigned char i)   //��� 0us,��������ʲô���־��Ƕ��ٸ�ʮ΢��
{
    unsigned char a,b;
	for(;i>0;i--)
    	for(b=1;b>0;b--)
        	for(a=2;a>0;a--);
}

void Rc522Test1(void)
{	if (PcdRequest(0x52, Temp) == MI_OK)
	   {
	   		if (PcdAnticoll(UID) == MI_OK)
			{
		
				if(strcmp(UID,UI0)==0)
				{
	 				fangshi=2;
				}
			if(strcmp(UID,UI1)==0)
			{
			  P3_3=0;
		Delay(100);
		P3_3=1;P3_3=0;
		Delay(100);
		P3_3=1;
			 fangshi=1;
			}
			}
	   }
   	 }


 void SetBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x00;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
}

char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn   = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
    switch (Command)
    {
       case PCD_AUTHENT:
          irqEn   = 0x12;
          waitFor = 0x10;
          break;
       case PCD_TRANSCEIVE:
          irqEn   = 0x77;
          waitFor = 0x30;
          break;
       default:
         break;
    }
   
    WriteRawRC(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pInData[i]);    }
    WriteRawRC(CommandReg, Command);
   
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }
    
//    i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
 i = 2000;//900,1100,1700,2000,2700
    do 
    {
         n = ReadRawRC(ComIrqReg);
         i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);
	      
    if (i!=0)
    {    
         if(!(ReadRawRC(ErrorReg)&0x1B))
         {
             status = MI_OK;
             if (n & irqEn & 0x01)
             {   status = MI_NOTAGERR;   }
             if (Command == PCD_TRANSCEIVE)
             {
               	n = ReadRawRC(FIFOLevelReg);
              	lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOutData[i] = ReadRawRC(FIFODataReg);    }
				
            }
         }
         else
         {   status = MI_ERR;   }
   }
   

   SetBitMask(ControlReg,0x80);           // stop timer now
   WriteRawRC(CommandReg,PCD_IDLE); 
   return status;
}
 sbit I2C_SCL=P2^1;
sbit I2C_SDA=P2^0;

void I2C_Start(void)
{
	I2C_SDA=1;
	I2C_SCL=1;
	I2C_SDA=0;
	I2C_SCL=0;
}


void I2C_Stop(void)
{
	I2C_SDA=0;
	I2C_SCL=1;
	I2C_SDA=1;
}

void I2C_SendByte(unsigned char Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		I2C_SDA=Byte&(0x80>>i);
		I2C_SCL=1;
		I2C_SCL=0;
	}
}


unsigned char I2C_ReceiveByte(void)
{
	unsigned char i,Byte=0x00;
	I2C_SDA=1;
	for(i=0;i<8;i++)
	{
		I2C_SCL=1;
		if(I2C_SDA){Byte|=(0x80>>i);}
		I2C_SCL=0;
	}
	return Byte;
}

void I2C_SendAck(unsigned char AckBit)
{
	I2C_SDA=AckBit;
	I2C_SCL=1;
	I2C_SCL=0;
}

unsigned char I2C_ReceiveAck(void)
{
	unsigned char AckBit;
	I2C_SDA=1;
	I2C_SCL=1;
	AckBit=I2C_SDA;
	I2C_SCL=0;
	return AckBit;
}
//i2c
sbit LCD_RS=P2^6;
sbit LCD_RW=P2^5;
sbit LCD_EN=P2^7;
#define LCD_DataPort P0
void LCD_Delay()
{
	unsigned char i, j;

	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
}

void LCD_WriteCommand(unsigned char Command)
{
	LCD_RS=0;
	LCD_RW=0;
	LCD_DataPort=Command;
	LCD_EN=1;
	LCD_Delay();
	LCD_EN=0;
	LCD_Delay();
}

void LCD_WriteData(unsigned char Data)
{
	LCD_RS=1;
	LCD_RW=0;
	LCD_DataPort=Data;
	LCD_EN=1;
	LCD_Delay();
	LCD_EN=0;
	LCD_Delay();
}


void LCD_SetCursor(unsigned char Line,unsigned char Column)
{
	if(Line==1)
	{
		LCD_WriteCommand(0x80|(Column-1));
	}
	else if(Line==2)
	{
		LCD_WriteCommand(0x80|(Column-1+0x40));
	}
}

void LCD_Init()
{
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x0c);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x01);
}

void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char)
{
	LCD_SetCursor(Line,Column);
	LCD_WriteData(Char);
}


void LCD_ShowString(unsigned char Line,unsigned char Column,char *String)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=0;String[i]!='\0';i++)
	{
		LCD_WriteData(String[i]);
	}
}

int LCD_Pow(int X,int Y)
{
	unsigned char i;
	int Result=1;
	for(i=0;i<Y;i++)
	{
		Result*=X;
	}
	return Result;
}


void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(10,i-1)%10+'0');
	}
}

void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length)
{
	unsigned char i;
	unsigned int Number1;
	LCD_SetCursor(Line,Column);
	if(Number>=0)
	{
		LCD_WriteData('+');
		Number1=Number;
	}
	else
	{
		LCD_WriteData('-');
		Number1=-Number;
	}
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number1/LCD_Pow(10,i-1)%10+'0');
	}
}

void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i,SingleNumber;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		SingleNumber=Number/LCD_Pow(16,i-1)%16;
		if(SingleNumber<10)
		{
			LCD_WriteData(SingleNumber+'0');
		}
		else
		{
			LCD_WriteData(SingleNumber-10+'A');
		}
	}
}

void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(2,i-1)%2+'0');
	}
}
unsigned char MatrixKey()
{
	unsigned char KeyNumber=0;
	
	P1=0xFF;
	P1_3=0;
	if(P1_7==0){Delay(20);while(P1_7==0);Delay(20);KeyNumber=1;}
	if(P1_6==0){Delay(20);while(P1_6==0);Delay(20);KeyNumber=5;}
	if(P1_5==0){Delay(20);while(P1_5==0);Delay(20);KeyNumber=9;}
	if(P1_4==0){Delay(20);while(P1_4==0);Delay(20);KeyNumber=13;}
	
	P1=0xFF;
	P1_2=0;
	if(P1_7==0){Delay(20);while(P1_7==0);Delay(20);KeyNumber=2;}
	if(P1_6==0){Delay(20);while(P1_6==0);Delay(20);KeyNumber=6;}
	if(P1_5==0){Delay(20);while(P1_5==0);Delay(20);KeyNumber=10;}
	if(P1_4==0){Delay(20);while(P1_4==0);Delay(20);KeyNumber=14;}
	
	P1=0xFF;
	P1_1=0;
	if(P1_7==0){Delay(20);while(P1_7==0);Delay(20);KeyNumber=3;}
	if(P1_6==0){Delay(20);while(P1_6==0);Delay(20);KeyNumber=7;}
	if(P1_5==0){Delay(20);while(P1_5==0);Delay(20);KeyNumber=11;}
	if(P1_4==0){Delay(20);while(P1_4==0);Delay(20);KeyNumber=15;}
	
	P1=0xFF;
	P1_0=0;
	if(P1_7==0){Delay(20);while(P1_7==0);Delay(20);KeyNumber=4;}
	if(P1_6==0){Delay(20);while(P1_6==0);Delay(20);KeyNumber=8;}
	if(P1_5==0){Delay(20);while(P1_5==0);Delay(20);KeyNumber=12;}
	if(P1_4==0){Delay(20);while(P1_4==0);Delay(20);KeyNumber=16;}
	
	return KeyNumber;
}


void AT24C02_WriteByte(unsigned char WordAddress,Data)
{
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS);
	I2C_ReceiveAck();
	I2C_SendByte(WordAddress);
	I2C_ReceiveAck();
	I2C_SendByte(Data);
	I2C_ReceiveAck();
	I2C_Stop();
}

unsigned char AT24C02_ReadByte(unsigned char WordAddress)
{
	unsigned char Data;
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS);
	I2C_ReceiveAck();
	I2C_SendByte(WordAddress);
	I2C_ReceiveAck();
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS|0x01);
	I2C_ReceiveAck();
	Data=I2C_ReceiveByte();
	I2C_SendAck(1);
	I2C_Stop();
	return Data;
}

