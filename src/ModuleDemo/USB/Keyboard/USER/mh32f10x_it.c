/**
  ******************************************************************************
  * @file    MH32_it.c
  * @author  NONE
  * @version NONE
  * @date    NONE
  * @brief   NONE
  ******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "mh32f10x_it.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t DevRemoteWakeup;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * Function Name  : NMI_Handler
  * Description    : This function handles NMI exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void NMI_Handler(void)
{
}

/**
  * Function Name  : HardFault_Handler
  * Description    : This function handles Hard Fault exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * Function Name  : MemManage_Handler
  * Description    : This function handles Memory Manage exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * Function Name  : BusFault_Handler
  * Description    : This function handles Bus Fault exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * Function Name  : UsageFault_Handler
  * Description    : This function handles Usage Fault exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * Function Name  : SVC_Handler
  * Description    : This function handles SVCall exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void SVC_Handler(void)
{
}

/**
  * Function Name  : DebugMon_Handler
  * Description    : This function handles Debug Monitor exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void DebugMon_Handler(void)
{
}

/**
  * Function Name  : PendSV_Handler
  * Description    : This function handles PendSVC exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void PendSV_Handler(void)
{
}

/**
  * Function Name  : SysTick_Handler
  * Description    : This function handles SysTick Handler.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 MH32 Peripherals Interrupt Handlers                       */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_MH32xxx.s).                                                */
/******************************************************************************/

/**
  * Function Name  : USB_IRQHandler
  * Description    : This function handles USB Low Priority interrupts
  *                  requests.
  * Input          : None
  * Output         : None
  * Return         : None
  */
#if defined(MH32L1XX_MD) || defined(MH32L1XX_HD)|| defined(MH32L1XX_MD_PLUS) || defined(MH32F37X)
void USB_LP_IRQHandler(void)
#else
void USB_LP_CAN1_RX0_IRQHandler(void)
#endif
{
  USB_Istr();
}

#if defined(MH32L1XX_MD) || defined(MH32L1XX_HD)|| defined(MH32L1XX_MD_PLUS)

/**
  * Function Name  : USB_FS_WKUP_IRQHandler
  * Description    : This function handles USB WakeUp interrupt request.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void USB_FS_WKUP_IRQHandler(void)
{
  EXTI_ClearITPendingBit(EXTI_Line18);
}
#endif

#if defined (USE_NUCLEO)
/**
  * Function Name  : EXTI_IRQHandler
  * Description    : This function handles External lines  interrupt request.
  * Input          : None
  * Output         : None
  * Return         : None
  */

void EXTI15_10_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line13) != RESET)
  {
    /* Check if the remote wakeup feature is enabled (it could be disabled 
        by the host through ClearFeature request) */
    if ((pInformation->Current_Feature & 0x20) && (DevRemoteWakeup == 0))
    {      
      pInformation->Current_Feature &= ~0x20; 
      /* Set DevRemoteWakeup when doing the remote wakeup */
      DevRemoteWakeup = 1;

      /* Exit low power mode and re-configure clocks */
      Resume(RESUME_INTERNAL);
    }
  
    /* Clear the EXTI line pending bit */
    EXTI_ClearITPendingBit(EXTI_Line13);
  }
}
#endif

#if !defined (USE_NUCLEO)
/**
  * Function Name  : EXTI_IRQHandler
  * Description    : This function handles External lines  interrupt request.
  * Input          : None
  * Output         : None
  * Return         : None
  */

#if defined(MH32L1XX_MD) || defined(MH32L1XX_HD)|| defined(MH32L1XX_MD_PLUS)
void EXTI0_IRQHandler(void)
#elif defined (MH32F37X)
void EXTI2_TS_IRQHandler(void)
#else
void EXTI9_5_IRQHandler(void)
#endif
{
  if (EXTI_GetITStatus(EXTI_Line8) != RESET)
  {
    /* Check if the remote wakeup feature is enabled (it could be disabled 
        by the host through ClearFeature request) */
    if ((pInformation->Current_Feature & 0x20) && (DevRemoteWakeup == 0))
    {      
      pInformation->Current_Feature &= ~0x20;  
      /* Set DevRemoteWakeup when doing the remote wakeup */
      DevRemoteWakeup = 1;

      /* Exit low power mode and re-configure clocks */
      Resume(RESUME_INTERNAL);
    }
  
    /* Clear the EXTI line pending bit */
    EXTI_ClearITPendingBit(EXTI_Line9);
  }
}
#endif /* USE_NUCLEO */
/**
  * Function Name  : USBWakeUp_IRQHandler
  * Description    : This function handles USB WakeUp interrupt request.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void USBWakeUp_IRQHandler(void)
{
  EXTI_ClearITPendingBit(EXTI_Line18);
}

#if defined(USE_MH3210E_EVAL)
  #if defined(USB_USE_VBUS_SENSING)

/**
  * Function Name  : EXTI_IRQHandler
  * Description    : This function handles External lines  interrupt request for VBUS.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void EXTI0_IRQHandler(void)
{
  /* Check The presence of VBUS */ 
  if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == SET) 
  {
    /* Normal USB operation: call USB_Init when cable connection */
    USB_Init();
  }
  else
  {
    /* Stop USB operation: call PowerOff when cable is disconected*/
    PowerOff();
  }
  
  EXTI_ClearITPendingBit(EXTI_Line0); 
}

  #endif /* USB_USE_VBUS_SENSING */
#endif /* USE_MH3210E_EVAL */

/******************* (C) COPYRIGHT  MHSEMICONDUCTOR *****END OF FILE****/
