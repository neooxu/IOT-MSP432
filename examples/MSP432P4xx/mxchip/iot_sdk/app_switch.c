#include "mx_debug.h"
#include "mx_common.h"
#include "alicloud_sds.h"

#include "drv_board.h"
#include "main.h"
/* DriverLib Includes */
#include "driverlib.h"

static bool  switch_1 = false;
static bool  switch_2 = false;


mx_status switch_task_init(void)
{
	alisds_attr_t attr;

	MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN4);
	MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN5);
	switch_2 = GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN4);
	switch_1 = GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5);
	
	attr.name = "switch1";
	attr.type = ALI_ATT_TYPE_BOOL;
	attr.read_func = handle_read_cur_switch_1;
	attr.write_func = NULL;
	alisds_attr_init(ALI_HANDLE_IO_SWITCH_1, attr);

	attr.name = "switch2";
	attr.type = ALI_ATT_TYPE_BOOL;
	attr.read_func = handle_read_cur_switch_2;
	attr.write_func = NULL;
	alisds_attr_init(ALI_HANDLE_IO_SWITCH_2, attr);
	
	return kNoErr;
}

void switch_task(void)
{
	if (switch_2 != GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN4)) {
		switch_2 = GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN4);
		alisds_attr_indicate_by_handle(ALI_HANDLE_IO_SWITCH_2);
	}

	if (switch_1 != GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5)) {
		switch_1 = GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5);
		alisds_attr_indicate_by_handle(ALI_HANDLE_IO_SWITCH_1);
	}
}


/* Switch attribute handlers */
mx_status handle_read_cur_switch_1		(alisds_att_val_t *value)
{
	(*value).boolValue = switch_1;
	return kNoErr;
}

mx_status handle_read_cur_switch_2		(alisds_att_val_t *value)
{
	(*value).boolValue = switch_2;
	return kNoErr;
}
