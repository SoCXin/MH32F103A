/**
  ******************************************************************************
  * @file    system_MH32f10x.h
  * @author  NONE
  * @version NONE
  * @date    NONE
  * @brief   NONE
  ******************************************************************************/

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup MH32f10x_system
  * @{
  */  
  
/**
  * @brief Define to prevent recursive inclusion
  */
#ifndef __SYSTEM_MH32F10X_H
#define __SYSTEM_MH32F10X_H

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup MH32F10x_System_Includes
  * @{
  */

/**
  * @}
  */


/** @addtogroup MH32F10x_System_Exported_types
  * @{
  */

extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */

/**
  * @}
  */

/** @addtogroup MH32F10x_System_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @addtogroup MH32F10x_System_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup MH32F10x_System_Exported_Functions
  * @{
  */
  
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_MH32F10X_H */

/**
  * @}
  */
  
/**
  * @}
  */  
/******************* (C) COPYRIGHT  MHSEMICONDUCTOR *****END OF FILE****/
