#ifndef NRF24L01_H 
#define NRF24L01_H

#include "stm32f10x.h"

#define NRF24L01_PORT	     GPIOB
#define NRF24L01_CSN		 GPIO_Pin_12
#define NRF24L01_MOSI		 GPIO_Pin_15
#define NRF24L01_CE	    	 GPIO_Pin_9
#define NRF24L01_SCK		 GPIO_Pin_13
#define NRF24L01_MISO		 GPIO_Pin_14	

#define NRF24L01_IRQ_PORT 	 GPIOB		 //
#define NRF24L01_IRQ_IO      GPIO_Pin_8    /* PC.13 */


#define NRF24L01_MOSI_L      GPIO_ResetBits(NRF24L01_PORT, NRF24L01_MOSI)      //
#define NRF24L01_MOSI_H      GPIO_SetBits(NRF24L01_PORT, NRF24L01_MOSI)        //	
#define NRF24L01_SCK_L       GPIO_ResetBits(NRF24L01_PORT, NRF24L01_SCK)      //
#define NRF24L01_SCK_H       GPIO_SetBits(NRF24L01_PORT, NRF24L01_SCK)        // 	
#define NRF24L01_CE_L        GPIO_ResetBits(NRF24L01_PORT, NRF24L01_CE)      //
#define NRF24L01_CE_H        GPIO_SetBits(NRF24L01_PORT, NRF24L01_CE)        //
#define NRF24L01_CSN_L       GPIO_ResetBits(NRF24L01_PORT, NRF24L01_CSN)      //
#define NRF24L01_CSN_H       GPIO_SetBits(NRF24L01_PORT, NRF24L01_CSN)        //

#define NRF24L01_IRQ_R       GPIO_ReadInputDataBit(NRF24L01_IRQ_PORT, NRF24L01_IRQ_IO) //
#define NRF24L01_MISO_R      GPIO_ReadInputDataBit(NRF24L01_PORT, NRF24L01_MISO)	//

//#define NRF24_TX_Mode  TX_Mode
//#define NRF24_RX_Mode  RX_Mode

/*******************相关函数声明**************************/	
extern void NRF_delay_nus(unsigned short int time);
extern void NRF24L01_GPIO_Config(void);
extern unsigned char NRF24L01_Check(void);
extern unsigned char NRF_TX_Check(void);
//extern unsigned char NRFACK(void);
extern unsigned char NRFSPI(unsigned char date);
extern unsigned char NRFReadReg(unsigned char RegAddr);
extern unsigned char NRFWriteReg(unsigned char RegAddr,unsigned char date);
extern unsigned char NRFReadRxDate(unsigned char RegAddr,unsigned char *RxDate,unsigned char DateLen);
extern unsigned char NRFWriteTxDate(unsigned char RegAddr,unsigned char *Tx_Date,unsigned char DateLen);
//extern unsigned char NRFRevDate(unsigned char *RevDate);
extern unsigned char NRFRevDate(void);
extern void NRFSetTxMode(void);
//extern void NRFSetTxMode(unsigned char *Tx_Date);
extern void NRF24L01Int(void);
extern void NRFSetRXMode(void);
extern unsigned char NRF24L01_TxPacket(unsigned char *Tx_Date);

///*********************************************NRF24L01*************************************
//#define TX_ADR_WIDTH    5   	// 5 uints TX address width
//#define RX_ADR_WIDTH    5   	// 5 uints RX address width
//
//#define RX_PLOAD_WIDTH  4  	// 20 uints RX payload
//#define TX_PLOAD_WIDTH  4  	// 20 uints TX payload	



/*******************************************************/
#define TX_ADDR_WITDH 5	//发送地址宽度设置为5个字节
#define RX_ADDR_WITDH 5	//接收地址宽度设置为5个字节
#define TX_DATA_WITDH 16//发送数据宽度
#define RX_DATA_WITDH 16//接收数据宽度

#define MAX_RT  	    0x10  //达到最大发送次数中断
#define TX_DS       	0x20  //TX发送完成中断
#define RX_DR   	    0x40  //接收到数据中断

//#define IRQ_FLAGS       0x07	//状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最大:6);bit4,达到最多次重发
//bit5:数据发送完成中断;bit6:接收数据中断;

/*******************命令寄存器***************************/
#define  R_REGISTER      0x00//读取配置寄存器
#define  W_REGISTER      0x20//写配置寄存器
#define  R_RX_PAYLOAD 	 0x61//读取RX有效数据
#define  W_TX_PAYLOAD	 0xa0//写TX有效数据
#define  FLUSH_TX		 0xe1//清除TXFIFO寄存器
#define  FLUSH_RX		 0xe2//清除RXFIFO寄存器
#define  REUSE_TX_PL     0xe3//重新使用上一包有效数据
#define  NOP             0xff//空操作
/******************寄存器地址****************************/
#define  CONFIG          0x00//配置寄存器
#define  EN_AA			 0x01//使能自动应答
#define  EN_RXADDR       0x02//接收通道使能0-5个通道
#define  SETUP_AW        0x03//设置数据通道地址宽度3-5
#define  SETUP_RETR      0x04//建立自动重发
#define  RF_CH           0x05//射频通道设置
#define  RF_SETUP        0x06//射频寄存器
#define  STATUS          0x07//状态寄存器
#define  OBSERVE_TX      0x08//发送检测寄存器
#define  CD              0x09//载波
#define  RX_ADDR_P0      0x0a//数据通道0接收地址
#define  RX_ADDR_P1      0x0b//数据通道1接收地址
#define  RX_ADDR_P2      0x0c//数据通道2接收地址
#define  RX_ADDR_P3      0x0d//数据通道3接收地址
#define  RX_ADDR_P4      0x0e//数据通道4接收地址
#define  RX_ADDR_P5      0x0f//数据通道5接收地址
#define  TX_ADDR         0x10//发送地址
#define  RX_PW_P0        0x11//P0通道数据宽度设置
#define  RX_PW_P1        0x12//P1通道数据宽度设置
#define  RX_PW_P2        0x13//P2通道数据宽度设置
#define  RX_PW_P3        0x14//P3通道数据宽度设置
#define  RX_PW_P4        0x15//P4通道数据宽度设置
#define  RX_PW_P5        0x16//P5通道数据宽度设置
#define  FIFO_STATUS     0x17//FIFO状态寄存器


#endif

