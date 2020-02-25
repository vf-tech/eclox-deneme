/**
 ******************************************************************************
 * @file deneme1.h
 *
 * @date 25 Şub 2020
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
#ifndef DENEME1_H_
#define DENEME1_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal_def.h"

/** @addtogroup eclox-deneme
  * @{
  */

/** @addtogroup deneme1
  * @{
  */
/* Exported types ------------------------------------------------------------*/
/** @defgroup deneme1_Exported_Types Exported Types
  * @{
  */

/**
  * @brief  ADC group regular oversampling structure definition
  */
typedef struct
{
  uint32_t Ratio;                         /*!< Configures the oversampling ratio.
                                               This parameter can be a value between 1 and 1024 */

  uint32_t RightBitShift;                 /*!< Configures the division coefficient for the Oversampler.
                                               This parameter can be a value of @ref ADC_HAL_EC_OVS_SHIFT */

  uint32_t TriggeredMode;                 /*!< Selects the regular triggered oversampling mode.
                                               This parameter can be a value of @ref ADC_HAL_EC_OVS_DISCONT_MODE */

  uint32_t OversamplingStopReset;         /*!< Selects the regular oversampling mode.
                                               The oversampling is either temporary stopped or reset upon an injected
                                               sequence interruption.
                                               If oversampling is enabled on both regular and injected groups, this parameter
                                               is discarded and forced to setting "ADC_REGOVERSAMPLING_RESUMED_MODE"
                                               (the oversampling buffer is zeroed during injection sequence).
                                               This parameter can be a value of @ref ADC_HAL_EC_OVS_SCOPE_REG */

} ADC_OversamplingTypeDef;

/**
  * @}
  */
/* Exported constants --------------------------------------------------------*/
/** @defgroup deneme1_Exported_Constants Exported Constants
  * @{
  */

/* States of ADC multi-mode */
#define HAL_ADC_STATE_MULTIMODE_SLAVE   (0x00100000UL)   /*!< ADC in multimode slave state, controlled by another ADC master (when feature available) */


/**
  * @}
  */
/* Exported macro ------------------------------------------------------------*/
/** @defgroup deneme1_Exported_Macros Exported Macros
  * @{
  */

/**
  * @brief Verify the ADC data conversion setting.
  * @param DATA : programmed DATA conversion mode.
  * @retval SET (DATA is a valid value) or RESET (DATA is invalid)
  */
#define IS_ADC_CONVERSIONDATAMGT(DATA)                                         \
   ((((DATA) == ADC_CONVERSIONDATA_DR))          || \
    (((DATA) == ADC_CONVERSIONDATA_DFSDM))       || \
    (((DATA) == ADC_CONVERSIONDATA_DMA_ONESHOT)) || \
    (((DATA) == ADC_CONVERSIONDATA_DMA_CIRCULAR)))

/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @addtogroup deneme1_Exported_Functions
  * @{
  */

HAL_StatusTypeDef       HAL_ADC_Start(ADC_HandleTypeDef *hadc);

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

#endif /* DENEME1_H_ */
