#include "NRF24l01.h" 
#include "UART.h"
#include "LED.h"

unsigned char  TxAddr[]= {0x34,0x43,0x10,0x10,0x01};	//本地地址
unsigned char  RxAddr[]= {0x34,0x43,0x10,0x10,0x01};	//接收地址 

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
      i=2;  //尹工用的17 ,实测为2？
      while(i--) ;    
   }
}
/*****************SPI时序函数******************************************/
unsigned char NRFSPI(unsigned char date)
{
    unsigned char i;
	NRF24L01_SCK_L;	  //时钟端清0
   	for(i=0;i<8;i++)          // 循环8次
   	{
	  if((date&0x80) == 0x80)	  //此处是关键？必须加括号？或者改为if (date & 0x80) 
	    NRF24L01_MOSI_H;
	  else
	    NRF24L01_MOSI_L;   // byte最高位输出到MOSI
   	  date<<=1;             // 低一位移位到最高位
   	  NRF24L01_SCK_H; 
	  if(NRF24L01_MISO_R)               // 拉高SCK，nRF24L01从MOSI读入1位数据，同时从MISO输出1位数据
   	    date|=0x01;       	// 读MISO到byte最低位
	  else      
        date &= 0xFE;      
   	  NRF24L01_SCK_L;            	// SCK置低
   	}
    return(date);           	// 返回读出的一字节
}
/**********************NRF24L01初始化函数*******************************/
void NRF24L01Int(void)
{
	
//	Delay(2);//让系统什么都不干
//	NRF24L01_CE_H;
	NRF24L01_CE_L; //待机模式1   
	NRF24L01_CSN_H;    //片选置高，不选中芯片	
//	 NRF24L01_SCK_L;            	// SCK置低

//	 NRFWriteReg(W_REGISTER+STATUS,0xff);      //清除TX_DS或MAX_RT中断标志,是写入sta?
//	 NRFWriteReg(FLUSH_TX,0x00);//用于清空FIFO 

//	IRQ=1;	  //中断管脚置为输入
}
/*****************SPI读寄存器一字节函数*********************************/
unsigned char NRFReadReg(unsigned char RegAddr)
{
   unsigned char BackDate;
   NRF24L01_CSN_L;//启动时序
   NRFSPI(RegAddr);//写寄存器地址
   BackDate=NRFSPI(0x00);//写入读寄存器指令  
   NRF24L01_CSN_H;
   return(BackDate); //返回状态
}
/*****************SPI写寄存器一字节函数*********************************/
unsigned char NRFWriteReg(unsigned char RegAddr,unsigned char date)
{
   unsigned char BackDate;
   NRF24L01_CSN_L;//启动时序
   BackDate=NRFSPI(RegAddr);//写入地址
   NRFSPI(date);//写入值
   NRF24L01_CSN_H;
   return(BackDate);
}
//上电检测NRF24L01是否在位
//写5个数据然后再读回来进行比较，相同时返回值:0，表示在位;否则返回1，
unsigned char NRF24L01_Check(void)
{
	unsigned char buf[4]={0XA5,0XA5,0XA5,0XA5};	 //volatile 
	unsigned char buf1[4];
	unsigned char i;  
//	USART1_Putc(buf[1]); 	 
	NRFWriteTxDate(W_REGISTER+TX_ADDR,buf,4);//写入4个字节的地址.	
	NRFReadRxDate(TX_ADDR,buf1,4);              //读出写入的地址  	
	for(i=0;i<4;i++)
	if(buf1[i]!=0XA5)break;					   
	if(i!=4)return 1;                               //NRF24L01不在位	
	return 0;		                                //NRF24L01在位
}
/*****************SPI读取RXFIFO寄存器的值********************************/
unsigned char NRFReadRxDate(unsigned char RegAddr,unsigned char *RxDate,unsigned char DateLen)
{  //寄存器地址//读取数据存放变量//读取数据长度//用于接收
    unsigned char BackDate,i;
	NRF24L01_CSN_L;//启动时序
	BackDate=NRFSPI(RegAddr);//写入要读取的寄存器地址
	for(i=0;i<DateLen;i++) //读取数据
	  {
	     RxDate[i]=NRFSPI(0);
	  } 
    NRF24L01_CSN_H;
   return(BackDate); 
}
/*****************SPI写入TXFIFO寄存器的值**********************************/
unsigned char NRFWriteTxDate(unsigned char RegAddr,unsigned char *Tx_Date,unsigned char DateLen)
{ //寄存器地址//写入数据存放变量//读取数据长度//用于发送
   unsigned char BackDate,i;
   NRF24L01_CSN_L;
   BackDate=NRFSPI(RegAddr);//写入要写入寄存器的地址
   for(i=0;i<DateLen;i++)//写入数据
     {
	    NRFSPI(*Tx_Date++);
	 }   
   NRF24L01_CSN_H;
   return(BackDate);
}
/*****************NRF设置为发送模式并发送数据******************************/
void NRFSetTxMode(void)
//void NRFSetTxMode(unsigned char *Tx_Date)
{//发送模式
    NRF24L01_CE_L;
   	NRFWriteTxDate(W_REGISTER+TX_ADDR,TxAddr,TX_ADDR_WITDH);   //写寄存器指令+接收地址使能指令+接收地址+地址宽度
	NRFWriteTxDate(W_REGISTER+RX_ADDR_P0,RxAddr,RX_ADDR_WITDH);//为了应答接收设备，接收通道0地址和发送地址相同
//	NRFWriteTxDate(W_TX_PAYLOAD,Tx_Date,TX_DATA_WITDH);//写入数据 
	/******下面有关寄存器配置**************/
  	NRFWriteReg(W_REGISTER+EN_AA,0x01);       // 使能接收通道0自动应答	
  	NRFWriteReg(W_REGISTER+EN_RXADDR,0x01);   // 使能接收通道0
  	NRFWriteReg(W_REGISTER+SETUP_RETR,0x0a);  // 自动重发延时等待250us+86us，自动重发10次
  	NRFWriteReg(W_REGISTER+RF_CH,0x40);         // 选择射频通道0x40
  	NRFWriteReg(W_REGISTER+RF_SETUP,0x07);    // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	NRFWriteReg(W_REGISTER+CONFIG,0x0e);      // CRC使能，16位CRC校验，上电  
	NRF24L01_CE_H;						//	 10us后启动发送
//	NRF_delay_nus(10);//保持10us秒以上	 CE从低跳到高，发送
}
/*****************NRF设置为接收模式并接收数据******************************/
//主要接收模式
void NRFSetRXMode(void)
{
	NRF24L01_CE_L;  
  	NRFWriteTxDate(W_REGISTER+RX_ADDR_P0,RxAddr,RX_ADDR_WITDH);  // 接收设备接收通道0使用和发送设备相同的发送地址
  	NRFWriteReg(W_REGISTER+EN_AA,0x01);               // 使能接收通道0自动应答
  	NRFWriteReg(W_REGISTER+EN_RXADDR,0x01);           // 使能接收通道0
  	NRFWriteReg(W_REGISTER+RF_CH,0x40);                 // 选择射频通道0x40
  	NRFWriteReg(W_REGISTER+RX_PW_P0,RX_DATA_WITDH);  // 接收通道0选择和发送通道相同有效数据宽度
  	NRFWriteReg(W_REGISTER+RF_SETUP,0x07);            // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益*/
  	NRFWriteReg(W_REGISTER+CONFIG,0x0f);              // CRC使能，16位CRC校验，上电，接收模式
  	NRF24L01_CE_H;
	NRF_delay_nus(10);//保持10us秒以上     
}
/***************************用于发射******************************/
unsigned char NRF24L01_TxPacket(unsigned char *Tx_Date)
{  
	unsigned char sta;	
	NRF24L01_CE_L;
	NRFWriteTxDate(W_TX_PAYLOAD,Tx_Date,TX_DATA_WITDH);//写入数据 
	NRF24L01_CE_H;
//	NRF_delay_nus(2);
	while(NRF24L01_IRQ_R!=0);  //等待发送完成,此时可能是达到最大发送次数或者收到接收节点的ack 
	sta=NRFReadReg(STATUS);	                 //读取状态寄存器的值                   
 	NRFWriteReg(W_REGISTER+STATUS,sta);      //清除TX_DS或MAX_RT中断标志, 写入0xFF最好？
	if((sta&MAX_RT)==MAX_RT) 				 //达到最大重发次数
	{
	   NRFWriteReg(FLUSH_TX,0x00);     //用于清空FIFO 
	   return MAX_RT;
	}
	if((sta&TX_DS)==TX_DS)                      //发送成功
	{
		return TX_DS;
	}
	return 0xff;                       //其他错误  	
//	NRF_delay_nus(10);//保持10us秒以上     	  	
}

/******************判断是否接收收到数据，接到就从RX取出*********************/
//用于接收模式
//unsigned char NRFRevDate(unsigned char *RevTempDate)
unsigned char NRFRevDate(void)
{
   	 unsigned char sta;
	 sta=NRFReadReg(R_REGISTER+STATUS);//发送数据后读取状态寄存器  	
	 NRFWriteReg(W_REGISTER+STATUS,sta);      //清除TX_DS或MAX_RT中断标志,是写入sta?
     if((sta&RX_DR)==RX_DR)				// 判断是否接收到数据
	 {
//	   NRFReadRxDate(R_RX_PAYLOAD,RevTempDate,RX_DATA_WITDH);// 从RXFIFO读取数据 接收4位即可，后一位位结束位
//	   NRFWriteReg(FLUSH_RX,0x00);	 //用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！
	   return 1;	
	 }
	  else
	  return 0;	
}
