#include "Internal_Flash.h"
#define INTERNAL_FLASH_BASE     (uint32_t)0x08008000
uint16_t Internal_Flash_Init(void);
uint16_t Internal_WriteMultiBlocks(uint32_t Memory_Offset, uint8_t *Writebuff, uint16_t Transfer_Length);
uint16_t Internal_ReadMultiBlocks(uint32_t Memory_Offset, uint8_t *Readbuff, uint16_t Transfer_Length);
uint16_t Internal_Flash_GetStatus(uint8_t lun);

uint8_t Internal_Flash_Inquiry_Data[] =
{
	0x00,          /* Direct Access Device */
	0x80,          /* RMB = 1: Removable Medium */
	0x02,          /* Version: No conformance claim to standard */
	0x02,

	36 - 4,          /* Additional Length */
	0x00,          /* SCCS = 1: Storage Controller Component */
	0x00,
	0x00,
	/* Vendor Identification */
	'M', 'H', ' ', ' ', ' ', ' ', ' ', ' ',
	/* Product Identification */
	'I', 'n', 't', 'e', 'r', '-', 'F', 'l', 'a', 's', 'h', ' ',
	'D', 'i', 's', 'k',
	/* Product Revision Level */
	'1', '.', '0', ' '
};
  
t_Mal_Lun Internal_Flash = 
{
    Internal_Flash_Init,
    Internal_WriteMultiBlocks,
    Internal_ReadMultiBlocks,
    Internal_Flash_GetStatus,
	Internal_Flash_Inquiry_Data,
};

uint16_t Internal_Flash_Init(void)
{
    return MAL_OK;
}

uint16_t Internal_WriteMultiBlocks(uint32_t Memory_Offset, uint8_t *Writebuff, uint16_t Transfer_Length)
{
	uint32_t Addr = INTERNAL_FLASH_BASE + Memory_Offset;
	FLASH_Unlock();
	FLASH_ErasePage(Addr);
	for(uint16_t i = 0; i < Transfer_Length; i += 2)
	{
		FLASH_ProgramHalfWord(Addr,*((u16 *)(&Writebuff[i])));
		Addr += 2;
	}
	FLASH_Lock();
//	memcpy(((uint8_t *)(Memory_Offset + INTERNAL_FLASH_BASE)),Writebuff,Transfer_Length);
	return MAL_OK;
}

uint16_t Internal_ReadMultiBlocks(uint32_t Memory_Offset, uint8_t *Readbuff, uint16_t Transfer_Length)
{
	memcpy(Readbuff,((uint8_t *)(Memory_Offset + INTERNAL_FLASH_BASE)),Transfer_Length);
	return MAL_OK;
}

uint16_t Internal_Flash_GetStatus(uint8_t lun)
{
    Mass_Block_Count[lun] = 20;
    Mass_Block_Size[lun] = 0x1000;
	return MAL_OK;
}


