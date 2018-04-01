/**
 ******************************************************************************
 * @file    hsb2rgb_led.h
 * @author  Eshen Wang
 * @version V1.0.0
 * @date    17-Mar-2015
 * @brief   converts HSB color values to RGB colors to control RGB LED.
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

#ifndef __HSB2RGB_LED_H_
#define __HSB2RGB_LED_H_

#include "color_led.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Initialize hsb2rgb device.
 *
 * @return none
 */
void hsb2rgb_led_init( void );


/**
 * @brief Set light parameters for hsb2rgb.
 *
 * @param hues:         hues data of hsb2rgb
 * @param saturation:   saturation data of hsb2rgb
 * @param brightness:   brightness data of hsb2rgb
 *
 * @return none
 */
void hsb2rgb_led_open(float hues, float saturation, float brightness);

/**
 * @brief Close hsb2rgb
 *
 * @return none
 */
void hsb2rgb_led_close(void);

#ifdef __cplusplus
}
#endif

#endif   // __HSB2RGB_LED_H_
