
#include <stdio.h>
#include "drv_board.h"
#include "driverlib.h"

void drv_board_init(void)
{
	/* Halting WDT  */
    MAP_WDT_A_holdTimer();

	/* System time ticker */
	mx_hal_ms_ticker_init();

	/* Stdio printf */
	mx_hal_stdio_init();

#if 0
	/* RGB color led initialize */
	color_led_init();
	color_led_open(0, 0, 0);
#endif
	/* init OLED */
	OLED_Init();
	OLED_Clear();
	
	/* Temperature and humidity sensor  */
	SHT2x_Init();

}
