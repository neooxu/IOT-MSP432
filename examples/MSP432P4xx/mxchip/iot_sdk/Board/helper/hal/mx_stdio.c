/**
 ******************************************************************************
 * @file    mx_stdio.c
 * @author  William Xu
 * @version V1.0.0
 * @date    9-Apr-2018
 * @brief   Initialize uart for printf output
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
#include <stdio.h>
#include "mx_common.h"

#include "system_msp432p401r.h"
#include "driverlib.h"

/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 115200 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 *http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
const eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        6,                                       // BRDIV = 6
        8,                                       // UCxBRF = 8
        32,                                      // UCxBRS = 32
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};

void mx_hal_stdio_init(void)
{
    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Setting DCO to 12MHz */
    //CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);
}

size_t __write( int handle, const unsigned char * buffer, size_t size )
{
	size_t i;
	UNUSED_PARAMETER(handle);
  
	if ( buffer == 0 ) {
    	return 0;
  	}

	for (i = 0; i < size; i++){
		while (EUSCI_A_UART_TRANSMIT_INTERRUPT & MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE));
		MAP_UART_transmitData(EUSCI_A0_BASE, *(buffer++));
	}
  
  return size;
}

