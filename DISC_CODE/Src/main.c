/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "math.h"
#include "string.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
//---Push Button Control
int start = 0;

//---Buffer Control
#define WINDOW_SIZE 500
uint8_t ping[WINDOW_SIZE];
uint8_t pong[WINDOW_SIZE];

int pointer = 0;
int count = 0;
int ntransfers = 0;
unsigned int adcVal = 0;	//stores sample from ADC


//---Transmission Control
uint8_t Tbuffer[] = {0x35, 0x36, 0x37};
const uint32_t TIMEOUT = 200;
const uint16_t SIZE = sizeof(Tbuffer)/sizeof(Tbuffer[0]);
HAL_StatusTypeDef transmit_status;
HAL_StatusTypeDef receive_status;

uint8_t Rbuffer[SIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void transmit(void);
void receive(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_ADC2_Init();
  MX_TIM2_Init();
	
	/* Start Timer as interrupt */
	//HAL_TIM_Base_Start_IT(&htim2);

	/* Start ADC */
	//HAL_ADC_Start(&hadc2);


  /* USER CODE BEGIN 2 */
	HAL_UART_MspInit(&huart2);
	
	//enable pushbutton interrupt
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 1);
	
	//enable USART2 interrupt
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	printf("SIZE = %d\n", SIZE);
	for(int i = 0; i < SIZE; i++) {
		printf("buffer[%d] = %c\n", i, Tbuffer[i]);
	}
	//transmit();
	
  while (1)
  {		
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		if(start)	{	//wait for a button press
			//inroduce a small delay
			HAL_Delay(100);
			
			//start TIM2
			HAL_TIM_Base_MspInit(&htim2);
			HAL_TIM_Base_Start_IT(&htim2);

			// Start ADC
			HAL_ADC_MspInit(&hadc2);
			HAL_ADC_Start(&hadc2);
			
			//LED ON to indicate start of recodring
			HAL_GPIO_WritePin(GPIOD, LD6_Pin, GPIO_PIN_SET);	//Green LED ON
			
			while(start) {	//record until start is reset (about 4s)
				
			}
			
			//LED OFF to indicate end of recodring
			HAL_GPIO_WritePin(GPIOD, LD6_Pin, GPIO_PIN_RESET);	//Green LED OFF
			
			//stop TIM and ADC (TURN GPIOs OFF)
			HAL_ADC_MspDeInit(&hadc2);
			HAL_TIM_Base_MspDeInit(&htim2);
		}
		
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
/* HELPER FUNCTIONS */
//--- Interrupts
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
		//range: 0 to 255 (8 bits)
		//System voltage = 2.98V
		
		//Measured voltage = (System voltage * adcVal)/255

		if(count == WINDOW_SIZE-1)	{ 
			count = 0;
			if(!pointer) {
				pointer = 1;
			}
			else {
				pointer = 0;
			}
			ntransfers++;
			//transmit();
			if(ntransfers == 64) {
				ntransfers = 0;
				start = 0;
			}
		}
		
		adcVal = HAL_ADC_GetValue(&hadc2);	//take a sample
		printf("%d\n", adcVal);
		
		//convrt adc value to volage
		if(!pointer) {
			ping[count] = adcVal;
		}
		else {
			pong[count] = adcVal;
		}
		
		//update valA and valB
		printf("%d\n", adcVal);

		count++;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == GPIO_PIN_0) {
		start = 1;
		printf("BUTTON is pressed!\n");
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	printf("Transmission Success!\n");
	transmit();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	printf("Receiving Success!\n");
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	printf("ERROR!\n");
	transmit();
}

void transmit(void) {
	if(!pointer) {
			transmit_status = HAL_UART_Transmit_IT(&huart2, pong, SIZE);
	}
	else {
			transmit_status = HAL_UART_Transmit_IT(&huart2, ping, SIZE);
	}
	
	//transmit_status = HAL_UART_Transmit(&huart2, Tbuffer, SIZE, TIMEOUT);
	
	if(transmit_status == HAL_OK) {
		printf("Transmission Status: OK!\n");
	}
	else if(HAL_TIMEOUT) {
		printf("Transmission Status: TIMEOUT!\n");
	}
	else if(HAL_BUSY) {
		printf("Transmission Status: BUSY!\n");
	}
	else if(HAL_ERROR) {
		printf("Transmission Status: ERROR!\n");
	}
}

void receive(void) {
	receive_status = HAL_UART_Receive_IT(&huart2, Rbuffer, SIZE);
	
	//receive_status = HAL_UART_Receive(&huart2, Rbuffer, SIZE, TIMEOUT);
	//__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
	
	if(receive_status == HAL_OK) {
		printf("Receiving Status: OK!\t%d\n", sizeof(Rbuffer));
	}
	else if(receive_status == HAL_TIMEOUT) {
		printf("Receiving Status: TIMEOUT!\n");
	}
	else if(receive_status == HAL_BUSY) {
		printf("Receiving Status: BUSY!\n");
	}
	else if(receive_status == HAL_ERROR) {
		printf("Receiving Status: ERROR!\n");
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
