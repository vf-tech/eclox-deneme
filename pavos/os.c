/**
 ******************************************************************************
 * @file os.c
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

/* Includes ------------------------------------------------------------------*/
#include "os.h"
#include "os_hal.h"

/** @defgroup PAVOS PAVOS
  * @brief PAVOS is a co-operative OS for internal usage.
  * @{
  */

/* Private types -------------------------------------------------------------*/
/** @defgroup PAVOS_Private_Types Private Types
  * @{
  */

/**
 * @struct	osTask_t
 * @brief 	PAVOS TaskObject
 */
typedef struct
{
	/*! Init Task Pointer */
	osInitTaskFunc_t  initTask;
	/*! Run Task Pointer */
	osRunTaskFunc_t   runTask;
}osTask_t;

/**
 * @struct	osPeriodicFunction_t
 * @brief 	PAVOS PeriodicFunctionObject
 */
typedef struct
{
	/*! Callback Function */
	osPeriodicFunc_t  callback;
	/*! Repeat Counter: -1 infinite */
	int32_t           repeat;
	/*! Overload Indicator */
	uint32_t          overload;
	/*! Period Register (in tick) */
	uint32_t          period;
	/*! Remaining Register (in tick) */
	uint32_t          remaining;
	/*! Arguments to be passed */
	uint32_t          arg;
}osPeriodicFunction_t;

/**
  * @struct	osCore_t
  * @brief 	PAVOS Control Struct
  */
typedef struct
{
	/*! OS Status */
	osCoreStatus_t        	status;
	/*! OS Tick */
	uint32_t              	tick;
	/*! Stores Max. Pending Message for Debugging */
	uint16_t              	maxPendingMessage;
	/*! Stores Error Count */
	uint32_t              	totalOSError;
	/*! Stores Error Count according to Types */
	uint32_t              	logOSError[osErrorUndefined];
	/*! Message Head Pointer (Last Processed) */
	osMessage_t           	*osMessageHead;
	/*! Message Tail Pointer (Last Added) */
	osMessage_t           	*osMessageTail;
	/*! Stores Pending Message Count */
	uint32_t              	osMessageInUse;
	/*! Message Pool */
	osMessage_t           	osMessagePool[osMaxMessagePool];
	/*! Periodic Function Pool */
	osPeriodicFunction_t  	osPeriodicFunctionPool[osMaxPeriodicFunPool];
	/*! Task Pool */
	osTask_t  				osTaskPool[osMaxTaskFunPool];
}osCore_t;

/**
  * @}
  */
/* Private variables ---------------------------------------------------------*/
/** @defgroup PAVOS_Private_Variables Private Variables
  * @{
  */

/**
  * @brief PAVOS Core Variable
  */
osCore_t  osCore;

/**
  * @}
  */
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup PAVOS_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  Inits OS Core variables
  * @note   Should be Called at startup
  */
void pOS_InitCore(void)
{
  uint16_t counter = 0;
  osCoreStatus_t local_status_flag = osCoreReset;

  if(osCore.status == osCoreRun)                //if OS Running, Stop it
  {
    local_status_flag = osCore.status;          //backup OS status
    osCore.status = osCoreReset;              //stop OS
  }
  // Clear OS Core Variables (except Task Pool)
  memset(osCore.logOSError, 0x00, sizeof(osCore.logOSError));
  osCore.maxPendingMessage = 0;
  osCore.osMessageInUse = 0;
  memset(osCore.osMessagePool, 0x00, sizeof(osCore.osMessagePool));
  memset(osCore.osPeriodicFunctionPool, 0x00, sizeof(osCore.osPeriodicFunctionPool));
  osCore.tick = 0;
  osCore.totalOSError = 0;
  osCore.osMessageHead = &osCore.osMessagePool[0];
  osCore.osMessageTail = &osCore.osMessagePool[0];

  // Call Init Tasks
  counter = 0;
  while(osCore.osTaskPool[counter].initTask != NULL)
  {
    osCore.osTaskPool[counter].initTask();
    counter++;
  }

  // Re-Load OS Status
  if(local_status_flag != osCoreReset)
  {
    osCore.status = local_status_flag;
  }
  pOS_HAL_InitTick();
  pOS_HAL_InitWDT();
  osCore.status = osCoreRun;
}

/**
  * @brief  Add Task Function, finds an empty slot, places task
  * @param  init    : Task Init Function
  * @param  run     : Task Message Handler Dunction
  * @retval  (-1) 	: Not applicable
  * @retval  else 	: Task ID
  */
int32_t pOS_AddTask( osInitTaskFunc_t  init,
				 	 osRunTaskFunc_t   run )
{
  osTask_t *to = &osCore.osTaskPool[0];
  if( (init == NULL) || (run == NULL) )
  {
    return -1;
  }
  pOS_HAL_DisableInterrupt();
  //
  while( to < &osCore.osTaskPool[osMaxTaskFunPool] )
  {
    if( ( to->initTask == NULL ) && ( to->runTask == NULL ) )
    {
      to->initTask = init;
      to->runTask = run;
      pOS_HAL_EnableInterrupt();
      return (int32_t)to;
    }
    to++;
  }
  pOS_HAL_EnableInterrupt();
  return -1;
}

/**
  * @brief  Send Signal, finds an empty slot, places signal
  * @param  destTask    : Signal Dest. Task
  * @param  signal      : Signal to Send
  * @param  data        : Pointer of Message Data
  * @param  data_length : Message Data Length
  * @retval  (-1)   	: Not applicable
  * @retval  (1)		: Signal Sent
  */
int32_t pOS_SendSignal(	uint32_t  destTask,
						uint32_t  signal,
						uint8_t   *data,
						uint8_t   data_length )
{
  int freesize = osMaxMessagePool - osCore.osMessageInUse;
  if( (data_length > osMaxOSMessageDataLen) ||
      (freesize <= 0) )
  {
    return -1;
  }
  pOS_HAL_DisableInterrupt();
  //
  osCore.osMessageTail->allocated   = 1;
  osCore.osMessageTail->destTask    = destTask;
  osCore.osMessageTail->signal      = signal;
  osCore.osMessageTail->data_length = data_length;
  if( data_length )
    memcpy(&osCore.osMessageTail->data[0] , data, data_length);
  osCore.osMessageTail++;
  if( osCore.osMessageTail >= &osCore.osMessagePool[osMaxMessagePool] )
    osCore.osMessageTail = &osCore.osMessagePool[0];
  //
  osCore.osMessageInUse++;
  if( osCore.osMessageInUse > osCore.maxPendingMessage )
    osCore.maxPendingMessage = osCore.osMessageInUse;
  //
  pOS_HAL_EnableInterrupt();
  return 1;
}

/**
  * @brief  Start Periodic Function
  * @param  callback    : Callback Function after Timer elapsed
  * @param  arg         : Argument Passed to Callback Function
  * @param  repeat      : Timeout Repeat Counter Infinite/One-Shot/X-Shot
  * @param  period      : Timeout Period (mSec)
  * @retval (-1)     	: Not applicable (Pool full, wrong parameter)
  * @retval else     	: Periodic Func. ID
  */
int32_t pOS_StartPeriodicFunc( osPeriodicFunc_t   callback,
                               uint32_t           arg,
                               int32_t            repeat,
                               uint32_t           period )
{
  osPeriodicFunction_t *pfo;
  if( (period == 0) || (callback == NULL) || (repeat == 0) )
    return -1;
  pfo = &osCore.osPeriodicFunctionPool[0];
  while( pfo < &osCore.osPeriodicFunctionPool[osMaxPeriodicFunPool] )
  {
    if( pfo->period == 0 )
    {
      pOS_HAL_DisableInterrupt();
      pfo->callback   = callback;
      pfo->arg        = arg;
      pfo->repeat     = repeat;
      pfo->period     = period;
      pfo->remaining  = pfo->period;
      pOS_HAL_EnableInterrupt();
      return (int32_t)pfo;
    }
    pfo++;
  }
  return -1;
}

/**
  * @brief  Stop Periodic Function
  * @param  pfoID 		: Periodic Function ID
  * @retval (0)     	: OK
  * @retval (-1)     	: Not applicable (Not found, wrong parameter)
  */
int32_t pOS_StopPeriodicFunc( int32_t pfoID )
{
  osPeriodicFunction_t *pfo;
  if( pfoID == (-1) )
    return -1;
  pfo = &osCore.osPeriodicFunctionPool[0];
  while( pfo < &osCore.osPeriodicFunctionPool[osMaxPeriodicFunPool] )
  {
    if( (int32_t)pfo == pfoID )
    {
      pOS_HAL_DisableInterrupt();
      memset(pfo,0x00,sizeof(osPeriodicFunction_t));
      pOS_HAL_EnableInterrupt();
      return 0;
    }
    pfo++;
  }
  return -1;
}

/**
  * @brief  OS Tick Program
  * @note   Should be Called every mSec
  */
void pOS_Tick(void)
{
  osPeriodicFunction_t *pfo;
  if(osCore.status != osCoreRun)
    return;
  //
  osCore.tick++;
  // Check Periodic Function Timer(s)
  pfo = &osCore.osPeriodicFunctionPool[0];
  while( pfo < &osCore.osPeriodicFunctionPool[osMaxPeriodicFunPool] )
  {
    if( ( pfo->remaining != 0 ) && ( pfo->period != 0 ) )
    {
      if((--pfo->remaining) == 0)
      { //Check Type of Signal Timeout & Re-Load if Needed (DONE in Scheduler)
      }
    }
    pfo++;
  }
}

/**
  * @brief  OS Scheduler Function, Calls Periodic Functions, Manages Signals
  * @note   Should be Called in while superloop
  */
void pOS_Scheduler(void)
{
  osPeriodicFunction_t *pfo;
  osTask_t  *to;
  if(osCore.status != osCoreRun)   //do not start before OS init
    return;
  // Periodic Non-Critic Function Calls
  pfo = &osCore.osPeriodicFunctionPool[0];
  while( pfo < &osCore.osPeriodicFunctionPool[osMaxPeriodicFunPool] )
  {
    if( ( pfo->remaining == 0 ) && ( pfo->period != 0 ) )
    {
      pfo->callback(pfo->arg);
      if( pfo->repeat > 0 )
        pfo->repeat--;
      if( pfo->repeat != 0 )
      {
        pfo->remaining = pfo->period;
      }
      else
      {
        //TODO (PAVOS)if( pOS_StopPeriodicFunc( pfo->callback ) == -1 )
          ;//TODO (PAVOS)Fatal Error
      }
    }
    pfo++;
  }
  // Signal Calls
  if(osCore.osMessageHead->allocated != 0)
  {
    to = (osTask_t*)&osCore.osTaskPool[0];
    while( to < &osCore.osTaskPool[osMaxTaskFunPool] )
    {
      if((uint32_t)to == osCore.osMessageHead->destTask)
      {
        to->runTask(osCore.osMessageHead);
        break;
      }
      to++;
    }
    pOS_HAL_DisableInterrupt();
    memset(osCore.osMessageHead, 0x00, sizeof(osMessage_t));
    osCore.osMessageHead++;
    if(osCore.osMessageHead >= &osCore.osMessagePool[osMaxMessagePool])
      osCore.osMessageHead = &osCore.osMessagePool[0];
    osCore.osMessageInUse--;
    pOS_HAL_EnableInterrupt();
  }
  // WDT
  pOS_HAL_ResetWDT();
}

/**
  * @brief  OS Error Logger
  * @note   Error List enum is in os.h
  * @param  error	: Error Type
  */
void pOS_LogError(osErrorType_t error)
{
  osCore.totalOSError++;
  osCore.logOSError[error]++;
  switch(error)
  {
    case osErrorWrongTask:
    break;
    case osErrorWrongSignal:
    break;
    case osErrorMessageQueFull:
    break;
    default:
	break;
  }
}

/**
  * @}
  */

/**
  * @}
  */
