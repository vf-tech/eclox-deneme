/**
 ******************************************************************************
 * @file 	os.h
 *
 * @date 	03-03-2020
 * @author 	mehmet.alinbay
 * @brief	OS common header file for all modules
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) PAVOTEK
 * All rights reserved.</center></h2>
 *
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OS_COMMON_H
#define __OS_COMMON_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* C-LIB */
#include  "string.h"
#include  "stdint.h"
#include  "stdio.h"
/* OS Configuration */
#include  "os_conf.h"
//
#include  "os_cirbuf.h"

/** @addtogroup PAVOS
  * @{
  */
/* Exported constants --------------------------------------------------------*/
/** @defgroup PAVOS_Exported_Constants Exported Constants
  * @{
  */

/**
 * @brief PAVOS Message Data Field Length in Bytes
 */
#define osMaxOSMessageDataLen   (64)

/**
  * @}
  */
/* Exported types ------------------------------------------------------------*/
/** @defgroup PAVOS_Exported_Types Exported Types
  * @{
  */

/**
 * @brief PAVOS Core Status Code
 */
typedef enum
{
  osCoreReset = 0,//!< osCoreReset
  osCoreRun    ,  //!< osCoreRun
}osCoreStatus_t;

/**
 * @brief PAVOS Timer Type
 */
enum
{
	osTimerSingleShot =  1,//!< osTimerSingleShot
	osTimerPeriodic   = -1,//!< osTimerPeriodic
};

/**
 * @brief PAVOS Task State
 */
typedef enum
{
  osTaskState_Reset = 0,//!< osTaskState_Reset
  osTaskState_Idle  ,   //!< osTaskState_Idle
  osTaskState_Active,   //!< osTaskState_Active
  osTaskState_Error ,   //!< osTaskState_Error
}osTaskState_t;

/**
 * @brief PAVOS Error Code
 */
typedef enum
{
	osErrorWrongTask = 0x00,//!< osErrorWrongTask
	osErrorWrongSignal,     //!< osErrorWrongSignal
	osErrorMessageQueFull,  //!< osErrorMessageQueFull
	osErrorMessageNotSent,  //!< osErrorMessageNotSent
	osErrorUndefined,       //!< osErrorUndefined
}osErrorType_t;

/**
  * @struct	osMessage_t
  * @brief 	PAVOS Message Type
  */
typedef struct
{
	/*! Allocated Flag */
	uint8_t   allocated;
	/*! Destination Task */
	uint32_t  destTask;
	/*! Signal to Sent */
	uint32_t  signal;
	/*! Data to Sent */
	uint8_t   data[osMaxOSMessageDataLen];
	/*! Length of Data */
	uint8_t   data_length;
}osMessage_t;

/**
  * @brief PAVOS Init Task Function Typedef
  */
typedef void(*osInitTaskFunc_t)(void);

/**
  * @brief PAVOS Run Task Function Typedef
  */
typedef void(*osRunTaskFunc_t)(osMessage_t *message);

/**
  * @brief PAVOS Periodic Function Typedef
  */
typedef void(*osPeriodicFunc_t)(uint32_t arg);

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup PAVOS_Exported_Macros Exported Macros
  * @{
  */

/**
 * @brief Get Count of Element in Any Array
 */
#define mCOUNTOFARRAY(x)    ( sizeof(x) / sizeof(x[0]) )

/**
 * @brief Set Bit of Any
 */
#define mBITSET(REG,BIT)    ( REG |= (1U<<BIT) )

/**
 * @brief Reset Bit of Any
 */
#define mBITRESET(REG,BIT)  ( REG &= ~(1U<<BIT) )

/**
 * @brief Mask Bit of Any
 */
#define mBITMASK(REG,BIT)   ( REG & (1U<<BIT) )

/**
 * @brief Read Bit of Any, Returns 1 or 0
 */
#define mBITREAD(REG,BIT)   ( (REG & (1U<<BIT)) ? 1U : 0U )
/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @addtogroup PAVOS_Exported_Functions
  * @{
  */

void pOS_InitCore(void);
void pOS_Scheduler(void);
void pOS_Tick(void);
//
int32_t pOS_AddTask(osInitTaskFunc_t  init,
                     osRunTaskFunc_t   run);
int32_t pOS_SendSignal( uint32_t destTask,
					    uint32_t signal,
						uint8_t  *data,
					    uint8_t  data_length);
// Periodic Function
int32_t pOS_StartPeriodicFunc( osPeriodicFunc_t callback,
                               uint32_t         arg,
                               int32_t          repeat,
                               uint32_t         period );
int32_t pOS_StopPeriodicFunc( int32_t pfoID );
//
void pOS_LogError(osErrorType_t error);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __OS_COMMON_H */
