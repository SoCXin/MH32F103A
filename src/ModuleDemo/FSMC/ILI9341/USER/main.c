#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "mh32f10x.h"
#include "bsp_ili9341_lcd.h"

#define PRINTF_LOG 	printf

USART_TypeDef* USART_TEST = USART1;

void UART_Configuration(uint32_t bound);
void GPIO_Configuration(void);
void LCD_Test(void);	
void Delay ( __IO uint32_t nCount );

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
	PRINTF_LOG("MH2103 FSMC ILI9341 Tset.\n");
	
	ILI9341_Init();
	
	ILI9341_GramScan ( 6 );
	while ( 1 )
	{
		LCD_Test();
	}
}
/*用于测试各种液晶的函数*/
void LCD_Test(void)
{
	/*演示显示变量*/
	static uint8_t testCNT = 0;	
	char dispBuff[100];

	testCNT++;	

	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);

	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
	/********显示字符串示例*******/
	ILI9341_DispStringLine_EN(LINE(0),"BH 3.2 inch LCD para:");
	ILI9341_DispStringLine_EN(LINE(1),"Image resolution:240x320 px");
	ILI9341_DispStringLine_EN(LINE(2),"ILI9341 LCD driver");
	ILI9341_DispStringLine_EN(LINE(3),"XPT2046 Touch Pad driver");

	/********显示变量示例*******/
	LCD_SetFont(&Font16x24);
	LCD_SetTextColor(GREEN);

	/*使用c标准库把变量转化成字符串*/
	sprintf(dispBuff,"Count : %d ",testCNT);
	LCD_ClearLine(LINE(4));	/* 清除单行文字 */

	/*然后显示该字符串即可，其它变量也是这样处理*/
	ILI9341_DispStringLine_EN(LINE(4),dispBuff);

	/*******显示图形示例******/
	LCD_SetFont(&Font24x32);
	/* 画直线 */

	LCD_ClearLine(LINE(4));/* 清除单行文字 */
	LCD_SetTextColor(BLUE);

	ILI9341_DispStringLine_EN(LINE(4),"Draw line:");

	LCD_SetTextColor(RED);
	ILI9341_DrawLine(50,170,210,230);  
	ILI9341_DrawLine(50,200,210,240);

	LCD_SetTextColor(GREEN);
	ILI9341_DrawLine(100,170,200,230);  
	ILI9341_DrawLine(200,200,220,240);

	LCD_SetTextColor(BLUE);
	ILI9341_DrawLine(110,170,110,230);  
	ILI9341_DrawLine(130,200,220,240);

	Delay(0xFFFFFF);

	ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全黑 */


	/*画矩形*/

	LCD_ClearLine(LINE(4));	/* 清除单行文字 */
	LCD_SetTextColor(BLUE);

	ILI9341_DispStringLine_EN(LINE(4),"Draw Rect:");

	LCD_SetTextColor(RED);
	ILI9341_DrawRectangle(50,200,100,30,1);

	LCD_SetTextColor(GREEN);
	ILI9341_DrawRectangle(160,200,20,40,0);

	LCD_SetTextColor(BLUE);
	ILI9341_DrawRectangle(170,200,50,20,1);


	Delay(0xFFFFFF);

	ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全黑 */

	/* 画圆 */
	LCD_ClearLine(LINE(4));	/* 清除单行文字 */
	LCD_SetTextColor(BLUE);

	ILI9341_DispStringLine_EN(LINE(4),"Draw Cir:");

	LCD_SetTextColor(RED);
	ILI9341_DrawCircle(100,200,20,0);

	LCD_SetTextColor(GREEN);
	ILI9341_DrawCircle(100,200,10,1);

	LCD_SetTextColor(BLUE);
	ILI9341_DrawCircle(140,200,20,0);

	Delay(0xFFFFFF);

	ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全黑 */

}


/**
  * @brief  简单延时函数
  * @param  nCount ：延时计数值
  * @retval 无
  */	
static void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
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

