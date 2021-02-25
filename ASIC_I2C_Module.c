//********************************************************************
//Title:        Universal I2C Communication Module
//Purpose:      Create universal I2C communication module for I2C devices 
//Created on:   Jun 17th 2020 @17:04:57
//Author:       Jianli Kang    
//Position:     Technical manager
//Location:     CSMC Shanghai Office, DS2          
//********************************************************************

/********************************************************************
函数名称：Write_1Byte
功    能：向EEPROM中写入1个字节的数据
参    数：Wdata--写入的数据
          Address--数据的写入地址
返回值  ：写入结果：1--成功，0--失败
********************************************************************/
uchar ReadBuffer[64] = {0};
uchar Write_1Byte(uchar SlaveAddr, uchar RegisterAddr, uchar ByteToWrite)
{
	SlaveAddr = (SlaveAddr<<1)&0xFE;
        start();
	write1byte(SlaveAddr);
	if(check()) 
          write1byte(RegisterAddr);
	else 
        {
          SCL_H;
          return 0x11;
        }
	if(check()) 
          write1byte(ByteToWrite);
	else 
          return 0x22;
	if(check())	    
          stop();
	else            
          return 0x33;
	return 0; 
}

/********************************************************************
函数名称：Write_NByte
功    能：向EEPROM中写入N个字节的数据
参    数：outbuf--指向写入数据存放首地址的指针
          n--数据个数，最大不能超过8，由页地址
             决定其最大长度
          Address--数据写入的首地址
返回值  ：写入结果：1--成功，0--失败
********************************************************************/
uchar Write_NByte(uchar * outbuf,uchar n,uchar Address)
{
	uchar  flag; 
    
	start();
	write1byte(EEPROMAddress);             //写入器件地址
	if(check() == 1) 
        write1byte(Address);               //写入数据字地址
	else 
        return 0;
	if(check()) 
        flag=writeNbyte(outbuf,n);
	else 
        return 0;
        delay_ms(10);                         //等待EEPROM完成内部写入
	if(flag)    return 1;
	else        return 0;

}

/********************************************************************
函数名称：Read_1Byte_currentaddress
功    能：从EEPROM的当前地址读取1个字节的数据
参    数：无
返回值  ：读取的数据
********************************************************************/
uchar Read_1Byte_currentaddress(void)
{
	uchar temp;
    
	start();
	write1byte((EEPROMAddress|0x01));
	if(check())	
        temp = read1byte();
	else 
        return 0;
	mnack();
	stop();
	return temp;
}

/********************************************************************
函数名称：Read_NByte_currentaddress
功    能：从EEPROM的当前地址读取N个字节的数据
参    数：readbuf--指向保存数据地址的指针
          n--读取数据的个数
返回值  ：读取结果：1--成功，0--失败
********************************************************************/
uchar Read_NByte_currentaddress(uchar * readbuf,uchar n)
{   
        start();
	write1byte((EEPROMAddress|0x01));
	if(check()) 
        readNbyte(readbuf,n);
	else 
        return 0;
    
	return  1;
}

/********************************************************************
函数名称：Read_1Byte_Randomaddress
功    能：从EEPROM的指定地址读取1个字节的数据
参    数：Address--数据读取的地址
返回值  ：读取的数据
********************************************************************/
uchar Read_1Byte(uchar SlaveAddr, uchar RegisterAddr)
{
	uchar temp;
        SlaveAddr = (SlaveAddr<<1)&0xFE;
	start();
	write1byte(SlaveAddr);
	if(check())	
          write1byte(RegisterAddr);
	else 
          return 0x44;
	if(check()) 
	  {
	    SlaveAddr = SlaveAddr|0x01;
            start();
	    write1byte(SlaveAddr);
	  }
	else    
          return 0x55;
	if(check())	
          temp = read1byte();
	else 
          return 0x66;

	mnack();
	stop();
	return temp;
}

/********************************************************************
函数名称：Read_NByte_Randomaddress
功    能：从EEPROM的指定地址读取N个字节的数据
参    数：readbuf--指向保存数据地址的指针
          n--读取数据的个数
          Address--数据读取的首地址
返回值  ：读取结果：1--成功，0--失败
********************************************************************/
uchar Read_NByte(uchar * readbuf,uchar SlaveAddr, uchar RegisterAddr, uchar n)
{
	int j=0;
        start();
        SlaveAddr = (SlaveAddr<<1)&0xFE;
	write1byte(SlaveAddr);
	if(check())	
            write1byte(RegisterAddr);
        
	else 
        {  
           for(j=0;j<n;j++)
            {
                ReadBuffer[j]=0xFF;        
            } 
           SCL_H;
           return 0x44;
        }
	if(check()) 
	{
	  SlaveAddr = SlaveAddr|0x01;
          start();
	  write1byte(SlaveAddr);
	}
	else 
        return 0x55;
	if(check()) 
        readNbyte(readbuf,n);
	else 
        return 0x66;
	
	return 0;	
}
