/**
 ******************************************************************************
 * @file    keys.c
 * @author  Eshen Wang
 * @version V1.0.0
 * @date    1-May-2015
 * @brief   user keys operation.
 ******************************************************************************
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 ******************************************************************************
 */
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>

#include <stdbool.h>
#include "io_button.h"
#include "mx_hal.h"

/*-------------------------------- VARIABLES ---------------------------------*/

/*------------------------------ USER INTERFACES -----------------------------*/


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



