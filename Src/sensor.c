/*
 * sensor.c
 *
 *  Created on: Nov 16, 2016
 *      Author: iqom
 */


#include "stm32f1xx_hal.h"
#include "main.h"
#include "sensor.h"

UART_HandleTypeDef huart2;
#define counterTimeout 15

int sensor1Detected(void){
	int hasil=0;
	int timeOut;

	if(!HAL_GPIO_ReadPin(SENSOR1_GPIO_Port,SENSOR1_Pin) ){
		HAL_Delay(20); // antibouncing
		if(!HAL_GPIO_ReadPin(SENSOR1_GPIO_Port,SENSOR1_Pin) ){ //antispiking
			timeOut=sec;
			while(!HAL_GPIO_ReadPin(SENSOR1_GPIO_Port,SENSOR1_Pin)&&(sec-timeOut)<counterTimeout);
			if(sec-timeOut<counterTimeout)hasil=1; //kode deteksi sensor ( normal )
			else{ hasil=5; //kode deteksi sensor error


			}
		}
	}

	return(hasil);
}

unsigned char sensor2Detected(){
	unsigned char hasil=0;
	int timeOut;
	if(!HAL_GPIO_ReadPin(SENSOR2_GPIO_Port,SENSOR2_Pin)){
		HAL_Delay(20);
		if(!HAL_GPIO_ReadPin(SENSOR2_GPIO_Port,SENSOR2_Pin)){
			timeOut=sec;
			while(!HAL_GPIO_ReadPin(SENSOR2_GPIO_Port,SENSOR2_Pin)&&(sec-timeOut)<counterTimeout);
			if(sec-timeOut<counterTimeout)hasil=1; //kode normal
			else{ hasil=5; //kode error_


			}
		}
	}

	return(hasil);
}
unsigned char sensor3Detected(){
	unsigned char hasil=0;
	int timeOut;
	if(!HAL_GPIO_ReadPin(SENSOR3_GPIO_Port,SENSOR3_Pin)){
		HAL_Delay(20);
		if(!HAL_GPIO_ReadPin(SENSOR3_GPIO_Port,SENSOR3_Pin)){
			timeOut=sec;
			while(!HAL_GPIO_ReadPin(SENSOR3_GPIO_Port,SENSOR3_Pin)&&(sec-timeOut)<counterTimeout);
			if(sec-timeOut<counterTimeout)hasil=1; //kode normal
			else{ hasil=5; //kode error_


			}
		}
	}

	return(hasil);
}

unsigned char sensorStartStopDetected(){
	unsigned char hasil=0;

	if(!HAL_GPIO_ReadPin(SENSORSTART_GPIO_Port, SENSORSTART_Pin)){
		HAL_Delay(40);
		if(!HAL_GPIO_ReadPin(SENSORSTART_GPIO_Port,SENSORSTART_Pin)){
			while(!HAL_GPIO_ReadPin(SENSORSTART_GPIO_Port,SENSORSTART_Pin)){
				HAL_Delay(1);
			}
			hasil=1;
		}
	}

	return(hasil);
}

unsigned char sensorResetDetected(){
	unsigned char hasil=0;

	if(!HAL_GPIO_ReadPin(SENSORRESET_GPIO_Port,SENSORRESET_Pin)){
		HAL_Delay(40);
		if(!HAL_GPIO_ReadPin(SENSORRESET_GPIO_Port,SENSORRESET_Pin)){
			while(!HAL_GPIO_ReadPin(SENSORRESET_GPIO_Port,SENSORRESET_Pin)){
				HAL_Delay(1);
			}
			hasil=1;
		}
	}

	return(hasil);
}
