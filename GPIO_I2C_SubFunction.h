/********************************************************************
//I2C������غ�������
//���Ի�����EW430 V5.30
//���ߣ����� www.avrgcc.com
//ʱ�䣺2011.09.19
********************************************************************/

void delay(void);
void start(void);
void stop(void);
void mack(void);
void mnack(void);
unsigned char check(void);
void write1(void);
void write0(void);
void write1byte(unsigned char wdata);
unsigned char writeNbyte(unsigned char * outbuffer,unsigned char n);
unsigned char read1byte(void);
void readNbyte(unsigned char * inbuffer,unsigned char n);
