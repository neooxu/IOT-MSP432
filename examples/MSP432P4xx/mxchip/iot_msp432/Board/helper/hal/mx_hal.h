#ifndef _MX_HAL_H_
#define _MX_HAL_H_


#include "mx_common.h"
#include "mx_debug.h"

#ifdef __cplusplus
extern "C"
{
#endif

void mx_hal_ms_ticker_init(void);

uint32_t mx_hal_ms_ticker_read(void);

void mx_hal_delay_ms(volatile uint32_t delay);


void mx_hal_serial_init(int timeout);
void mx_hal_serial_set_timeout(int timeout);
int mx_hal_serial_putc(char c);
int mx_hal_serial_getc(void);
bool mx_hal_serial_readable(void);
void mx_hal_serial_flush(void);

void mx_hal_stdio_init(void);


void* mx_hal_i2c_init(void *config);
int mx_hal_i2c_cmd_write(void *instance, uint16_t slave_addr, uint8_t reg, uint8_t *buffer, uint8_t length);
int32_t i2c_m_sync_cmd_read(void *instance, uint16_t slave_addr, uint8_t reg, uint8_t *buffer, uint8_t length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif