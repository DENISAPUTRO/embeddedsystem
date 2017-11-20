/*
 * lcd.c
 *
 *  Created on: Nov 17, 2016
 *      Author: iqom
 */


#include "stm32f1xx_hal.h"
#include "lcd.h"
#define lcd_wait_const 	1
#define lcd_delay		75000



void wait_lcd(unsigned long int xx)
{
//	xx*= lcd_wait_const;
//    while(xx){xx--;}
		HAL_Delay(1);
}

void lcd_en_clk(void)
{		HAL_GPIO_WritePin(PORT_e, PIN_e,GPIO_PIN_SET);

wait_lcd(10);					 			// tunggu 2us
HAL_GPIO_WritePin(PORT_e,PIN_e, GPIO_PIN_RESET );				// reset enable
}

void lcd_ins(unsigned short xx)
{
	HAL_GPIO_WritePin(PORT_rs,PIN_rs ,GPIO_PIN_RESET );
	HAL_GPIO_WritePin(PORT_d4,PIN_d4,(xx&(1<<0))>>0);
	HAL_GPIO_WritePin(PORT_d5,PIN_d5,(xx&(1<<1))>>1);
	HAL_GPIO_WritePin(PORT_d6,PIN_d6,(xx&(1<<2))>>2);
	HAL_GPIO_WritePin(PORT_d7,PIN_d7,(xx&(1<<3))>>3);

	lcd_en_clk();
}
void lcd_ins2(unsigned short xx)
{
	HAL_GPIO_WritePin(PORT_rs,PIN_rs,GPIO_PIN_SET);
	// set enable
			HAL_GPIO_WritePin(PORT_d4,PIN_d4,(xx&(1<<0))>>0);
			HAL_GPIO_WritePin(PORT_d5,PIN_d5,(xx&(1<<1))>>1);
			HAL_GPIO_WritePin(PORT_d6,PIN_d6,(xx&(1<<2))>>2);
			HAL_GPIO_WritePin(PORT_d7,PIN_d7,(xx&(1<<3))>>3);

	lcd_en_clk();
}

void lcd_ins3(unsigned short xx)
{
	HAL_GPIO_WritePin(PORT_d4,PIN_d4,(xx&(1<<0))>>0);
	HAL_GPIO_WritePin(PORT_d5,PIN_d5,(xx&(1<<1))>>1);
	HAL_GPIO_WritePin(PORT_d6,PIN_d6,(xx&(1<<2))>>2);
	HAL_GPIO_WritePin(PORT_d7,PIN_d7,(xx&(1<<3))>>3);


}

void lcd_cmd (unsigned char cmd)
{		lcd_ins ((cmd>>4) & 0x0F);	// kirim nibble high
lcd_ins (cmd & 0x0F);		// kirim nibble low
wait_lcd(50);				// tunggu 0.05ms
}

void lcd_data (unsigned char dat)
{		lcd_ins2 ((dat>>4) & 0x0F);	// kirim nibble high
lcd_ins2 (dat & 0x0F);		// kirim nibble low
wait_lcd(50);				// tunggu 0.05ms
}

void lcd_reset(void)
{
	//LCD_PORT |= (0xF<<__LCD_DATA_BIT);
	lcd_ins3(0xf);
	HAL_GPIO_WritePin(PORT_rs,PIN_rs, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PORT_e, PIN_e, GPIO_PIN_SET);
	wait_lcd(20000);			// tunggu 20ms
	lcd_ins(3);					// reset #1
	wait_lcd(15000);			// tunggu 15ms
	lcd_ins(3);					// reset #2
	wait_lcd(5000);				// tunggu 5ms
	lcd_ins(3);					// reset #3
	wait_lcd(5000);				// tunggu 5ms
	lcd_ins(2);					// set data transfer 4 bit
	wait_lcd(5000);				// tunggu 5ms
}

void lcd_init(void)
{
	lcd_reset();
lcd_cmd(0x28);				//LCD yang digunakan  = data 4 bit, 2 baris, 5x7 dots
wait_lcd(1000);
lcd_cmd(0x0c);				//Display ON cursor OFF
wait_lcd(1000);
lcd_cmd(0x06);				//Set entry mode - auto increement
wait_lcd(1000);
lcd_cmd(0x01);
wait_lcd(2000);
lcd_cmd(0x80);
wait_lcd(2000);
}

void lcd_gotoxy(unsigned char x, unsigned char y)
{
	lcd_cmd(0x80 | ((y*0x40) + x));
}

void lcd_clear(void)
{	 lcd_cmd(1);}

// menampilkan string ke lCD

void lcd_string(const char *xx)
{	while(*xx) lcd_data(*xx++);
}

//void lcd_Test(unsigned char xx)
//{	ClrBit(LCD_EN,__LCD_EN_BIT);				// set enable
//	SetBit(LCD_RS,__LCD_RS_BIT);				// set enable
//    LCD_PORT  = (LCD_PORT & ~(0xF << __LCD_DATA_BIT)) | (xx<<__LCD_DATA_BIT);	// kirim data
//}


// menampilkan data integer ke LCD
void lcd_uint32(unsigned short xx)
{
	lcd_data(xx/10000 + 0x30);
	lcd_data((xx%10000)/1000 + 0x30);
	lcd_data((xx%1000)/100 + 0x30);
	lcd_data((xx%100)/10 + 0x30);
	lcd_data(xx%10 + 0x30);
}

void lcd_uint16(unsigned short xx)
{  //lcd_data(xx/10000 + 0x30);
	lcd_data((xx%10000)/1000 + 0x30);
	lcd_data((xx%1000)/100 + 0x30);
	lcd_data((xx%100)/10 + 0x30);
	lcd_data(xx%10 + 0x30);
}

void lcd_uint2(unsigned short xx)
{
	lcd_data(xx/10 + 0x30);
	lcd_data(xx%10 + 0x30);
}

//void lcd_float6(float xx)
//{
//	sprintf(data_f,"%10.8f",xx);
//	lcd_data(data_f[0]);
//	lcd_data(data_f[1]);
//	lcd_data(data_f[2]);
//	lcd_data(data_f[3]);
//	lcd_data(data_f[4]);
//	lcd_data(data_f[5]);
//}

void lcd_int16 (short int xx)
{  if(xx<0)
{
	lcd_data('-');
	xx=-xx;
}
else lcd_data(' ');
lcd_uint16(xx);
}
