/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 *
 * COPYRIGHT(c) 2016 STMicroelectronics
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
#include "stm32f1xx_hal.h"
#include "sensor.h"
#include "konfigurasi.h"
#include "glcd.h"
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart2;


#define errorDetected 5
#define normalDetected 1


void Error_Handler(void);


int waktu=0,detik,ms100;
int menit=0,iddle,counterSec;
unsigned char mainFlagTimer,mainLap1,mainLap2,mainLap3;
char kata[30];
void delay_us(uint32_t micros);
unsigned char status,flagErrorLed,flagErrorSystem=0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if(htim->Instance== TIM2){
		iddle=1;

		if(mainFlagTimer==1){
			ms100++;
			if(ms100>=10){
				detik++;
				if(detik>=60){
					detik=0;
					menit++;
				}
				ms100=0;
			}
		}



		sec++;

			HAL_GPIO_TogglePin(LD1_GPIO_Port,LD1_Pin);
		switch(status){
		case 0: R(0);G(0);B(0);break;
		case 1: R(0);G(1);B(0); break;
		case 2:

			if(sec%5==0){
				flagErrorLed^=1;
			}
			if(flagErrorLed){
				R(1);G(0);B(0);
			}
			else{
				R(0);G(0);B(0);
			}
		break;
		}

	}

}

int sensor1,sensor2,sensor3;
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM2_Init();
	MX_USART2_UART_Init();
//
	HAL_Delay(20);
	glcdInitGPIO();
	LcdInitialise();
//
//	lcdSetTextColor(BLACK,WHITE);
//	lcdSetTextSize(1);
	lcdSetCursor(0,0);
	lcdPrint("  project  ");
	lcdSetCursor(kolom(0),baris(1));
	lcdPrint("  embedded ");
	lcdSetCursor(kolom(0),baris(1));
	lcdGambar();
	HAL_Delay(1000);
	/* USER COembeEGIN 2 */
	/* USER CODE END 2 */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	HAL_TIM_Base_Start_IT(&htim2);
	lcdClear();

	while (1)
	{


		if(iddle){
			lcdSetCursor(0,0);
			lcdPrint("   Timer  ");
			sprintf(kata," %2d:%2d:%2d\r\n",menit,detik,ms100);
			HAL_UART_Transmit(&huart2,kata,11,10);
			lcdSetCursor(kolom(0),baris(1));
			lcdPrint(kata);
			lcdSetCursor(kolom(0),baris(3));
			lcdPrint("    Lap   ");
			sprintf(kata," %2d-%2d-%2d\r\n",mainLap1,mainLap2,mainLap3);
			lcdSetCursor(kolom(0),baris(4));
			lcdPrint(kata);
			lcdGambar();
			iddle=0;
		}
//		/*pembacaan sensor 1 */
		if(sensor1Detected()==normalDetected){
			if(mainLap1<4)mainLap1++;
			if(mainLap1<=4){

			sprintf(kata,"mobil 1 -> lap%d = %2d:%2d:%2d\r\n",mainLap1,menit,detik,ms100);
			HAL_UART_Transmit(&huart2,kata,28,10);
			}
			if(mainLap1>=4){
				sprintf(kata,"mobil 1 -> FINISHED\r\n",mainLap1,menit,detik,ms100);
				HAL_UART_Transmit(&huart2,kata,21,10);
			}
		}
		else if(sensor1Detected()==errorDetected){
			flagErrorSystem=1;
			sprintf(kata,"SENSOR 1 error\r\n");
			HAL_UART_Transmit(&huart2,kata,16,10);
			HAL_Delay(2000);
		}

		/*pembacaan sensor 2 */
		if(sensor2Detected()==normalDetected){
			if(mainLap2<4)mainLap2++;
			if(mainLap2<=4){

			sprintf(kata,"mobil 2 -> lap%d = %2d:%2d:%2d\r\n",mainLap2,menit,detik,ms100);
			HAL_UART_Transmit(&huart2,kata,28,10);
			}
			if(mainLap2>=4){
				sprintf(kata,"mobil 2 -> FINISHED\r\n",mainLap1,menit,detik,ms100);
				HAL_UART_Transmit(&huart2,kata,22,10);
			}
		}
		else if(sensor2Detected()==errorDetected){
			flagErrorSystem=1;
			sprintf(kata,"SENSOR 2 error\r\n");
			HAL_UART_Transmit(&huart2,kata,16,10);
			HAL_Delay(2000);
		}

		/*pembacaan sensor 3 */
		if(sensor3Detected()==normalDetected){
			if(mainLap3<4)mainLap3++;
			if(mainLap3<=4){

			sprintf(kata,"mobil 3 -> lap%1d = %2d:%2d:%2d\r\n",mainLap3,menit,detik,ms100);
			HAL_UART_Transmit(&huart2,kata,28,10);
			}
			if(mainLap3>=4){
				sprintf(kata,"mobil 3-> FINISHED\r\n",mainLap1,menit,detik,ms100);
				HAL_UART_Transmit(&huart2,kata,21,10);
			}
		}
		else if(sensor3Detected()==errorDetected){
			flagErrorSystem=1;
			sprintf(kata,"SENSOR 3 error\r\n");
			HAL_UART_Transmit(&huart2,kata,16,10);
			HAL_Delay(2000);
		}

		/*pembacaan start/stop */
		if(sensorStartStopDetected()){
			mainFlagTimer^=1;

		}

		/*pembacaan reset */
		if(sensorResetDetected()){
			menit=0;
			detik=0;
			ms100=0;
			mainLap1=mainLap2=mainLap3=0;

		}
		if(!flagErrorSystem){
			status=mainFlagTimer;
		}
		else{
			status=2;
		}
	}
}


/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */

void delay_us(uint32_t micros){
	while(micros>0){
		micros--;
	}
}
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler */
	/* User can add his own implementation to report the HAL error return state */
	while(1)
	{
	}
	/* USER CODE END Error_Handler */
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
