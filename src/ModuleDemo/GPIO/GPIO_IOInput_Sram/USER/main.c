#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "mh32f10x.h"

#define PRINTF_LOG 	printf

#define GPIO_GROUP_TEST		GPIOA
#define GPIO_MODE_TEST		GPIO_Mode_IPU
#define GPIO_SPEED_TEST		GPIO_Speed_50MHz
#define GPIO_PIN1_TEST		GPIO_Pin_0

USART_TypeDef* USART_TEST = USART1;

void UART_Configuration(uint32_t bound);
void GPIO_Configuration(void);

int main(void)
{	
	RCC_ClocksTypeDef clocks;
	Delay_Init();
	UART_Configuration(115200);
	RCC_GetClocksFreq(&clocks);
	
	PRINTF_LOG("\n");
	PRINTF_LOG("SYSCLK: %3.1fMhz, HCLK: %3.1fMhz, PCLK1: %3.1fMhz, PCLK2: %3.1fMhz, ADCCLK: %3.1fMhz\n", \
	(float)clocks.SYSCLK_Frequency/1000000, (float)clocks.HCLK_Frequency/1000000, \
	(float)clocks.PCLK1_Frequency/1000000, (float)clocks.PCLK2_Frequency / 1000000, (float)clocks.ADCCLK_Frequency / 1000000);
	PRINTF_LOG("MH32F103 GPIO IO Input Tset.\n");
	
	GPIO_Configuration();
	
	SCB->VTOR = SRAM_BASE;
	
	memcpy((void *)SRAM_BASE,(void *)FLASH_BASE,0x00000220);
	
	while(1)
	{
		
	}
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN1_TEST;
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_TEST;
	GPIO_InitStructure.GPIO_Mode = GPIO_MODE_TEST;	
	GPIO_Init(GPIO_GROUP_TEST, &GPIO_InitStructure);
	
	
	if(GPIO_GROUP_TEST == GPIOA && GPIO_PIN1_TEST == GPIO_Pin_0)
	{
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//EXTI0中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器	
		
		
		EXTI_InitStructure.EXTI_Line=EXTI_Line0;
		EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd=ENABLE;
		EXTI_Init(&EXTI_InitStructure);
	}
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

//#pragma arm section code = "SRMACODE"
//void EXTI0_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line0)==SET)
//	{
//		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==RESET)
//		{
//			PRINTF_LOG("The key is pressed\n");
//		}	
//	}
//	EXTI_ClearITPendingBit(EXTI_Line0);
//}
//#pragma arm section


void EXTI0_IRQHandler(void ) __attribute__((section("SRMACODE")));
void EXTI0_IRQHandler(void )
{
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==RESET)
		{
			PRINTF_LOG("The key is pressed\n");
		}	
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
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

