
#include <stdio.h>
#include "mx_common.h"

#include "system_msp432p401r.h"
#include "driverlib.h"

static volatile uint32_t _ticker_count = 0;

void SysTick_Handler(void)
{
	_ticker_count++;
}


void mx_hal_ms_ticker_init(void)
{
	SysTick_Config(SystemCoreClock / 1000);
}

uint32_t mx_hal_ms_ticker_read(void)
{
	return _ticker_count;
}

MX_WEAK void mx_hal_delay_ms(volatile uint32_t delay)
{
	uint32_t tickstart = 0;
	tickstart = mx_hal_ms_ticker_read();
	while((mx_hal_ms_ticker_read() - tickstart) < delay);
}