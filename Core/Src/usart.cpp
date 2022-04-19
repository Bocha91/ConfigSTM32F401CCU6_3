/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "main.hpp"
#include "cmsis_os.h"
#include "usart.hpp"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
t_COM com1;
/* USART1 init function */


void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void Init_USART1(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Peripheral clock enable
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    // USART1 GPIO Configuration
    // PA9     ------> USART1_TX
    // PA10     ------> USART1_RX
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    /* Disable the peripheral */
    ((USART1)->CR1 &= ~USART_CR1_UE); //__HAL_UART_DISABLE(huart);

    /* Set the UART Communication parameters */
    // UART_SetConfig(huart);
    {
        /*-------------------------- USART CR2 Configuration -----------------------*/
        /* Configure the UART Stop Bits: Set STOP[13:12] bits
          according to huart->Init.StopBits value */
        MODIFY_REG(USART1->CR2, USART_CR2_STOP, UART_STOPBITS_1);
        /*-------------------------- USART CR1 Configuration -----------------------*/
        /* Configure the UART Word Length, Parity and mode:
          Set the M bits according to huart->Init.WordLength value
          Set PCE and PS bits according to huart->Init.Parity value
          Set TE and RE bits according to huart->Init.Mode value
          Set OVER8 bit according to huart->Init.OverSampling value */
        uint32_t tmpreg = (uint32_t)UART_WORDLENGTH_8B | UART_PARITY_NONE | UART_MODE_TX_RX | UART_OVERSAMPLING_16;
        MODIFY_REG(USART1->CR1,
                   (uint32_t)(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8),
                   tmpreg);

        /*-------------------------- USART CR3 Configuration -----------------------*/
        /* Configure the UART HFC: Set CTSE and RTSE bits according to huart->Init.HwFlowCtl value */
        MODIFY_REG(USART1->CR3, (USART_CR3_RTSE | USART_CR3_CTSE), UART_HWCONTROL_NONE);

        uint32_t pclk = HAL_RCC_GetPCLK2Freq(); // USATR1 � USATR6
        // uint32_t pclk = HAL_RCC_GetPCLK1Freq(); // USATR2

        /*-------------------------- USART BRR Configuration ---------------------*/
        // USART1->BRR = UART_BRR_SAMPLING8(pclk, 115200);
        USART1->BRR = UART_BRR_SAMPLING16(pclk, 115200);
    }

    /* In asynchronous mode, the following bits must be kept cleared:
       - LINEN and CLKEN bits in the USART_CR2 register,
       - SCEN, HDSEL and IREN  bits in the USART_CR3 register.*/
    CLEAR_BIT(USART1->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
    CLEAR_BIT(USART1->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

    /* Enable the peripheral */
    (USART1->CR1 |= USART_CR1_UE); //__HAL_UART_ENABLE(huart);

    // Initialize the UART state
    // com1.ErrorCode = HAL_UART_ERROR_NONE;
    // huart->gState = HAL_UART_STATE_READY;
    // huart->RxState = HAL_UART_STATE_READY;
}

// обработка прерывания от USART1, упрастил как мог
void MY_UART1_IRQHandler(void)
{
    uint32_t isrflags = READ_REG(USART1->SR);
    uint32_t cr1its = READ_REG(USART1->CR1);
    uint32_t cr3its = READ_REG(USART1->CR3);
    uint32_t errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));

    /* If some errors occur */
    if ((errorflags != RESET) && (((cr3its & USART_CR3_EIE) != RESET) || ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != RESET)))
    {
        /* UART parity error interrupt occurred ----------------------------------*/
        if (((isrflags & USART_SR_PE) != RESET) && ((cr1its & USART_CR1_PEIE) != RESET))
        {
            com1.ErrorCode |= HAL_UART_ERROR_PE;
        }

        /* UART noise error interrupt occurred -----------------------------------*/
        if (((isrflags & USART_SR_NE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
        {
            com1.ErrorCode |= HAL_UART_ERROR_NE;
        }

        /* UART frame error interrupt occurred -----------------------------------*/
        if (((isrflags & USART_SR_FE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
        {
            com1.ErrorCode |= HAL_UART_ERROR_FE;
        }

        /* UART Over-Run interrupt occurred --------------------------------------*/
        if (((isrflags & USART_SR_ORE) != RESET) && (((cr1its & USART_CR1_RXNEIE) != RESET) || ((cr3its & USART_CR3_EIE) != RESET)))
        {
            com1.ErrorCode |= HAL_UART_ERROR_ORE;
        }
    } /* End if some error occurs */

    /* UART in mode Receiver -------------------------------------------------*/
    if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
    {
        com1.UART_Receive_IT();
        return;
    }

    /* UART in mode Transmitter ------------------------------------------------*/
    if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
    {
        com1.UART_Transmit_IT();
        return;
    }

    /* UART in mode Transmitter end --------------------------------------------*/
    if (((isrflags & USART_SR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))
    {
        com1.UART_EndTransmit_IT();
        return;
    }
}


/* USER CODE END 1 */
