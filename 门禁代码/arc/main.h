#ifndef __DCMOTOR_H__
#define __DCMOTOR_H__

/*
**********************************************************************
*                         ͷ�ļ�����
**********************************************************************
*/
#include "common.h"


/*
**********************************************************************
*                         ���غ궨��
**********************************************************************
*/
#define  OSC_FREQ        	(22118400L)
#define  RCAP2_50us      	(65536L - OSC_FREQ/40417L)
#define  RCAP2_1ms       	(65536L - OSC_FREQ/2000L)
#define  RCAP2_10ms      	(65536L - OSC_FREQ/1200L)
#define  TIME0_500us     	(65536L - OSC_FREQ/8000L)
#define  TIME0_10ms      	(65536L - OSC_FREQ/200)
#define  CALL_isr_UART() 	(TI = 1)
#define  TRUE 				1
#define  FALSE 				0
void AT24C02_WriteByte(unsigned char WordAddress,Data);
unsigned char AT24C02_ReadByte(unsigned char WordAddress);
void LCD_Init();
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char);
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String);
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length);
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
unsigned char MatrixKey();





/*
**********************************************************************
*                         �ⲿ����ԭ������
**********************************************************************
*/
void SysInit(void);
//void BuzzerOnce(void);
void delay1ms(unsigned int i);
//void UartSendBytes(u8 *c);
//void UartSendByte(u8 c);
void delay10us(u8 i);

#endif