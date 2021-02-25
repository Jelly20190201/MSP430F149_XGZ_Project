//********************************************************************
//Title:        MSP430F149 NSA2300 Communication
//Purpose:      Implement MSP430F149 to communication with NSA2300 through I2C and OWI 
//Created on:   Jun 17th 2020 @16:45:50
//Author:       Jianli Kang    
//Position:     Technical manager
//Location:     CSMC Shanghai Office, DS2          
//********************************************************************

#include <msp430x14x.h>
#include "Config.h"                  //from supplier demo borad header file
#include "GPIO_I2C_SubFunction.c"                     //I2C��������������ģ�ⷽʽ
#include "ASIC_I2C_Module.c"                  
    
uchar Flag=0x99, Flag1=0x99;                       //��־λ�������Ƿ�ɹ�
static uchar receive_flag = 0x00;
int i=0;
int count=0;
uchar msg[4]={0};
uchar data=0;
uchar readByte=0x00;
//uchar ReadBuffer[64] = {0};
uchar OWIBuffer[64] = {0xFF};

#define SlaveAddress 0x50

//***********************************************************************
//               MSP430IO�ڳ�ʼ��
//***********************************************************************
void Port_Init()
{
  LED8SEL  = 0x00;                      //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
  LED8DIR  = 0xFF;                      //����IO�ڷ���Ϊ���
  LED8PORT = 0xFF;                      //P2�ڳ�ʼ����ΪFF
  
  DATASEL  = 0x00;                      //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
  DATADIR  = 0xFF;                      //����IO�ڷ���Ϊ���
  DATAPORT = 0xFF;                      //P4�ڳ�ʼ����ΪFF
  
  KEYSEL = 0x00;
  KEYDIR = 0xFF;                            //���ƿڷ���Ĵ���
  KEYPORT = 0xFF;
  
  P5SEL = 0x00;
  P5DIR = 0xFF;                            //���ƿڷ���Ĵ���
  P5OUT = 0xFF;  
  
  CTRSEL  =  0x00;                      //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
  CTRDIR |=  BIT3 + BIT4;               //����IO�ڷ���Ϊ���,���ƿ���P63,P64
  CTRPORT =  0xFF;                      //P6�ڳ�ʼ����ΪFF  
  
  P2DIR |= BIT5 + BIT6;                 //����24C16���Ӷ˿�Ϊ���״̬
}


//*************************************************************************
//               MSP430���ڳ�ʼ��
//*************************************************************************
void UART_Init()
{
  U0CTL|=SWRST + CHAR;                //��λSWRST��8λ����ģʽ
  U0TCTL|=SSEL1;                      //SMCLKΪ����ʱ��
  U0BR1=baud_h;                       //BRCLK=8MHZ,Baud=BRCLK/N
  U0BR0=baud_l;                       //N=UBR+(UxMCTL)/8
  U0MCTL=0x00;                        //΢���Ĵ���Ϊ0��������9600bps
  ME1|=UTXE0;                         //UART0����ʹ��
  ME1|=URXE0;                         //UART0����ʹ��
  U0CTL&=~SWRST;
  IE1|=URXIE0;                        //�����ж�ʹ��λ
  
  P3SEL|= BIT4 + BIT5;                //����IO��Ϊ�ڶ�����ģʽ������UART����
  P3DIR|= BIT4;                       //����TXD0�ڷ���Ϊ���
}

//*************************************************************************
//              ����0�������ݺ���
//*************************************************************************

void Send_Byte(uchar data)
{
  while(!(IFG1&UTXIFG0));          //���ͼĴ����յ�ʱ��������
    U0TXBUF=data;
}

//*************************************************************************
//               �������Դ���0�Ľ����ж�
//*************************************************************************

#pragma vector=UART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{

  data=U0RXBUF;                       //���յ������ݴ�����
  receive_flag=0x11;
}

unsigned char CMPS1524_read_byte(void) 
{
  unsigned char i;
  unsigned char value = 0;
  for (i = 8; i != 0; i--) 
  {
    value >>= 1;
    DQ_OUT;
    DQ_CLR;
    delay_us(6);			        //*��ʱ4uS	
    //DQ_SET;
    //delay_us(1);
    DQ_IN;
    //delay_us(7);			        //*��ʱ10uS	
    if (DQ_R) 
    {
      value|=0x80;		
    }
    delay_us(90);			        //*��ʱ60uS	
  }
  return(value);
}

unsigned char OWI2_read_byte(void) 
{
  unsigned char i;
  unsigned char value = 0;
  for (i = 8; i != 0; i--) 
  {
    value >>= 1;
    OWI2_OUT;
    OWI2_CLR;
    delay_us(6);			        //*��ʱ4uS	
    //DQ_SET;
    //delay_us(1);
    OWI2_IN;
    //delay_us(7);			        //*��ʱ10uS	
    if (OWI2_R) 
    {
      value|=0x80;		
    }
    delay_us(90);			        //*��ʱ60uS	
  }
  return(value);
}


void CMPS1524_write_byte(unsigned char value) 
{
  unsigned char i;
  for (i = 8; i != 0; i--) 
  {
    DQ_OUT;
    DQ_CLR;
    delay_us(8);			  //��ʱ4uS
    if (value & 0x01) 
    {
      DQ_SET;		
    }
    delay_us(90);			  //��ʱ80uS
    DQ_SET;	
    delay_us(10);//λ����
    value >>= 1;	
  }
}

void OWI2_write_byte(unsigned char value) 
{
  unsigned char i;
  for (i = 8; i != 0; i--) 
  {
    OWI2_OUT;
    OWI2_CLR;
    delay_us(8);			  //��ʱ4uS
    if (value & 0x01) 
    {
      OWI2_SET;		
    }
    delay_us(90);			  //��ʱ80uS
    OWI2_SET;	
    delay_us(10);//λ����
    value >>= 1;	
  }
}

unsigned char CMPS1524_Reset(void)                //��ʼ���͸�λ
{
  unsigned char i;
  DQ_OUT;
  DQ_CLR;
  delay_us(570);				//��ʱ500uS(480-960)
  DQ_SET;
  DQ_IN;
  delay_us(80);				        //��ʱ80uS
  i = DQ_R;
  delay_us(300);				//��ʱ500uS(����>480uS)
	
  if (i) 
  {
    return 0x00;
  }
  else 
  {
    return 0x01;
  }
}

unsigned char OWI2_Reset(void)
{
  unsigned char i;
  OWI2_OUT;
  OWI2_CLR;
  delay_us(570);				//��ʱ500uS(480-960)
  OWI2_SET;
  OWI2_IN;
  delay_us(80);				        //��ʱ80uS
  i = OWI2_R;
  delay_us(300);				//��ʱ500uS(����>480uS)
	
  if (i) 
  {
    return 0x00;
  }
  else 
  {
    return 0x01;
  }
}


unsigned int CMPS1524_read_1byte(uchar * readbuf, uchar RegisterAddr, uchar n ) 
{
  unsigned int i;

  CMPS1524_write_byte(0xCC);	          
  RegisterAddr = RegisterAddr | 0x80;
  CMPS1524_write_byte(RegisterAddr);	          
  for (i = 0; i < n; i++) 
  {
    OWIBuffer[i] = CMPS1524_read_byte();	
  }

  return 0;
}

unsigned int OWI2_read_1byte(uchar * readbuf, uchar RegisterAddr, uchar n ) 
{
  unsigned int i;

  OWI2_write_byte(0xCC);	          
  RegisterAddr = RegisterAddr | 0x80;
  OWI2_write_byte(RegisterAddr);	          
  for (i = 0; i < n; i++) 
  {
    OWIBuffer[i] = OWI2_read_byte();	
  }

  return 0;
}

unsigned int CMPS1524_write_1byte(uchar RegisterAddr, uchar ByteToWrite) 
{

  CMPS1524_write_byte(0xCC);	          
  CMPS1524_write_byte(RegisterAddr);	          
  CMPS1524_write_byte(ByteToWrite);
  return 0;
}

unsigned int OWI2_write_1byte(uchar RegisterAddr, uchar ByteToWrite) 
{

  OWI2_write_byte(0xCC);	          
  OWI2_write_byte(RegisterAddr);	          
  OWI2_write_byte(ByteToWrite);
  return 0;
}



//***********************************************************************
//            ������
//***********************************************************************
void main(void)
{   
  WDT_Init();                         //���Ź���ʼ��
  Clock_Init();                       //ʱ�ӳ�ʼ��
  Port_Init();                        //�˿ڳ�ʼ�������ڿ���IO����������
  UART_Init();
  Close_LED();                        //LED������ȹر�
  
  _EINT();                            //���ж�

  Flag += 0;                          //�˾���Ϊ���������뾯�棬��ʵ������
  while(1)
  {
      if(receive_flag==0x11)
      {
        receive_flag=0x00;
        for(i=0;i<3;i++)
        {
          msg[i]=msg[i+1];           
        }
        msg[3]=data;
       // Send_Byte(data);
        count++;
      /*  if(count==4)
        {
         //count=0;
          Send_Byte(msg[0]); 
         Send_Byte(msg[1]); 
         Send_Byte(msg[2]); 
         Send_Byte(msg[3]); 
          
        }*/
      }
    
      if(count==0x04 && msg[0]==0x20)
      {
        count=0;
        //Send_Byte(0x20); 
        P2DIR  = 0xFC;  //OWI1 and OWI2���ó����룬�������I2C
        P4DIR  = 0xFF;
        delay_ms(1);
        Flag = Write_1Byte(msg[1], msg[2], msg[3]);
        Send_Byte(Flag);
        //P2DIR  = 0xFF;
      }
      
      if(count==0x04 && msg[0]==0x21)
      {
        count=0;
        //Send_Byte(0x21);
        P2DIR  = 0xFC;  //OWI1 and OWI2���ó����룬�������I2C 
        P4DIR  = 0xFF;
        //P2OUT &= ~BIT6;   
        //delay_ms(1);
        Flag1=Read_NByte(ReadBuffer, msg[1], msg[2], msg[3]);
        //Send_Byte(Flag1);
        for(i=0;i<msg[3];i++)
        {
          Send_Byte(ReadBuffer[i]);        
        }
        //P2DIR  = 0xFF;
      }
        
      if(count==0x04 && msg[0]==0x31)
      {
       count=0;
       P2DIR  = 0xFF;
       P2OUT  = 0xFD;
       delay_ms(1);
       Flag1=CMPS1524_Reset();
       CMPS1524_read_1byte(OWIBuffer, msg[2], msg[3]);
       //Send_Byte(Flag1); 
       for(i=0;i<msg[3];i++)
        {
          Send_Byte(OWIBuffer[i]);        
        }
      }
      
      if(count==0x04 && msg[0]==0x30)
      {
       count=0;
       P2DIR  = 0xFF;
       P2OUT  = 0xFD;
       delay_ms(1);
       Flag1=CMPS1524_Reset();
       CMPS1524_write_1byte(msg[2], msg[3]);
       Send_Byte(Flag1); 
      }
      
      if(count==0x04 && msg[0]==0x41)
      {
       count=0;
       P2DIR  = 0xFF;
       P2OUT  = 0xFE;
       delay_ms(1);
       Flag1=OWI2_Reset();
       OWI2_read_1byte(OWIBuffer, msg[2], msg[3]);
       //Send_Byte(Flag1); 
       for(i=0;i<msg[3];i++)
        {
          Send_Byte(OWIBuffer[i]);        
        }
      }
      
      
      if(count==0x04 && msg[0]==0x40)
      {
       count=0;
       P2DIR  = 0xFF;
       P2OUT  = 0xFE;
       delay_ms(1);
       Flag1=OWI2_Reset();
       OWI2_write_1byte(msg[2], msg[3]);
       Send_Byte(Flag1); 
      }
       
      if(count==0x04 && msg[0]==0x50)
      {
        count=0;
        P2DIR  = 0xFF;
        P2OUT  = 0xFC;
      }
      
      
      
 /*           
      if(count==0x04 && msg[0]==0x50)
      {
        count=0;
        //Send_Byte(0x20); 
        P2DIR  = 0xFF;
        P4DIR  = 0xFF;
        delay_ms(1);
        Flag = Write_1Byte(0x23,0x01 , 0x09);
        Send_Byte(Flag); 
        P2OUT  &= ~BIT3;
      }
*/
//MCU GPIO0      
      if(count==0x04 && msg[0]==0x51)
      {
        count=0;
        //Send_Byte(0x20); 
        P2DIR  = 0xFF;
        P4DIR  = 0xFF;
        
        delay_ms(1);
        P2OUT  |= BIT3;
        delay_ms(1);
        Flag = Write_1Byte(0x23,0x01 , 0x0B);
        Send_Byte(Flag); 
       
      }
      
      if(count==0x04 && msg[0]==0x60)
      {
        count=0;
        Send_Byte(0xCC); 
       
      }
           
  }
  
}