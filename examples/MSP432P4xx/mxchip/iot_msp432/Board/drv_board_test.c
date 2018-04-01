#include <string.h>
#include "drv_board.h"

#include "driverlib.h"

#include "mx_utils/mx_ringbuffer.h"


static int32_t usart_async_read(uint8_t *const buf, const uint16_t length);


const char TEST_AT_CMD[] = "AT+FWVER?\r";
uint8_t TEST_AT_CMD_RETURN[50];
void EUSCIA2_IRQHandler(void);

#if 0

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

const eUSCI_UART_Config wifi_uartConfig =
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
uint8_t rx_buffer[1024];
struct ringbuffer rx;

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

	ringbuffer_init(&rx, rx_buffer, 50);


    /* Selecting P3.2 and P3.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A2_BASE, &wifi_uartConfig);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A2_BASE);

    /* Enabling UART interrupts */
    MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_UART_registerInterrupt(EUSCI_A2_BASE, EUSCIA2_IRQHandler);

	/* Replace with your application code */
	while (1) {
		MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
		printf("[Time: %d] helloworld!\r\n", (int)mx_hal_ms_ticker_read());
#if 0
		/* Test SHT20 sensor */
		sprintf(sensor_display[0], "Temp: %.2f C", SHT2x_GetTempPoll());
		sprintf(sensor_display[1], "humi: %.2f %%", SHT2x_GetHumiPoll());
		
		OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_2, sensor_display[0]);
		OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_3, sensor_display[1]);

		if(i%6==0) color_led_open(255, 0, 0);
		else if(i%6==1) color_led_open(0, 255, 0);
		else if(i%6==2) color_led_open(0, 0, 255);
		else if(i%6==3) color_led_open(30, 30, 30);
		else if(i%6==4) color_led_open(130, 130, 130);
		else if(i%6==5) color_led_open(255, 255, 255);
		i++;
#endif
		
		/* Test Wi-Fi module */
		for (i = 0; i < strlen(TEST_AT_CMD); i++) {
			while ( !(EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG & MAP_UART_getInterruptStatus(EUSCI_A2_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)));
			MAP_UART_transmitData(EUSCI_A2_BASE, TEST_AT_CMD[i]);
		}


		//io_write(io, (uint8_t *)TEST_AT_CMD, strlen(TEST_AT_CMD));
		
		n = at_read_result(TEST_AT_CMD_RETURN, 100, 100);
		TEST_AT_CMD_RETURN[n]=0x0;
		printf("[Time: %d] %s\r\n", (int)mx_hal_ms_ticker_read(), TEST_AT_CMD_RETURN);

		mx_hal_delay_ms(1000);

	}
}


void EUSCIA2_IRQHandler(void)
{
	uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE);

	uint8_t RXData;

	MAP_UART_clearInterruptFlag(EUSCI_A2_BASE, status);

	if(status & EUSCI_A_UART_RECEIVE_INTERRUPT)
	{
		RXData = MAP_UART_receiveData(EUSCI_A2_BASE);
		ringbuffer_put(&rx, RXData);
	}
}


static int32_t usart_async_read(uint8_t *const buf, const uint16_t length)
{
	uint16_t                       was_read = 0;
	uint32_t                       num;

	if (buf == 0 || length == 0) return 0;

	CRITICAL_SECTION_ENTER()
	num = ringbuffer_num(&rx);
	CRITICAL_SECTION_LEAVE()

	while ((was_read < num) && (was_read < length)) {
		ringbuffer_get(&rx, &buf[was_read++]);
	}

	return (int32_t)was_read;
}


int32_t at_read_result(uint8_t *const buf, const uint16_t length, uint32_t timeout)
{
	uint32_t current = mx_hal_ms_ticker_read();
	uint16_t offset = 0, remain = length;
	uint32_t n;
	do {
		n = usart_async_read(buf+offset, length);
		offset += n;
		remain -= n;

		if ((mx_hal_ms_ticker_read() - current) > timeout)
			break;
	} while(remain);

	return offset;
}



