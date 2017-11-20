/*
 * glcd.c
 *
 *  Created on: Nov 23, 2016
 *      Author: iqom
 */


/*
 * lcd.c
 *
 *  Created on: 1 Des 2015
 *      Author: ACER
 */
#include "glcd.h"
#include <stdio.h>
#include "stm32f1xx_hal.h"


#define LCD_C 0
#define LCD_D 1

#define LCD_X 84
#define LCD_Y 48

#define LSBFIRST 1
#define MSBFIRST 2

char kata[16];
unsigned char cursor_x,cursor_y,textbgcolor,textcolor,textsize;

unsigned char buffer[]=
{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,

		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,

		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};


void glcdInitGPIO(){

		GPIO_InitTypeDef GPIO_InitStruct;

		GPIO_InitStruct.Pin = PIN_RESET|PIN_SCE|PIN_DC;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
		GPIO_InitStruct.Pull= GPIO_NOPULL;
		HAL_GPIO_Init(PORT_RESET, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = PIN_SDIN|PIN_SCLK|PIN_LIGHT;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
		GPIO_InitStruct.Pull= GPIO_NOPULL;
		HAL_GPIO_Init(PORT_SDIN, &GPIO_InitStruct);

		HAL_GPIO_WritePin(PORT_LIGHT,PIN_LIGHT,GPIO_PIN_RESET);

}
void shiftOut( uint8_t bitOrder, uint8_t val)
{
	uint8_t i;

	for (i = 0; i < 8; i++)  {

		if((val & (1 << (7 - i))))
			 HAL_GPIO_WritePin(PORT_SDIN,PIN_SDIN,GPIO_PIN_SET); //set(PIN_SDIN);
		else
			HAL_GPIO_WritePin(PORT_SDIN,PIN_SDIN,GPIO_PIN_RESET); //clear(PIN_SDIN);

		//		}
		HAL_GPIO_WritePin(PORT_SCLK,PIN_SCLK, GPIO_PIN_SET); //set(PIN_SCLK);
		HAL_GPIO_WritePin(PORT_SCLK,PIN_SCLK,GPIO_PIN_RESET);//clear(PIN_SCLK);
	}
}

void lcdSetCursor(unsigned char x, unsigned char y){
	cursor_x=x;
	cursor_y=y;

}

void lcdSetTextColor(unsigned char color, unsigned char bgColor){
	textcolor= color;
	textbgcolor= bgColor;

}

void lcdSetTextSize(unsigned char size){
	textsize= size;
}


void LcdWrite(unsigned char dc, unsigned char data)
{
	if(dc==LCD_D)
		HAL_GPIO_WritePin(PORT_DC,PIN_DC,GPIO_PIN_SET);//set(PIN_DC);
	else
		HAL_GPIO_WritePin(PORT_DC,PIN_DC,GPIO_PIN_RESET);//clear(PIN_DC);

	HAL_GPIO_WritePin(PORT_SCE,PIN_SCE,GPIO_PIN_RESET);// clear(PIN_SCE);
	shiftOut( MSBFIRST, data);
	HAL_GPIO_WritePin(PORT_SCE,PIN_SCE,GPIO_PIN_SET); //set(PIN_SCE);
}

//void LcdCharacter(unsigned char character)
//{
//
//	int index;
//	LcdWrite(LCD_D, 0x00);
//	for (index = 0; index < 5; index++)
//	{
//		LcdWrite(LCD_D, ASCII[character-32][index]);
//	}
//	LcdWrite(LCD_D, 0x00);
//}


void lcdDrawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= LCD_X) || (y < 0) || (y >=LCD_Y))
    return;

  // check rotation, move pixel around if necessary
//  switch (getRotation()) {
//  case 1:
//    ssd1306_swap(x, y);
//    x = WIDTH - x - 1;
//    break;
//  case 2:
//    x = WIDTH - x - 1;
//    y = HEIGHT - y - 1;
//    break;
//  case 3:
//    ssd1306_swap(x, y);
//    y = HEIGHT - y - 1;
//    break;
//  }

  // x is which column
    switch (color)
    {
      case BLACK:   buffer[x+ (y/8)*LCD_X] |=  (1 << (y&7)); break;
      case WHITE:   buffer[x+ (y/8)*LCD_X] &= ~(1 << (y&7)); break;
      case INVERSE: buffer[x+ (y/8)*LCD_X] ^=  (1 << (y&7)); break;
    }

}
void lcdDrawFastVLine(int16_t x, int16_t __y, int16_t __h, uint16_t color) {

  // do nothing if we're off the left or right side of the screen
  if(x < 0 || x >= LCD_X) { return; }

  // make sure we don't try to draw below 0
  if(__y < 0) {
    // __y is negative, this will subtract enough from __h to account for __y being 0
    __h += __y;
    __y = 0;

  }

  // make sure we don't go past the height of the display
  if( (__y + __h) > LCD_Y) {
    __h = ( LCD_Y - __y);
  }

  // if our height is now negative, punt
  if(__h <= 0) {
    return;
  }

  // this display doesn't need ints for coordinates, use local byte registers for faster juggling
  register uint8_t y = __y;
  register uint8_t h = __h;


  // set up the pointer for fast movement through the buffer
  register uint8_t *pBuf = buffer;
  // adjust the buffer pointer for the current row
  pBuf += ((y/8) * LCD_X);
  // and offset x columns in
  pBuf += x;

  // do the first partial byte, if necessary - this requires some masking
  register uint8_t mod = (y&7);
  if(mod) {
    // mask off the high n bits we want to set
    mod = 8-mod;

    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    // register uint8_t mask = ~(0xFF >> (mod));
    static uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
    register uint8_t mask = premask[mod];

    // adjust the mask if we're not going to reach the end of this byte
    if( h < mod) {
      mask &= (0XFF >> (mod-h));
    }

  switch (color)
    {
    case BLACK:   *pBuf |=  mask;  break;
    case WHITE:   *pBuf &= ~mask;  break;
    case INVERSE: *pBuf ^=  mask;  break;
    }

    // fast exit if we're done here!
    if(h<mod) { return; }

    h -= mod;

    pBuf += LCD_X;
  }


  // write solid bytes while we can - effectively doing 8 rows at a time
  if(h >= 8) {
    if (color == INVERSE)  {          // separate copy of the code so we don't impact performance of the black/white write version with an extra comparison per loop
      do  {
      *pBuf=~(*pBuf);

        // adjust the buffer forward 8 rows worth of data
        pBuf += LCD_X;

        // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
        h -= 8;
      } while(h >= 8);
      }
    else {
      // store a local value to work with
      register uint8_t val = (color == BLACK) ? 255 : 0;

      do  {
        // write our value in
      *pBuf = val;

        // adjust the buffer forward 8 rows worth of data
        pBuf += LCD_X;

        // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
        h -= 8;
      } while(h >= 8);
      }
    }

  // now do the final partial byte, if necessary
  if(h) {
    mod = h & 7;
    // this time we want to mask the low bits of the byte, vs the high bits we did above
    // register uint8_t mask = (1 << mod) - 1;
    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    static uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
    register uint8_t mask = postmask[mod];
    switch (color)
    {
      case BLACK:   *pBuf |=  mask;  break;
      case WHITE:   *pBuf &= ~mask;  break;
      case INVERSE: *pBuf ^=  mask;  break;
    }
  }
}


void lcdFillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
  // Update in subclasses if desired!
int i;
  for (i=x; i<x+w; i++) {
    lcdDrawFastVLine(i, y, h, color);
  }
}
void lcdDrawChar(int16_t x, int16_t y, unsigned char c,uint16_t color, uint16_t bg, uint8_t size) {
	uint8_t line;
	int8_t i;
  if((x >= LCD_X)            || // Clip right
     (y >= LCD_Y)           || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

  if( (c >= 176)) c++; // Handle 'classic' charset behavior

  for ( i=0; i<6; i++ ) {

    if (i == 5)
      line = 0x0;
    else
      line = (font[(c*5)+i]);
    for (int8_t j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          lcdDrawPixel(x+i, y+j, textcolor);
        else {  // big size
          lcdFillRect(x+(i*size), y+(j*size), size, size, textcolor);
        }
      }
      else  {
        if (size == 1) // default size
          lcdDrawPixel(x+i, y+j, textbgcolor);
        else {  // big size
          lcdFillRect(x+i*size, y+j*size, size, size, textbgcolor);
        }
      }
      line >>= 1;
    }
  }
}

void lcdClear(){
	memset(buffer, 0, (LCD_X*LCD_Y/8));
}
void lcdWrite(uint8_t c){

	  if (c == '\n') {
	    cursor_y += textsize*8;
	    cursor_x  = 0;
	  }
	  else if (c == '\r') {
	    // skip em
	  }
	  else {
	    lcdDrawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
	    cursor_x += textsize*6;
	    if ((cursor_x > (LCD_X - textsize*6))) {
	      cursor_y += textsize*8;
	      cursor_x = 0;
	    }
	  }
}
void lcdPrint(unsigned char* x){

	while(*x){
		lcdWrite(*x);
		x++;
		cursor_x++;
		if(cursor_x>LCD_X)
			cursor_y++;
	}

}

//void lcdStringSize(char *character, unsigned char size){
//	int index,ii,iii,iterasiHuruf,nData;
//	unsigned char iterasi;
//	unsigned char data[12/size][5][size];
//	iterasiHuruf=0; nData=0;
//
//	for(iterasiHuruf=0;iterasiHuruf<(12/size);iterasiHuruf++){
//		for(index=0;index<5;index++){
//			for(nData=0;nData<size;nData++)
//				data[iterasiHuruf][index][nData]=0;
//		}
//
//
//	}
//	nData=0;
//	for(iterasiHuruf=0; iterasiHuruf<(12/size);iterasiHuruf++){
//		for (index = 0; index < 5; index++)
//		{
//			for(ii=0;ii<8;ii++){
//				for(iii=0;iii<size;iii++){
//					data[iterasiHuruf][index][nData]|=  (((ASCII[*(character+iterasiHuruf)-32][index]&(1<<ii))>>(ii))<<((size*ii)+iii)%8);
//
//
//					iterasi++;
//					if(iterasi>7){
//						iterasi=0;
//						nData++;
//					}
//				}
//
//			}
//			//	    LcdWrite(LCD_D, ASCII[character-32][index]);
//			nData=0;
//			iterasi=0;
//		}
//	}
//
//	for(nData=0;nData<size;nData++){
//		for(iterasiHuruf=0;iterasiHuruf<(12/size);iterasiHuruf++){
//
//			for(iii=0;iii<size;iii++)
//				LcdWrite(LCD_D,0x00);
//			for (index = 0; index < 5; index++)
//			{
//				for(iii=0;iii<size;iii++){
//					LcdWrite(LCD_D, data[iterasiHuruf][index][nData]);
//
//				}
//			}
//			for(iii=0;iii<size;iii++)
//				LcdWrite(LCD_D,0x00);
//		}
//	}
//
//}
void debugNilai(unsigned char cursorX, unsigned char cursorY, int nilai){
	lcdSetCursor(cursorX, cursorY);
	sprintf(kata,"%4d",nilai);
	lcdPrint(kata);
}
void lcdGambar()
{

	int index,ii;
	for(ii=0; ii<6; ii++){
//		for (index = 0; index <18 ; index++)
//			LcdWrite(LCD_D,0x00);
//
		for (index = 0; index <84 ; index++)
		{

			LcdWrite(LCD_D,buffer[(ii*84)+index]);
		}
//		for (index = 0; index <18 ; index++)
//			LcdWrite(LCD_D,0x00);
	}

}
void LcdClear(void)
{
	memset(buffer, 0, (LCD_X*LCD_Y/8));

}
//
//void LcdString(char *characters)
//{
//	while (*characters)
//	{
//		LcdCharacter(*characters++);
//	}
//}

void LcdInitialise(void)
{

	HAL_GPIO_WritePin(PORT_RESET,PIN_RESET,GPIO_PIN_RESET); //clear(PIN_RESET);
	HAL_GPIO_WritePin(PORT_RESET,PIN_RESET,GPIO_PIN_SET); //set(PIN_RESET);
	LcdWrite(LCD_C, 0x21 ); // LCD Extended Commands.
	LcdWrite(LCD_C, 0xB1 ); // Set LCD Vop (Contrast).
	LcdWrite(LCD_C, 0x04 ); // Set Temp coefficent. //0x04
	LcdWrite(LCD_C, 0x14 ); // LCD bias mode 1:48. //0x13
	LcdWrite(LCD_C, 0x0C ); // LCD in normal mode.
	LcdWrite(LCD_C, 0x20 );
	LcdWrite(LCD_C, 0x0C );
}

void enter(void )
{
	int index;
	for ( index = 0; index < LCD_X; index++)
	{
		LcdWrite(LCD_D, 0x00);
	}

}
void lcdTemplate(){
	 int index;
 LcdClear();
 HAL_Delay(100);

 	for ( index = 0; index < LCD_X * LCD_Y / 8; index++)
 	{
 		LcdWrite(LCD_D, 0xAA);
 	}

}
