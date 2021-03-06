//********************************************************************
//Title:        MSP430F149 I2C Communication
//Purpose:      Create I2C sub function for MSP430F149  
//Created on:   Jun 17th 2020 @16:57:55
//Author:       Jianli Kang    
//Position:     Technical manager
//Location:     CSMC Shanghai Office, DS2          
//********************************************************************

/********************************************************************
函数名称：start
功    能：完成I2C的起始条件操作
参    数：无
返回值  ：无
********************************************************************/
void start(void)
{
	SCL_H;
	SDA_H;
	delay_us(5);
	SDA_L;
	delay_us(5);
	SCL_L;
	delay_us(5);
}
/********************************************************************
函数名称：stop
功    能：完成I2C的终止条件操作
参    数：无
返回值  ：无
********************************************************************/
void stop(void)
{
	SDA_L;
	delay_us(5);
	SCL_H;
	delay_us(5);
	SDA_H;
	delay_us(5);	   
}
/********************************************************************
函数名称：mack
功    能：完成I2C的主机应答操作
参    数：无
返回值  ：无
********************************************************************/
void mack(void)
{
	SDA_OUT;
        SDA_L;
	_NOP(); _NOP();
	SCL_H;
	delay_us(1);
	SCL_L;
	_NOP();_NOP();
	SDA_H;     
	delay_us(1);
}
/********************************************************************
函数名称：mnack
功    能：完成I2C的主机无应答操作
参    数：无
返回值  ：无
********************************************************************/
void mnack(void)
{
	SDA_OUT;
        SDA_H;
	_NOP(); _NOP();
	SCL_H;
	delay_us(5);
	SCL_L;
	_NOP(); _NOP();
	SDA_L;   
	delay_us(5);       
}

/********************************************************************
函数名称：check
功    能：检查从机的应答操作
参    数：无
返回值  ：从机是否有应答：1--有，0--无
********************************************************************/
uchar check(void)
{
	uchar slaveack;
    
	SDA_L;
        //SDA_H;
        
        //P2REN |= BIT6; 
        delay_us(2);
	//_NOP();
        
        SDA_IN;
        
        //_NOP(); _NOP();
       // _NOP(); _NOP();
        delay_us(5);
        
	SCL_H;
	//_NOP(); _NOP();
        //_NOP(); _NOP();
        delay_us(3);

	slaveack = SDA_VAL;                     //读入SDA数值
	SCL_L;
	delay_us(1);
        //SCL_H;
        //add later
        //SDA_L;
        
        SDA_OUT;
        if(slaveack)    return FALSE;
        else            return TRUE;
}

/********************************************************************
函数名称：write1
功    能：向I2C总线发送一个1
参    数：无
返回值  ：无
********************************************************************/
void write1(void)
{
	//SDA_OUT;_NOP();_NOP();_NOP();
        SDA_H;
	delay_us(3);
	SCL_H;
	delay_us(5);
	SCL_L;		
	delay_us(1);
}
/********************************************************************
函数名称：write0
功    能：向I2C总线发送一个0
参    数：无
返回值  ：无
********************************************************************/
void write0(void)
{
	//SDA_OUT;_NOP();_NOP();_NOP();
        SDA_L;
	delay_us(3);
	SCL_H;
	delay_us(5);			 
	SCL_L;		
	delay_us(1);
}
/********************************************************************
函数名称：write1byte
功    能：向I2C总线发送一个字节的数据
参    数：WriteData--发送的数据
返回值  ：无
********************************************************************/
void write1byte(uchar WriteData)
{
	uchar i;
    
	for(i = 8;i > 0;i--)
	{
	  if(WriteData & 0x80)	write1();
	  else 		        write0();
	  WriteData <<= 1;
	}                  
	
	//SDA_H;
	//_NOP();   
}
/********************************************************************
函数名称：writeNbyte
功    能：向I2C总线发送N个字节的数据
参    数：outbuffer--指向发送数据存放首地址
          的指针
          n--数据的个数
返回值  ：发送是否成功的标志：1--成功，0--失败
********************************************************************/
uchar writeNbyte(uchar * outbuffer,uchar n)
{
	uchar i;
    
	for(i = 0;i < n;i++)
	{
		write1byte(* outbuffer);
		if(check())      
		{
		  outbuffer++;			
		}
		else 
		{
		  stop();
		  return FALSE;
		}
	}

	stop();
	return TRUE;			
}
/********************************************************************
函数名称：read1byte
功    能：从I2C总线读取一个字节
参    数：无
返回值  ：读取的数据
********************************************************************/
uchar read1byte(void)
{
	uchar  ReadData = 0x00,i;
        uchar flag;
    
	for(i = 0;i < 8;i++)
	{
	  //SDA_H;
          SDA_L;
	  delay_us(2);
          SDA_IN;
	  delay_us(2);
	  SCL_H;
          delay_us(3);
	  flag = SDA_VAL;
	  ReadData <<= 1;
	  if(flag)	ReadData |= 0x01;
          SCL_L;
          SDA_OUT;
	  
	  delay_us(1);
	}

	return ReadData;
}
/********************************************************************
函数名称：readNbyte
功    能：从I2C总线读取N个字节的数据
参    数：inbuffer--读取后数据存放的首地址
          n--数据的个数
返回值  ：无
********************************************************************/
void readNbyte(uchar * inbuffer,uchar n)
{
	uchar i;
    
	for(i = 0;i < n;i++)
	{
	  inbuffer[i] = read1byte();
	  if(i < (n-1))	mack();
	  else            mnack();
	}
	 
	stop();
}
