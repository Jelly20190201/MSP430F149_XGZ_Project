//********************************************************************
//Title:        Universal I2C Communication Module
//Purpose:      Create universal I2C communication module for I2C devices 
//Created on:   Jun 17th 2020 @17:04:57
//Author:       Jianli Kang    
//Position:     Technical manager
//Location:     CSMC Shanghai Office, DS2          
//********************************************************************

/********************************************************************
�������ƣ�Write_1Byte
��    �ܣ���EEPROM��д��1���ֽڵ�����
��    ����Wdata--д�������
          Address--���ݵ�д���ַ
����ֵ  ��д������1--�ɹ���0--ʧ��
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
�������ƣ�Write_NByte
��    �ܣ���EEPROM��д��N���ֽڵ�����
��    ����outbuf--ָ��д�����ݴ���׵�ַ��ָ��
          n--���ݸ���������ܳ���8����ҳ��ַ
             ��������󳤶�
          Address--����д����׵�ַ
����ֵ  ��д������1--�ɹ���0--ʧ��
********************************************************************/
uchar Write_NByte(uchar * outbuf,uchar n,uchar Address)
{
	uchar  flag; 
    
	start();
	write1byte(EEPROMAddress);             //д��������ַ
	if(check() == 1) 
        write1byte(Address);               //д�������ֵ�ַ
	else 
        return 0;
	if(check()) 
        flag=writeNbyte(outbuf,n);
	else 
        return 0;
        delay_ms(10);                         //�ȴ�EEPROM����ڲ�д��
	if(flag)    return 1;
	else        return 0;

}

/********************************************************************
�������ƣ�Read_1Byte_currentaddress
��    �ܣ���EEPROM�ĵ�ǰ��ַ��ȡ1���ֽڵ�����
��    ������
����ֵ  ����ȡ������
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
�������ƣ�Read_NByte_currentaddress
��    �ܣ���EEPROM�ĵ�ǰ��ַ��ȡN���ֽڵ�����
��    ����readbuf--ָ�򱣴����ݵ�ַ��ָ��
          n--��ȡ���ݵĸ���
����ֵ  ����ȡ�����1--�ɹ���0--ʧ��
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
�������ƣ�Read_1Byte_Randomaddress
��    �ܣ���EEPROM��ָ����ַ��ȡ1���ֽڵ�����
��    ����Address--���ݶ�ȡ�ĵ�ַ
����ֵ  ����ȡ������
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
�������ƣ�Read_NByte_Randomaddress
��    �ܣ���EEPROM��ָ����ַ��ȡN���ֽڵ�����
��    ����readbuf--ָ�򱣴����ݵ�ַ��ָ��
          n--��ȡ���ݵĸ���
          Address--���ݶ�ȡ���׵�ַ
����ֵ  ����ȡ�����1--�ɹ���0--ʧ��
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
