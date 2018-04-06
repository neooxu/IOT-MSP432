

#ifndef _EMH_ARG_H_
#define _EMH_ARG_H_

#include "mx_toolchain.h"
#include "mx_common.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define EMH_ARG_ERR 0xFF

enum{
	EMH_ARG_ALI_CONN_DISCONNECTED,
	EMH_ARG_ALI_CONN_CONNECTED,
	EMH_ARG_ALI_CONN_MAX,
};
typedef uint8_t emh_arg_ali_conn_e;
extern const char* EMH_ARG_ALI_CONN[];

enum{
	EMH_ARG_ALI_STATUS_UNINITIALIZED,
	EMH_ARG_ALI_STATUS_INITIALIZED,
	EMH_ARG_ALI_STATUS_CONNECTED,
	EMH_ARG_ALI_STATUS_DISCONNECTED,
	EMH_ARG_ALI_STATUS_MAX,
};
typedef uint8_t emh_arg_ali_status_e;
extern const char* EMH_ARG_ALI_STATUS[];

enum{
	EMH_ARG_ALI_FORMAT_JSON,
	EMH_ARG_ALI_FORMAT_RAW,
	EMH_ARG_ALI_FORMAT_MAX,
};
typedef uint8_t emh_arg_ali_format_e;
extern const char* EMH_ARG_ALI_FORMAT[];

enum{
	EMH_ARG_ALI_EV_ALINK,
	EMH_ARG_ALI_EV_SET,
	EMH_ARG_ALI_EV_GET,
	EMH_ARG_ALI_EV_MAX,
};
typedef uint8_t emh_arg_ali_ev_e;
extern const char* EMH_ARG_ALI_EV[];

enum{
	EMH_ARG_WLAN_EV_UAP_ESTABLISHED,
	EMH_ARG_WLAN_EV_UAP_DESTROYED,
	EMH_ARG_WLAN_EV_STA_CONNECTED,
	EMH_ARG_WLAN_EV_STA_DISCONNECTED,
	EMH_ARG_WLAN_EV_MAX,
};
typedef uint8_t emh_arg_wlan_ev_e;
extern const char* EMH_ARG_WLAN_EV[];

enum{
	EMH_ARG_WLAN_STA_CONNECTED,
	EMH_ARG_WLAN_STA_DISCONNECTED,
	EMH_ARG_WLAN_STA_CONNECTING,
	EMH_ARG_WLAN_STA_MAX,
};
typedef uint8_t emh_arg_wlan_sta_e;
extern const char* EMH_ARG_WLAN_STA[];

const char* emh_arg_for_type(const char* emh_arg[], uint8_t type);
uint8_t emh_arg_for_arg(const char* emh_arg[], char *arg);

typedef struct _dfd {
	int32_t len;
	uint8_t* data;
	emh_arg_ali_format_e format;
} emh_ali_local_attrs_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
