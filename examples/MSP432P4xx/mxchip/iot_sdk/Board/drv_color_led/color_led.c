/**
 ******************************************************************************
 * @file    color_led.c
 * @author  William Xu
 * @version V1.0.0
 * @date    9-Apr-2018
 * @brief   PWM driver for RGB led
 ******************************************************************************
 *
 * Copyright (c) 2009-2018 MXCHIP Co.,Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include "color_led.h"
 

 
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

/******************************************************************************
 *                              Function Definitions
 ******************************************************************************/

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

void color_led_open_rgb(uint8_t red, uint8_t green, uint8_t blue)
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
