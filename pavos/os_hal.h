/**
 ******************************************************************************
 * @file os_hal.h
 *
 * @date 3 Mar 2020
 * @author mehmet.alinbay
 * @brief
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) PAVOTEK
 * All rights reserved.</center></h2>
 *
 *******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OS_HAL_H_
#define OS_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/** @addtogroup PAVOS
  * @{
  */

/** @addtogroup OS_HAL
  * @{
  */
/* Exported types ------------------------------------------------------------*/
/** @defgroup OS_HAL_Exported_Types Exported Types
  * @{
  */

/**
  * @}
  */
/* Exported constants --------------------------------------------------------*/
/** @defgroup OS_HAL_Exported_Constants Exported Constants
  * @{
  */

/**
  * @}
  */
/* Exported macro ------------------------------------------------------------*/
/** @defgroup OS_HAL_Exported_Macros Exported Macros
  * @{
  */

/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @addtogroup OS_HAL_Exported_Functions Exported Functions
  * @{
  */

void pOS_HAL_InitTick(void);
void pOS_HAL_DisableInterrupt(void);
void pOS_HAL_EnableInterrupt(void);
void pOS_HAL_ResetWDT(void);
void pOS_HAL_InitWDT(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* OS_HAL_H_ */
