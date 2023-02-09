#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "system.h"
#include "delay.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "mh.h"

USART_TypeDef* USART_TEST = USART1;
void UART_Configuration(uint32_t bound);
void RCC_ClkConfiguration(void);

static u8 temp_buf[2048];
int main()
{
    int ret;
    uint16_t times=0, recv_len = 0;
    uint32_t  send_off = 0;
    uint8_t usbstatus=0;
    RCC_ClocksTypeDef clocks;
    
    
    RCC_ClkConfiguration();
	Delay_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	UART_Configuration(115200);
	RCC_GetClocksFreq(&clocks);
	
	printf("\n");
	printf("USB Printer\n");
	printf("SYSCLK: %3.1fMhz, HCLK: %3.1fMhz, PCLK1: %3.1fMhz, PCLK2: %3.1fMhz, ADCCLK: %3.1fMhz\n", \
	(float)clocks.SYSCLK_Frequency/1000000, (float)clocks.HCLK_Frequency/1000000, \
	(float)clocks.PCLK1_Frequency/1000000, (float)clocks.PCLK2_Frequency / 1000000, (float)clocks.ADCCLK_Frequency / 1000000);
	
	Delay_Ms(1800);
	USB_Port_Set(0); 	//USB先断开
	Delay_Ms(700);
	USB_Port_Set(1);	//USB再次连接
 	Set_USBClock();   
 	USB_Interrupts_Config();    
 	USB_Init();	    
	DP_PUUP = 1;
    
	while(1)
	{
		if(usbstatus!=bDeviceState)//USB连接状态发生了改变.
		{
			usbstatus=bDeviceState;//记录新的状态
			if(usbstatus==CONFIGURED)
			{
				printf("USB connect ok \n");
			}else
			{
				printf("USB diconnect\n");
			}
		}
		recv_len = USB_Printer_GetRecvLen();
		if(recv_len > 0)
		{
            ret = USB_Printer_RecvData(temp_buf, 1024);
            printf("USB recv data len %d-%d\n", recv_len, ret);
            if (temp_buf[0] == 0x10 && temp_buf[1] == 0x04)
            {
                uint8_t txBuffer[1];
                switch (temp_buf[2])
                {
                case 0x01:
                    txBuffer[0] = 0x16;
                    break;
                case 0x02:
                    txBuffer[0] = 0x12;
                    break;
                case 0x04:
                    txBuffer[0] = 0x12;
                    break;
                case 0x05:
                    txBuffer[0] = 0x14;
                    break;
                }
                ret = USB_Printer_WriterData(txBuffer, 1);
                if(1 != ret)printf("USB retry status error %d\n", ret);
            }
            else {
                for(int i = 0 ; i < ret; i++) printf("%02X ", temp_buf[i]);
                printf("\r\n");
                send_off = 0;
                while(send_off < recv_len) {
                    ret = USB_Printer_WriterData(temp_buf + send_off, recv_len - send_off);
                    if(ret < 0) break;
                    else if (ret == 0) Delay_Ms(10);
                    else send_off += ret;
                }
                if(recv_len != send_off)printf("USB send data len Not Ok %d--%d\n", recv_len, send_off);
            }
		}else
		{
			times++;
//			if(times%200==0)usb_printf("请输入数据,以回车键结束\r\n");  
			Delay_Ms(10);   
		}	
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

void RCC_ClkConfiguration(void)
{
	RCC_DeInit();

	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	
	RCC_PLLCmd(DISABLE);
	MH_RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_27,1);
	
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

