/**
 ******************************************************************************
 * @file    mx_hal_serial.c
 * @author  William Xu
 * @version V1.0.0
 * @date    9-Apr-2018
 * @brief   UART driver used for AT parser
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

#include "mx_hal.h"
#include "driverlib.h"

#include "mx_utils/mx_ringbuffer.h"

#define MCU_DRVIER_RECV     1

/******************************************************************************
 *                              Variable Definitions
 ******************************************************************************/
 
static int _timeout = 100;

const eUSCI_UART_Config at_uartConfig =
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


uint8_t at_buffer[MX_SERIAL_RX_BUF_SIZE];
struct ringbuffer at_rx;

/******************************************************************************
 *                             Function Declarations
 ******************************************************************************/
 
void ATCMD_RX_ISR(void);

/******************************************************************************
 *                              Function Definitions
 ******************************************************************************/

static int32_t at_async_read(uint8_t *const buf, const uint16_t length);


void mx_hal_serial_init(int timeout)
{
	_timeout = timeout;

#if MCU_DRVIER_RECV
	ringbuffer_init(&at_rx, at_buffer, MX_SERIAL_RX_BUF_SIZE);
#endif

	/* Selecting P3.2 and P3.3 in UART mode */
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
	            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

	/* Configuring UART Module */
	MAP_UART_initModule(EUSCI_A2_BASE, &at_uartConfig);

	/* Enable UART module */
	MAP_UART_enableModule(EUSCI_A2_BASE);

	/* Enabling UART interrupts */
	MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
	MAP_UART_registerInterrupt(EUSCI_A2_BASE, ATCMD_RX_ISR);
}

void mx_hal_serial_set_timeout(int timeout)
{
	_timeout = timeout;	
}

int mx_hal_serial_putc(char c)
{
	uint32_t current = mx_hal_ms_ticker_read();
	
	do {
		if ( EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG & MAP_UART_getInterruptStatus(EUSCI_A2_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)) {
			MAP_UART_transmitData(EUSCI_A2_BASE, c);
			return 0;
		}
	} while((mx_hal_ms_ticker_read() - current) < _timeout);

	return -1;
}

int mx_hal_serial_getc(void)
{
	uint32_t current = mx_hal_ms_ticker_read();
	uint8_t ch;
	
	do {
		if (at_async_read(&ch, 1) == 1) return ch;
	} while((mx_hal_ms_ticker_read() - current) < _timeout);
	
	return -1;
}

bool mx_hal_serial_readable(void)
{
	if(ringbuffer_num(&at_rx)) return true;
	return false;
}


void mx_hal_serial_flush(void)
{
	uint32_t                       num;
	uint8_t                        tmp;
	
	CRITICAL_SECTION_ENTER()
	for(num = ringbuffer_num(&at_rx); num>0; num--) {
		ringbuffer_get(&at_rx, &tmp);
	}
	CRITICAL_SECTION_LEAVE()
}

/////////// Hardware spec functions //////////////
#if MCU_DRVIER_RECV

void ATCMD_RX_ISR(void)
{
	uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE);

	uint8_t RXData;

	MAP_UART_clearInterruptFlag(EUSCI_A2_BASE, status);

	if(status & EUSCI_A_UART_RECEIVE_INTERRUPT)
	{
		RXData = MAP_UART_receiveData(EUSCI_A2_BASE);
		ringbuffer_put(&at_rx, RXData);
	}
}


static int32_t at_async_read(uint8_t *const buf, const uint16_t length)
{
	uint16_t                       was_read = 0;
	uint32_t                       num;

	if (buf == 0 || length == 0) return 0;

	CRITICAL_SECTION_ENTER()
	num = ringbuffer_num(&at_rx);
	CRITICAL_SECTION_LEAVE()

	while ((was_read < num) && (was_read < length)) {
		ringbuffer_get(&at_rx, &buf[was_read++]);
	}

	return (int32_t)was_read;
}

#endif
