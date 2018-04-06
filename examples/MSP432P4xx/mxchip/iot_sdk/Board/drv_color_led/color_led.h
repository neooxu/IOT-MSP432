/**
 ******************************************************************************
 * @file    color_led.h
 * @author  Eshen Wang
 * @version V1.0.0
 * @date    17-Mar-2015
 * @brief   Color led PWM controller header files.
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

#ifndef __COLOR_LED_H_
#define __COLOR_LED_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


//-------------------- user interfaces ---------------------------

/**
 * @brief Initialize RGB LED device.
 *
 * @return none
 */
void color_led_init( void );


/**
 * @brief Set light parameters for RGB LED
 *
 * @param red:    Red light parameter
 * @param green:  Green light parameter
 * @param blue:   Blue light parameter
 *
 * @return none
 */
void color_led_open_rgb(uint8_t red, uint8_t green, uint8_t blue);


/**
 * @brief Set light parameters for RGB LED
 *
 * @param hues:         hues data of RGB LED
 * @param saturation:   saturation data of RGB LED
 * @param brightness:   brightness data of RGB LED
 *
 * @return none
 */
void color_led_open_hsb(float hues, float saturation, float brightness);


/**
 * @brief Close RGB LED
 *
 * @return none
 */
void color_led_close(void);

#ifdef __cplusplus
}
#endif

#endif  // __RGB_LED_H_
