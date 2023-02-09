/**
  ******************************************************************************
  * @file    opt_if.h
  * @author  NONE
  * @version NONE
  * @date    NONE
  * @brief   NONE
  ******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OPT_IF_MAL_H
#define __OPT_IF_MAL_H

/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

uint16_t OPT_If_Init(void);
uint16_t OPT_If_Erase (uint32_t SectorAddress);
uint16_t OPT_If_Write(uint32_t SectorAddress, uint32_t DataLength);
uint8_t *OPT_If_Read (uint32_t SectorAddress, uint32_t DataLength);

#endif /* __FLASH_IF_MAL_H */

/******************* (C) COPYRIGHT  MHSEMICONDUCTOR *****END OF FILE****/
