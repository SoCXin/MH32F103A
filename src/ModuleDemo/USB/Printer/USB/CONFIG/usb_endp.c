/**
  ******************************************************************************
  * @file    usb_endp.c
  * @author  NONE
  * @version NONE
  * @date    NONE
  * @brief   NONE
  ******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
#include "usb_pwr.h"

#include <stdio.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Interval between sending IN packets in frame number (1 frame = 1ms) */
#define VCOMPORT_IN_FRAME_INTERVAL             5
uint8_t USB_Rx_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback (void)
{
	u16 i, USB_Tx_ptr;
	u16 USB_Tx_length; 
	u8 tx_buf[VIRTUAL_COM_PORT_DATA_SIZE];

	USB_Tx_ptr = prt_txfifo.readptr;
	USB_Tx_length = RING_CNT(prt_txfifo.writeptr, USB_Tx_ptr, USART_TX_DATA_SIZE);
	if(USB_Tx_length == 0) return;

	USB_Tx_length = USB_Tx_length > VIRTUAL_COM_PORT_DATA_SIZE ? VIRTUAL_COM_PORT_DATA_SIZE: USB_Tx_length;

	for(i = 0; i < USB_Tx_length; i++)
	{
		tx_buf[i] =  prt_txfifo.buffer[USB_Tx_ptr++];
		if(USB_Tx_ptr == USART_TX_DATA_SIZE) USB_Tx_ptr = 0;
	}
	prt_txfifo.readptr = (prt_txfifo.readptr + USB_Tx_length)%USART_TX_DATA_SIZE;

	//printf("%s o:%d-%d-%d\r\n", __func__, USB_Tx_ptr, prt_txfifo.readptr, USB_Tx_length);

	UserToPMABufferCopy(tx_buf, GetEPTxAddr(ENDP1), USB_Tx_length);
	SetEPTxCount(ENDP1, USB_Tx_length);
	SetEPTxValid(ENDP1);   

}

/*******************************************************************************
* Function Name  : EP2_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP2_OUT_Callback(void)
{
#if 1
	u16 USB_push_Cnt;
	u16 USB_Rx_Cnt, i;

	USB_Rx_Cnt = USB_SIL_Read(EP2_OUT, USB_Rx_Buffer);	//得到USB接收到的数据及其长度  
	USB_push_Cnt = RING_SPACE(prt_rxfifo.writeptr, prt_rxfifo.readptr, USART_RX_DATA_SIZE);

	//printf("%s o:%d-%d-%d\r\n", __func__, USB_Rx_Cnt, USB_push_Cnt, prt_rxfifo.writeptr);
	if(USB_push_Cnt < USB_Rx_Cnt)
		prt_rxfifo.readptr = (prt_rxfifo.readptr +  (USB_Rx_Cnt - USB_push_Cnt))%USART_RX_DATA_SIZE;
    
    for(i = 0; i < USB_Rx_Cnt; i++ ) {
        prt_rxfifo.buffer[prt_rxfifo.writeptr++] = USB_Rx_Buffer[i];
        if(prt_rxfifo.writeptr == USART_RX_DATA_SIZE) prt_rxfifo.writeptr = 0;

    }
	//prt_rxfifo.writeptr = (prt_rxfifo.writeptr +  USB_Rx_Cnt)%USART_RX_DATA_SIZE;

	SetEPRxValid(ENDP2);								//使能端点3的数据接收
#endif
}

#if 0
/*******************************************************************************
* Function Name  : EP3_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP3_OUT_Callback(void)
{

	u16 USB_push_Cnt;
	u16 USB_Rx_Cnt, i;

	USB_Rx_Cnt = USB_SIL_Read(EP3_OUT, USB_Rx_Buffer);	//得到USB接收到的数据及其长度  
	USB_push_Cnt = RING_SPACE(prt_rxfifo.writeptr, prt_rxfifo.readptr, USART_RX_DATA_SIZE);

	//printf("%s o:%d-%d-%d\r\n", __func__, USB_Rx_Cnt, USB_push_Cnt, prt_rxfifo.writeptr);
	if(USB_push_Cnt < USB_Rx_Cnt)
		prt_rxfifo.readptr = (prt_rxfifo.readptr +  (USB_Rx_Cnt - USB_push_Cnt))%USART_RX_DATA_SIZE;
    
    for(i = 0; i < USB_Rx_Cnt; i++ ) {
        prt_rxfifo.buffer[prt_rxfifo.writeptr++] = USB_Rx_Buffer[i];
        if(prt_rxfifo.writeptr == USART_RX_DATA_SIZE) prt_rxfifo.writeptr = 0;

    }
	//prt_rxfifo.writeptr = (prt_rxfifo.writeptr +  USB_Rx_Cnt)%USART_RX_DATA_SIZE;

	SetEPRxValid(ENDP3);								//使能端点3的数据接收

}
#endif
/*******************************************************************************
* Function Name  : SOF_Callback / INTR_SOFINTR_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SOF_Callback(void)
{
	static uint32_t FrameCount = 0;

	if(bDeviceState == CONFIGURED)
	{
		if (FrameCount++ == VCOMPORT_IN_FRAME_INTERVAL)
		{
			/* Reset the frame counter */
			FrameCount = 0;

			/* Check the data to be sent through IN pipe */
			EP1_IN_Callback();//通过EP1_IN_Callback函数实现TX数据发送给USB
			//Handle_USBAsynchXfer();
		}
	}  
}
/******************* (C) COPYRIGHT  MHSEMICONDUCTOR *****END OF FILE****/

