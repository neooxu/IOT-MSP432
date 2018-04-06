/**
 ******************************************************************************
 * @file    oled.h
 * @author  Eshen Wang
 * @version V1.0.0
 * @date    17-Mar-2015
 * @brief   OLED control operations.
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ******************************************************************************
 */

#ifndef __OLED_H_
#define __OLED_H_

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OLED_MODE 0
#define CHAR_SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

//----------------- OLED PIN ----------------  	

	     
#define OLED_CMD  0
#define OLED_DATA 1

//-------------------------------- display define ------------------------------
// for 8*16 char can only display 4 rows, 16 chars each row.
#define OLED_DISPLAY_ROW_1    0    // yellow
#define OLED_DISPLAY_ROW_2    2    // blue
#define OLED_DISPLAY_ROW_3    4    // blue
#define OLED_DISPLAY_ROW_4    6    // blue

#define OLED_DISPLAY_COLUMN_START    0    // colloum from left pos 0

#define OLED_DISPLAY_MAX_CHAR_PER_ROW    16   // max 16 chars each row

void OLED_WR_Byte(uint8_t dat, uint8_t cmd);   
void OLED_WR_Bytes(uint8_t *dat, uint8_t len, uint8_t cmd);   
void OLED_Display_On(void);
void OLED_Display_Off(void);	  

void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
void OLED_FillAll(void);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);

void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);


/*-------------------------------------------------------- USER INTERFACES -----------------------------------------*/


/**
 * @brief Initialize OLED device.
 *
 * @return none
 */
void OLED_Init(void);


/**
 * @brief Clear up all data shown on OLED
 *
 * @return none
 */
void OLED_Clear(void);


/**
 * @brief show string in OLED specified place
 *
 * @param x: Position the X axis of the string to display
 * @param y: Position the Y axis of the string to display
 * @param p: String to be displayed in OLED
 * 
 * @return none
 */
void OLED_ShowString(uint8_t x,uint8_t y, const char *p);

#ifdef __cplusplus
}
#endif

#endif  
	 



