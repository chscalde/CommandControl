/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PowerModes.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBUG (0)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t globalIntterupt = 0;
States globalState = Detumble;
volatile States state;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  /* USER CODE BEGIN 2 */
	
	/* Initialize Variables */
	system_function functions;

	/* Initialize the starting state of each of the systems that need power */
	Initialize_Functions(&functions);

	/* Set Initial state */
	state = Detumble;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	volatile int tmp;
  while (1)
  {
	/* Enter the state machine */
		while(1)
		{
			globalState = state;
			switch (state)
			{
				/* In Detumble mode */
				case (Detumble): 
					//if (DEBUG) printf("\nEntering Detumble mode\n");
					Set_PowerModes(&functions, Detumble);
					//Print_System_Settings(&functions);	// Used for testing and debugging
					// Set the pins high for each rail that should be one here //
					state = Transition(Detumble);
					tmp = state == Detumble ? GPIO_PIN_SET : GPIO_PIN_RESET;
					HAL_GPIO_WritePin(GPIOA, DETUMBLE_Pin, tmp);
					break;
				/* In Kill mode */
				case (Kill): 
					//if (DEBUG) printf("\nEntering Kill mode\n");
					Set_PowerModes(&functions, Kill);
					//Print_System_Settings(&functions);	// Used for testing and debugging
					// Set the pins high for each rail that should be one here //
					state = Transition(Kill);
					tmp = state == Kill ? GPIO_PIN_SET : GPIO_PIN_RESET;
					HAL_GPIO_WritePin(GPIOB, DEAD_Pin, tmp);
					break;
				/* In Normal mode */
				case (Normal): 
					//if (DEBUG) printf("\nEntering Normal mode\n");
					Set_PowerModes(&functions, Normal);
					//Print_System_Settings(&functions);	// Used for testing and debugging
					// Set the pins high for each rail that should be one here //
					state = Transition(Normal);
					tmp = state == Normal ? GPIO_PIN_SET : GPIO_PIN_RESET;
					HAL_GPIO_WritePin(GPIOB, NORMAL_Pin, tmp);
					break;
				/* In UltraLowPower mode */	
				case (UltraLowPower): 
					//if (DEBUG) printf("\nEntering UltraLowPower mode\n");
					Set_PowerModes(&functions, UltraLowPower);
					//Print_System_Settings(&functions);	// Used for testing and debugging
					// Set the pins high for each rail that should be one here //
					state = Transition(UltraLowPower);
					tmp = state == UltraLowPower ? GPIO_PIN_SET : GPIO_PIN_RESET;
					HAL_GPIO_WritePin(GPIOA, ULOW_Pin, tmp);
					break;
				/* In LowPower mode */
				case (LowPower): 
					//if (DEBUG) printf("\nEntering LowPower mode\n");
					Set_PowerModes(&functions, LowPower);
					//Print_System_Settings(&functions);	// Used for testing and debugging
					// Set the pins high for each rail that should be one here //
					state = Transition(LowPower);
					tmp = state == LowPower ? GPIO_PIN_SET : GPIO_PIN_RESET;
					HAL_GPIO_WritePin(GPIOA, LOW_Pin, tmp);
					break;
				/* In Eclipse mode */
				case (Eclipse): 
					//if (DEBUG) printf("\nEntering Eclipse mode\n");
					Set_PowerModes(&functions, Eclipse);
					//Print_System_Settings(&functions);	// Used for testing and debugging
					// Set the pins high for each rail that should be one here //
					state = Transition(Eclipse);
					tmp = state == Eclipse ? GPIO_PIN_SET : GPIO_PIN_RESET;
					HAL_GPIO_WritePin(GPIOC, ECLIPSE_Pin, tmp);
					break;
				/* In ScienceOnly mode */
				case (ScienceOnly): 
					//if (DEBUG) printf("\nEntering ScienceOnly mode\n");
					Set_PowerModes(&functions, ScienceOnly);
					//Print_System_Settings(&functions);	// Used for testing and debugging
					// Set the pins high for each rail that should be one here //
					state = Transition(ScienceOnly);
					tmp = state == ScienceOnly ? GPIO_PIN_SET : GPIO_PIN_RESET;
					HAL_GPIO_WritePin(GPIOB, SCILOG_Pin, tmp);
					break;
				/* An error occurred */
				default:
					state = 255; // Error
					//fprintf(stderr, "\nInvalid state in the state machine reached...\nExiting\n");
					continue;
			}
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

  /**Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, BOARD_LED_Pin|ULOW_Pin|LOW_Pin|DETUMBLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ECLIPSE_GPIO_Port, ECLIPSE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, NORMAL_Pin|DEAD_Pin|SCILOG_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BOARD_LED_Pin ULOW_Pin LOW_Pin DETUMBLE_Pin */
  GPIO_InitStruct.Pin = BOARD_LED_Pin|ULOW_Pin|LOW_Pin|DETUMBLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : BATT_INT_Pin DIE_INT_Pin STABLE_INT_Pin SOLAR_INT_Pin */
  GPIO_InitStruct.Pin = BATT_INT_Pin|DIE_INT_Pin|STABLE_INT_Pin|SOLAR_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ECLIPSE_Pin */
  GPIO_InitStruct.Pin = ECLIPSE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ECLIPSE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SCI_INT_Pin */
  GPIO_InitStruct.Pin = SCI_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SCI_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : NORMAL_Pin DEAD_Pin SCILOG_Pin */
  GPIO_InitStruct.Pin = NORMAL_Pin|DEAD_Pin|SCILOG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
	{
		//HAL_GPIO_TogglePin(GPIOA , GPIO_PIN_5);
		if (GPIO_PIN == STABLE_INT_Pin)
		{
			change_variables(STABLE);
			globalIntterupt = STABLE_INT_Pin;
			//HAL_GPIO_TogglePin(GPIOA , GPIO_PIN_5);
		}
		else if (GPIO_PIN == SCI_INT_Pin)
		{
			change_variables(SCI_EVENT);
			globalIntterupt = SCI_INT_Pin;
			//HAL_GPIO_TogglePin(GPIOA , GPIO_PIN_5);
		}
		else if (GPIO_PIN == DIE_INT_Pin)
		{
			change_variables(DIE);
			globalIntterupt = DIE_INT_Pin;
			HAL_GPIO_TogglePin(GPIOA , GPIO_PIN_5);
		}
		else if (GPIO_PIN == BATT_INT_Pin)
		{
			change_variables(BATT);
			globalIntterupt = BATT_INT_Pin;
			//HAL_GPIO_TogglePin(GPIOA , GPIO_PIN_5);
		}
		else if (GPIO_PIN == SOLAR_INT_Pin)
		{
			change_variables(SOLAR);
			globalIntterupt = SOLAR_INT_Pin;
			//HAL_GPIO_TogglePin(GPIOA , GPIO_PIN_5);
		}
		else 
		{
			__NOP();
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
