#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "mh32f10x.h"

#define PRINTF_LOG 	printf

USART_TypeDef* USART_TEST = USART1;

void UART_Configuration(uint32_t bound);
void NVIC_Configuration(void);
void RCC_ClkConfiguration(void);
void RTC_Configuration(void);
void Time_Adjust(void);
void Time_Show(void);

__IO uint32_t TimeDisplay = 0;

int main(void)
{	
	RCC_ClocksTypeDef clocks;
	
	Delay_Init();
	
	RCC_ClkConfiguration();
	
	UART_Configuration(115200);
	PRINTF_LOG("MH2103 RTC Calendar.\n");
	RCC_GetClocksFreq(&clocks);
	
	PRINTF_LOG("\n");
	PRINTF_LOG("SYSCLK: %3.1fMhz, HCLK: %3.1fMhz, PCLK1: %3.1fMhz, PCLK2: %3.1fMhz, ADCCLK: %3.1fMhz\n", \
	(float)clocks.SYSCLK_Frequency/1000000, (float)clocks.HCLK_Frequency/1000000, \
	(float)clocks.PCLK1_Frequency/1000000, (float)clocks.PCLK2_Frequency / 1000000, (float)clocks.ADCCLK_Frequency / 1000000);
	
	NVIC_Configuration();
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		PRINTF_LOG("\rRTC not yet configured....\n");
		RTC_Configuration();

		PRINTF_LOG("RTC configured....\n");
		
		Time_Adjust();
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
		  PRINTF_LOG("Power On Reset occurred....\n");
		}
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
		  PRINTF_LOG("External Reset occurred....\n");
		}

		PRINTF_LOG("No need to configure RTC....\n");
		RTC_WaitForSynchro();

		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		RTC_WaitForLastTask();
	}

	RCC_ClearFlag();

	Time_Show();
	
	while(1);
}

void Time_Display(uint32_t TimeVar)
{
	uint32_t THH = 0, TMM = 0, TSS = 0;

	if (RTC_GetCounter() == 0x0001517F)
	{
		 RTC_SetCounter(0x0);
		 RTC_WaitForLastTask();
	}

	THH = TimeVar / 3600;
	TMM = (TimeVar % 3600) / 60;
	TSS = (TimeVar % 3600) % 60;

	PRINTF_LOG("Time: %0.2d:%0.2d:%0.2d\n", THH, TMM, TSS);
}

void Time_Show(void)
{
	PRINTF_LOG("\n\r");
	while (1)
	{
		if (TimeDisplay == 1)
		{
		  Time_Display(RTC_GetCounter());
		  TimeDisplay = 0;
		}
	}
}

void RTC_Configuration(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	PWR_BackupAccessCmd(ENABLE);

	BKP_DeInit();

	RCC_LSICmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

	RCC_RTCCLKCmd(ENABLE);

	RTC_WaitForSynchro();

	RTC_WaitForLastTask();

	RTC_ITConfig(RTC_IT_SEC, ENABLE);

	RTC_WaitForLastTask();

	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

	RTC_WaitForLastTask();
}

uint8_t USART_Scanf(uint32_t value)
{
	uint32_t index = 0;
	uint32_t tmp[2] = {0, 0};

	while (index < 2)
	{
		while (USART_GetFlagStatus(USART_TEST, USART_FLAG_RXNE) == RESET)
		{}
		tmp[index++] = (USART_ReceiveData(USART_TEST));
		if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
		{
		  PRINTF_LOG("\n\rPlease enter valid number between 0 and 9");
		  index--;
		}
	}
	index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);

	if (index > value)
	{
		PRINTF_LOG("\n\rPlease enter valid number between 0 and %d", value);
		return 0xFF;
	}
	return index;
}

uint32_t Time_Regulate(void)
{
	uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;

	PRINTF_LOG("\r\n==============Time Settings=====================================");
	PRINTF_LOG("\r\n  Please Set Hours");

	while (Tmp_HH == 0xFF)
	{
		Tmp_HH = USART_Scanf(23);
	}
	PRINTF_LOG(":  %d", Tmp_HH);
	PRINTF_LOG("\r\n  Please Set Minutes");
	while (Tmp_MM == 0xFF)
	{
		Tmp_MM = USART_Scanf(59);
	}
	PRINTF_LOG(":  %d", Tmp_MM);
	PRINTF_LOG("\r\n  Please Set Seconds");
	while (Tmp_SS == 0xFF)
	{
		Tmp_SS = USART_Scanf(59);
	}
	PRINTF_LOG(":  %d", Tmp_SS);

	return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
}

void Time_Adjust(void)
{
	RTC_WaitForLastTask();
	RTC_SetCounter(Time_Regulate());
	RTC_WaitForLastTask();
}

void RCC_ClkConfiguration(void)
{
	RCC_DeInit();

	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	
	RCC_PLLCmd(DISABLE);
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
	
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	
	RCC_LSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void UART_Configuration(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART_TEST, &USART_InitStructure);
	USART_Cmd(USART_TEST, ENABLE);
}


int SER_PutChar (int ch)
{
	while(!USART_GetFlagStatus(USART_TEST,USART_FLAG_TC));
	USART_SendData(USART_TEST, (uint8_t) ch);

	return ch;
}

int fputc(int c, FILE *f)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	if (c == '\n')
	{
		SER_PutChar('\r');
	}
	return (SER_PutChar(c));
}

