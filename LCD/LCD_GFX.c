/*
 * LCD_GFX.c
 *
 * Created: 9/20/2021 6:54:25 PM
 *  Author: You
 */ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LCD_GFX.h"
#include "ST7735.h"

/******************************************************************************
* Local Functions
******************************************************************************/


/******************************************************************************
* Global Functions
******************************************************************************/

/**************************************************************************//**
* @fn			uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
* @brief		Convert RGB888 value to RGB565 16-bit color data
* @note
*****************************************************************************/
uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((((31*(red+4))/255)<<11) | (((63*(green+2))/255)<<5) | ((31*(blue+4))/255));
}

/**************************************************************************//**
* @fn			void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color)
* @brief		Draw a single pixel of 16-bit rgb565 color to the x & y coordinate
* @note
*****************************************************************************/
void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color) {
	LCD_setAddr(x,y,x,y);
	SPI_ControllerTx_16bit(color);
}

/**************************************************************************//**
* @fn			void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor)
* @brief		Draw a character starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor){
	uint16_t row = character - 0x20;		//Determine row of ASCII table starting at space
	int i, j;
	if ((LCD_WIDTH-x>7)&&(LCD_HEIGHT-y>7)){
		for(i=0;i<5;i++){
			uint8_t pixels = ASCII[row][i]; //Go through the list of pixels
			for(j=0;j<8;j++){
				if ((pixels>>j)&1==1){
					LCD_drawPixel(x+i,y+j,fColor);
				}
				else {
					LCD_drawPixel(x+i,y+j,bColor);
				}
			}
		}
	}
}


/******************************************************************************
* LAB 4 TO DO. COMPLETE THE FUNCTIONS BELOW.
* You are free to create and add any additional files, libraries, and/or
*  helper function. All code must be authentically yours.
******************************************************************************/

/**************************************************************************//**
* @fn			void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
* @brief		Draw a colored circle of set radius at coordinates
* @note
*****************************************************************************/
void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
{
	uint8_t x = radius;
    uint8_t y = 0;
    int16_t t1 = radius / 16;  // midpoint
	int16_t t2 = 0;

    while (x >= y) {
        LCD_drawPixel(x0 + x, y0 + y, color);
        LCD_drawPixel(x0 + y, y0 + x, color);
        LCD_drawPixel(x0 - y, y0 + x, color);
        LCD_drawPixel(x0 - x, y0 + y, color);
        LCD_drawPixel(x0 - x, y0 - y, color);
        LCD_drawPixel(x0 - y, y0 - x, color);
        LCD_drawPixel(x0 + y, y0 - x, color);
        LCD_drawPixel(x0 + x, y0 - y, color); // all symmetric pixels are colored (8 times)

        y++;
		t1 = t1 + y;
		t2 = t1 - x;
        if (t2 >= 0) {
			t1 = t2;
			x--;
		}
    }

}


/**************************************************************************//**
* @fn			void LCD_fillCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
* @brief		Draw a colored and filled circle of set radius at coordinates
* @note
*****************************************************************************/
void LCD_fillCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
{
	uint8_t x = radius;
    uint8_t y = 0;
    int16_t t1 = radius / 16;  // midpoint
	int16_t t2 = 0;

    while (x >= y) {
		LCD_drawLine(x0 - x, y0 + y, x0 + x, y0 + y,color);
        LCD_drawLine(x0 - x, y0 - y, x0 + x, y0 - y, color);
        LCD_drawLine(x0 - y, y0 + x, x0 + y, y0 + x, color);
        LCD_drawLine(x0 - y, y0 - x, x0 + y, y0 - x, color);

        y++;
		t1 = t1 + y;
		t2 = t1 - x;
        if (t2 >= 0) {
			t1 = t2;
			x--;
		}
    }
}

/**************************************************************************//**
* @fn			void LCD_fillCircle_fast(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color, uint16_t bg_color)
* @brief		Draw a colored and filled circle of set radius at coordinates in a fast way
* @note
*****************************************************************************/

void LCD_fillCircle_fast(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color, uint16_t bg_color)
{
 	int16_t x_start = (x0 > radius) ? (x0 - radius) : 0;
    int16_t y_start = (y0 > radius) ? (y0 - radius) : 0;
    int16_t x_end   = (x0 + radius < LCD_WIDTH)  ? (x0 + radius) : (LCD_WIDTH - 1);
    int16_t y_end   = (y0 + radius < LCD_HEIGHT) ? (y0 + radius) : (LCD_HEIGHT - 1);

    LCD_setAddr(x_start, y_start, x_end, y_end);

    for (int16_t y = y_start; y <= y_end; y++) {
        for (int16_t x = x_start; x <= x_end; x++) {
            int16_t dx = x - x0;
            int16_t dy = y - y0;
			
            if (dx*dx + dy*dy <= (radius*radius - radius)) // looser determination to avoid squared shape
                SPI_ControllerTx_16bit(color);
            else
                SPI_ControllerTx_16bit(bg_color); 
        }
    }
}
/**************************************************************************//**
* @fn			void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
* @brief		Draw a line from and to a point with a color
* @note
*****************************************************************************/
void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
{	
	short dx = abs(x1 - x0);
	short sx = x0 < x1 ? 1 : -1;
	short dy = -abs(y1 - y0);
	short sy = y0 < y1 ? 1 : -1;
	short error = dx + dy;
	
	while (1) {
		LCD_drawPixel(x0,y0,c);
		if (x0 == x1 && y0 == y1) break;
		short e2 = 2 * error;
		if (e2 >= dy) {
			if (x0 == x1) break;
			error += dy;
			x0 += sx;
		}
		if (e2 <= dx) {
			if (y0 == y1) break;
			error += dx;
			y0 += sy;
		}
	}
}


/**************************************************************************//**
* @fn			void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
* @brief		Draw a colored block at coordinates
* @note
*****************************************************************************/
void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
{
	LCD_setAddr(x0,y0,x1,y1);
	uint16_t area = (x1 - x0 + 1) * (y1 - y0 + 1);
    for (uint16_t i = 0; i < area; i++) {
       	SPI_ControllerTx_16bit(color);
    }
}

/**************************************************************************//**
* @fn			void LCD_setScreen(uint16_t color)
* @brief		Draw the entire screen to a color
* @note
*****************************************************************************/
void LCD_setScreen(uint16_t color) 
{
	LCD_setAddr(0,0,159,127);
	uint16_t area = 160 * 128;
    for (uint16_t i = 0; i < area; i++) {
       	SPI_ControllerTx_16bit(color);
    }
}

/**************************************************************************//**
* @fn			void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
* @brief		Draw a string starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
{
	uint8_t x_draw = x, y_draw = y;
	for (int idx = 0; idx < strlen(str); idx++) {
		LCD_drawChar(x_draw, y_draw, str[idx], fg, bg);
		x_draw +=5;
		if (x_draw >= 159) {
			x_draw = x;
			y_draw += 8;
		} 
		if (y_draw >= 127) {
			y_draw = y;
		}
	}
}

void LCD_drawStringFast(uint8_t x, uint8_t y, char *str, uint16_t fg, uint16_t bg)
{
    uint16_t i, j, k;
    uint16_t len = strlen(str);

    if (len == 0) return;

    uint16_t char_w = 5;
    uint16_t char_h = 8;
    uint16_t spacing = 1;
    uint16_t pitch = char_w + spacing;

    uint16_t width = len * pitch;
    uint16_t height = char_h;

    //  Prepare pixel buffer
    uint16_t buf[width * height]; 
    memset(buf, 0, sizeof(buf));

    // Fill buffer with characters
    for (k = 0; k < len; k++)
    {
        uint8_t c = str[k];
        uint16_t row = c - 0x20;

        for (i = 0; i < char_w; i++)
        {
            uint8_t pixels = (uint8_t)ASCII[row][i];

            for (j = 0; j < char_h; j++)
            {
                uint16_t px_index = (j * width) + (k * pitch + i);

                if (pixels & (1 << j))
                    buf[px_index] = fg;
                else
                    buf[px_index] = bg;
            }
        }

        // fill spacing with bg
        for (j = 0; j < char_h; j++)
        {
            uint16_t px_index = (j * width) + (k * pitch + char_w);
            buf[px_index] = bg;
        }
    }

    // set drawing window
    LCD_setAddr(x, y, x + width - 1, y + height - 1);

    // stream out pixel data
    for (i = 0; i < width * height; i++)
        SPI_ControllerTx_16bit(buf[i]);
}

