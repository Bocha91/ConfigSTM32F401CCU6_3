/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.hpp"
#include "cmsis_os.h"
#include "usart.hpp"
#include "adc.hpp"
#include <stdlib.h>


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticTimer_t osStaticTimerDef_t;
typedef StaticEventGroup_t osStaticEventGroupDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[128];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .cb_mem = &defaultTaskControlBlock,
    .cb_size = sizeof(defaultTaskControlBlock),
    .stack_mem = &defaultTaskBuffer[0],
    .stack_size = sizeof(defaultTaskBuffer),
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for scanTask02 */
osThreadId_t scanTask02Handle;
uint32_t scanTask02Buffer[128];
osStaticThreadDef_t scanTask02ControlBlock;
const osThreadAttr_t scanTask02_attributes = {
    .name = "scanTask02",
    .cb_mem = &scanTask02ControlBlock,
    .cb_size = sizeof(scanTask02ControlBlock),
    .stack_mem = &scanTask02Buffer[0],
    .stack_size = sizeof(scanTask02Buffer),
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for Rx1Task03 */
osThreadId_t Rx1Task03Handle;
uint32_t Rx1Task03Buffer[128];
osStaticThreadDef_t Rx1Task03ControlBlock;
const osThreadAttr_t Rx1Task03_attributes = {
    .name = "Rx1Task03",
    .cb_mem = &Rx1Task03ControlBlock,
    .cb_size = sizeof(Rx1Task03ControlBlock),
    .stack_mem = &Rx1Task03Buffer[0],
    .stack_size = sizeof(Rx1Task03Buffer),
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for Tx1Task04 */
osThreadId_t Tx1Task04Handle;
uint32_t Tx1Task04Buffer[128];
osStaticThreadDef_t Tx1Task04ControlBlock;
const osThreadAttr_t Tx1Task04_attributes = {
    .name = "Tx1Task04",
    .cb_mem = &Tx1Task04ControlBlock,
    .cb_size = sizeof(Tx1Task04ControlBlock),
    .stack_mem = &Tx1Task04Buffer[0],
    .stack_size = sizeof(Tx1Task04Buffer),
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for myQueue01 */
osMessageQueueId_t myQueue01Handle;
const osMessageQueueAttr_t myQueue01_attributes = {
    .name = "myQueue01"};
/* Definitions for iTimer01 */
osTimerId_t iTimer01Handle;
osStaticTimerDef_t iTimer01ControlBlock;
const osTimerAttr_t iTimer01_attributes = {
    .name = "iTimer01",
    .cb_mem = &iTimer01ControlBlock,
    .cb_size = sizeof(iTimer01ControlBlock),
};
/* Definitions for myMutex01 */
osMutexId_t myMutex01Handle;
const osMutexAttr_t myMutex01_attributes = {
    .name = "myMutex01"};
/* Definitions for myEvent01 */
osEventFlagsId_t myEvent01Handle;
osStaticEventGroupDef_t myEvent01ControlBlock;
const osEventFlagsAttr_t myEvent01_attributes = {
    .name = "myEvent01",
    .cb_mem = &myEvent01ControlBlock,
    .cb_size = sizeof(myEvent01ControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartScanTask02(void *argument);
void StartRxTask03(void *argument);
void StartTxTask04(void *argument);
void iCallback01(void *argument);

#ifdef __cplusplus
extern "C"
{
#endif

    void MX_USB_DEVICE_Init(void);
    void SystemClock_Config(void);

#ifdef __cplusplus
}
#endif

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */
    /* Create the mutex(es) */
    /* creation of myMutex01 */
    myMutex01Handle = osMutexNew(&myMutex01_attributes);

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* Create the timer(s) */
    /* creation of iTimer01 */
    iTimer01Handle = osTimerNew(iCallback01, osTimerPeriodic, NULL, &iTimer01_attributes);

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* Create the queue(s) */
    /* creation of myQueue01 */
    myQueue01Handle = osMessageQueueNew(16, sizeof(uint16_t), &myQueue01_attributes);

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of defaultTask */
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* creation of scanTask02 */
    scanTask02Handle = osThreadNew(StartScanTask02, NULL, &scanTask02_attributes);

    /* creation of Rx1Task03 */
    Rx1Task03Handle = osThreadNew(StartRxTask03, NULL, &Rx1Task03_attributes);

    /* creation of Tx1Task04 */
    Tx1Task04Handle = osThreadNew(StartTxTask04, NULL, &Tx1Task04_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* creation of myEvent01 */
    myEvent01Handle = osEventFlagsNew(&myEvent01_attributes);

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
    /* init code for USB_DEVICE */
    MX_USB_DEVICE_Init();
    /* USER CODE BEGIN StartDefaultTask */
    MY_ADC_Start_DMA();
    /* Infinite loop */
    for (;;)
    {
        //HAL_GPIO_TogglePin(LED_GREEN_BOARD_GPIO_Port, LED_GREEN_BOARD_Pin);
        osDelay(750);
    }
    /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartScanTask02 */
/**
 * @brief Function implementing the scanTask02 thread.
 * @param argument: Not used
 * @retval None
 */
uint8_t key = 0, old_key = 0;
#include "StrLib.hpp"
/* USER CODE END Header_StartScanTask02 */
void StartScanTask02(void *argument)
{
    static uint8_t scan[4]={0,0,0,0};
    static int krutilka[2]={0,0};
    static int GSR=0;
    /* USER CODE BEGIN StartScanTask02 */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);  // 1000 Гц
        static int del5 = 0;
        if(++del5>=25){ // 40Гц

            del5 = 0;
            uint16_t in = ~(KEY_ESC_GPIO_Port->IDR & (KEY_ESC_Pin | KEY_ENTER_Pin | KEY_LEFT_Pin | KEY_RIGHT_Pin | KEY_UP_Pin | KEY_DOWN_Pin));
            scan[1] = scan[0];
            scan[2] = scan[1];
            scan[3] = scan[2];
            scan[0] = ((in >> 4) & 0x0F) | ((in >> 10) & 0x30);

            uint8_t down = scan[0] & scan[1] & scan[2] & scan[3];
            uint8_t up = scan[0] | scan[1] | scan[2] | scan[3];
            key = (old_key & up) | down;
        }
        // *************** out *************
        static _MyString<64>  str;
        str.Clear();
        str.Write('[');

        //[f7,ff,33f,12D] 
        if (key != old_key)
        {
            str.uint8toH(old_key);
            str.Write(',');
            str.uint8toH(key);
            str.Write(',');

            old_key = key;
        }else{
            str.Write(',');
            str.Write(',');
        }
        
        if(ADCxCOMPLIT == 1){  // 100 гц
            ADCxCOMPLIT = 0;

//HAL_GPIO_TogglePin(LED_GREEN_BOARD_GPIO_Port, LED_GREEN_BOARD_Pin);

            if( (abs(krutilka[0]-ADCxConvertedValue[0]) > 256)
            ||  (abs(krutilka[1]-ADCxConvertedValue[1]) > 256) 
            ||  (str.GetCount() > 5) // если кнопки изменились
            )
            {
                str.uint16toH(ADCxConvertedValue[0]/16);
                str.Write(',');
                str.uint16toH(ADCxConvertedValue[1]/16);
                str.Write(',');
                krutilka[0]=ADCxConvertedValue[0];
                krutilka[1]=ADCxConvertedValue[1];
            }

            static int GSR_count = 0;
            GSR += ADCxConvertedValue[2];
            if( ++GSR_count >= 10 )
            {
                if((str.GetCount() < 5)) // если кнопки изменились
                {
                    str.Write(',');
                    str.Write(',');
                }
                str.uint16toH(GSR/16);
                GSR_count=0;
                GSR=0;

HAL_GPIO_TogglePin(LED_GREEN_BOARD_GPIO_Port, LED_GREEN_BOARD_Pin);

            }
        }else  if((str.GetCount() > 5)) // если кнопки изменились
        {
            str.uint16toH(ADCxConvertedValue[0]/16);
            str.Write(',');
            str.uint16toH(ADCxConvertedValue[1]/16);
            str.Write(',');
        }

        if(str.GetCount() > 5)
        {
            str.Writes("]\n\r");
            com1.Puts(str.Reads());
        }

        // *************** out *************


    }
    /* USER CODE END StartScanTask02 */
}

/* USER CODE BEGIN Header_StartRxTask03 */
/**
 * @brief Function implementing the Rx1Task03 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartRxTask03 */
void StartRxTask03(void *argument)
{
    /* USER CODE BEGIN StartRxTask03 */
    /* Infinite loop */
    com1.Rx.EnableInterrupt();

    for (;;)
    {
        HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, (com1.Get() & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        // osDelay(1);
    }
    /* USER CODE END StartRxTask03 */
}

/* USER CODE BEGIN Header_StartTxTask04 */
/**
 * @brief Function implementing the Tx1Task04 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTxTask04 */
void StartTxTask04(void *argument)
{
    /* USER CODE BEGIN StartTxTask04 */
    /* Infinite loop */

    //com1.Puts("\r\nDmitry Hello!\r\n");

    for (;;)
    {
        osDelay(1000);
/*
        _MyString<6*5+3>  str;
        str.uint16toH(ADCxConvertedValue[0]);
        str.Write(' ');
        str.uint16toH(ADCxConvertedValue[1]);
        str.Write(' ');
        str.uint16toH(ADCxConvertedValue[2]);
        str.Write(' ');
        str.uint16toH(ADCxConvertedValue[3]);
        str.Write(' ');
        str.uint16toH(ADCxConvertedValue[4]);
        str.Write(' ');
        str.uint16toH(ADCxConvertedValue[5]);
        str.Writes("\n\r");
        com1.Puts(str.Reads());
*/        

    }
    /* USER CODE END StartTxTask04 */
}

/* iCallback01 function */
void iCallback01(void *argument)
{
    /* USER CODE BEGIN iCallback01 */

    /* USER CODE END iCallback01 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
