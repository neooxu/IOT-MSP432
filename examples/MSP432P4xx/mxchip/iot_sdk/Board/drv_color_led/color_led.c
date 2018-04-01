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
 
/*-------------------------------------------------- USER INTERFACES ------------------------------------------------*/

/* Timer_A PWM Configuration Parameter */
Timer_A_PWMConfig pwm_blue_Config =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        32000,
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        32000
};

Timer_A_PWMConfig pwm_green_Config =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        32000,
        TIMER_A_CAPTURECOMPARE_REGISTER_3,
        TIMER_A_OUTPUTMODE_RESET_SET,
        32000
};

Timer_A_PWMConfig pwm_red_Config =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        32000,
        TIMER_A_CAPTURECOMPARE_REGISTER_4,
        TIMER_A_OUTPUTMODE_RESET_SET,
        32000
};

/*P2.4 blue,P2.6 green,P2.7 red*/
void color_led_init( void )
{
	/* PWM pins configured in driver_init.c */
    /* Configuring GPIO2.4,GPIO2.6,GPIO2.7 as peripheral output for PWM*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4 + GPIO_PIN6 + GPIO_PIN7,
            GPIO_PRIMARY_MODULE_FUNCTION);
    /* Configuring Timer_A to have a period of approximately 500ms and
     * an initial duty cycle of 10% of that (3200 ticks)  */
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwm_blue_Config);
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwm_green_Config);
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwm_red_Config);
}

void color_led_open(uint8_t red, uint8_t green, uint8_t blue)
{
	pwm_blue_Config.dutyCycle = 32000 * blue / 255;
	pwm_red_Config.dutyCycle = 32000 * red / 255;
	pwm_green_Config.dutyCycle = 32000 * green / 255;

    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwm_blue_Config);
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwm_green_Config);
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwm_red_Config);
}

void color_led_close(void)
{
	pwm_blue_Config.dutyCycle = 0;
	pwm_red_Config.dutyCycle = 0;
	pwm_green_Config.dutyCycle = 0;

	MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwm_blue_Config);
	MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwm_green_Config);
	MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwm_red_Config);
}
