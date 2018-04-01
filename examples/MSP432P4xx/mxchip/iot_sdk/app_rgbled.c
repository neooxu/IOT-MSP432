#include "mx_debug.h"
#include "mx_common.h"
#include "alicloud_sds.h"

#include "drv_board.h"
#include "main.h"

static float bright = 100.0;
static float saturation = 100.0;
static float hue = 50.0;
static bool  light_on = true;

mx_status rgbled_task_init(void)
{
	ali_dev_attr_t attr;
	hsb2rgb_led_init();
	hsb2rgb_led_open(hue, saturation, bright);

	attr.name = "Saturation";
	attr.att_type = ALI_ATT_TYPE_INT;
	attr.read_func = handle_read_cur_saturation;
	attr.write_func = handle_write_cur_saturation;
	alisds_attr_init(ALI_HANDLE_SATURATION, attr);

	attr.name = "Luminance";
	attr.att_type = ALI_ATT_TYPE_INT;
	attr.read_func = handle_read_cur_bright;
	attr.write_func = handle_write_cur_bright;
	alisds_attr_init(ALI_HANDLE_BRIGHTNESS, attr);

	attr.name = "Hue";
	attr.att_type = ALI_ATT_TYPE_INT;
	attr.read_func = handle_read_cur_hue;
	attr.write_func = handle_write_cur_hue;
	alisds_attr_init(ALI_HANDLE_HUE, attr);

	attr.name = "lampswitch";
	attr.att_type = ALI_ATT_TYPE_BOOL;
	attr.read_func = handle_read_cur_light_switch;
	attr.write_func = handle_write_cur_light_switch;
	alisds_attr_init(ALI_HANDLE_LIGHT_SWITCH, attr);
	
	return kNoErr;
}


/* RGB led attribute handlers */
mx_status handle_write_cur_saturation	(ali_att_val value)
{
	saturation = (float)value.intValue;
	if (light_on == true) {
		hsb2rgb_led_open(hue, saturation, bright);
	}
	return kNoErr;
}
mx_status handle_write_cur_bright		(ali_att_val value)
{
	bright = (float)value.intValue;
	if (light_on == true) {
		hsb2rgb_led_open(hue, saturation, bright);
	}
	return kNoErr;
}

mx_status handle_write_cur_hue			(ali_att_val value)
{
	hue = (float)value.intValue;
	if (light_on == true) {
		app_log("Light: %.2f, %.2f, %.2f", hue, saturation, bright);
		hsb2rgb_led_open(hue, saturation, bright);
	}
	return kNoErr;
}

mx_status handle_write_cur_light_switch(ali_att_val value)
{
	light_on = value.boolValue;
	if (light_on == true) {
		hsb2rgb_led_open(hue, saturation, bright);
	}
	else {
		hsb2rgb_led_close();
	}
	return kNoErr;
}


mx_status handle_read_cur_saturation(ali_att_val *value)
{
	(*value).intValue = (int)saturation;
	return kNoErr;
}

mx_status handle_read_cur_bright(ali_att_val *value)
{
	(*value).intValue = (int)bright;
	return kNoErr;
}

mx_status handle_read_cur_hue(ali_att_val *value)
{
	(*value).intValue = (int)hue;
	return kNoErr;
}

mx_status handle_read_cur_light_switch(ali_att_val *value)
{
	(*value).boolValue = light_on;
	return kNoErr;
}
