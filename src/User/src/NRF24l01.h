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

/*******************��غ�������**************************/	
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
#define TX_ADDR_WITDH 5	//���͵�ַ�������Ϊ5���ֽ�
#define RX_ADDR_WITDH 5	//���յ�ַ�������Ϊ5���ֽ�
#define TX_DATA_WITDH 16//�������ݿ��
#define RX_DATA_WITDH 16//�������ݿ��

#define MAX_RT  	    0x10  //�ﵽ����ʹ����ж�
#define TX_DS       	0x20  //TX��������ж�
#define RX_DR   	    0x40  //���յ������ж�

//#define IRQ_FLAGS       0x07	//״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
//bit5:���ݷ�������ж�;bit6:���������ж�;

/*******************����Ĵ���***************************/
#define  R_REGISTER      0x00//��ȡ���üĴ���
#define  W_REGISTER      0x20//д���üĴ���
#define  R_RX_PAYLOAD 	 0x61//��ȡRX��Ч����
#define  W_TX_PAYLOAD	 0xa0//дTX��Ч����
#define  FLUSH_TX		 0xe1//���TXFIFO�Ĵ���
#define  FLUSH_RX		 0xe2//���RXFIFO�Ĵ���
#define  REUSE_TX_PL     0xe3//����ʹ����һ����Ч����
#define  NOP             0xff//�ղ���
/******************�Ĵ�����ַ****************************/
#define  CONFIG          0x00//���üĴ���
#define  EN_AA			 0x01//ʹ���Զ�Ӧ��
#define  EN_RXADDR       0x02//����ͨ��ʹ��0-5��ͨ��
#define  SETUP_AW        0x03//��������ͨ����ַ���3-5
#define  SETUP_RETR      0x04//�����Զ��ط�
#define  RF_CH           0x05//��Ƶͨ������
#define  RF_SETUP        0x06//��Ƶ�Ĵ���
#define  STATUS          0x07//״̬�Ĵ���
#define  OBSERVE_TX      0x08//���ͼ��Ĵ���
#define  CD              0x09//�ز�
#define  RX_ADDR_P0      0x0a//����ͨ��0���յ�ַ
#define  RX_ADDR_P1      0x0b//����ͨ��1���յ�ַ
#define  RX_ADDR_P2      0x0c//����ͨ��2���յ�ַ
#define  RX_ADDR_P3      0x0d//����ͨ��3���յ�ַ
#define  RX_ADDR_P4      0x0e//����ͨ��4���յ�ַ
#define  RX_ADDR_P5      0x0f//����ͨ��5���յ�ַ
#define  TX_ADDR         0x10//���͵�ַ
#define  RX_PW_P0        0x11//P0ͨ�����ݿ������
#define  RX_PW_P1        0x12//P1ͨ�����ݿ������
#define  RX_PW_P2        0x13//P2ͨ�����ݿ������
#define  RX_PW_P3        0x14//P3ͨ�����ݿ������
#define  RX_PW_P4        0x15//P4ͨ�����ݿ������
#define  RX_PW_P5        0x16//P5ͨ�����ݿ������
#define  FIFO_STATUS     0x17//FIFO״̬�Ĵ���


#endif

