/**
 ******************************************************************************
 * @file    color_led.c
 * @author  Eshen Wang
 * @version V1.0.0
 * @date    17-Mar-2015
 * @brief   rgb led controller.
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

#include "color_led.h"

#include <hpl_tcc_config.h>
#include <hpl_tc_config.h>

#include <hal_pwm.h>
 
/*-------------------------------------------------- USER INTERFACES ------------------------------------------------*/

extern struct pwm_descriptor PWM_B;
extern struct pwm_descriptor PWM_R;
extern struct pwm_descriptor PWM_G;

void color_led_init( void )
{
	/* PWM pins configured in driver_init.c */
}

void color_led_open(uint8_t red, uint8_t green, uint8_t blue)
{
	/* 1M Hz PWM */
	pwm_set_parameters(&PWM_R, CONF_TCC0_PER_REG, ((uint32_t)(((double)(double)CONF_TCC0_PER_REG * red) / 255)));
	pwm_set_parameters(&PWM_G, CONF_TCC2_PER_REG, ((uint32_t)(((double)(double)CONF_TCC0_PER_REG * green) / 255)));
	pwm_set_parameters(&PWM_B, CONF_TC0_CC0, ((CONF_TC0_CC0 * blue) / 255));
	pwm_enable(&PWM_R);
	pwm_enable(&PWM_G);
	pwm_enable(&PWM_B);
}

void color_led_close(void)
{
	pwm_disable(&PWM_R);
	pwm_disable(&PWM_G);
	pwm_disable(&PWM_B);
}
