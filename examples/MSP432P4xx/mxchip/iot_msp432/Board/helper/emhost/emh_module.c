

#include "ATCmdParser/ATCmdParser.h"
#include "emh_api.h"

static char _fw_version[32];

extern void emh_ali_event_handler(void);
extern void emh_wlan_event_handler(void);


mx_status emh_module_reset(void)
{
	if (!(ATCmdParser_send("AT+REBOOT")
	&&  ATCmdParser_recv("OK\r\n"))) {
		return kGeneralErr;
	}
	for (int i = 0; i < 5; i++) {
		if (ATCmdParser_send("AT")
		&&  ATCmdParser_recv("OK\r\n")
		&&  ATCmdParser_send("AT+UARTE=OFF")
		&&  ATCmdParser_recv("OK\r\n")) {
			return kNoErr;
		}
	}
	return kGeneralErr;
}

mx_status emh_module_restore_settings(void)
{
	if (!(ATCmdParser_send("AT+FACTORY")
	&&  ATCmdParser_recv("OK\r\n"))) {
		return kGeneralErr;
	}
	
	for (int i = 0; i < 5; i++) {
		if (ATCmdParser_send("AT")
		&&  ATCmdParser_recv("OK\r\n")
		&&  ATCmdParser_send("AT+UARTE=OFF")
		&&  ATCmdParser_recv("OK\r\n")) {
			return kNoErr;
		}
	}

	return kGeneralErr;
}

mx_status emh_module_init(void)
{
	ATCmdParser_init("\r","\r\n", 1000, false);
		
	for (int i = 0; i < 2; i++) {
		if ( kNoErr == emh_module_reset()) {
			ATCmdParser_add_oob("+ALINKEVENT:",	emh_ali_event_handler);
			ATCmdParser_add_oob("+WEVENT:",		emh_wlan_event_handler);
			return kNoErr;
		}
	}
		
	return kGeneralErr;
}



const char *emh_module_get_fw_version(void)
{
	if (!(ATCmdParser_send("AT+FWVER?")
       && ATCmdParser_recv("+FWVER:%32[^\r]\r\nOK\r\n", _fw_version))) {
        return NULL;
    }
	return _fw_version;
}

uint32_t emh_module_get_tick(void)
{
	uint32_t tick;
	if (!(ATCmdParser_send("AT+SYSTIME?")
	   && ATCmdParser_recv("+SYSTIME:%d\r\nOK\r\n", &tick))) {
		return 0;
	}
	return tick;	
}

void emh_module_task(void) {
	while (ATCmdParser_process_oob());
}
