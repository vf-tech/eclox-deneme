/**
  ******************************************************************************
  * @file    os_hal.c
  * @author  Mehmet Alinbay
  * @version v0.0.1
             Initial Release
  * @date    14-11-2019
  * @brief   OS MCU Specific Functions
  *
  @verbatim
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2019 </center></h2>
  *
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <os.h>

/* === BOARD SPECIFIC CONFIGURATION CODE BEGIN === */
/* MCU Specific Includes */
//#include "stm32f0xx.h"
//#include "stm32f0xx_ll_iwdg.h"
/* ==== BOARD SPECIFIC CONFIGURATION CODE END ==== */

/** @addtogroup PAVOS
  * @{
  */

/** @defgroup OS_HAL PAVOS HAL
  * @brief MCU Specific Functions
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup OS_HAL_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  Inits Timer
  * @note   Init Timer for 1mS Interrupt
  */
void pOS_HAL_InitTick(void)
{
  //SysTick_Config(SystemCoreClock / 1000);
  //NVIC_SetPriority(SysTick_IRQn, 0);
  //NVIC_EnableIRQ(SysTick_IRQn);
}

/**
  * @brief  Disables All Interrupt
  */
void pOS_HAL_DisableInterrupt(void)
{
  //__disable_irq();
}

/**
  * @brief  Enables All Interrupt
  */
void pOS_HAL_EnableInterrupt(void)
{
  //__enable_irq();
}

/**
  * @brief  Init WDT
  */
void pOS_HAL_InitWDT(void)
{

}

/**
  * @brief  Reset WDT, This function called by pOS_Scheduler
  */
void pOS_HAL_ResetWDT(void)
{

}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
