#include "emh_arg.h"

const char* EMH_ARG_ALI_FORMAT[] =
{
	[EMH_ARG_ALI_FORMAT_JSON] = "JSON",
	[EMH_ARG_ALI_FORMAT_RAW]  = "RAW",
	[EMH_ARG_ALI_FORMAT_MAX]  = "\0",
};

const char* EMH_ARG_ALI_CONN[] =
{
	[EMH_ARG_ALI_CONN_CONNECTED]     = "CONNECT",
	[EMH_ARG_ALI_CONN_DISCONNECTED]  = "DISCONNECT",
	[EMH_ARG_ALI_CONN_MAX]           = "\0",
};

const char* EMH_ARG_ALI_STATUS[] =
{
	[EMH_ARG_ALI_STATUS_UNINITIALIZED]	= "NONE",
	[EMH_ARG_ALI_STATUS_INITIALIZED]	= "INIT",
	[EMH_ARG_ALI_STATUS_CONNECTED]		= "CONNECT",
	[EMH_ARG_ALI_STATUS_DISCONNECTED]	= "DISCONNECT",
	[EMH_ARG_ALI_STATUS_MAX]			= "\0",
};

const char* EMH_ARG_ALI_EV[] =
{
	[EMH_ARG_ALI_EV_ALINK]     = "ALINK",
	[EMH_ARG_ALI_EV_GET]       = "GET",
	[EMH_ARG_ALI_EV_SET]       = "SET",
	[EMH_ARG_ALI_EV_MAX]       = "\0",
};

const char* EMH_ARG_WLAN_EV[] =
{
	[EMH_ARG_WLAN_EV_UAP_ESTABLISHED]     = "AP_UP",
	[EMH_ARG_WLAN_EV_UAP_DESTROYED]       = "AP_DOWN",
	[EMH_ARG_WLAN_EV_STA_CONNECTED]       = "STATION_UP",
	[EMH_ARG_WLAN_EV_STA_DISCONNECTED]    = "STATION_DOWN",
	[EMH_ARG_WLAN_EV_MAX]                 = "\0",
};

const char* EMH_ARG_WLAN_STA[] =
{
	[EMH_ARG_WLAN_STA_CONNECTED]       = "STATION_UP",
	[EMH_ARG_WLAN_STA_DISCONNECTED]    = "STATION_DOWN",
	[EMH_ARG_WLAN_STA_DISCONNECTED]    = "CONNECTING",
	[EMH_ARG_WLAN_STA_MAX]             = "\0",
};




const char* emh_arg_for_type(const char* emh_arg[], uint8_t type)
{
	return emh_arg[type];
}

uint8_t emh_arg_for_arg(const  char* emh_arg[], char *arg)
{
	uint8_t i;
	for (i = 0; i < EMH_ARG_ERR; i++) {
		if (*emh_arg[i] == 0x0) break;
		if (memcmp(emh_arg[i], arg, strlen(emh_arg[i])) == 0) {
			return i;
		}
		
	}
	return EMH_ARG_ERR;
}


