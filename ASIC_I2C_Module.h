/********************************************************************
//EEPROM操作相关函数声明
//调试环境：EW430 V5.30
//作者：阿迪 www.avrgcc.com
//时间：2011.09.19
********************************************************************/

unsigned char Write_1Byte(unsigned char wdata,unsigned char dataaddress);
unsigned char Write_NByte(unsigned char * outbuf,unsigned char n,unsigned char dataaddress);
unsigned char Read_1Byte_currentaddress(void);
unsigned char Read_NByte_currentaddress(unsigned char * readbuf,unsigned char n);
unsigned char Read_1Byte_Randomaddress(unsigned char dataaddress);
unsigned char Read_NByte_Randomaddress(unsigned char * readbuf,unsigned char n,unsigned char dataaddress);
