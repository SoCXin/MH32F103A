#include "hw_config.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_istr.h"
#include "usb_pwr.h" 
#include "string.h"	
#include "stdarg.h"		 
#include "stdio.h"	

uint8_t  USB_Printer_Rx_Buffer [USART_RX_DATA_SIZE];
uint8_t  USB_Printer_Tx_Buffer [USART_TX_DATA_SIZE];
_usb_printer_fifo prt_txfifo = {USB_Printer_Tx_Buffer, 0, 0};		//USB打印机发送FIFO
_usb_printer_fifo prt_rxfifo= {USB_Printer_Rx_Buffer, 0, 0};		//USB打印机接收FIFO

//USB唤醒中断服务函数
void USBWakeUp_IRQHandler(void) 
{
	EXTI_ClearITPendingBit(EXTI_Line18);//清除USB唤醒中断挂起位
} 

//USB中断处理函数
void USB_LP_CAN1_RX0_IRQHandler(void) 
{
	USB_Istr();
} 

//USB时钟配置函数,USBclk=48Mhz@HCLK=72Mhz
void Set_USBClock(void)
{
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_4Div5);//USBclk=PLLclk/1.5=48Mhz	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);	 //USB时钟使能		 
} 

//USB进入低功耗模式
//当USB进入suspend模式时,MCU进入低功耗模式
//需自行添加低功耗代码(比如关时钟等)
void Enter_LowPowerMode(void)
{
 	printf("usb enter low power mode\r\n");
	bDeviceState=SUSPENDED;
} 

//USB退出低功耗模式
//用户可以自行添加相关代码(比如重新配置时钟等)
void Leave_LowPowerMode(void)
{
	DEVICE_INFO *pInfo=&Device_Info;
	printf("leave low power mode\r\n"); 
	if (pInfo->Current_Configuration!=0)bDeviceState=CONFIGURED; 
	else bDeviceState = ATTACHED; 
} 

//USB中断配置
void USB_Interrupts_Config(void)
{ 
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 
	/* Configure the EXTI line 18 connected internally to the USB IP */
	EXTI_ClearITPendingBit(EXTI_Line18);
											  //  开启线18上的中断
	EXTI_InitStructure.EXTI_Line = EXTI_Line18; // USB resume from suspend mode
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//line 18上事件上升降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_Init(&EXTI_InitStructure); 	 

	/* Enable the USB interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	//组2，优先级次之 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USB Wake-up interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;   //组2，优先级最高	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&NVIC_InitStructure);   
}	

//USB接口配置
//NewState:DISABLE,不上拉
//         ENABLE,上拉
void USB_Cable_Config (FunctionalState NewState)
{ 
	//if (NewState!=DISABLE)printf("usb pull up enable\r\n"); 
	//else printf("usb pull up disable\r\n"); 
}

//USB使能连接/断线
//enable:0,断开
//1,允许连接
void USB_Port_Set(u8 enable)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    //使能PORTA时钟
	if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//退出断电模式
	else
	{
		_SetCNTR(_GetCNTR()|(1<<1));  // 断电模式
		GPIOA->CRH&=0XFFF00FFF;
		GPIOA->CRH|=0X00033000;
		PAout(12)=0;
	}
}

//用于USB配置信息
void Get_SerialNum(void)
{
	u32 Device_Serial0, Device_Serial1, Device_Serial2;
	Device_Serial0 = *(u32*)(0x1FFFF7E8);
	Device_Serial1 = *(u32*)(0x1FFFF7EC);
	Device_Serial2 = *(u32*)(0x1FFFF7F0);
	Device_Serial0 += Device_Serial2;
	if (Device_Serial0 != 0)
	{
	    Virtual_Com_Port_StringSerial[2] = (u8)(Device_Serial0 & 0x000000FF);
	    Virtual_Com_Port_StringSerial[4] = (u8)((Device_Serial0 & 0x0000FF00) >> 8);
	    Virtual_Com_Port_StringSerial[6] = (u8)((Device_Serial0 & 0x00FF0000) >> 16);
	    Virtual_Com_Port_StringSerial[8] = (u8)((Device_Serial0 & 0xFF000000) >> 24);

	    Virtual_Com_Port_StringSerial[10] = (u8)(Device_Serial1 & 0x000000FF);
	    Virtual_Com_Port_StringSerial[12] = (u8)((Device_Serial1 & 0x0000FF00) >> 8);
	    Virtual_Com_Port_StringSerial[14] = (u8)((Device_Serial1 & 0x00FF0000) >> 16);
	    Virtual_Com_Port_StringSerial[16] = (u8)((Device_Serial1 & 0xFF000000) >> 24);

	    Virtual_Com_Port_StringSerial[18] = (u8)(Device_Serial2 & 0x000000FF);
	    Virtual_Com_Port_StringSerial[20] = (u8)((Device_Serial2 & 0x0000FF00) >> 8);
	    Virtual_Com_Port_StringSerial[22] = (u8)((Device_Serial2 & 0x00FF0000) >> 16);
	    Virtual_Com_Port_StringSerial[24] = (u8)((Device_Serial2 & 0xFF000000) >> 24);
	}
} 

//将32位的值转换成unicode.
//value,要转换的值(32bit)
//pbuf:存储地址
//len:要转换的长度
void IntToUnicode (u32 value , u8 *pbuf , u8 len)
{
	u8 idx = 0;
	for( idx = 0 ; idx < len ; idx ++)
	{
		if( ((value >> 28)) < 0xA )
		{
			pbuf[ 2* idx] = (value >> 28) + '0';
		}
		else
		{
			pbuf[2* idx] = (value >> 28) + 'A' - 10; 
		} 
		value = value << 4; 
		pbuf[ 2* idx + 1] = 0;
	}
}
/////////////////////////////////////////////////////////////////////////////////
 
//USB COM口的配置信息,通过此函数打印出来. 
bool USB_Buffer_Config(void)
{
	prt_txfifo.readptr=0;	//清空读指针
	prt_txfifo.writeptr=0;	//清空写指针
	prt_txfifo.buffer= USB_Printer_Tx_Buffer;

	memset(prt_txfifo.buffer, 0x00, sizeof(USART_TX_DATA_SIZE));

	prt_rxfifo.readptr=0;	//清空读指针
	prt_rxfifo.writeptr=0;	//清空写指针
	prt_rxfifo.buffer= USB_Printer_Rx_Buffer;
	memset(prt_rxfifo.buffer, 0x00, sizeof(USART_RX_DATA_SIZE));
	//printf("linecoding.format:%d\r\n", prt_rxfifo.readptr);
  	//printf("linecoding.paritytype:%d\r\n",linecoding.paritytype);
	//printf("linecoding.datatype:%d\r\n",linecoding.datatype);
	//printf("linecoding.bitrate:%d\r\n",linecoding.bitrate);
	return (TRUE);
}
 
//处理从USB 打印接收到的数据
//databuffer:数据缓存区
//Nb_bytes:接收到的字节数.
int USB_Printer_RecvData(u8* data_buffer, u32 Nb_bytes)
{
	int i, recv_byte = 0;
	recv_byte = RING_CNT(prt_rxfifo.writeptr, prt_rxfifo.readptr, USART_RX_DATA_SIZE);
	recv_byte = recv_byte > Nb_bytes ? Nb_bytes : recv_byte;

    for(i = 0; i < recv_byte; i++) {
        data_buffer[i] = prt_rxfifo.buffer[prt_rxfifo.readptr++];
        if(prt_rxfifo.readptr == USART_RX_DATA_SIZE)  prt_rxfifo.readptr = 0;
    }

	//memcpy(data_buffer, (u8 *)&prt_rxfifo.buffer[prt_rxfifo.readptr], recv_byte);
	//prt_rxfifo.readptr = (prt_rxfifo.readptr + recv_byte)%USART_RX_DATA_SIZE;

	return recv_byte;
} 

int USB_Printer_GetRecvLen( void)
{
	int recv_byte = 0;
	recv_byte = RING_CNT(prt_rxfifo.writeptr, prt_rxfifo.readptr, USART_RX_DATA_SIZE);
	return recv_byte;
}

//发送数据数据到USB打印
int  USB_Printer_WriterData(u8 *data, u32 Nb_bytes)
{
	int w_byte = 0; //, pack_num = 0;
	int i = 0;

	if( bDeviceState != CONFIGURED ) return 0;


	w_byte = RING_SPACE(prt_txfifo.writeptr, prt_txfifo.readptr, USART_TX_DATA_SIZE);
	if(w_byte == 0) return 0;

	w_byte = Nb_bytes > w_byte ? w_byte : Nb_bytes;
	for(i = 0; i < w_byte; i++) {
		prt_txfifo.buffer[prt_txfifo.writeptr++] = data[i];
		if(prt_txfifo.writeptr == USART_TX_DATA_SIZE)  prt_txfifo.writeptr = 0;
	}
	//memcpy((u8 *)prt_txfifo.buffer[prt_txfifo.writeptr], (u8 *)data, w_byte);
	//prt_txfifo.writeptr = (prt_txfifo.writeptr + w_byte)%USART_TX_DATA_SIZE;

	return w_byte;
}

#if 0
char USART_PRINTF_Buffer[512];
//usb虚拟串口,printf 函数
//确保一次发送数据不超USB_USART_REC_LEN字节
void usb_printf(char* fmt,...)  
{  
	u16 i;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART_PRINTF_Buffer,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART_PRINTF_Buffer);//此次发送数据的长度
	USB_Printer_WriterData((u8 *)USART_PRINTF_Buffer, i); 

} 
#endif
