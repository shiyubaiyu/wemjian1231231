
#include <reg52.h>
#include <intrins.h>
#include <string.h> 
#include "main.h"
#include "rc522.h"

sfr   RCAP2LH =   0xCA;
sfr   T2LH    =   0xCC;                        

// RC522 SPI�ӿڶ���
sbit     MF522_NSS  =    P2^2;		//����SDA
sbit     MF522_SCK  =    P2^3;
sbit     MF522_SI   =    P3^4;
sbit     MF522_SO   =    P2^4;	  
sbit     MF522_RST  =    P3^5;		
char PcdRequest(unsigned char req_code,unsigned char *pTagType){
   char status;  
   unsigned int  unLen;
   unsigned char ucComMF522Buf[MAXRLEN]; 
   ClearBitMask(Status2Reg,0x08);
   WriteRawRC(BitFramingReg,0x07);
   SetBitMask(TxControlReg,0x03);
   ucComMF522Buf[0] = req_code;
   status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
   if ((status == MI_OK) && (unLen == 0x10))
   {    
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];
   }
   else
   {   status = MI_ERR;   }   
   return status;
}


char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

char PcdReset(void){
    MF522_RST=1;
    _nop_();//�ջ���ָ��
    MF522_RST=0;
    _nop_();
    MF522_RST=1;
     _nop_();
    WriteRawRC(CommandReg,PCD_RESETPHASE); //���ĳ�ȥ��ַ01��д0x0F
	_nop_();
	    

    WriteRawRC(ModeReg,0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363��CRCPreset=01
    WriteRawRC(TReloadRegL,30);          //��ʱ����װֵ����Ϊ����8λ�Ĵ�������ʼֵ��Ϊ0x00      
    WriteRawRC(TReloadRegH,0);
    WriteRawRC(TModeReg,0x8D);           //�ڲ���ʱ��������1000 1101,����λΪTPrescalerReg�ĸ���λ
    WriteRawRC(TPrescalerReg,0x3E);      //ΪTPregScaler��8λ��Ftimer = 6.78MHZ/TPreScaler
    WriteRawRC(TxAutoReg,0x40);     
    return MI_OK;
}

unsigned char ReadRawRC(unsigned char Address){
     unsigned char i, ucAddr;
     unsigned char ucResult=0;

     MF522_SCK = 0;
     MF522_NSS = 0;
     ucAddr = ((Address<<1)&0x7E)|0x80;

     for(i=8;i>0;i--)
     {
         MF522_SI = ((ucAddr&0x80)==0x80);
         MF522_SCK = 1;
         ucAddr <<= 1;
         MF522_SCK = 0;
     }

     for(i=8;i>0;i--)
     {
         MF522_SCK = 1;
         ucResult <<= 1;
         ucResult|=(bit)MF522_SO;
         MF522_SCK = 0;
     }

     MF522_NSS = 1;
     MF522_SCK = 1;
     return ucResult;
} 

void WriteRawRC(unsigned char Address, unsigned char value){  
    unsigned char i, ucAddr;

    MF522_SCK = 0;
    MF522_NSS = 0;
    ucAddr = ((Address<<1)&0x7E);

    for(i=8;i>0;i--)
    {
        MF522_SI = ((ucAddr&0x80)==0x80);
        MF522_SCK = 1;
        ucAddr <<= 1;
        MF522_SCK = 0;
    }

    for(i=8;i>0;i--)
    {
        MF522_SI = ((value&0x80)==0x80);
        MF522_SCK = 1;
        value <<= 1;
        MF522_SCK = 0;
    }
    MF522_NSS = 1;
    MF522_SCK = 1;
}









