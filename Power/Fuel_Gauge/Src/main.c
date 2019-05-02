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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// I2C slave address
#define FG_SLAVE_ADDR 	(0x36 << 1)

// Bit masks
#define POR								0x0002
#define FSTAT_DNR					0x0001
#define MODEL_CFG_REFRESH	0x8000

// configuration registers
#define DESIGN_CAP_REG 	0x18 // sets design capacity of the battery
#define V_EMPTY_REG 		0x3A // sets empty voltage and recovery voltage thresholds
#define	MODEL_CFG_REG		0xDB // sets options for the EZ algorithm dependant on battery model
#define I_CHG_REG 			0x1E // sets charge termination based on current threshold
#define CONF_REG				0x1D // enables/disables various features of the chip
#define CONF2_REG				0xBB // enables/disables various features of the chip
#define DPACC_REG				0x46 // tracks change in battery state of charge (1/16% per LSB)
#define DQACC_REG				0x45

// output registers
#define REP_CAP_REG			0x05
#define REP_SOC_REG			0x06
#define FULL_CAP_REG		0x10
#define TTE_REG					0x11
#define TTF_REG					0x20
#define STAT_REG				0x00
#define FSTAT_REG				0x3D
#define HIBCFG_REG			0xBA

// Command register
#define SOFT_WAKEUP_REG	0x60
#define SOFT_WAKE				0x90

// other
#define resistSensor 	0.01 // Ohms
#define CLEAR					0x00
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
typedef struct fg_config_t {
	uint16_t design_cap;
	uint16_t v_empty;
	uint16_t model_cfg;
	uint16_t current_chg;
	uint16_t hibcfg;
	uint16_t dQAcc;
	uint16_t dPAcc;
	uint16_t config1;
	uint16_t config2;
} fg_config_t;

static fg_config_t config = {	6700, 		// design capacity of 3350mAh
															0x7D61, 	// empty voltage target = 2.5V, recovery voltage = 3.88V
															0x8020, 	// model cfg set for lithium NCR/NCA cell
															0x0780, 	// charge termination current = 0.3A
															6700/32,
															(6700/32)*51200/6700,
															0x8214,	 	// config1
															0x3658};	// config2


// Multipliers are constants used to multiply register value in order to get final result
float capacity_multiplier_mAH = (0.005)/resistSensor; //refer to row "Capacity"
float current_multiplier_mV = (.0015625)/resistSensor; //refer to row "Current"
float voltage_multiplier_V = .000078125; //refer to row "Voltage"
float time_multiplier_Hours = 5.625/3600.0; //Least Significant Bit= 5.625 seconds, 3600 converts it to Hours. refer to AN6358 pg 13 figure 1.3 in row "Time"
float percentage_multiplier = 1.0/256.0; //refer to row "Percentage"
															
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void readReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *recv);
void writeReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *send);
void init(I2C_HandleTypeDef *hi2c, fg_config_t conf);
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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	
	// Initialize UART buffers
	char uartTest[100] = {0};
	char clear[100] = {0};
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////// Create the message beginning the test ///////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	snprintf(uartTest, sizeof(uartTest), "\n-----Test Begin-----\n");
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
	if (recv[0] != 0x8020) {
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
	recv[0] = 0;
	readReg(&hi2c1, REP_CAP_REG, recv);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nREP_CAP_REG (capacity in mAh): 0x%02x\n", recv[0]);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the reported state-of-charge percentage output
	recv[0] = 0;
	readReg(&hi2c1, REP_SOC_REG, recv);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nREP_SOC_REG (state of charge in percent): %d%%\n", recv[0]);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the  full capacity register
	recv[0] = 0;
	readReg(&hi2c1, FULL_CAP_REG, recv);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nFULL_CAP_REG (full capacity in mAh): 0x%02x\n", recv[0]);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the estimated time to empty register
	recv[0] = 0;
	readReg(&hi2c1, TTE_REG, recv);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nTTE_REG (estimated time to empty): 0x%02x\n", recv[0]);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the status register
	recv[0] = 0;
	readReg(&hi2c1, TTF_REG, recv);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nTTF_REG (estimated time to full): 0x%02x\n", recv[0]);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	// Test the reported remaining capacity register
	recv[0] = 0;
	readReg(&hi2c1, STAT_REG, recv);
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\nSTAT_REG (status of fuel gauge): 0x%02x\n", recv[0]);
	HAL_UART_Transmit(&huart2, (uint8_t *) uartTest , sizeof(uartTest), 10);
	
	
	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////// Create the message ending the test ////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	memcpy(uartTest, clear, 100);
	snprintf(uartTest, sizeof(uartTest), "\n-----Test End-----\n\n");
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

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
void readReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *recv)
{
	
	uint8_t recv_buff[2] = {0};
	
	HAL_I2C_Master_Transmit(hi2c, FG_SLAVE_ADDR, &reg, 1, 10);
	HAL_I2C_Master_Receive(hi2c, FG_SLAVE_ADDR, recv_buff, 2, 10);
	
		*recv |= recv_buff[1];
		*recv <<= 8;
		*recv |= recv_buff[0];
}

void writeReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *send)
{
	uint8_t send_buff[3] = {reg, (uint8_t)(*send & 0x00FF), (uint8_t)((*send >> 8) & 0x00FF)};
	HAL_I2C_Master_Transmit(hi2c, FG_SLAVE_ADDR, send_buff, 3, 10);
}

void init(I2C_HandleTypeDef *hi2c, fg_config_t conf)
{
	/**** Check chip status ****/
	uint16_t status = 0;
	readReg(hi2c, STAT_REG, &status);
	
	// if POR (Power On Reset) bit is set no need to re-configure
	
	if (status & POR) {
		
		// Wait until FSTAT.DNR bit is unset indicating completion
		// of startup operations
		do {readReg(hi2c, FSTAT_REG, &status);}
		while (status & FSTAT_DNR);
		
		/**** Initialize configuration ****/
		
		// store HiBCFG value
		uint16_t HiBCFG = 0;
		readReg(hi2c, HIBCFG_REG, &HiBCFG);
		
		// soft wake up command (step 1 of exiting hibernate mode)
		uint16_t cmd = SOFT_WAKE;
		writeReg(hi2c, SOFT_WAKEUP_REG, &cmd);
		
		// Clear HiBCFG and Soft WakeUp Registers
		// (step 2 of exiting hibernate mode)
		cmd = CLEAR;
		writeReg(hi2c, HIBCFG_REG, &cmd);
		writeReg(hi2c, SOFT_WAKEUP_REG, &cmd);
		
		// Configure OPTION 1 EZ Config
		writeReg(hi2c, DESIGN_CAP_REG, &conf.design_cap);
		writeReg(hi2c, V_EMPTY_REG, &conf.v_empty);
		writeReg(hi2c, I_CHG_REG, &conf.current_chg);
		writeReg(hi2c, CONF_REG, &conf.config1);
		writeReg(hi2c, CONF2_REG, &conf.config2);
		writeReg(hi2c, DQACC_REG, &conf.dQAcc);
		writeReg(hi2c, DPACC_REG, &conf.dPAcc);
		writeReg(hi2c, MODEL_CFG_REG, &conf.model_cfg);
		
		// Poll modelCFG refresh bit (MSb) until it is unset
		do {readReg(hi2c, MODEL_CFG_REG, &status);}
		while (status & MODEL_CFG_REFRESH);
		
		/**** Initialization is complete so we need to clear the POR bit ****/
		
		// Store the current status value
		readReg(hi2c, STAT_REG, &status);
		
		// loop until POR bit has been successfully cleared
		while (status & POR) { 
			status &= 0xFFFD;
			writeReg(hi2c, STAT_REG, &status);
			HAL_Delay(1);
			readReg(hi2c, STAT_REG, &status);
		}
	}
	
	// SUCCESS!!!!
	
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