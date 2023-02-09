#ifndef __CAN_H
#define __CAN_H	 
#include "mh32f10x.h"
 
//CAN接收RX0中断使能
#define CAN_RX0_INT_ENABLE	0		//0,不使能;1,使能.								    
										 							 				    
uint8_t CAN_Mode_Init(uint8_t tsjw,uint8_t tbs2,uint8_t tbs1,uint16_t brp,uint8_t mode);
uint8_t Can_Send_Msg(uint8_t* msg,uint8_t len);
uint8_t Can_Receive_Msg(u8 *buf);
#endif

















