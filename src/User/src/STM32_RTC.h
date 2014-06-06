#ifndef STM32_RTC_H
#define STM32_RTC_H

#include "stm32f10x.h"
//硬件初始化


//时间结构体
typedef struct _t_sct{
u8 year;	//	0-99
u8 month;
u8 day;
u8 hour;
u8 minute;
u8 sec;
u8 date;
}T_STRUCT;

//用于时间
#define BKP_TIME_SET		BKP_DR1	//RTC设置与否标志
#define BKP_TIME_YEAR		BKP_DR2
#define BKP_TIME_MONTH		BKP_DR3
#define BKP_TIME_DAY		BKP_DR4
#define BKP_TIME_DATE		BKP_DR5
extern bool RTCInit(void);

//没有设置时间下的默认时间
#define DEFAULT_TIME_YEAR	13
#define DEFAULT_TIME_MONTH	3
#define DEFAULT_TIME_DAY	3
#define DEFAULT_TIME_HOUR	17
#define DEFAULT_TIME_MINUTE	19
#define DEFAULT_TIME_SECOND	30

//RTC
extern void GetRTCTime(T_STRUCT*);
extern void SetRTCTime(T_STRUCT*);
extern u8 GetDate(T_STRUCT* time);
extern void RTC2Time(void);

#endif
