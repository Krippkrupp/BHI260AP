/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *	Made for NUCLEO-H753ZI board using the Shuttle Board adapter
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bhi260ap.h"
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

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, GPIO_PIN_SET);

  // Set reset high
  HAL_GPIO_WritePin(BHI_SPI_NRST_BUS, BHI_SPI_NRST_PIN, GPIO_PIN_SET);
  uint8_t addr = 0x17 | (1<<7);
  uint8_t read_status[2];
//  HAL_Delay(1);
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//  HAL_Delay(1);
//  HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, GPIO_PIN_RESET);
////  HAL_SPI_Transmit(&BHI_SPI, &addr, 1, 100);
//  HAL_SPI_Transmit(&BHI_SPI, &(uint8_t){addr}, 1, 100);
////  &(uint8_t){ADS_CMD_SDATAC}
////  HAL_SPI_TransmitReceive(&BHI_SPI, (uint8_t[2]){0}, read_status, 2, 100);
//  HAL_SPI_Receive(&hspi1, read_status, 2, 100);
//  HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, GPIO_PIN_SET);

//  addr = 0x03 | (1<<7);
  addr = 0b10000011;
  uint8_t read_smth[2];
//  HAL_Delay(1);
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//  HAL_Delay(1);
//  HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, GPIO_PIN_RESET);
//
////  HAL_SPI_Transmit(&BHI_SPI, &addr, 1, 100);
//  HAL_SPI_Transmit(&BHI_SPI, &(uint8_t){addr}, 1, 100);
////  HAL_SPI_TransmitReceive(&BHI_SPI, (uint8_t[2]){0}, read_smth, 2, 100);
//  HAL_SPI_Receive(&hspi1, read_smth, 2, 100);
////  HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, GPIO_PIN_SET);
//
//  HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, GPIO_PIN_SET);
//  addr = 0x17 | (1<<7);
//  HAL_Delay(1);
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//  HAL_Delay(1);
//  HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, GPIO_PIN_RESET);
////  HAL_SPI_Transmit(&BHI_SPI, &addr, 1, 100);
//  HAL_SPI_Transmit(&BHI_SPI, &(uint8_t){addr}, 1, 100);
////  &(uint8_t){ADS_CMD_SDATAC}
////  HAL_SPI_TransmitReceive(&BHI_SPI, (uint8_t[2]){0}, read_status, 2, 100);
//  HAL_SPI_Receive(&hspi1, read_status, 2, 100);
//  HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, GPIO_PIN_SET);

  HAL_GPIO_WritePin(BHI_SPI_NRST_BUS, BHI_SPI_NRST_PIN, GPIO_PIN_RESET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(BHI_SPI_NRST_BUS, BHI_SPI_NRST_PIN, GPIO_PIN_SET);
  HAL_Delay(5);


  // test
//  uint8_t *arr = malloc(10*sizeof(uint8_t));
//  for(int i = 0; i < 10; i++)
//  {
//	  arr[i]=i;
//  }
//  uint8_t *arrptr;
//  arrptr = &arr[0];
//  HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, LOW);
//  HAL_SPI_Transmit(&hspi1, arrptr, 3, 100);
//  HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, HIGH);
//  arrptr = &arrptr[3];
//  HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, LOW);
//  HAL_SPI_Transmit(&hspi1, arrptr, 3, 100);
//  HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, HIGH);
//  arrptr = &arrptr[3];
//  HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, LOW);
//  HAL_SPI_Transmit(&hspi1, arrptr, 3, 100);
//  HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, HIGH);

  tmpread(0x16);	//	0x96			(0x00?)
  bhi_hwrite(0x16, (uint8_t[1]){0x02}, 1);
  tmpread(0x16);	//	0x96			(0x02)
  bhi_hwrite(0x16, (uint8_t[1]){0x00}, 1);
  tmpread(0x16);	//	0x96			(0x00)

  tmpread(0x25);	//	A5?				()

  // Host interrupt control


  // Upload FW to RAM
  tmp_upload_ram(); // Verkar OK

  // Verify uploaded RAM
  wip_verify_fw();

  // Boot firmware from RAM
  cmd_input(0x0003, 0, NULL);

  // Todo: Add a function which checks for bit 7 on 0x25 BOOT STATUS, it should be zero
  tmpread(0x25);
  tmpread(0x25);


  // The "Initialized" Meta Events will be inserted in Wake-up and Non-wake-up FIFOs
  // and the host interrupt pin will be asserted
  tmpreadmsg(0x01);	//	0x81
  tmpreadmsg(0x02);	//	0x82

  ////////////////////////////
  // Why are these four registers still nill?
  tmpread(0x20);	//	0xa0
  tmpread(0x21);	//	0xa1
  // Check firmware, 0x22-23 should NOT return 0
  tmpread(0x22);	//	0xa2
  tmpread(0x23);	//	0xa3






  // GAME ROTATION VECTOR = 37 (non wakeup) eller 38 (wakeup)

//  cmd_input(0x000D, 8, &(uint8_t){37, 0x00,0x00,0x10,0x10, 0x00,0x00,0x00});
  cmd_input(0x000D, 8, (uint8_t[8]){37, 0x10,0x10,0x10,0x10, 0x00,0x00,0x00});
//  cmd_input(0x000D, 8, (uint8_t[8]){38, 0x00,0x00,0x10,0x10, 0x00,0x00,0x00});

//  tmpread(0x25);	//	A5?				()
//  tmpread(0x25);	//	A5?				()
//  tmpread(0x25);	//	A5?				()
//  tmpread(0x25);	//	A5?				()
//  tmpread(0x25);	//	A5?				()
//  tmpread(0x25);	//	A5?				()
//  tmpread(0x25);	//	A5?				()

//  tmpread(0x17);	//	0x97			(0x03?)
//  tmpread(0x2b);	//	0xab			(return either 0x70 or 0xf0)
//  tmpread(0x1c);	//	0x9c			(return 0x89)
////  tmpread(0x1d);	//	0x9d
//  tmpread(0x1e);	//	0x9e			(0x14-0x1f should return 0x142E - but which order?!?!)
//  tmpread(0x1f);	//	0x9f
//  tmpread(0x22);	//	0x9f
//  tmpread(0x23);	//	0x9f
//  tmpread(0x2b);	//	0xab



//  tmpreadmsg(0x03);	//	0x97

		//  cmd_input(0x0012, 0, (uint8_t[1]){0x00}  );

  // Boot firmware flash 				--- Ger fortfarande 0x00 på register 0x22-23, dvs ingen Fw laddad
//  cmd_input(0x0006, 0, NULL);
//  tmpread(0x22);	//	0x9f
//  tmpread(0x23);	//	0x9f

  //	Boot program RAM - This command is required to start a firmware image in RAM
//  cmd_input(0x0003, 0, NULL);
//  tmpread(0x22);	//	0x9f
//  tmpread(0x23);	//	0x9f
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
				//	  tmpread(0x01);	//	0x97
					  tmpreadmsg(0x02);	//	0x97
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 18;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 75;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOMEDIUM;
  RCC_OscInitStruct.PLL.PLLFRACN = 6144;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
//  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 0x0;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi1.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
  hspi1.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BHI_nRST_GPIO_Port, BHI_nRST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BHI_CS_GPIO_Port, BHI_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BHI_nRST_Pin */
  GPIO_InitStruct.Pin = BHI_nRST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BHI_nRST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BHI_CS_Pin */
  GPIO_InitStruct.Pin = BHI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BHI_CS_GPIO_Port, &GPIO_InitStruct);

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
