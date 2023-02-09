#ifndef __EEPROM_H__
#define __EEPROM_H__
#include "mh32f10x.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define MH_FLASH_SIZE 512 	 		//所选MH32的FLASH容量大小(单位为K)
#define MH_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define MH_FLASH_BASE 0x08000000 	//MH32 FLASH的起始地址
//FLASH解锁键值
 

u16 MHFLASH_ReadHalfWord(u32 faddr);		  //读出半字  
void MHFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
u32 MHFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//指定地址开始读取指定长度数据
void MHFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
void MHFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据

//测试写入
void Test_Write(u32 WriteAddr,u16 WriteData);								   
#endif

















