#include "mx_debug.h"
#include "mx_common.h"
#include "alicloud_sds.h"

#include "drv_board.h"
#include "main.h"


mx_status console_task_init(void)
{
	ali_dev_attr_t attr;
	attr.name = "DebVal";
	attr.att_type = ALI_ATT_TYPE_STRING;
	attr.read_func = handle_read_console;
	attr.write_func = handle_write_console;
	alisds_attr_init(ALI_HANDLE_CONSOLE, attr);
	
	return kNoErr;
}

#if 0
static uint32_t latest_console_send_tick = 0;
#define CONSOLE_PACKET_SIZE   512
int con_seq = 0;

char console_buff[CONSOLE_PACKET_SIZE+20];

void console_task(void)
{
	mx_status err = kNoErr;
	struct json_str jstr;
	
	char console_string[CONSOLE_PACKET_SIZE];

	json_str_init(&jstr, console_buff, sizeof(console_buff));
	
	if ( mx_hal_ms_ticker_read()-latest_console_send_tick > SENSOR_READ_INTERVAL) {
		
		snprintf(console_string, CONSOLE_PACKET_SIZE, "This is Packet %d\r\n", con_seq);

		err = json_start_object(&jstr);
		require_noerr(err, exit);
		
		json_set_val_str(&jstr, "value", console_string);
		json_set_val_int(&jstr, "seq", con_seq);
		
		err = json_close_object(&jstr);
		require_noerr(err, exit);
		
		alisds_attr_indicate_by_handle(ALI_HANDLE_CONSOLE);
		con_seq++;
	}
	exit:
	return;
}
#endif



char console_buff[SDS_ATTR_VAL_MAX_LEN]="hello, world!";
const char oled_clear_line[OLED_DISPLAY_MAX_CHAR_PER_ROW]="                ";

void OLED_ShowStatusString(const char *status_str)
{
	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, (char *)oled_clear_line);
	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, (char *)status_str);
}


mx_status handle_read_console (ali_att_val *value)
{
	(*value).stringValue = console_buff;
	return kNoErr;
}

mx_status handle_write_console(ali_att_val value)
{
	app_log("recv %s", value.stringValue);
	strncpy(console_buff, value.stringValue, SDS_ATTR_VAL_MAX_LEN);
	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, (char *)oled_clear_line);
	OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, (char *)console_buff);
	return kNoErr;
}
