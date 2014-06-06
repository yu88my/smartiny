#include "NRF24l01.h" 
#include "UART.h"
#include "LED.h"

unsigned char  TxAddr[]= {0x34,0x43,0x10,0x10,0x01};	//���ص�ַ
unsigned char  RxAddr[]= {0x34,0x43,0x10,0x10,0x01};	//���յ�ַ 

//extern unsigned char TxDate[4];  //NRF24L01

void NRF24L01_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		/* Configure  pins: SCK, CE,CSN and MOSI ---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CSN | NRF24L01_MOSI | NRF24L01_CE | NRF24L01_SCK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(NRF24L01_PORT, &GPIO_InitStructure);  

	/* Configure  MISO (PB.14) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = NRF24L01_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(NRF24L01_PORT, &GPIO_InitStructure); 
	/* Configure NRF24L01_IRQ_IO as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = NRF24L01_IRQ_IO;	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(NRF24L01_IRQ_PORT, &GPIO_InitStructure); 
}
void NRF_delay_nus(unsigned short int time)
{    
   unsigned short int i=0;  
   while(time--)
   {
      i=2;  //�����õ�17 ,ʵ��Ϊ2��
      while(i--) ;    
   }
}
/*****************SPIʱ����******************************************/
unsigned char NRFSPI(unsigned char date)
{
    unsigned char i;
	NRF24L01_SCK_L;	  //ʱ�Ӷ���0
   	for(i=0;i<8;i++)          // ѭ��8��
   	{
	  if((date&0x80) == 0x80)	  //�˴��ǹؼ�����������ţ����߸�Ϊif (date & 0x80) 
	    NRF24L01_MOSI_H;
	  else
	    NRF24L01_MOSI_L;   // byte���λ�����MOSI
   	  date<<=1;             // ��һλ��λ�����λ
   	  NRF24L01_SCK_H; 
	  if(NRF24L01_MISO_R)               // ����SCK��nRF24L01��MOSI����1λ���ݣ�ͬʱ��MISO���1λ����
   	    date|=0x01;       	// ��MISO��byte���λ
	  else      
        date &= 0xFE;      
   	  NRF24L01_SCK_L;            	// SCK�õ�
   	}
    return(date);           	// ���ض�����һ�ֽ�
}
/**********************NRF24L01��ʼ������*******************************/
void NRF24L01Int(void)
{
	
//	Delay(2);//��ϵͳʲô������
//	NRF24L01_CE_H;
	NRF24L01_CE_L; //����ģʽ1   
	NRF24L01_CSN_H;    //Ƭѡ�øߣ���ѡ��оƬ	
//	 NRF24L01_SCK_L;            	// SCK�õ�

//	 NRFWriteReg(W_REGISTER+STATUS,0xff);      //���TX_DS��MAX_RT�жϱ�־,��д��sta?
//	 NRFWriteReg(FLUSH_TX,0x00);//�������FIFO 

//	IRQ=1;	  //�жϹܽ���Ϊ����
}
/*****************SPI���Ĵ���һ�ֽں���*********************************/
unsigned char NRFReadReg(unsigned char RegAddr)
{
   unsigned char BackDate;
   NRF24L01_CSN_L;//����ʱ��
   NRFSPI(RegAddr);//д�Ĵ�����ַ
   BackDate=NRFSPI(0x00);//д����Ĵ���ָ��  
   NRF24L01_CSN_H;
   return(BackDate); //����״̬
}
/*****************SPIд�Ĵ���һ�ֽں���*********************************/
unsigned char NRFWriteReg(unsigned char RegAddr,unsigned char date)
{
   unsigned char BackDate;
   NRF24L01_CSN_L;//����ʱ��
   BackDate=NRFSPI(RegAddr);//д���ַ
   NRFSPI(date);//д��ֵ
   NRF24L01_CSN_H;
   return(BackDate);
}
//�ϵ���NRF24L01�Ƿ���λ
//д5������Ȼ���ٶ��������бȽϣ���ͬʱ����ֵ:0����ʾ��λ;���򷵻�1��
unsigned char NRF24L01_Check(void)
{
	unsigned char buf[4]={0XA5,0XA5,0XA5,0XA5};	 //volatile 
	unsigned char buf1[4];
	unsigned char i;  
//	USART1_Putc(buf[1]); 	 
	NRFWriteTxDate(W_REGISTER+TX_ADDR,buf,4);//д��4���ֽڵĵ�ַ.	
	NRFReadRxDate(TX_ADDR,buf1,4);              //����д��ĵ�ַ  	
	for(i=0;i<4;i++)
	if(buf1[i]!=0XA5)break;					   
	if(i!=4)return 1;                               //NRF24L01����λ	
	return 0;		                                //NRF24L01��λ
}
/*****************SPI��ȡRXFIFO�Ĵ�����ֵ********************************/
unsigned char NRFReadRxDate(unsigned char RegAddr,unsigned char *RxDate,unsigned char DateLen)
{  //�Ĵ�����ַ//��ȡ���ݴ�ű���//��ȡ���ݳ���//���ڽ���
    unsigned char BackDate,i;
	NRF24L01_CSN_L;//����ʱ��
	BackDate=NRFSPI(RegAddr);//д��Ҫ��ȡ�ļĴ�����ַ
	for(i=0;i<DateLen;i++) //��ȡ����
	  {
	     RxDate[i]=NRFSPI(0);
	  } 
    NRF24L01_CSN_H;
   return(BackDate); 
}
/*****************SPIд��TXFIFO�Ĵ�����ֵ**********************************/
unsigned char NRFWriteTxDate(unsigned char RegAddr,unsigned char *Tx_Date,unsigned char DateLen)
{ //�Ĵ�����ַ//д�����ݴ�ű���//��ȡ���ݳ���//���ڷ���
   unsigned char BackDate,i;
   NRF24L01_CSN_L;
   BackDate=NRFSPI(RegAddr);//д��Ҫд��Ĵ����ĵ�ַ
   for(i=0;i<DateLen;i++)//д������
     {
	    NRFSPI(*Tx_Date++);
	 }   
   NRF24L01_CSN_H;
   return(BackDate);
}
/*****************NRF����Ϊ����ģʽ����������******************************/
void NRFSetTxMode(void)
//void NRFSetTxMode(unsigned char *Tx_Date)
{//����ģʽ
    NRF24L01_CE_L;
   	NRFWriteTxDate(W_REGISTER+TX_ADDR,TxAddr,TX_ADDR_WITDH);   //д�Ĵ���ָ��+���յ�ַʹ��ָ��+���յ�ַ+��ַ���
	NRFWriteTxDate(W_REGISTER+RX_ADDR_P0,RxAddr,RX_ADDR_WITDH);//Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
//	NRFWriteTxDate(W_TX_PAYLOAD,Tx_Date,TX_DATA_WITDH);//д������ 
	/******�����йؼĴ�������**************/
  	NRFWriteReg(W_REGISTER+EN_AA,0x01);       // ʹ�ܽ���ͨ��0�Զ�Ӧ��	
  	NRFWriteReg(W_REGISTER+EN_RXADDR,0x01);   // ʹ�ܽ���ͨ��0
  	NRFWriteReg(W_REGISTER+SETUP_RETR,0x0a);  // �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
  	NRFWriteReg(W_REGISTER+RF_CH,0x40);         // ѡ����Ƶͨ��0x40
  	NRFWriteReg(W_REGISTER+RF_SETUP,0x07);    // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
	NRFWriteReg(W_REGISTER+CONFIG,0x0e);      // CRCʹ�ܣ�16λCRCУ�飬�ϵ�  
	NRF24L01_CE_H;						//	 10us����������
//	NRF_delay_nus(10);//����10us������	 CE�ӵ������ߣ�����
}
/*****************NRF����Ϊ����ģʽ����������******************************/
//��Ҫ����ģʽ
void NRFSetRXMode(void)
{
	NRF24L01_CE_L;  
  	NRFWriteTxDate(W_REGISTER+RX_ADDR_P0,RxAddr,RX_ADDR_WITDH);  // �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
  	NRFWriteReg(W_REGISTER+EN_AA,0x01);               // ʹ�ܽ���ͨ��0�Զ�Ӧ��
  	NRFWriteReg(W_REGISTER+EN_RXADDR,0x01);           // ʹ�ܽ���ͨ��0
  	NRFWriteReg(W_REGISTER+RF_CH,0x40);                 // ѡ����Ƶͨ��0x40
  	NRFWriteReg(W_REGISTER+RX_PW_P0,RX_DATA_WITDH);  // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
  	NRFWriteReg(W_REGISTER+RF_SETUP,0x07);            // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������*/
  	NRFWriteReg(W_REGISTER+CONFIG,0x0f);              // CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
  	NRF24L01_CE_H;
	NRF_delay_nus(10);//����10us������     
}
/***************************���ڷ���******************************/
unsigned char NRF24L01_TxPacket(unsigned char *Tx_Date)
{  
	unsigned char sta;	
	NRF24L01_CE_L;
	NRFWriteTxDate(W_TX_PAYLOAD,Tx_Date,TX_DATA_WITDH);//д������ 
	NRF24L01_CE_H;
//	NRF_delay_nus(2);
	while(NRF24L01_IRQ_R!=0);  //�ȴ��������,��ʱ�����Ǵﵽ����ʹ��������յ����սڵ��ack 
	sta=NRFReadReg(STATUS);	                 //��ȡ״̬�Ĵ�����ֵ                   
 	NRFWriteReg(W_REGISTER+STATUS,sta);      //���TX_DS��MAX_RT�жϱ�־, д��0xFF��ã�
	if((sta&MAX_RT)==MAX_RT) 				 //�ﵽ����ط�����
	{
	   NRFWriteReg(FLUSH_TX,0x00);     //�������FIFO 
	   return MAX_RT;
	}
	if((sta&TX_DS)==TX_DS)                      //���ͳɹ�
	{
		return TX_DS;
	}
	return 0xff;                       //��������  	
//	NRF_delay_nus(10);//����10us������     	  	
}

/******************�ж��Ƿ�����յ����ݣ��ӵ��ʹ�RXȡ��*********************/
//���ڽ���ģʽ
//unsigned char NRFRevDate(unsigned char *RevTempDate)
unsigned char NRFRevDate(void)
{
   	 unsigned char sta;
	 sta=NRFReadReg(R_REGISTER+STATUS);//�������ݺ��ȡ״̬�Ĵ���  	
	 NRFWriteReg(W_REGISTER+STATUS,sta);      //���TX_DS��MAX_RT�жϱ�־,��д��sta?
     if((sta&RX_DR)==RX_DR)				// �ж��Ƿ���յ�����
	 {
//	   NRFReadRxDate(R_RX_PAYLOAD,RevTempDate,RX_DATA_WITDH);// ��RXFIFO��ȡ���� ����4λ���ɣ���һλλ����λ
//	   NRFWriteReg(FLUSH_RX,0x00);	 //�������FIFO �����ؼ�������Ȼ��������벻���ĺ����������Ҽ�ס����
	   return 1;	
	 }
	  else
	  return 0;	
}
