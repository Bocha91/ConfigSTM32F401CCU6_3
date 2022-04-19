/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    adc.c
 * @brief   This file provides code for the configuration
 *          of the ADC instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "adc.hpp"

/* USER CODE BEGIN 0 */
uint16_t ADCxConvertedValue[ADC_CHENNAL];

__IO uint16_t uhADCxConvertedValue[2][LENGHT_BUFF];

/* USER CODE END 0 */
#if 0
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */

    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
     */
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8; //
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = ENABLE; //
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = ADC_CHENNAL;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_2;
    sConfig.Rank = 2;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank = 3;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = 4;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_VREFINT;
    sConfig.Rank = 5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    sConfig.Channel = ADC_CHANNEL_VBAT;
    sConfig.Rank = 6;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /*##-3- Start the conversion process and enable interrupt ##################*/
    /* Note: Considering IT occurring after each number of ADC conversions      */
    /*       (IT by DMA end of transfer), select sampling time and ADC clock    */
    /*       with sufficient duration to not create an overhead situation in    */
    /*        IRQHandler. */
    if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&uhADCxConvertedValue, 1) != HAL_OK)
    {
        /* Start Conversation Error */
        Error_Handler();
    }

    /* USER CODE END ADC1_Init 2 */
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *adcHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (adcHandle->Instance == ADC1)
    {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */
        /* ADC1 clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**ADC1 GPIO Configuration
        PA1     ------> ADC1_IN1
        PA2     ------> ADC1_IN2
        PA3     ------> ADC1_IN3
        PA4     ------> ADC1_IN4
        */
        GPIO_InitStruct.Pin = LEFT_RESISTOR_Pin | RIGHT_RESISTOR_Pin | BRASLET_Pin | SHUP_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* ADC1 DMA Init */
        /* ADC1 Init */
        hdma_adc1.Instance = DMA2_Stream0;
        hdma_adc1.Init.Channel = DMA_CHANNEL_0;
        hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
        hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; //
        hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    //
        hdma_adc1.Init.Mode = DMA_CIRCULAR;
        hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;            //
        hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_ENABLE;          //
        hdma_adc1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL; //
        hdma_adc1.Init.MemBurst = DMA_MBURST_SINGLE;
        hdma_adc1.Init.PeriphBurst = DMA_PBURST_SINGLE;
        if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc1);

        /* ADC1 interrupt Init */
        HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle)
{

    if (adcHandle->Instance == ADC1)
    {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */

        /* USER CODE END ADC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC1_CLK_DISABLE();

        /**ADC1 GPIO Configuration
        PA1     ------> ADC1_IN1
        PA2     ------> ADC1_IN2
        PA3     ------> ADC1_IN3
        PA4     ------> ADC1_IN4
        */
        HAL_GPIO_DeInit(GPIOA, LEFT_RESISTOR_Pin | RIGHT_RESISTOR_Pin | BRASLET_Pin | SHUP_Pin);

        /* ADC1 DMA DeInit */
        HAL_DMA_DeInit(adcHandle->DMA_Handle);

        /* ADC1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(ADC_IRQn);
        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    }
}
#endif
/* USER CODE BEGIN 1 */
/* Private types -------------------------------------------------------------*/
typedef struct
{
    __IO uint32_t ISR; /*!< DMA interrupt status register */
    __IO uint32_t Reserved0;
    __IO uint32_t IFCR; /*!< DMA interrupt flag clear register */
} DMA_Base_Registers;
DMA_Base_Registers * const REGS =  (DMA_Base_Registers *)DMA2_BASE; // для DMA2_Stream0
//const uint32_t STREAM_INDEX = 0U;  // для DMA2_Stream0


void iKPYT_ADC1_Init(void)
{
    uint32_t tickstart;

    // if (HAL_ADC_Init(&hadc1) != HAL_OK){Error_Handler();}
    {
        // HAL_ADC_MspInit(hadc);
        {
            GPIO_InitTypeDef GPIO_InitStruct = {0};
            /* ADC1 clock enable */
            __HAL_RCC_ADC1_CLK_ENABLE();
            __HAL_RCC_GPIOA_CLK_ENABLE();
            /**ADC1 GPIO Configuration
            PA1     ------> ADC1_IN1
            PA2     ------> ADC1_IN2
            PA3     ------> ADC1_IN3
            PA4     ------> ADC1_IN4
            */
            GPIO_InitStruct.Pin = LEFT_RESISTOR_Pin | RIGHT_RESISTOR_Pin | BRASLET_Pin | SHUP_Pin;
            GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

            /* ADC1 DMA Init */
            /* ADC1 Init */
            //if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
            {
                tickstart = HAL_GetTick();
                /* Disable the peripheral */
                (DMA2_Stream0->CR &= ~(DMA_SxCR_EN));// __HAL_DMA_DISABLE(hdma);

                #define HAL_TIMEOUT_DMA_ABORT 5U /* 5 ms */
                /* Check if the DMA Stream is effectively disabled */
                while ((DMA2_Stream0->CR & DMA_SxCR_EN) != RESET)
                {
                    /* Check for the Timeout */
                    if ((HAL_GetTick() - tickstart) > HAL_TIMEOUT_DMA_ABORT)
                    {
                        Error_Handler();
                    }
                }

                /* Get the CR register value */
                uint32_t tmp = DMA2_Stream0->CR;

                /* Clear CHSEL, MBURST, PBURST, PL, MSIZE, PSIZE, MINC, PINC, CIRC, DIR, CT and DBM bits */
                tmp &= ((uint32_t) ~(DMA_SxCR_CHSEL | DMA_SxCR_MBURST | DMA_SxCR_PBURST |
                                     DMA_SxCR_PL | DMA_SxCR_MSIZE | DMA_SxCR_PSIZE |
                                     DMA_SxCR_MINC | DMA_SxCR_PINC | DMA_SxCR_CIRC |
                                     DMA_SxCR_DIR | DMA_SxCR_CT | DMA_SxCR_DBM));

                /* Prepare the DMA Stream configuration */
                //   tmp |=  hdma->Init.Channel             | hdma->Init.Direction        |
                //           hdma->Init.PeriphInc           | hdma->Init.MemInc           |
                //           hdma->Init.PeriphDataAlignment | hdma->Init.MemDataAlignment |
                //           hdma->Init.Mode                | hdma->Init.Priority;
                tmp |= DMA_CHANNEL_0 | DMA_PERIPH_TO_MEMORY |
                       DMA_PINC_DISABLE | DMA_MINC_ENABLE |
                       DMA_PDATAALIGN_HALFWORD | DMA_MDATAALIGN_HALFWORD |
                       DMA_CIRCULAR | DMA_PRIORITY_HIGH;

                /* the Memory burst and peripheral burst are not used when the FIFO is disabled */
                // if(hdma->Init.FIFOMode == DMA_FIFOMODE_ENABLE)
                {
                    /* Get memory burst and peripheral burst */
                    tmp |= DMA_MBURST_SINGLE | DMA_PBURST_SINGLE;
                }

                /* Write to DMA Stream CR register */
                DMA2_Stream0->CR = tmp;

                /* Get the FCR register value */
                tmp = DMA2_Stream0->FCR;

                /* Clear Direct mode and FIFO threshold bits */
                tmp &= (uint32_t) ~(DMA_SxFCR_DMDIS | DMA_SxFCR_FTH);

                /* Prepare the DMA Stream FIFO configuration */
                tmp |= DMA_FIFOMODE_ENABLE;

                /* The FIFO threshold is not used when the FIFO mode is disabled */
                // if(hdma->Init.FIFOMode == DMA_FIFOMODE_ENABLE)
                {
                    /* Get the FIFO threshold */
                    tmp |= DMA_FIFO_THRESHOLD_FULL; // hdma->Init.FIFOThreshold;

                    /* Check compatibility between FIFO threshold level and size of the memory burst */
                    /* for INCR4, INCR8, INCR16 bursts */
                    // if (hdma->Init.MemBurst != DMA_MBURST_SINGLE)
                    // {
                    //   if (DMA_CheckFifoParam(hdma) != HAL_OK)
                    //   {
                    //     /* Update error code */
                    //     hdma->ErrorCode = HAL_DMA_ERROR_PARAM;

                    //     /* Change the DMA state */
                    //     hdma->State = HAL_DMA_STATE_READY;

                    //     return HAL_ERROR;
                    //   }
                    // }
                }

                /* Write to DMA Stream FCR */
                DMA2_Stream0->FCR = tmp;


                //DMA_Base_Registers *regs;
                //uint32_t StreamIndex;
                /* Initialize StreamBaseAddress and StreamIndex parameters to be used to calculate
                   DMA steam Base Address needed by HAL_DMA_IRQHandler() and HAL_DMA_PollForTransfer() */
                // regs = (DMA_Base_Registers *)DMA_CalcBaseAndBitshift(hdma);
                // {
                //     uint32_t stream_number = (((uint32_t)DMA2_Stream0 & 0xFFU) - 16U) / 24U;

                //     /* lookup table for necessary bitshift of flags within status registers */
                //     static const uint8_t flagBitshiftOffset[8U] = {0U, 6U, 16U, 22U, 0U, 6U, 16U, 22U};
                //     StreamIndex = flagBitshiftOffset[stream_number];
                //     uint32_t StreamBaseAddress;
                //     if (stream_number > 3U)
                //     {
                //         /* return pointer to HISR and HIFCR */
                //         StreamBaseAddress = (((uint32_t)DMA2_Stream0 & (uint32_t)(~0x3FFU)) + 4U);
                //     }
                //     else
                //     {
                //         /* return pointer to LISR and LIFCR */
                //         StreamBaseAddress = ((uint32_t)DMA2_Stream0 & (uint32_t)(~0x3FFU));
                //     }

                //     regs = (DMA_Base_Registers *)StreamBaseAddress;
                // }

                /* Clear all interrupt flags */
                //regs->IFCR = 0x3FU << StreamIndex;
                REGS->IFCR = (0x3FU << 0U); // для DMA2_Stream0
                /* Initialize the error code */
                // hdma->ErrorCode = HAL_DMA_ERROR_NONE;

                /* Initialize the DMA state */
                // hdma->State = HAL_DMA_STATE_READY;
            }// HAL_DMA_Init()

            //__HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc1);
            // do{
            //     (adcHandle)->DMA_Handle = &(hdma_adc1);
            //     (hdma_adc1).Parent = (adcHandle);
            // } while(0U)

            /* ADC1 interrupt Init */
            HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(ADC_IRQn);
        } // HAL_ADC_MspInit

        /* Initialize ADC error code */
        // ADC_CLEAR_ERRORCODE(hadc);
        __IO uint32_t ErrorCode = HAL_ADC_ERROR_NONE;

        /* Set ADC parameters */
        // ADC_Init(hadc);
        {
            /* Set the ADC clock prescaler */
            ADC1_COMMON->CCR &= ~(ADC_CCR_ADCPRE);
            ADC1_COMMON->CCR |= ADC_CLOCK_SYNC_PCLK_DIV8; // hadc->Init.ClockPrescaler;

            /* Set ADC scan mode */
            ADC1->CR1 &= ~(ADC_CR1_SCAN);
            ADC1->CR1 |= ADC_CR1_SCANCONV(ENABLE);

            /* Set ADC resolution */
            ADC1->CR1 &= ~(ADC_CR1_RES);
            ADC1->CR1 |= ADC_RESOLUTION_12B;

            /* Set ADC data alignment */
            ADC1->CR2 &= ~(ADC_CR2_ALIGN);
            ADC1->CR2 |= ADC_DATAALIGN_RIGHT;

            /* Enable external trigger if trigger selection is different of software start. */
            // if (hadc->Init.ExternalTrigConv != ADC_SOFTWARE_START)
            // {
            //     /* Select external trigger to start conversion */
            //     ADC1->CR2 &= ~(ADC_CR2_EXTSEL);
            //     ADC1->CR2 |= hadc->Init.ExternalTrigConv;

            //     /* Select external trigger polarity */
            //     ADC1->CR2 &= ~(ADC_CR2_EXTEN);
            //     ADC1->CR2 |= hadc->Init.ExternalTrigConvEdge;
            // }
            // else
            {
                /* Reset the external trigger */
                ADC1->CR2 &= ~(ADC_CR2_EXTSEL);
                ADC1->CR2 &= ~(ADC_CR2_EXTEN);
            }

            /* Enable or disable ADC continuous conversion mode */
            ADC1->CR2 &= ~(ADC_CR2_CONT);
            ADC1->CR2 |= ADC_CR2_CONTINUOUS((uint32_t)ENABLE);

            // if (hadc->Init.DiscontinuousConvMode != DISABLE)
            // {
            //     assert_param(IS_ADC_REGULAR_DISC_NUMBER(hadc->Init.NbrOfDiscConversion));

            //     /* Enable the selected ADC regular discontinuous mode */
            //     ADC1->CR1 |= (uint32_t)ADC_CR1_DISCEN;

            //     /* Set the number of channels to be converted in discontinuous mode */
            //     ADC1->CR1 &= ~(ADC_CR1_DISCNUM);
            //     ADC1->CR1 |= ADC_CR1_DISCONTINUOUS(hadc->Init.NbrOfDiscConversion);
            // }
            // else
            {
                /* Disable the selected ADC regular discontinuous mode */
                ADC1->CR1 &= ~(ADC_CR1_DISCEN);
            }

            /* Set ADC number of conversion */
            ADC1->SQR1 &= ~(ADC_SQR1_L);
            ADC1->SQR1 |= ADC_SQR1(ADC_CHENNAL); // hadc->Init.NbrOfConversion

            /* Enable or disable ADC DMA continuous request */
            ADC1->CR2 &= ~(ADC_CR2_DDS);
            ADC1->CR2 |= ADC_CR2_DMAContReq((uint32_t)ENABLE);

            /* Enable or disable ADC end of conversion selection */
            ADC1->CR2 &= ~(ADC_CR2_EOCS);
            ADC1->CR2 |= ADC_CR2_EOCSelection(ADC_EOC_SEQ_CONV);
        }// ADC_Init(hadc);
    }//HAL_ADC_Init(&hadc1)

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    // sConfig.Channel = ADC_CHANNEL_1;
    // sConfig.Rank = 1;
    // sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    // if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){Error_Handler();}
    {
        /* ADC_Channel include in ADC_Channel_[0..9] */
        {
            /* Clear the old sample time */
            ADC1->SMPR2 &= ~ADC_SMPR2(ADC_SMPR2_SMP0, ADC_CHANNEL_1);
            /* Set the new sample time */
            ADC1->SMPR2 |= ADC_SMPR2(ADC_SAMPLETIME_480CYCLES, ADC_CHANNEL_1);
        }

        /* For Rank 1 to 6 */
        {
            /* Clear the old SQx bits for the selected rank */
            ADC1->SQR3 &= ~ADC_SQR3_RK(ADC_SQR3_SQ1, 1);
            /* Set the SQx bits for the selected rank */
            ADC1->SQR3 |= ADC_SQR3_RK(ADC_CHANNEL_1, 1);
        }
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    // sConfig.Channel = ADC_CHANNEL_2;
    // sConfig.Rank = 2;
    // if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){Error_Handler();}
    {

        /* ADC_Channel include in ADC_Channel_[0..9] */
        {
            /* Clear the old sample time */
            ADC1->SMPR2 &= ~ADC_SMPR2(ADC_SMPR2_SMP0, ADC_CHANNEL_2);
            /* Set the new sample time */
            ADC1->SMPR2 |= ADC_SMPR2(ADC_SAMPLETIME_480CYCLES, ADC_CHANNEL_2);
        }

        /* For Rank 1 to 6 */
        {
            /* Clear the old SQx bits for the selected rank */
            ADC1->SQR3 &= ~ADC_SQR3_RK(ADC_SQR3_SQ1, 2);
            /* Set the SQx bits for the selected rank */
            ADC1->SQR3 |= ADC_SQR3_RK(ADC_CHANNEL_2, 2);
        }
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    // sConfig.Channel = ADC_CHANNEL_3;
    // sConfig.Rank = 3;
    //  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){Error_Handler();}
    {
        /* ADC_Channel include in ADC_Channel_[0..9] */
        {
            /* Clear the old sample time */
            ADC1->SMPR2 &= ~ADC_SMPR2(ADC_SMPR2_SMP0, ADC_CHANNEL_3);
            /* Set the new sample time */
            ADC1->SMPR2 |= ADC_SMPR2(ADC_SAMPLETIME_480CYCLES, ADC_CHANNEL_3);
        }

        /* For Rank 1 to 6 */
        {
            /* Clear the old SQx bits for the selected rank */
            ADC1->SQR3 &= ~ADC_SQR3_RK(ADC_SQR3_SQ1, 3);
            /* Set the SQx bits for the selected rank */
            ADC1->SQR3 |= ADC_SQR3_RK(ADC_CHANNEL_1, 3);
        }
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    // sConfig.Channel = ADC_CHANNEL_4;
    // sConfig.Rank = 4;
    //  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){Error_Handler();}
    {
        /* ADC_Channel include in ADC_Channel_[0..9] */
        {
            /* Clear the old sample time */
            ADC1->SMPR2 &= ~ADC_SMPR2(ADC_SMPR2_SMP0, ADC_CHANNEL_4);
            /* Set the new sample time */
            ADC1->SMPR2 |= ADC_SMPR2(ADC_SAMPLETIME_480CYCLES, ADC_CHANNEL_4);
        }

        /* For Rank 1 to 6 */
        {
            /* Clear the old SQx bits for the selected rank */
            ADC1->SQR3 &= ~ADC_SQR3_RK(ADC_SQR3_SQ1, 4);
            /* Set the SQx bits for the selected rank */
            ADC1->SQR3 |= ADC_SQR3_RK(ADC_CHANNEL_1, 4);
        }
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    // sConfig.Channel = ADC_CHANNEL_VREFINT;
    // sConfig.Rank = 5;
    //  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){Error_Handler();}
    {
        /* if ADC_Channel_10 ... ADC_Channel_18 is selected */
        // if (ADC_CHANNEL_VREFINT > ADC_CHANNEL_9)
        {
            /* Clear the old sample time */
            ADC1->SMPR1 &= ~ADC_SMPR1(ADC_SMPR1_SMP10, ADC_CHANNEL_VREFINT);
            /* Set the new sample time */
            ADC1->SMPR1 |= ADC_SMPR1(ADC_SAMPLETIME_480CYCLES, ADC_CHANNEL_VREFINT);
        }

        /* For Rank 1 to 6 */
        {
            /* Clear the old SQx bits for the selected rank */
            ADC1->SQR3 &= ~ADC_SQR3_RK(ADC_SQR3_SQ1, 5);
            /* Set the SQx bits for the selected rank */
            ADC1->SQR3 |= ADC_SQR3_RK(ADC_CHANNEL_VREFINT, 5);
        }

        /* if ADC1 Channel_16 or Channel_18 is selected for Temperature sensor or
        Channel_17 is selected for VREFINT enable TSVREFE */
        // if ((ADC1 == ADC1) && ((ADC_CHANNEL_VREFINT == ADC_CHANNEL_TEMPSENSOR) || (ADC_CHANNEL_VREFINT == ADC_CHANNEL_VREFINT)))
        {
            /* Enable the Temperature sensor and VREFINT channel*/
            ADC1_COMMON->CCR |= ADC_CCR_TSVREFE;
        }
    }

    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
     */
    // sConfig.Channel = ADC_CHANNEL_VBAT;
    // sConfig.Rank = 6;
    //  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){Error_Handler();}
    {
        /* if ADC_Channel_10 ... ADC_Channel_18 is selected */
        // if (ADC_CHANNEL_VBAT > ADC_CHANNEL_9)
        {
            /* Clear the old sample time */
            ADC1->SMPR1 &= ~ADC_SMPR1(ADC_SMPR1_SMP10, ADC_CHANNEL_VBAT);
            /* Set the new sample time */
            ADC1->SMPR1 |= ADC_SMPR1(ADC_SAMPLETIME_480CYCLES, ADC_CHANNEL_VBAT);
        }

        /* For Rank 1 to 6 */
        // if (1 < 7U)
        {
            /* Clear the old SQx bits for the selected rank */
            ADC1->SQR3 &= ~ADC_SQR3_RK(ADC_SQR3_SQ1, 6);
            /* Set the SQx bits for the selected rank */
            ADC1->SQR3 |= ADC_SQR3_RK(ADC_CHANNEL_VBAT, 6);
        }

        /* Pointer to the common control register to which is belonging hadc    */
        /* (Depending on STM32F4 product, there may be up to 3 ADCs and 1 common */
        /* control register)                                                    */
        // tmpADC_Common = ADC_COMMON_REGISTER(hadc);

        /* if ADC1 Channel_18 is selected for VBAT Channel ennable VBATE */
        // if ((ADC1 == ADC1) && (ADC_CHANNEL_VBAT == ADC_CHANNEL_VBAT))
        {
            /* Disable the TEMPSENSOR channel in case of using board with multiplixed ADC_CHANNEL_VBAT & ADC_CHANNEL_TEMPSENSOR*/
            // if ((uint16_t)ADC_CHANNEL_TEMPSENSOR == (uint16_t)ADC_CHANNEL_VBAT)
            // {
            //     ADC1_COMMON->CCR &= ~ADC_CCR_TSVREFE;
            // }
            /* Enable the VBAT channel*/
            ADC1_COMMON->CCR |= ADC_CCR_VBATE;
        }
    }
}

/* USER CODE BEGIN ADC1_Init 2 */

/*##-3- Start the conversion process and enable interrupt ##################*/
/* Note: Considering IT occurring after each number of ADC conversions      */
/*       (IT by DMA end of transfer), select sampling time and ADC clock    */
/*       with sufficient duration to not create an overhead situation in    */
/*        IRQHandler. */
// if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&uhADCxConvertedValue, 1) != HAL_OK){Error_Handler(); }
//                        hadc         pData            Length
void MY_ADC_Start_DMA(void)
{
    __IO uint32_t counter = 0U;
    //ADC_Common_TypeDef *tmpADC_Common;
    /* Check the parameters */
    //assert_param(IS_FUNCTIONAL_STATE(ENABLE));
    //assert_param(IS_ADC_EXT_TRIG_EDGE(ADC_EXTERNALTRIGCONVEDGE_NONE));
    /* Process locked */
    //__HAL_LOCK(hadc);
    /* Enable the ADC peripheral */
    /* Check if ADC peripheral is disabled in order to enable it and wait during
    Tstab time the ADC's stabilization */
    if ((ADC1->CR2 & ADC_CR2_ADON) != ADC_CR2_ADON)
    {
        /* Enable the Peripheral */
        ADC1->CR2 |= ADC_CR2_ADON; //__HAL_ADC_ENABLE(hadc);
        /* Delay for ADC stabilization time */
        /* Compute number of CPU cycles to wait for */
        counter = (ADC_STAB_DELAY_US * (SystemCoreClock / 1000000U));
        while (counter != 0U)
        {
            counter--;
        }
    }

    /* Check ADC DMA Mode                                                     */
    /* - disable the DMA Mode if it is already enabled                        */
    if ((ADC1->CR2 & ADC_CR2_DMA) == ADC_CR2_DMA)
    {
        CLEAR_BIT(ADC1->CR2, ADC_CR2_DMA);
    }
    /* Start conversion if ADC is effectively enabled */
    if (HAL_IS_BIT_SET(ADC1->CR2, ADC_CR2_ADON))
    {
        /* Set ADC state                                                          */
        /* - Clear state bitfield related to regular group conversion results     */
        /* - Set state bitfield related to regular group operation                */
        // ADC_STATE_CLR_SET(hadc->State,
        //                 HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR,
        //                 HAL_ADC_STATE_REG_BUSY);
        /* If conversions on group regular are also triggering group injected,    */
        /* update ADC state.                                                      */
        // if (READ_BIT(ADC1->CR1, ADC_CR1_JAUTO) != RESET)
        // {
        //     ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
        // }
        /* State machine update: Check if an injected conversion is ongoing */
        // if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_INJ_BUSY))
        // {
        //     /* Reset ADC error code fields related to conversions on group regular */
        //     CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));
        // }
        // else
        // {
        //     /* Reset ADC all error code fields */
        //     ADC_CLEAR_ERRORCODE(hadc);
        // }
        /* Process unlocked */
        /* Unlock before starting ADC conversions: in case of potential           */
        /* interruption, to let the process to ADC IRQ Handler.                   */
        //__HAL_UNLOCK(hadc);
        /* Pointer to the common control register to which is belonging hadc    */
        /* (Depending on STM32F4 product, there may be up to 3 ADCs and 1 common */
        /* control register)                                                    */
        //tmpADC_Common = ADC_COMMON_REGISTER(hadc);
        /* Set the DMA transfer complete callback */
        //hadc->DMA_Handle->XferCpltCallback = ADC_DMAConvCplt;
        /* Set the DMA half transfer complete callback */
        //hadc->DMA_Handle->XferHalfCpltCallback = ADC_DMAHalfConvCplt;
        /* Set the DMA error callback */
        //hadc->DMA_Handle->XferErrorCallback = ADC_DMAError;
        /* Manage ADC and DMA start: ADC overrun interruption, DMA start, ADC     */
        /* start (in case of SW start):                                           */
        /* Clear regular group conversion flag and overrun flag */
        /* (To ensure of no unknown state from potential previous ADC operations) */
        //__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC | ADC_FLAG_OVR);
        ((ADC1->SR) = ~(ADC_FLAG_EOC | ADC_FLAG_OVR));
        /* Enable ADC overrun interrupt */
        //__HAL_ADC_ENABLE_IT(hadc, ADC_IT_OVR);
        ((ADC1->CR1) |= (ADC_IT_OVR));
        /* Enable ADC DMA mode */
        ADC1->CR2 |= ADC_CR2_DMA;
        /* Start the DMA channel */
        //HAL_DMA_Start_IT(hadc->DMA_Handle, (uint32_t)&ADC1->DR, (uint32_t)pData, Length);
        //                  hdma              SrcAddress        DstAddress       DataLength
        {
            /* USER CODE END Error_Handler_Debug */
            //HAL_StatusTypeDef status = HAL_OK;
            /* calculate DMA base and stream number */
            //DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;
            /* Check the parameters */
            //assert_param(IS_DMA_BUFFER_SIZE(DataLength));
            /* Process locked */
            //__HAL_LOCK(hdma);
            /* Configure the source, destination address and the data length */
            //DMA_SetConfig(hdma, SrcAddress, DstAddress, DataLength);
            {
                /* Set DBM bit */
                DMA2_Stream0->CR |= (uint32_t)(DMA_SxCR_DBM);
                /* Configure DMA Stream data length */
                DMA2_Stream0->NDTR = LENGHT_BUFF; //DataLength;
                /* Memory to Peripheral */
                // if(DMA_PERIPH_TO_MEMORY == DMA_MEMORY_TO_PERIPH)
                // {
                //     /* Configure DMA Stream destination address */
                //     DMA2_Stream0->PAR = DstAddress;
                //     /* Configure DMA Stream source address */
                //     DMA2_Stream0->M0AR = SrcAddress;
                // }
                /* Peripheral to Memory */
                // else
                {
                    /* Configure DMA Stream source address */
                    DMA2_Stream0->PAR = (uint32_t)&(ADC1->DR); //SrcAddress;
                    /* Configure DMA Stream destination address */
                    DMA2_Stream0->M0AR = (uint32_t)&uhADCxConvertedValue[0][0]; //DstAddress;
                    DMA2_Stream0->M1AR = (uint32_t)&uhADCxConvertedValue[1][0]; //DstAddress;
                }
            }
            /* Clear all interrupt flags at correct offset within the register */
            REGS->IFCR = (0x3FU << 0U);//regs->IFCR = 0x3FU << hdma->StreamIndex;
            /* Enable Common interrupts*/
            DMA2_Stream0->CR |= (DMA_IT_TC | DMA_IT_TE | DMA_IT_DME);
            
            //if (hdma->XferHalfCpltCallback != NULL)
            // {
            //     DMA2_Stream0->CR |= DMA_IT_HT;
            // }
            /* Enable the Peripheral */
            //__HAL_DMA_ENABLE(hdma);
            DMA2_Stream0->CR |= DMA_SxCR_EN;
        }
        
        /* if instance of handle correspond to ADC1 and  no external trigger present enable software conversion of regular channels */
        //if ((ADC1->CR2 & ADC_CR2_EXTEN) == RESET)
        {
            /* Enable the selected ADC software conversion for regular group */
            ADC1->CR2 |= (uint32_t)ADC_CR2_SWSTART;
        }
        
    }
    else
    {
        Error_Handler();
        /* Update ADC state machine to error */
        //SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
        /* Set ADC error code to ADC IP internal error */
        //SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
    }
    /* Return function status */
    //return HAL_OK;
}


// hdma->XferM1CpltCallback(hdma);
// hdma->XferCpltCallback(hdma);

/* Set the DMA transfer complete callback */
void MY_ADC_DMAConvCplt(bool CT)
{
//   /* Retrieve ADC handle corresponding to current DMA handle */
//   ADC_HandleTypeDef* hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  
//   /* Update state machine on conversion status if not in error state */
//   if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL | HAL_ADC_STATE_ERROR_DMA))
//   {
//     /* Update ADC state machine */
//     SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);
    
//     /* Determine whether any further conversion upcoming on group regular   */
//     /* by external trigger, continuous mode or scan sequence on going.      */
//     /* Note: On STM32F4, there is no independent flag of end of sequence.   */
//     /*       The test of scan sequence on going is done either with scan    */
//     /*       sequence disabled or with end of conversion flag set to        */
//     /*       of end of sequence.                                            */
//     if(ADC_IS_SOFTWARE_START_REGULAR(hadc)                   &&
//        (hadc->Init.ContinuousConvMode == DISABLE)            &&
//        (HAL_IS_BIT_CLR(hadc->Instance->SQR1, ADC_SQR1_L) || 
//         HAL_IS_BIT_CLR(hadc->Instance->CR2, ADC_CR2_EOCS)  )   )
//     {
//       /* Disable ADC end of single conversion interrupt on group regular */
//       /* Note: Overrun interrupt was enabled with EOC interrupt in          */
//       /* HAL_ADC_Start_IT(), but is not disabled here because can be used   */
//       /* by overrun IRQ process below.                                      */
//       __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC);
      
//       /* Set ADC state */
//       CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY);   
      
//       if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_INJ_BUSY))
//       {
//         SET_BIT(hadc->State, HAL_ADC_STATE_READY);
//       }
//     }
    
//     /* Conversion complete callback */
// #if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
//     hadc->ConvCpltCallback(hadc);
// #else
//     HAL_ADC_ConvCpltCallback(hadc);
// #endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
//   }
//   else /* DMA and-or internal error occurred */
//   {
//     if ((hadc->State & HAL_ADC_STATE_ERROR_INTERNAL) != 0UL)
//     {
//       /* Call HAL ADC Error Callback function */
// #if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
//       hadc->ErrorCallback(hadc);
// #else
//       HAL_ADC_ErrorCallback(hadc);
// #endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
//     }
// 	else
// 	{
//       /* Call DMA error callback */
//       hadc->DMA_Handle->XferErrorCallback(hdma);
//     }
//   }
    HAL_GPIO_TogglePin(LED_GREEN_BOARD_GPIO_Port, LED_GREEN_BOARD_Pin);
    
    //__HAL_RCC_ADC1_CLK_DISABLE();
    /* частота семплирования:
        частота PCLK2 = 82MHz
        частота тактирования ADC:  82000000/8 = 10250000
        тактов на канал 480+12 = 492 или 480 ? 
        частота оцифровки ADC /480 = 21354 гц 
        частота оцифровки канала /6 = 3559 гц
        частота прерывания полубуфера = 444.87 гц
        
    */
    static uint32_t temp[ADC_CHENNAL] = {0,0,0,0,0,0};
    for(int i=0; i< LENGHT_BUFF; i+=ADC_CHENNAL )
    {
        temp[0] += uhADCxConvertedValue[ CT ? 0:1 ][i+0];
        temp[1] += uhADCxConvertedValue[ CT ? 0:1 ][i+1];
        temp[2] += uhADCxConvertedValue[ CT ? 0:1 ][i+2];
        temp[3] += uhADCxConvertedValue[ CT ? 0:1 ][i+3];
        temp[4] += uhADCxConvertedValue[ CT ? 0:1 ][i+4];
        temp[5] += uhADCxConvertedValue[ CT ? 0:1 ][i+5];
    }
    if( CT ){
        ADCxConvertedValue[0]=temp[0];
        ADCxConvertedValue[1]=temp[1];
        ADCxConvertedValue[2]=temp[2];
        ADCxConvertedValue[3]=temp[3];
        ADCxConvertedValue[4]=temp[4];
        ADCxConvertedValue[5]=temp[5];
        temp[0]=0;
        temp[1]=0;
        temp[2]=0;
        temp[3]=0;
        temp[4]=0;
        temp[5]=0;
    }
    //HAL_GPIO_TogglePin(LED_GREEN_BOARD_GPIO_Port, LED_GREEN_BOARD_Pin);
}

//   hdma->XferHalfCpltCallback(hdma);
//   hdma->XferM1HalfCpltCallback(hdma);

// не включаю half transfer complete, его не должно быть
/* Set the DMA half transfer complete callback */
void MY_ADC_DMAHalfConvCplt(bool CT)
{
//   ADC_HandleTypeDef* hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
//    /* Half conversion callback */
// #if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
//   hadc->ConvHalfCpltCallback(hadc);
// #else
//   HAL_ADC_ConvHalfCpltCallback(hadc);
// #endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

    /* Disable the half transfer interrupt */
    DMA2_Stream0->CR &= ~(DMA_IT_HT);
    Error_Handler();
}


// /* Set the DMA error callback */
// void MY_ADC_DMAError(void)
// {
//   ADC_HandleTypeDef* hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
//   hadc->State= HAL_ADC_STATE_ERROR_DMA;
//   /* Set ADC error code to DMA error */
//   hadc->ErrorCode |= HAL_ADC_ERROR_DMA;
//    /* Error callback */
// #if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
//   hadc->ErrorCallback(hadc);
// #else
//   HAL_ADC_ErrorCallback(hadc);
// #endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
// }



void MY_ADC_IRQHandler(void)
{
//HAL_ADC_IRQHandler(&hadc1);
    Error_Handler();
}


/* USER CODE END 1 */



