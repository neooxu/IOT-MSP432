#include <string.h>
#include "drv_board.h"

#include "driverlib.h"


//MX_ALIGN(8) char sensor_display[2][OLED_DISPLAY_MAX_CHAR_PER_ROW+1];

#if 0
const char TEST_AT_CMD[] = "AT+FWVER?\r";
uint8_t TEST_AT_CMD_RETURN[100];


static void rx_cb_USART_AT(const struct usart_async_descriptor *const io_descr)
{
}

int32_t at_read_result(struct io_descriptor *const io_descr, uint8_t *const buf, const uint16_t length, uint32_t timeout)
{
	uint32_t current = mx_hal_ms_ticker_read();
	uint16_t offset = 0, remain = length;
	uint32_t n;
	do {
		n = io_read(io_descr, buf+offset, length);
		offset += n;
		remain -= n;
		
		if ((mx_hal_ms_ticker_read() - current) > timeout)
			break;
	} while(remain);
	
	return offset;
}


struct io_descriptor *io;
#endif

float temp = 24.25f;
float humi = 50.69f;

char sensor_display[2][OLED_DISPLAY_MAX_CHAR_PER_ROW + 1];

void board_test(void)
{
	int i = 0;
	int32_t n;

	drv_board_init();
		
	//USART_AT_example();
//	usart_async_register_callback(&USART_AT, USART_ASYNC_RXC_CB, rx_cb_USART_AT);
//	usart_async_get_io_descriptor(&USART_AT, &io);
//	usart_async_enable(&USART_AT);

	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_1, "IOT-MSP43x");
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

	/* Replace with your application code */
	while (1) {
		MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
		printf("[Time: %d] helloworld!\r\n", (int)mx_hal_ms_ticker_read());

		temp += 1.2f;
		humi += 1.2f;

		/* Test SHT20 sensor */
		//temp = SHT2x_GetTempPoll();
		//humi = SHT2x_GetHumiPoll();
		//sprintf(sensor_display[0], "Temp: %.2f C", SHT2x_GetTempPoll());
		//sprintf(sensor_display[1], "humi: %.2f %%", SHT2x_GetHumiPoll());

		printf("Temp: %.2f C\r\n", temp);
		printf("Humi: %.2f %%\r\n", humi);
		
		snprintf(sensor_display[0], OLED_DISPLAY_MAX_CHAR_PER_ROW, "Temp: %.2f C", temp);
		snprintf(sensor_display[1], OLED_DISPLAY_MAX_CHAR_PER_ROW,"humi: %.2f %%", humi);
		
		OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_2, sensor_display[0]);
		OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_3, sensor_display[1]);
#if 0
		if(i%6==0) color_led_open(255, 0, 0);
		else if(i%6==1) color_led_open(0, 255, 0);
		else if(i%6==2) color_led_open(0, 0, 255);
		else if(i%6==3) color_led_open(30, 30, 30);
		else if(i%6==4) color_led_open(130, 130, 130);
		else if(i%6==5) color_led_open(255, 255, 255);
		i++;
		
		/* Test Wi-Fi module */
		io_write(io, (uint8_t *)TEST_AT_CMD, strlen(TEST_AT_CMD));
		
		n = at_read_result(io, TEST_AT_CMD_RETURN, 100, 100);
		TEST_AT_CMD_RETURN[n]=0x0;
		printf("[Time: %d] %s\r\n", (int)mx_hal_ms_ticker_read(), TEST_AT_CMD_RETURN);
#endif	
		mx_hal_delay_ms(1000);

	}
}