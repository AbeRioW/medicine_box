/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "RC522.h"
#include "DHT11.h"
#include "ds18b20.h"
#include "string.h"
#include "esp8266.h"

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void ui_setting(void);
bool ui_come=false;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  float num_d;
		 uint8_t status,i;
	uint8_t g_ucTempbuf[20]; 
		uint8_t cardid[4]={0x00,0x00,0x00,0x00};
		uint8_t card1[4]={0x43,0xea,0x26,0x2d};
		uint8_t card2[4]={0x03,0x83,0x35,0x29};
		static int i_count=0;
		static int time_p=0;
		char data_show[20];
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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	OLED_Init();
	OLED_ColorTurn(0);
  OLED_DisplayTurn(0);
	OLED_Clear();

	PCD_Reset();
  PCD_AntennaOff(); 
  PCD_AntennaOn(); 
	start_esp8266();
	if(DS_Init()==0)
	{
	}
//	HAL_GPIO_WritePin(GPIOB, LED_Pin, GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		DHT11_READ_DATA();
		num_d = Get_DS_Temperature();		
		oled_showFnum(1,20,num_d,8,1);
		OLED_Refresh();
		
		i_count++;
		if(i_count/1000*5)
		{
			i_count=0;
			sprintf(data_show,"temperature:%0.2f\r\n",num_d);			
			send_wifi(data_show,19);
		}
		
		if(time_right)
		{
			 time_right=false;
			time_count=0;
			 HAL_GPIO_WritePin(GPIOB, BEEP_Pin, GPIO_PIN_RESET);
			 HAL_Delay(100);
			 HAL_GPIO_WritePin(GPIOB, BEEP_Pin, GPIO_PIN_SET);
		}
		
			OLED_ShowString(0,30,(uint8_t*)"ibuprofen",16,1);
		  
			OLED_ShowString(0,47,(uint8_t*)"999 dermatitis",16,1);
  		OLED_Refresh();
		  handle_esp8266();
		 if(num_d>20)  //温度
		  {
			 //手机发短信
		  }
		 
		 	if(botton == RIGHT)
			{
				  botton = UNPRESS;
					OLED_Clear();
				  ui_setting();
			}

		
		status = PCD_Request(PICC_REQALL, g_ucTempbuf);//???
		 if(status)
		 {
				PCD_Reset();
				PCD_AntennaOff(); 
				PCD_AntennaOn(); 
			  continue;
		 }
		 status = PCD_Anticoll(cardid);  //防冲撞，获取ID信息

		 if(memcmp(cardid,card1,4)==0)
		 {
				HAL_UART_Transmit(&huart1,buluofen,17,0xffff);
			 		 HAL_Delay(1500);
		 }
		 
		 if(memcmp(cardid,card2,4)==0)
		 {
				HAL_UART_Transmit(&huart1,ninenine,22,0xffff);
			 		 HAL_Delay(1500);
		 }
		 

		//HAL_Delay(1000);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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

/* USER CODE BEGIN 4 */
static void ui_setting(void)
{
	    ui_come= true;
			char text[20];
			OLED_ShowString(20,0,(uint8_t*)"Setting time",16,1);
  		OLED_Refresh();	
			while(1)
			{
					sprintf(text,"%02d",time_set);
				  OLED_ShowString(40,30,(uint8_t*)text,16,1);
					OLED_Refresh();	
				  
				  if(botton == LEFT)
					{
							botton = UNPRESS;
						  time_set--;
						  if(time_set<2)
								time_set=10;
					}
					
					if(botton == RIGHT)
					{
							botton = UNPRESS;
						  time_set++;
						  if(time_set>10)
								time_set=2;
					}
					
					if(botton == MIDLE)		
					{
							botton = UNPRESS;	
              ui_come=false;						
							return;
					}						
					
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
