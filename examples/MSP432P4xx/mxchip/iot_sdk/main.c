#include "mx_debug.h"
#include "mx_common.h"
#include "alicloud_sds.h"

#include "drv_board.h"
#include "main.h"
/* DriverLib Includes */
#include "driverlib.h"
#include "io_button.h"


void usr_btn_isr(void);
void usr_clicked_handler(void);
void usr_long_pressed_handler(void);
void PORT4_IRQHandler(void);

btn_instance_t usr_btn =
{
	.port                   = GPIO_PORT_P4,
	.pin                    = GPIO_PIN6,
	.io_irq                 = PORT4_IRQHandler,
	.idle			        = IOBUTTON_IDLE_STATE_HIGH,
	.long_pressed_timeout   = 5000,
	.pressed_func           = usr_clicked_handler,
	.long_pressed_func		= usr_long_pressed_handler,
};

void PORT4_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getInterruptStatus(GPIO_PORT_P4,0xFFFF);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P4, status);

    /* Toggling the output on the LED */
    if(status & GPIO_PIN6)
    {
    		button_irq_handler(&usr_btn);
    }

}

void usr_clicked_handler(void)
{
	alisds_provision();
}

void usr_long_pressed_handler(void)
{
	app_log("Restore default settings");
	
	OLED_ShowStatusString("Restore default");
	
	alisds_restore();
}

int main(void)
{
	mx_status err = kNoErr;

	drv_board_init();

	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_1, "TI University");

	err = alisds_init(ALI_HANDLE_MAX);
	require_noerr(err, exit);
	

	rgbled_task_init();
	SHT20_task_init();
	switch_task_init();
	console_task_init();
	
	button_init(&usr_btn);

	while(1)
	{
		/* Application tick */
		alisds_loop();
		SHT20_task();
		switch_task();
		button_srv(&usr_btn);
	}
	
exit:
	app_log("App exit reason %d", err);
	while(1);
}
