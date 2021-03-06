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
#include "string.h"

#include "Fuel_Gauge_Functions.h"
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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
static fg_config_t config = {	6700, 		// design capacity of 3350mAh
															0x7D61, 	// empty voltage target = 2.5V, recovery voltage = 3.88V
															0x8020, 	// model cfg set for lithium NCR/NCA cell
															0x01A0, 	// charge termination current = 0.3A
															0x0000,
															6700/32,
															(6700/32)*44138/6700,
															0x8214,	 	// config1
															0x3658};	// config2							
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	// Initialize UART buffers
	char uartTest[100] = {0};
	char clear[100] = {0};
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////// Create the message beginning the test ///////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	snprintf(uartTest, sizeof(uartTest), "\n\n-----Test Begin-----\n");
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 1);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////// Test the initialization and the value of the configuration registers //////////////////////////// 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Initialize variables to track messages and errors
	uint16_t recv[1] = {0};
	uint8_t err_cnt = 0;

	// Initialize the fuel gauge registers
	init(&hi2c1, config);
	
	// Test each configuration register after initialization
	recv[0] = 0;
	readReg(&hi2c1, DESIGN_CAP_REG, recv);
	if (recv[0] != 6700) {
		err_cnt++;
	}
	
	recv[0] = 0;
	readReg(&hi2c1, V_EMPTY_REG, recv);
	if (recv[0] != 0x7D61) {
		err_cnt++;
	}
	
	recv[0] = 0;
	readReg(&hi2c1, MODEL_CFG_REG, recv);
	if (recv[0] != 0x0020) {
		err_cnt++;
	}

	recv[0] = 0;
	readReg(&hi2c1, I_CHG_REG, recv);
	if (recv[0] != 0x0780) {
		err_cnt++;
	}
	
	recv[0] = 0;
	readReg(&hi2c1, CONF_REG, recv);
	if (recv[0] != 0x8214) {
		err_cnt++;
	}
	
	recv[0] = 0;
	readReg(&hi2c1, CONF2_REG, recv);
	if (recv[0] != 0x3658) {
		err_cnt++;
	}

	// Create the message for the number of config registers that are not what they should be
	snprintf(uartTest, 100, "\nConfig registers not equal to what they should after initialization: %u\n", err_cnt);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , 100, 10);
	if (err_cnt == 6)
	{
		memcpy(uartTest, clear, 100);
		snprintf(uartTest, sizeof(uartTest), "\n-----Error: Test End-----\n\n");
		HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
		return 0;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////// Test the registers with outputs of the fuel gauge //////////////////////////// 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Test the reported remaining capacity register
	float output = 0.0;
	float outputV = 0.0;
	output = Get_Remaining_Capacity(&hi2c1);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nREP_CAP_REG (capacity in mAh): %f\n", output);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the reported state-of-charge percentage output
	output = Get_Charge_Percentage(&hi2c1);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nREP_SOC_REG (state of charge in percent): %f%%\n", output);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the  full capacity register
	output = Get_Max_Capacity(&hi2c1);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nFULL_CAP_REG (full capacity in mAh): %f\n", output);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the estimated time to empty register
	output = Get_TTE(&hi2c1);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nTTE_REG (estimated time to empty): %f\n", output);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the estimated time to full register
	output = Get_TTF(&hi2c1);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nTTF_REG (estimated time to full): %f\n", output);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the status register
	recv[0] = Get_Status(&hi2c1);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nStatus (status of fuel gauge): 0x%02x\n", recv[0]);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the output voltage
	output = Get_Voltage(&hi2c1, 0);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nVoltage (voltage in volts): %f\n", output);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the average output voltage
	output = Get_Voltage(&hi2c1, 1);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nAverage voltage (voltage in volts): %f\n", output);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the output current
	output = Get_Current(&hi2c1, 0);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nCurrent (current in mA): %f\n", output);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the average output current
	output = Get_Current(&hi2c1, 1);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nAverage current (current in mA): %f\n", output);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the temperature
	output = Get_Temp(&hi2c1);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nTemperature (in degrees celcius): %f\n", output);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\n\nVoltage and current every .2 seconds, 30 times\n");
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	for(int i = 0; i < 30; i++)
	{
		outputV = Get_Current(&hi2c1, 0);
		output  = Get_Voltage(&hi2c1, 0);
		memcpy(uartTest, clear, 100);
		snprintf(uartTest, sizeof(uartTest), "\nVoltage (V): %f    Current (mA): %f\n", output, outputV);
		HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
		HAL_Delay(200);
	}
	
	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////// Create the message ending the test ////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\n-----Test End-----\n");
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
