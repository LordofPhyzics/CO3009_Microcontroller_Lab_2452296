/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
const int MAX_LED = 4;
int index_led = 0;
int led_buffer[4] = {0, 0, 0, 0};

int hour = 15;
int minute = 8;
int second = 50;

// For ex3.9 , 3.10
const int MAX_LED_MATRIX = 8;
int index_led_matrix = 0;
uint8_t matrix_buffer[8] = {0x00, 0xFC, 0x12, 0x11, 0x11, 0x12, 0xFC, 0x00};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void display7SEG(int num);
void update7SEG(int index);
void updateClockBuffer(void);
void updateLEDMatrix(int index);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// --- TIMER 0 (for clock and DOT 1000ms) ---
int timer0_counter = 0;
int timer0_flag = 0;

// --- TIMER 1 (7 led segment - 250ms) ---
int timer1_counter = 0;
int timer1_flag = 0;

// --- TIMER 2 ( LED Matrix - 20ms) ---
int timer2_counter = 0;
int timer2_flag = 0;

int TIMER_CYCLE = 10; // TIM2 10ms

// --- TIMER 3 (Animation shifting- 250ms) ---
int timer3_counter = 0;
int timer3_flag = 0;


void setTimer0(int duration) {
    timer0_counter = duration / TIMER_CYCLE;
    timer0_flag = 0;
}

void setTimer1(int duration) {
    timer1_counter = duration / TIMER_CYCLE;
    timer1_flag = 0;
}

void setTimer2(int duration) {
    timer2_counter = duration / TIMER_CYCLE;
    timer2_flag = 0;
}
void setTimer3(int duration) {
    timer3_counter = duration / TIMER_CYCLE;
    timer3_flag = 0;
}

void timer_run() {
    if (timer0_counter > 0) {
        timer0_counter--;
        if (timer0_counter == 0) timer0_flag = 1;
    }
    if (timer1_counter > 0) {
        timer1_counter--;
        if (timer1_counter == 0) timer1_flag = 1;
    }
    if (timer2_counter > 0) {
        timer2_counter--;
        if (timer2_counter == 0) timer2_flag = 1;
    }
    if (timer3_counter > 0) {
           timer3_counter--;
           if (timer3_counter == 0) timer3_flag = 1;
       }
}

void shiftLeft(void) {
    uint8_t temp = matrix_buffer[0]; // Keep the first column
    for (int i = 0; i < 7; i++) {
        matrix_buffer[i] = matrix_buffer[i + 1]; // Shift columns to the left
    }
    matrix_buffer[7] = temp; //Move the first column to the end to rotate.
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  // Function prototype
  HAL_TIM_Base_Start_IT (& htim2 );
  HAL_GPIO_WritePin(GPIOA, EN0_Pin | EN1_Pin | EN2_Pin | EN3_Pin, GPIO_PIN_SET);

  //Update initial val to led_buff
  updateClockBuffer();
  setTimer0(1000);
  setTimer1(250);
  setTimer2(20);
  setTimer3(250);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // --- TASK 1: 7 led segment ( 250ms) ---
	        if (timer1_flag == 1) {
	            setTimer1(250);

	            update7SEG(index_led);
	            index_led++;
	            if (index_led >= MAX_LED) {
	                index_led = 0;
	            }
	        }

	        // --- TASK 2:CLOCK & DOT (1000ms) ---
	        if (timer0_flag == 1) {
	            setTimer0(1000);

	            // DOT
	            HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);

	            second++;
	            if (second >= 60) {
	                second = 0;
	                minute++;
	            }
	            if (minute >= 60) {
	                minute = 0;
	                hour++;
	            }
	            if (hour >= 24) {
	                hour = 0;
	            }

	            updateClockBuffer();
	        }

	        // --- TASK 3: matrix LED 8x8 (every 20ms) ---
	             if (timer2_flag == 1) {
	                 setTimer2(20);

	                 updateLEDMatrix(index_led_matrix);
	                 index_led_matrix++;
	                 if (index_led_matrix >= MAX_LED_MATRIX) {
	                     index_led_matrix = 0;
	                 }
	             }
	         //TASK 4 : Shifting
	             if (timer3_flag == 1) {
	                       setTimer3(250);
	                       shiftLeft();    // Shift the columns of the letter 'A' one step to the left.
	                   }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|DOT_Pin|LED_RED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SEG0_Pin|SEG1_Pin|SEG2_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|ROW0_Pin|ROW1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENM0_Pin ENM1_Pin DOT_Pin LED_RED_Pin
                           EN0_Pin EN1_Pin EN2_Pin EN3_Pin
                           ENM2_Pin ENM3_Pin ENM4_Pin ENM5_Pin
                           ENM6_Pin ENM7_Pin */
  GPIO_InitStruct.Pin = ENM0_Pin|ENM1_Pin|DOT_Pin|LED_RED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SEG0_Pin SEG1_Pin SEG2_Pin ROW2_Pin
                           ROW3_Pin ROW4_Pin ROW5_Pin ROW6_Pin
                           ROW7_Pin SEG3_Pin SEG4_Pin SEG5_Pin
                           SEG6_Pin ROW0_Pin ROW1_Pin */
  GPIO_InitStruct.Pin = SEG0_Pin|SEG1_Pin|SEG2_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|ROW0_Pin|ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void updateClockBuffer(void) {
    // hour: 15 -> led_buffer[0] = 1, led_buffer[1] = 5
    led_buffer[0] = hour / 10;
    led_buffer[1] = hour % 10;

    // minute: 8 -> led_buffer[2] = 0, led_buffer[3] = 8
    led_buffer[2] = minute / 10;
    led_buffer[3] = minute % 10;
}

void display7SEG(int num){
	//On = 0, OFF =1
	uint8_t hex_code[10] = {0x40 , 0x79 , 0x24 , 0x30 , 0x19 , 0x12 ,0x02 , 0x78 , 0x00 , 0x10 };

	if (num<0 || num>9) return;
	uint8_t code = hex_code[num];
	// Using shifting bit & AND logic to isolate the state of each pin
		HAL_GPIO_WritePin(GPIOB , SEG0_Pin, (( code >> 0) & 0x01 ) ? GPIO_PIN_SET : GPIO_PIN_RESET ) ;
		HAL_GPIO_WritePin(GPIOB , SEG1_Pin, (( code >> 1) & 0x01 ) ? GPIO_PIN_SET : GPIO_PIN_RESET ) ;
		HAL_GPIO_WritePin(GPIOB , SEG2_Pin, (( code >> 2) & 0x01 ) ? GPIO_PIN_SET : GPIO_PIN_RESET ) ;
		HAL_GPIO_WritePin(GPIOB , SEG3_Pin, (( code >> 3) & 0x01 ) ? GPIO_PIN_SET : GPIO_PIN_RESET ) ;
		HAL_GPIO_WritePin(GPIOB , SEG4_Pin, (( code >> 4) & 0x01 ) ? GPIO_PIN_SET : GPIO_PIN_RESET ) ;
		HAL_GPIO_WritePin(GPIOB , SEG5_Pin, (( code >> 5) & 0x01 ) ? GPIO_PIN_SET : GPIO_PIN_RESET ) ;
		HAL_GPIO_WritePin(GPIOB , SEG6_Pin, (( code >> 6) & 0x01 ) ? GPIO_PIN_SET : GPIO_PIN_RESET ) ;


}


void update7SEG(int index) {
    // Turn off all four LEDs before turning on a new one to prevent light bleed.
    HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);

    switch (index) {
        case 0:
            display7SEG(led_buffer[0]);
            HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET);
            break;
        case 1:
            display7SEG(led_buffer[1]);
            HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);
            break;
        case 2:
            display7SEG(led_buffer[2]);
            HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);
            break;
        case 3:
            display7SEG(led_buffer[3]);
            HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
	        // The interrupt function is dedicated solely to running the timer; it is extremely lightweight and fast.
	        timer_run();
	    }
}
//Function to output data to 8 lines (PB8 -> PB15)
void setRowMatrix(uint8_t value) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,  (value & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,  (value & 0x02) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (value & 0x04) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, (value & 0x08) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, (value & 0x10) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, (value & 0x20) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, (value & 0x40) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, (value & 0x80) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
// Function to disable all 8 ENM columns to prevent image ghosting.
void clearAllColumns(void) {
	 HAL_GPIO_WritePin(GPIOA, ENM0_Pin | ENM1_Pin | ENM2_Pin | ENM3_Pin |
	                             ENM4_Pin | ENM5_Pin | ENM6_Pin | ENM7_Pin, GPIO_PIN_SET);
}
void updateLEDMatrix(int index) {
	// off all cols
	    clearAllColumns();

	    // extract row data
	    setRowMatrix(matrix_buffer[index]);

	    // Enable ONLY the index column: Output RESET (0) to trigger the ULN interrupt;
	    switch (index) {
	        case 0:
	            HAL_GPIO_WritePin(GPIOA, ENM0_Pin, GPIO_PIN_RESET);
	            break;
	        case 1:
	            HAL_GPIO_WritePin(GPIOA, ENM1_Pin, GPIO_PIN_RESET);
	            break;
	        case 2:
	            HAL_GPIO_WritePin(GPIOA, ENM2_Pin, GPIO_PIN_RESET);
	            break;
	        case 3:
	            HAL_GPIO_WritePin(GPIOA, ENM3_Pin, GPIO_PIN_RESET);
	            break;
	        case 4:
	            HAL_GPIO_WritePin(GPIOA, ENM4_Pin, GPIO_PIN_RESET);
	            break;
	        case 5:
	            HAL_GPIO_WritePin(GPIOA, ENM5_Pin, GPIO_PIN_RESET);
	            break;
	        case 6:
	            HAL_GPIO_WritePin(GPIOA, ENM6_Pin, GPIO_PIN_RESET);
	            break;
	        case 7:
	            HAL_GPIO_WritePin(GPIOA, ENM7_Pin, GPIO_PIN_RESET);
	            break;
	        default:
	            break;
	    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
