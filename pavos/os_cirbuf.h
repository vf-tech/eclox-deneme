/**
 ******************************************************************************
 * @file os_cirbuf.h
 *
 * @date 	03-03-2020
 * @author 	mehmet.alinbay
 * @brief	Circular Buffer Header File
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) PAVOTEK
 * All rights reserved.</center></h2>
 *
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OS_CIRBUF_H
#define OS_CIRBUF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <os.h>

/** @addtogroup PAVOS
  * @{
  */

/** @addtogroup OS_CIR_BUFF
  * @{
  */
/* Exported types ------------------------------------------------------------*/
/** @defgroup OS_CIR_BUFF_Exported_Types Exported Types
  * @brief Circular Buffer
  * @{
  */

/**
 * @struct	osCirBuf_t
 * @brief  	Circular Buffer structure definition
 */
typedef struct
{
	/*! Buffer Pointer */
	uint8_t   *ptr;
	/*! Head Pointer */
	uint8_t   *head;
	/*! Tail Pointer */
	uint8_t   *tail;
	/*! Size of Buffer (in bytes) */
	uint32_t  bufsize;
	/*! Stores Buffer's Last Item Address*/
	uint32_t  last;
	/*! Stores Active Used Bytes */
	uint32_t  used;
}osCirBuf_t;

/**
  * @}
  */
/* Exported constants --------------------------------------------------------*/
/** @defgroup OS_CIR_BUFF_Exported_Constants Exported Constants
  * @brief Circular Buffer
  * @{
  */

/**
  * @}
  */
/* Exported macro ------------------------------------------------------------*/
/** @defgroup OS_CIR_BUFF_Exported_Macros Exported Macros
  * @brief Circular Buffer
  * @{
  */

/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @addtogroup OS_CIR_BUFF_Exported_Functions Exported Functions
  * @{
  */

int osCirBufCreate(osCirBuf_t *cb, uint8_t *bytes, uint32_t size);
int osCirBufClear(osCirBuf_t *cb);
int osCirBufWrite(osCirBuf_t *cb, uint8_t *bytes, uint32_t len);
int osCirBufRead(osCirBuf_t *cb, uint8_t *bytes, uint32_t len);

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

#endif /* OSCIRBUF_H_ */
