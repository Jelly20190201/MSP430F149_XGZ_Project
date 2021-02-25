#define CPU_F ((double)8000000) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

/*��BRCLK=CPU_Fʱ������Ĺ�ʽ���Լ��㣬����Ҫ�������ü����Ƶϵ��*/
#define baud           9600                                //���ò����ʵĴ�С
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //�����ʼ��㹫ʽ
#define baud_h         (uchar)(baud_setting>>8)            //��ȡ��λ
#define baud_l         (uchar)(baud_setting)               //��λ

/*8��LED�ƽӿ�*/
#define LED8PORT        P2OUT                   //P2��8��LED��
#define LED8SEL         P2SEL                   //P2��8��LED��
#define LED8DIR         P2DIR                   //P2��8��LED��

/*����ܹܽŶ���*/
#define DATAPORT	P4OUT                   //���ݿ����ڶ˿�P4
#define DATASEL         P4SEL                   //���ݿڹ��ܼĴ��������ƹ���ģʽ
#define DATADIR         P4DIR                   //���ݿڷ���Ĵ���

#define CTRPORT	        P6OUT                   //���������ڵĶ˿�P6
#define CTRSEL          P6SEL                   //���ƿڹ��ܼĴ��������ƹ���ģʽ
#define CTRDIR          P6DIR                   //���ƿڷ���Ĵ���

#define DCTR0	        P6OUT &= ~BIT4          //����ܶο���λ�ź��õ�
#define DCTR1           P6OUT |=  BIT4          //����ܶο���λ�ź��ø�
#define WCTR0	        P6OUT &= ~BIT3          //�����λ����λ�ź��õ�
#define WCTR1           P6OUT |=  BIT3          //�����λ����λ�ź��ø�

#define KEYPORT	        P1OUT                   //�������ڵĶ˿�P1
#define KEYSEL          P1SEL                   //���ƿڹ��ܼĴ��������ƹ���ģʽ
#define KEYDIR          P1DIR                   //���ƿڷ���Ĵ���
#define KEYIN           P1IN                    //����ɨ���ж���Ҫ��ȡIO��״ֵ̬

#define DACCS0          P5OUT &= ~BIT4          //DACƬѡ�ź��õ�
#define DACCS1          P5OUT |=  BIT4          //DACƬѡ�ź��ø�
#define DIN0            P3OUT &= ~BIT1          //DAC DIN�ܽ�λ����Ϊ0
#define DIN1            P3OUT |=  BIT1          //DAC DIN�ܽ�λ����Ϊ1
#define DOUT0           P3OUT &= ~BIT2          //DAC DOUT�ܽ�λ����Ϊ0
#define DOUT1           P3OUT |=  BIT2          //DAC DOUT�ܽ�λ����Ϊ0
#define SCLK0           P3OUT &= ~BIT3          //DAC SCLK�ܽ�λ����Ϊ0
#define SCLK1           P3OUT |=  BIT3          //DAC SCLK�ܽ�λ����Ϊ0

#define SOUNDON         P6OUT &= ~BIT2          //������������
#define SOUNDOFF        P6OUT |=  BIT2          //������������

#define RS_CLR	        P5OUT &= ~BIT5           //RS�õ�
#define RS_SET	        P5OUT |=  BIT5           //RS�ø�

#define RW_CLR	        P5OUT &= ~BIT6           //RW�õ�
#define RW_SET	        P5OUT |=  BIT6           //RW�ø�

#define EN_CLR	        P5OUT &= ~BIT7           //E�õ�
#define EN_SET	        P5OUT |=  BIT7           //E�ø�

#define PSB_CLR	        P5OUT &= ~BIT0            //PSB�õͣ����ڷ�ʽ
#define PSB_SET	        P5OUT |=  BIT0            //PSB�øߣ����ڷ�ʽ

#define RST_CLR	        P5OUT &= ~BIT1            //RST�õ�
#define RST_SET	        P5OUT |= BIT1             //RST�ø�

#define DataPort        P4OUT                     //P4��Ϊ���ݿ�

/*12864Ӧ��ָ��*/
#define CLEAR_SCREEN	0x01		          //����ָ�������ACֵΪ00H
#define AC_INIT		0x02		          //��AC����Ϊ00H�����α��Ƶ�ԭ��λ��
#define CURSE_ADD	0x06		          //�趨�α��Ƶ�����ͼ�������ƶ�����Ĭ���α����ƣ�ͼ�����岻����
#define FUN_MODE	0x30		          //����ģʽ��8λ����ָ�
#define DISPLAY_ON	0x0c		          //��ʾ��,��ʾ�α꣬���α�λ�÷���
#define DISPLAY_OFF	0x08		          //��ʾ��
#define CURSE_DIR	0x14		          //�α������ƶ�:AC=AC+1
#define SET_CG_AC	0x40		          //����AC����ΧΪ��00H~3FH
#define SET_DD_AC	0x80                      //����DDRAM AC

/*DS18B20�ܽŶ���*/
#define DQ_IN	        P2DIR &= ~BIT0		  //�������룬DS18B20�ӵ�Ƭ��P53��
#define DQ_OUT	        P2DIR |= BIT0		  //�������
#define DQ_CLR	        P2OUT &= ~BIT0	          //�õ͵�ƽ
#define DQ_SET	        P2OUT |= BIT0	          //�øߵ�ƽ
#define DQ_R	        P2IN & BIT0		  //����ƽ

#define OWI2_IN	        P2DIR &= ~BIT1		  //�������룬DS18B20�ӵ�Ƭ��P53��
#define OWI2_OUT	P2DIR |= BIT1		  //�������
#define OWI2_CLR	P2OUT &= ~BIT1	          //�õ͵�ƽ
#define OWI2_SET	P2OUT |= BIT1	          //�øߵ�ƽ
#define OWI2_R	        P2IN & BIT1		  //����ƽ



/*DS1302�ܽŶ���*/
/*DS1302��λ��*/
#define RESET_CLR	P6OUT &= ~BIT7            //��ƽ�õ�
#define RESET_SET	P6OUT |=  BIT7            //��ƽ�ø�
#define RESET_IN	P6DIR &= ~BIT7            //��������
#define RESET_OUT	P6DIR |=  BIT7            //�������

/*DS1302˫������*/
#define IO_CLR	        P6OUT &= ~BIT6            //��ƽ�õ�
#define IO_SET	        P6OUT |=  BIT6            //��ƽ�ø�
#define IO_R	        P6IN & BIT6               //��ƽ��ȡ
#define IO_IN	        P6DIR &= ~BIT6            //��������
#define IO_OUT	        P6DIR |=  BIT6            //�������

/*DS1302ʱ���ź�*/
#define SCK_CLR	        P6OUT &= ~BIT5            //ʱ���ź�
#define SCK_SET	        P6OUT |=  BIT5            //��ƽ�ø�
#define SCK_IN	        P6DIR &= ~BIT5            //��������
#define SCK_OUT	        P6DIR |=  BIT5            //�������

/*DS1302��������ص�ַ*/
#define DS1302_sec_add		0x80		  //�����ݵ�ַ
#define DS1302_min_add		0x82		  //�����ݵ�ַ
#define DS1302_hr_add		0x84		  //ʱ���ݵ�ַ
#define DS1302_date_add		0x86		  //�����ݵ�ַ
#define DS1302_month_add	0x88		  //�����ݵ�ַ
#define DS1302_day_add		0x8a		  //�������ݵ�ַ
#define DS1302_year_add		0x8c		  //�����ݵ�ַ
#define DS1302_control_add	0x8e		  //�������ݵ�ַ
#define DS1302_charger_add	0x90 					 
#define DS1302_clkburst_add	0xbe

uchar   key=0xFF;                                 //��ֵ����
uint    temp_value;
uint    temp,A1,A2,A3,A4;             		  //����ı���,��ʾ���ݴ���

/*I2C/EEPROM�ܽŶ���*/

#define SCL_H P2OUT |= BIT5; P4OUT = 0xFF;                       //SCL�ܽ��ø�
 

#define SCL_L P2OUT &= ~BIT5; P4OUT = 0x00;                      //SCL�ܽ��õ�


#define SDA_H P2OUT |= BIT6                       //SDA�ܽ��ø�


#define SDA_L P2OUT &= ~BIT6                      //SDA�ܽ��õ�


#define SDA_IN  P2DIR &= ~BIT6                    //SDA����ģʽ
#define SDA_OUT P2DIR |= BIT6                     //SDA���ģʽ
#define SDA_VAL P2IN&BIT6                         //SDA��λֵ��ȡ
 
#define TRUE    1
#define FALSE   0

#define EEPROMAddress 0xa0                        //AT24C16���豸��ַ

//***********************************************************************
//			�����������ʾ�Ķ����
//***********************************************************************

uchar table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
               0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

//***********************************************************************
//                   ϵͳʱ�ӳ�ʼ��
//***********************************************************************
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //��XT2����
  BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG; 
}
//***********************************************************************
//               MSP430�ڲ����Ź���ʼ��
//***********************************************************************
void WDT_Init()
{
   WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
}

//***********************************************************************
//		�ر������
//***********************************************************************
void Close_LED()
{
  DATASEL  = 0x00;                      //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
  DATADIR  = 0xFF;                      //����IO�ڷ���Ϊ���
  DATAPORT = 0xFF;                      //P4�ڳ�ʼ����ΪFF
  
  CTRSEL  =  0x00;                      //����IO��Ϊ��ͨI/Oģʽ���˾��ʡ
  CTRDIR |=  BIT3 + BIT4;               //����IO�ڷ���Ϊ���,���ƿ���P63,P64
  CTRPORT =  0xFF;                      //P6�ڳ�ʼ����ΪFF  
  
  DCTR1;                                //�ص�����ܣ�������ʾ����                   
  WCTR1;                     
  DATAPORT=0xFF;                        //�������            
  WCTR0; 
}