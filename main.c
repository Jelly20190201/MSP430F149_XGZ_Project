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
#include "GPIO_I2C_SubFunction.c"                     //I2C操作函数，采用模拟方式
#include "ASIC_I2C_Module.c"                  
    
uchar Flag=0x99, Flag1=0x99;                       //标志位，操作是否成功
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
//               MSP430IO口初始化
//***********************************************************************
void Port_Init()
{
  LED8SEL  = 0x00;                      //设置IO口为普通I/O模式，此句可省
  LED8DIR  = 0xFF;                      //设置IO口方向为输出
  LED8PORT = 0xFF;                      //P2口初始设置为FF
  
  DATASEL  = 0x00;                      //设置IO口为普通I/O模式，此句可省
  DATADIR  = 0xFF;                      //设置IO口方向为输出
  DATAPORT = 0xFF;                      //P4口初始设置为FF
  
  KEYSEL = 0x00;
  KEYDIR = 0xFF;                            //控制口方向寄存器
  KEYPORT = 0xFF;
  
  P5SEL = 0x00;
  P5DIR = 0xFF;                            //控制口方向寄存器
  P5OUT = 0xFF;  
  
  CTRSEL  =  0x00;                      //设置IO口为普通I/O模式，此句可省
  CTRDIR |=  BIT3 + BIT4;               //设置IO口方向为输出,控制口在P63,P64
  CTRPORT =  0xFF;                      //P6口初始设置为FF  
  
  P2DIR |= BIT5 + BIT6;                 //设置24C16连接端口为输出状态
}


//*************************************************************************
//               MSP430串口初始化
//*************************************************************************
void UART_Init()
{
  U0CTL|=SWRST + CHAR;                //复位SWRST，8位数据模式
  U0TCTL|=SSEL1;                      //SMCLK为串口时钟
  U0BR1=baud_h;                       //BRCLK=8MHZ,Baud=BRCLK/N
  U0BR0=baud_l;                       //N=UBR+(UxMCTL)/8
  U0MCTL=0x00;                        //微调寄存器为0，波特率9600bps
  ME1|=UTXE0;                         //UART0发送使能
  ME1|=URXE0;                         //UART0接收使能
  U0CTL&=~SWRST;
  IE1|=URXIE0;                        //接收中断使能位
  
  P3SEL|= BIT4 + BIT5;                //设置IO口为第二功能模式，启用UART功能
  P3DIR|= BIT4;                       //设置TXD0口方向为输出
}

//*************************************************************************
//              串口0发送数据函数
//*************************************************************************

void Send_Byte(uchar data)
{
  while(!(IFG1&UTXIFG0));          //发送寄存器空的时候发送数据
    U0TXBUF=data;
}

//*************************************************************************
//               处理来自串口0的接收中断
//*************************************************************************

#pragma vector=UART0RX_VECTOR
__interrupt void UART0_RX_ISR(void)
{

  data=U0RXBUF;                       //接收到的数据存起来
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
    delay_us(6);			        //*延时4uS	
    //DQ_SET;
    //delay_us(1);
    DQ_IN;
    //delay_us(7);			        //*延时10uS	
    if (DQ_R) 
    {
      value|=0x80;		
    }
    delay_us(90);			        //*延时60uS	
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
    delay_us(6);			        //*延时4uS	
    //DQ_SET;
    //delay_us(1);
    OWI2_IN;
    //delay_us(7);			        //*延时10uS	
    if (OWI2_R) 
    {
      value|=0x80;		
    }
    delay_us(90);			        //*延时60uS	
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
    delay_us(8);			  //延时4uS
    if (value & 0x01) 
    {
      DQ_SET;		
    }
    delay_us(90);			  //延时80uS
    DQ_SET;	
    delay_us(10);//位结束
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
    delay_us(8);			  //延时4uS
    if (value & 0x01) 
    {
      OWI2_SET;		
    }
    delay_us(90);			  //延时80uS
    OWI2_SET;	
    delay_us(10);//位结束
    value >>= 1;	
  }
}

unsigned char CMPS1524_Reset(void)                //初始化和复位
{
  unsigned char i;
  DQ_OUT;
  DQ_CLR;
  delay_us(570);				//延时500uS(480-960)
  DQ_SET;
  DQ_IN;
  delay_us(80);				        //延时80uS
  i = DQ_R;
  delay_us(300);				//延时500uS(保持>480uS)
	
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
  delay_us(570);				//延时500uS(480-960)
  OWI2_SET;
  OWI2_IN;
  delay_us(80);				        //延时80uS
  i = OWI2_R;
  delay_us(300);				//延时500uS(保持>480uS)
	
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
//            主程序
//***********************************************************************
void main(void)
{   
  WDT_Init();                         //看门狗初始化
  Clock_Init();                       //时钟初始化
  Port_Init();                        //端口初始化，用于控制IO口输入或输出
  UART_Init();
  Close_LED();                        //LED数码管先关闭
  
  _EINT();                            //开中断

  Flag += 0;                          //此句是为了消除编译警告，无实际意义
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
        P2DIR  = 0xFC;  //OWI1 and OWI2设置成输入，避免干扰I2C
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
        P2DIR  = 0xFC;  //OWI1 and OWI2设置成输入，避免干扰I2C 
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