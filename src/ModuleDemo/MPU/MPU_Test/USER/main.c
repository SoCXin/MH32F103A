#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "mh32f10x.h"

#define PRINTF_LOG 	printf

USART_TypeDef* USART_TEST = USART1;

void UART_Configuration(uint32_t bound);
void SHOW_PrintFlash(uint32_t addr, uint32_t size);
void SHOW_PrintFlash(uint32_t addr, uint32_t size) __attribute__((section(".ARM.__at_0x08010000")));
void MPU_Set(void);

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
	PRINTF_LOG("MH2103 MPU Test.\n");

	PRINTF_LOG("MPU->TYPE, Value: 0x%x\n",MPU->TYPE);
	PRINTF_LOG("MPU->CTRL, Value: 0x%x\n",MPU->CTRL);
	PRINTF_LOG("MPU->RNR, Value: 0x%x\n",MPU->RNR);
	PRINTF_LOG("MPU->RBAR, Value: 0x%x\n",MPU->RBAR);	
	PRINTF_LOG("MPU->RASR, Value: 0x%x\n",MPU->RASR);
	PRINTF_LOG("MPU->RBAR_A1;, Value: 0x%x\n",MPU->RBAR_A1);
	PRINTF_LOG("MPU->RASR_A1, Value: 0x%x\n",MPU->RASR_A1);	
	PRINTF_LOG("MPU->RBAR_A2, Value: 0x%x\n",MPU->RBAR_A2);
	PRINTF_LOG("MPU->RASR_A2, Value: 0x%x\n",MPU->RASR_A2);
	PRINTF_LOG("MPU->RBAR_A3, Value: 0x%x\n",MPU->RBAR_A3);
	PRINTF_LOG("MPU->RASR_A3, Value: 0x%x\n\n",MPU->RASR_A3);
	
	PRINTF_LOG("LimiteToPrivilege Access\n");
	SHOW_PrintFlash(0x08000000,64);
	
	MPU_Set();
	
	PRINTF_LOG("LimiteToUser Access\n");
	SHOW_PrintFlash(0x08000000,64);
	while(1);
}

void mpu_disable(void)
{
	__asm("dmb");
    MPU->CTRL &= ~MPU_CTRL_ENABLE_Msk;
}

void mpu_enable(void)
{
    MPU->CTRL |= MPU_CTRL_ENABLE_Msk;
    __asm("dsb");
    __asm("isb");
}

void mpu_region_disable(uint8_t region_num)
{
    MPU->RNR = region_num;
	MPU->RBAR = 0;
	MPU->RASR = 0;
}

void mpu_region_config(uint8_t region_num,uint32_t addr,uint32_t size, uint32_t attr)
{
	MPU->RNR = region_num;
	MPU->RBAR = addr;
	MPU->RASR = size|attr;
}

#define MPU_DEFS_RASR_SIZE_1KB			(0x09<<MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_16KB			(0x0D<<MPU_RASR_SIZE_Pos)
#define MPU_DEFS_RASR_SIZE_64KB			(0x0F<<MPU_RASR_SIZE_Pos)
#define MPU_DEFS_NORMAL_MEMORY_WT		(MPU_RASR_C_Msk|MPU_RASR_S_Msk)
#define MPU_DEFS_RASE_AP_FULL_ACCESS	(0x3<<MPU_RASR_AP_Pos)

void MPU_Set(void)
{
	SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
	
	mpu_disable();
	mpu_region_config(0,0x8000000,MPU_DEFS_RASR_SIZE_64KB,\
	MPU_DEFS_NORMAL_MEMORY_WT|MPU_DEFS_RASE_AP_FULL_ACCESS|MPU_RASR_ENABLE_Msk);
	mpu_region_config(1,0x20000000,MPU_DEFS_RASR_SIZE_16KB,\
	MPU_DEFS_NORMAL_MEMORY_WT|MPU_DEFS_RASE_AP_FULL_ACCESS|MPU_RASR_ENABLE_Msk);
	mpu_region_config(2,USART1_BASE,MPU_DEFS_RASR_SIZE_1KB,\
	MPU_DEFS_NORMAL_MEMORY_WT|MPU_DEFS_RASE_AP_FULL_ACCESS|MPU_RASR_ENABLE_Msk);
	mpu_region_disable(3);
	mpu_region_disable(4);
	mpu_region_disable(5);
	mpu_region_disable(6);
	mpu_region_disable(7);	
	
	mpu_enable();
}
	
void SHOW_PrintFlash(uint32_t addr, uint32_t size)
{
	uint32_t i;
	for(i = 0; i<+size; i= i+4)
	{
		PRINTF_LOG("Addr: %x, Value: %x\n",(addr+i),*(uint32_t *)(addr+i));
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

