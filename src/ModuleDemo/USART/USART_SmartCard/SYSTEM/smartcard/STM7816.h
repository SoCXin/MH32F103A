#ifndef _STM7816_H
#define _STM7816_H

#include "mh32f10x.h"

/*引脚定义*/
#define STM_USARTx				USART2
#define STM_USARTx_IRQn			USART2_IRQn

#define STM_CLK_GPIOx			GPIOA
#define STM_CLK_Pinx			GPIO_Pin_4
#define STM_IO_GPIOx			GPIOA
#define STM_IO_Pinx				GPIO_Pin_2

#define STM_RST_GPIOx			GPIOC
#define STM_RST_Pinx			GPIO_Pin_8
#define STM_VCC_GPIOx			GPIOC
#define STM_VCC_Pinx			GPIO_Pin_6
#define STM_OFFN_GPIOx			GPIOC
#define STM_OFFN_Pinx			GPIO_Pin_7
#define STM_5V3VN_GPIOx			GPIOC
#define STM_5V3VN_Pinx			GPIO_Pin_5


#define STM_RCC_APBxPeriphClockCmd  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);\
                                    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE)


extern uint8_t ATR_TA1;                                                         //卡ATR中TA1的值,TA1包含FD
extern uint8_t STM_T1;                                                          //卡是否为T=1,0为T=0,1为T=1
extern uint32_t STM_WT;			                                                //通信超时时间WT=9600

extern uint8_t STM_F;                                                           //F=1
extern uint8_t STM_D;                                                           //D=1
extern uint32_t STM_ClkHz;                                                      //频率=3.6MHz


void STM7816_Init(void);                                                        //初始化
void STM7816_TIMxInt(void);                                                     //1MS定时中断调用,为通信超时提供时间依据

void STM7816_SetClkHz(uint32_t hz);                                             //设置时钟频率
void STM7816_SetFD(uint8_t F, uint8_t D);                                       //设置FD
void STM7816_SetWT(uint32_t wt);                                                //设置通信超时时间
void STM7816_SetVCC(uint8_t on);                                                //设置VCC

uint8_t ColdReset(uint8_t *atr, uint16_t *len);                                 //冷复位
uint8_t WarmReset(uint8_t *atr, uint16_t *len);                                 //热复位
uint8_t PPS(uint8_t F, uint8_t D);                                              //PPS
                                                                                //发送APDU,并接收返回数据,没有出错重发机制
uint8_t ExchangeTPDU(uint8_t *sData, uint16_t len_sData, uint8_t *rData, uint16_t *len_rData);

#endif

