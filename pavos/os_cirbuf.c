/**
 ******************************************************************************
 * @file os_cirbuf.c
 *
 * @date 	03-03-2020
 * @author 	mehmet.alinbay
 * @brief	Generic Circular Buffer
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) PAVOTEK
 * All rights reserved.</center></h2>
 *
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "os_cirbuf.h"

/** @addtogroup PAVOS
  * @{
  */

/** @defgroup OS_CIR_BUFF PAVOS Circular Buffer
  * @brief Generic Circular Buffer Functions
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup OS_CIR_BUFF_Exported_Functions Exported Functions
  * @brief Circular Buffer
  * @{
  */

/**
 * @brief Create & Init circular buffer.
 * @param cb 	: Circular Buffer Pointer
 * @param bytes	: Circular Buffer Data Header
 * @param size	: Circular Buffer Data Size
 * @retval 	-1	: Error
 * @retval	 1	: OK
 */
int osCirBufCreate(osCirBuf_t *cb, uint8_t *bytes, uint32_t size)
{
  if( ( size == 0 ) || ( cb == NULL ) || (bytes == NULL) )
    return -1;
  cb->ptr = bytes;
  cb->bufsize = size;
  cb->last = ((uint32_t)cb->ptr + cb->bufsize - 1u);
  if( osCirBufClear(cb) == -1 )
    return -1;
  return 1;
}

/**
 * @brief Clear Circular Buffer
 * @param cb 	: Circular Buffer Pointer
 * @retval 	-1	: Error
 * @retval	 0	: OK
 */
int osCirBufClear(osCirBuf_t *cb)
{
  if( ( cb == NULL ) || ( cb->ptr == NULL ) || ( cb->bufsize == 0 ) )
    return -1;
  memset(cb->ptr, 0x00, cb->bufsize);
  cb->head = cb->ptr;
  cb->tail = cb->ptr;
  return 0;
}


/**
 * @brief Read Data from Circular Buffer and copy to (a) Buffer
 * returns number of bytes is copied
 * @param cb 	: Circular Buffer Pointer
 * @param bytes	: Read Data Buffer
 * @param len	: Number of bytes to Read
 * @retval 	-1	: Error
 * @retval	 0	: No Data READ, buffer is empty
 * @retval len	: Number of Bytes copied
 */
int osCirBufRead(osCirBuf_t *cb, uint8_t *bytes, uint32_t len)
{
  uint32_t counter;
  if( ( len == 0 ) || ( cb == NULL ) || ( cb->ptr == NULL ) ||
      ( cb->bufsize == 0 )  || (bytes == NULL) )
    return -1;
  /* check if there is data */
  if (cb->used == 0)
    return 0;
  if (len > cb->used)
    len = cb->used;
  cb->used -= len;
  for (counter = 0; counter < len; counter++)
  {
    bytes[counter] = *(cb->head++);
    /* wrap if needed */
    if ( (uint32_t)cb->head > cb->last )
      cb->head = cb->ptr;
  }
  return len;
}

/**
 * @brief Write Data to Circular Buffer. Returns number of bytes written.
 * @param cb 	: Circular Buffer Pointer
 * @param bytes	: Read Data Buffer
 * @param len	: Number of bytes to Read
 * @retval 	-1	: Error
 * @retval	 0	: No Data READ, buffer is empty
 * @retval  len	: Number of Bytes copied
 */
int osCirBufWrite(osCirBuf_t *cb, uint8_t *bytes, uint32_t len)
{
  int freesize;
  uint32_t counter;
  if( ( len == 0 ) || ( cb == NULL ) || ( cb->ptr == NULL ) ||
      ( cb->bufsize == 0 )  || (bytes == NULL) )
    return -1;
  /* check if there is space */
  freesize = cb->bufsize - cb->used;
  if ( len > freesize )
    return 0;
  cb->used += len;
  for (counter = 0; counter < len; counter++)
  {
    *(cb->tail++) = bytes[counter];
    /* wrap if needed */
    if ( (uint32_t)cb->tail > cb->last )
      cb->tail = cb->ptr;
  }
  return len;
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
