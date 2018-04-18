/**
 ******************************************************************************
 * @file    io_button.c
 * @author  William Xu
 * @version V1.0.0
 * @date    9-Apr-2018
 * @brief   Drive an GPIO pin as a button, trigger click and long pressed handler
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
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>

#include <stdbool.h>
#include "io_button.h"
#include "mx_hal.h"


/******************************************************************************
 *                              Function Definitions
 ******************************************************************************/

void button_irq_handler(btn_instance_t * const btn)
{
    int interval = -1;

    if ( MAP_GPIO_getInputPinValue(btn->port, btn->pin) == ((btn->idle == IOBUTTON_IDLE_STATE_HIGH) ? 0 : 1) ) {

        btn->start_time = mx_hal_ms_ticker_read();
        btn->timer_enabled = true;
        MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN6, GPIO_LOW_TO_HIGH_TRANSITION);
    }
    else {
        interval = (int) mx_hal_ms_ticker_read() - btn->start_time;
        if ( (btn->start_time != 0) && interval > 50 && interval < btn->long_pressed_timeout ) {
            /* button clicked once */
            btn->clicked = true;
        }
        btn->timer_enabled = false;
        MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
    }
}


void button_srv(btn_instance_t * const btn)
{
	bool clicked = false;
	bool long_pressed = false;
	
	__disable_irq();
	if (btn->clicked == true) {
		btn->clicked = false;
		btn->timer_enabled = false;
		clicked = true;

	}
	else if (btn->timer_enabled == true) {
		int interval = mx_hal_ms_ticker_read() - btn->start_time;
		
		if (btn->long_pressed_timeout < interval){
			btn->timer_enabled = false;
			long_pressed = true;
		}
	}
	__enable_irq();
	
	if (clicked && btn->pressed_func) btn->pressed_func();
	if (long_pressed && btn->long_pressed_func) btn->long_pressed_func();
	
}

void button_init(btn_instance_t * const btn)
{
    btn->start_time = 0;
	btn->clicked = false;
	btn->timer_enabled = false;

    /* Configuring P4.6 as an input and enabling interrupts */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN6);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN6);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN6);
    MAP_Interrupt_enableInterrupt(INT_PORT4);

    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
}



