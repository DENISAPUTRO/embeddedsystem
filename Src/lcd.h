/*
 * lcd.h
 *
 *  Created on: Nov 17, 2016
 *      Author: iqom
 */

#ifndef LCD_H_
#define LCD_H_


#define PIN_rs  GPIO_PIN_14
#define PIN_e	GPIO_PIN_15
#define PIN_d4	GPIO_PIN_4
#define PIN_d5	GPIO_PIN_5
#define PIN_d6 	GPIO_PIN_6
#define PIN_d7  GPIO_PIN_7

#define PORT_rs GPIOC
#define PORT_e GPIOC
#define PORT_d4	GPIOA
#define PORT_d5 GPIOA
#define PORT_d6 GPIOA
#define PORT_d7 GPIOA
#define PortLcd_data  GPIOA
#define PortLcd_control GPIOC

void lcd_init(void);
void lcd_string(const char *xx);
void lcd_gotoxy(unsigned char x, unsigned char y);
void wait_lcd(unsigned long int xx);
void lcd_clear(void);
#endif /* LCD_H_ */
