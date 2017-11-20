/*
 * sensor.h
 *
 *  Created on: Nov 16, 2016
 *      Author: iqom
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#define LD1_Pin GPIO_PIN_13
#define LD1_GPIO_Port GPIOC
//iqom
#define SENSOR1_Pin				GPIO_PIN_5
#define SENSOR1_GPIO_Port    	GPIOA

#define SENSOR2_Pin 			GPIO_PIN_6
#define SENSOR2_GPIO_Port		GPIOA

#define SENSOR3_Pin 			GPIO_PIN_7
#define SENSOR3_GPIO_Port		GPIOA

#define SENSORSTART_Pin			GPIO_PIN_0
#define SENSORSTART_GPIO_Port	GPIOA

#define SENSORRESET_Pin			GPIO_PIN_10
#define SENSORRESET_GPIO_Port	GPIOB



#define TX_pin_Pin GPIO_PIN_2
#define TX_pin_GPIO_Port GPIOA
#define RX_pin_Pin GPIO_PIN_3
#define RX_pin_GPIO_Port GPIOA

int sensor1Detected(void);
unsigned char sensor2Detected(void);
unsigned char sensor3Detected(void);
unsigned char sensorStartStopDetected(void);
unsigned char sensorResetDetected(void);
#endif /* SENSOR_H_ */
