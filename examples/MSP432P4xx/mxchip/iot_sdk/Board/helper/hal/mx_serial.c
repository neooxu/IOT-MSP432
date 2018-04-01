#include <hal_usart_async.h>
#include <utils.h>

#include "mx_hal.h"

extern struct usart_async_descriptor USART_AT;
static struct io_descriptor *io_at;
static volatile bool tx_complete = true;
static int _timeout = 100;

static void rx_cb_USART_AT(const struct usart_async_descriptor *const io_descr)
{
	/* Driver will store the received data to its buffer */
}

static void tx_cb_USART_AT(const struct usart_async_descriptor *const io_descr)
{
	tx_complete = true;
}

void mx_hal_serial_init(int timeout)
{
	_timeout = timeout;
	usart_async_register_callback(&USART_AT, USART_ASYNC_RXC_CB, rx_cb_USART_AT);
	usart_async_register_callback(&USART_AT, USART_ASYNC_TXC_CB, tx_cb_USART_AT);
	usart_async_get_io_descriptor(&USART_AT, &io_at);
	usart_async_enable(&USART_AT);
}

void mx_hal_serial_set_timeout(int timeout)
{
	_timeout = timeout;	
}

int mx_hal_serial_putc(char c)
{
	uint32_t current = mx_hal_ms_ticker_read();
	
	do {
		if(tx_complete) {
			tx_complete = false;
			return io_write(io_at, (uint8_t *)&c, 1) == 1 ? 0 : -1;
		}
	} while((mx_hal_ms_ticker_read() - current) < _timeout);

	return -1;
}

int mx_hal_serial_getc(void)
{
	uint32_t current = mx_hal_ms_ticker_read();
	uint8_t ch;
	
	do {
		if (io_read(io_at, &ch, 1) == 1) return ch;
	} while((mx_hal_ms_ticker_read() - current) < _timeout);
	
	return -1;
}

bool mx_hal_serial_readable(void)
{
	struct usart_async_descriptor *descr = CONTAINER_OF(io_at, struct usart_async_descriptor, io);
	if(ringbuffer_num(&descr->rx)) return true;
	return false;
}


void mx_hal_serial_flush(void)
{
	uint32_t                       num;
	uint8_t                        tmp;
	struct usart_async_descriptor *descr = CONTAINER_OF(io_at, struct usart_async_descriptor, io);
	
	CRITICAL_SECTION_ENTER()
	for(num = ringbuffer_num(&descr->rx); num>0; num--) {
		ringbuffer_get(&descr->rx, &tmp);
	}
	CRITICAL_SECTION_LEAVE()
}