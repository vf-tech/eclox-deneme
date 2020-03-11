/**
 ******************************************************************************
 * @file os_conf.h
 *
 * @date 03-03-2020
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
#ifndef __OS_CONF_H
#define __OS_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/** @addtogroup PAVOS
  * @{
  */
/* OS Runtime Constants ------------------------------------------------------*/
/** @defgroup PAVOS_Runtime_Constants Runtime Constants
  * @{
  */

/**
 * @brief Define Max. OS Messages Here
 */
#define osMaxMessagePool        10
/**
 * @brief Define Max. OS Task Here
 */
#define osMaxTaskFunPool        10
/**
 * @brief Define Max. OS Periodic Func. Here
 */
#define osMaxPeriodicFunPool    10
/**
 * @brief Define Max. OS Signal Here
 */
#define osMaxSignalTimeoutPool  10
/**
  * @}
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __OS_CONF_H */

/*****************************END OF FILE**************************************/
