/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma.c
  * @brief   This file provides code for the configuration
  *          of all the requested memory to memory DMA transfers.
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

/* Includes ------------------------------------------------------------------*/
#include "dma.hpp"


/*----------------------------------------------------------------------------*/
/* Configure DMA                                                              */
/*----------------------------------------------------------------------------*/
uint32_t hdma_ErrorCode  = HAL_DMA_ERROR_NONE; 

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}
/* Private types -------------------------------------------------------------*/
typedef struct
{
    __IO uint32_t ISR; /*!< DMA interrupt status register */
    __IO uint32_t Reserved0;
    __IO uint32_t IFCR; /*!< DMA interrupt flag clear register */
} DMA_Base_Registers;
DMA_Base_Registers * const REGS =  (DMA_Base_Registers *)DMA2_BASE; // для DMA2_Stream0
const uint32_t STREAM_INDEX = 0U;  // для DMA2_Stream0


#define MY__HAL_DMA_GET_IT_SOURCE(__HANDLE__,__INTERRUPT__) (((__INTERRUPT__) != DMA_IT_FE)? ((__HANDLE__)->CR & (__INTERRUPT__)) : ((__HANDLE__)->FCR & (__INTERRUPT__)))
/* USER CODE BEGIN 2 */
void MY_DMA_IRQHandler(void)
{
  //HAL_DMA_IRQHandler(&hdma_adc1);
  uint32_t tmpisr;
  //__IO uint32_t count = 0U;
  //uint32_t timeout = SystemCoreClock / 9600U;

  /* calculate DMA base and stream number */
  //DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->StreamBaseAddress;

  tmpisr = REGS->ISR; //tmpisr = regs->ISR;
  
  /* Transfer Error Interrupt management ***************************************/
  if ((tmpisr & (DMA_FLAG_TEIF0_4 << STREAM_INDEX)) != RESET)
  {
    if(MY__HAL_DMA_GET_IT_SOURCE(DMA2_Stream0, DMA_IT_TE) != RESET)
    {
      /* Disable the transfer error interrupt */
      DMA2_Stream0->CR  &= ~(DMA_IT_TE);
      
      /* Clear the transfer error flag */
      REGS->IFCR = DMA_FLAG_TEIF0_4 << STREAM_INDEX;
      
      /* Update error code */
      hdma_ErrorCode |= HAL_DMA_ERROR_TE;
    }
  }
  /* FIFO Error Interrupt management ******************************************/
  if ((tmpisr & (DMA_FLAG_FEIF0_4 << STREAM_INDEX)) != RESET)
  {
    if(MY__HAL_DMA_GET_IT_SOURCE(DMA2_Stream0, DMA_IT_FE) != RESET)
    {
      /* Clear the FIFO error flag */
      REGS->IFCR = DMA_FLAG_FEIF0_4 << STREAM_INDEX;

      /* Update error code */
      hdma_ErrorCode |= HAL_DMA_ERROR_FE;
    }
  }
  /* Direct Mode Error Interrupt management ***********************************/
  if ((tmpisr & (DMA_FLAG_DMEIF0_4 << STREAM_INDEX)) != RESET)
  {
    if(MY__HAL_DMA_GET_IT_SOURCE(DMA2_Stream0, DMA_IT_DME) != RESET)
    {
      /* Clear the direct mode error flag */
      REGS->IFCR = DMA_FLAG_DMEIF0_4 << STREAM_INDEX;

      /* Update error code */
      hdma_ErrorCode |= HAL_DMA_ERROR_DME;
    }
  }
  /* Half Transfer Complete Interrupt management ******************************/
  if ((tmpisr & (DMA_FLAG_HTIF0_4 << STREAM_INDEX)) != RESET)
  {
    if(MY__HAL_DMA_GET_IT_SOURCE(DMA2_Stream0, DMA_IT_HT) != RESET)
    {
      /* Clear the half transfer complete flag */
      REGS->IFCR = DMA_FLAG_HTIF0_4 << STREAM_INDEX;
      
      /* Multi_Buffering mode enabled */
      //if(((DMA2_Stream0->CR) & (uint32_t)(DMA_SxCR_DBM)) != RESET)
      // {
      //   /* Current memory buffer used is Memory 0 */
      //   if((DMA2_Stream0->CR & DMA_SxCR_CT) == RESET)
      //   {
      //     if(hdma->XferHalfCpltCallback != NULL)
      //     {
      //       /* Half transfer callback */
      //       hdma->XferHalfCpltCallback(hdma);
      //       hdma->XferM1HalfCpltCallback(hdma);
      //     }
      //   }
      //   /* Current memory buffer used is Memory 1 */
      //   else
      //   {
      //     if(hdma->XferM1HalfCpltCallback != NULL)
      //     {
      //       /* Half transfer callback */
      //       hdma->XferM1HalfCpltCallback(hdma);
      //     }
      //   }
      // }
      // else
      // {
      //   /* Disable the half transfer interrupt if the DMA mode is not CIRCULAR */
      //   if((DMA2_Stream0->CR & DMA_SxCR_CIRC) == RESET)
      //   {
      //     /* Disable the half transfer interrupt */
      //     DMA2_Stream0->CR &= ~(DMA_IT_HT);
      //   }
        
      //   if(hdma->XferHalfCpltCallback != NULL)
      //   {
      //     /* Half transfer callback */
      //     hdma->XferHalfCpltCallback(hdma);
      //   }
      // }
      MY_ADC_DMAHalfConvCplt(DMA2_Stream0->CR & DMA_SxCR_CT);

    }
  }
  /* Transfer Complete Interrupt management ***********************************/
  if ((tmpisr & (DMA_FLAG_TCIF0_4 << STREAM_INDEX)) != RESET)
  {
    if(MY__HAL_DMA_GET_IT_SOURCE(DMA2_Stream0, DMA_IT_TC) != RESET)
    {
      /* Clear the transfer complete flag */
      REGS->IFCR = DMA_FLAG_TCIF0_4 << STREAM_INDEX;
      
      // if(HAL_DMA_STATE_ABORT == hdma->State)
      // {
      //   /* Disable all the transfer interrupts */
      //   DMA2_Stream0->CR  &= ~(DMA_IT_TC | DMA_IT_TE | DMA_IT_DME);
      //   DMA2_Stream0->FCR &= ~(DMA_IT_FE);
        
      //   if((hdma->XferHalfCpltCallback != NULL) || (hdma->XferM1HalfCpltCallback != NULL))
      //   {
      //     DMA2_Stream0->CR  &= ~(DMA_IT_HT);
      //   }

      //   /* Clear all interrupt flags at correct offset within the register */
      //   REGS->IFCR = 0x3FU << STREAM_INDEX;

      //   /* Change the DMA state */
      //   hdma->State = HAL_DMA_STATE_READY;

      //   /* Process Unlocked */
      //   //__HAL_UNLOCK(hdma);

      //   if(hdma->XferAbortCallback != NULL)
      //   {
      //     hdma->XferAbortCallback(hdma);
      //   }
      //   return;
      // }

      // if(((DMA2_Stream0->CR) & (uint32_t)(DMA_SxCR_DBM)) != RESET)
      // {
      //   /* Current memory buffer used is Memory 0 */
      //   if((DMA2_Stream0->CR & DMA_SxCR_CT) == RESET)
      //   {
      //     if(hdma->XferM1CpltCallback != NULL)
      //     {
      //       /* Transfer complete Callback for memory1 */
      //       hdma->XferM1CpltCallback(hdma);
      //       hdma->XferCpltCallback(hdma);
      //     }
      //   }
      //   /* Current memory buffer used is Memory 1 */
      //   else
      //   {
      //     if(hdma->XferCpltCallback != NULL)
      //     {
      //       /* Transfer complete Callback for memory0 */
      //       hdma->XferCpltCallback(hdma);
      //     }
      //   }
      // }
      // /* Disable the transfer complete interrupt if the DMA mode is not CIRCULAR */
      // else
      // {
      //   if((DMA2_Stream0->CR & DMA_SxCR_CIRC) == RESET)
      //   {
      //     /* Disable the transfer complete interrupt */
      //     DMA2_Stream0->CR  &= ~(DMA_IT_TC);

      //     /* Change the DMA state */
      //     hdma->State = HAL_DMA_STATE_READY;

      //     /* Process Unlocked */
      //     __HAL_UNLOCK(hdma);
      //   }

      //   if(hdma->XferCpltCallback != NULL)
      //   {
      //     /* Transfer complete callback */
      //     hdma->XferCpltCallback(hdma);
      //   }
      // }
      MY_ADC_DMAConvCplt( DMA2_Stream0->CR & DMA_SxCR_CT );
    }
  }
  
  /* manage error case */
  if(hdma_ErrorCode != HAL_DMA_ERROR_NONE)
  {

  //   if((hdma_ErrorCode & HAL_DMA_ERROR_TE) != RESET)
  //   {
  //     hdma->State = HAL_DMA_STATE_ABORT;

  //     /* Disable the stream */
  //     __HAL_DMA_DISABLE(hdma);

  //     do
  //     {
  //       if (++count > timeout)
  //       {
  //         break;
  //       }
  //     }
  //     while((DMA2_Stream0->CR & DMA_SxCR_EN) != RESET);

  //     /* Change the DMA state */
  //     hdma->State = HAL_DMA_STATE_READY;

  //     /* Process Unlocked */
  //     __HAL_UNLOCK(hdma);
  //   }

  //   if(hdma->XferErrorCallback != NULL)
  //   {
  //     /* Transfer error callback */
  //     hdma->XferErrorCallback(hdma);
  //   }
  // }
    Error_Handler();
  }
}
/* USER CODE END 2 */

