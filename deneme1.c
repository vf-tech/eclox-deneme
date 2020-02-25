/**
 ******************************************************************************
 * @file deneme1.c
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


/* Includes ------------------------------------------------------------------*/

/** @addtogroup eclox-deneme
  * @{
  */

/** @defgroup deneme1 deneme1
  * @brief XXX
  * @{
  */
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup deneme1_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  Enable ADC, start conversion of regular group.
  * @note   Interruptions enabled in this function: None.
  * @note   Case of multimode enabled (when multimode feature is available):
  *           if ADC is Slave, ADC is enabled but conversion is not started,
  *           if ADC is master, ADC is enabled and multimode conversion is started.
  * @param hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef *hadc)
{
  HAL_StatusTypeDef tmp_hal_status;
  const ADC_TypeDef *tmpADC_Master;
  uint32_t tmp_multimode_config = LL_ADC_GetMultimode(__LL_ADC_COMMON_INSTANCE(hadc->Instance));

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* Perform ADC enable and conversion start if no conversion is on going */
  if (LL_ADC_REG_IsConversionOngoing(hadc->Instance) == 0UL)
  {
    /* Process locked */
    __HAL_LOCK(hadc);

    /* Enable the ADC peripheral */
    tmp_hal_status = ADC_Enable(hadc);

    /* Start conversion if ADC is effectively enabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state                                                        */
      /* - Clear state bitfield related to regular group conversion results   */
      /* - Set state bitfield related to regular operation                    */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR | HAL_ADC_STATE_REG_EOSMP,
                        HAL_ADC_STATE_REG_BUSY);

      /* Reset HAL_ADC_STATE_MULTIMODE_SLAVE bit
        - if ADC instance is master or if multimode feature is not available
        - if multimode setting is disabled (ADC instance slave in independent mode) */
      if ((__LL_ADC_MULTI_INSTANCE_MASTER(hadc->Instance) == hadc->Instance)
          || (tmp_multimode_config == LL_ADC_MULTI_INDEPENDENT)
         )
      {
        CLEAR_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
      }

      /* Set ADC error code */
      /* Check if a conversion is on going on ADC group injected */
      if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_INJ_BUSY))
      {
        /* Reset ADC error code fields related to regular conversions only */
        CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));
      }
      else
      {
        /* Reset all ADC error code fields */
        ADC_CLEAR_ERRORCODE(hadc);
      }

      /* Clear ADC group regular conversion flag and overrun flag               */
      /* (To ensure of no unknown state from potential previous ADC operations) */
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));

      /* Process unlocked */
      /* Unlock before starting ADC conversions: in case of potential         */
      /* interruption, to let the process to ADC IRQ Handler.                 */
      __HAL_UNLOCK(hadc);

      /* Enable conversion of regular group.                                  */
      /* If software start has been selected, conversion starts immediately.  */
      /* If external trigger has been selected, conversion will start at next */
      /* trigger event.                                                       */
      /* Case of multimode enabled (when multimode feature is available):     */
      /*  - if ADC is slave and dual regular conversions are enabled, ADC is  */
      /*    enabled only (conversion is not started),                         */
      /*  - if ADC is master, ADC is enabled and conversion is started.       */
      if ((__LL_ADC_MULTI_INSTANCE_MASTER(hadc->Instance) == hadc->Instance)
          || (tmp_multimode_config == LL_ADC_MULTI_INDEPENDENT)
          || (tmp_multimode_config == LL_ADC_MULTI_DUAL_INJ_SIMULT)
          || (tmp_multimode_config == LL_ADC_MULTI_DUAL_INJ_ALTERN)
         )
      {
        /* ADC instance is not a multimode slave instance with multimode regular conversions enabled */
        if (READ_BIT(hadc->Instance->CFGR, ADC_CFGR_JAUTO) != 0UL)
        {
          ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
        }

        /* Start ADC group regular conversion */
        LL_ADC_REG_StartConversion(hadc->Instance);
      }
      else
      {
        /* ADC instance is a multimode slave instance with multimode regular conversions enabled */
        SET_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
        /* if Master ADC JAUTO bit is set, update Slave State in setting
           HAL_ADC_STATE_INJ_BUSY bit and in resetting HAL_ADC_STATE_INJ_EOC bit */
        tmpADC_Master = __LL_ADC_MULTI_INSTANCE_MASTER(hadc->Instance);
        if (READ_BIT(tmpADC_Master->CFGR, ADC_CFGR_JAUTO) != 0UL)
        {
          ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
        }

      }
    }
    else
    {
      /* Process unlocked */
      __HAL_UNLOCK(hadc);
    }
  }
  else
  {
    tmp_hal_status = HAL_BUSY;
  }

  /* Return function status */
  return tmp_hal_status;
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
