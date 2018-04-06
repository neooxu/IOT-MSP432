
#ifndef _CLOUD_CHAT_H_
#define _CLOUD_CHAT_H_

#include "emh_api.h"
#include "alicloud_sds_db.h"

#define ALI_HANDLE_NONE  (-1)

#define SDS_ATTR_VAL_MAX_LEN  1024
#define SDS_ATTR_NAME_MAX_LEN 32

enum{
	ALISDS_EVENT_WLAN_CONFIG_STARTED,
	ALISDS_EVENT_WLAN_CONNECTED,
	ALISDS_EVENT_WLAN_DISCONNECTED,
	ALISDS_EVENT_CLOUD_CONNECTED,
	ALISDS_EVENT_CLOUD_DISCONNECTED,
};
typedef uint8_t alisds_event_e;

typedef int cc_device_handle;


mx_status alisds_init(const emh_ali_config_t *config, int num_handles);
mx_status alisds_loop(void);

mx_status alisds_attr_init(cc_device_handle handle, ali_dev_attr_t attr);


void alisds_attr_indicate_by_handle			(int handle);
void alisds_indicate_local_atts		(int attr_handles[], int num);

void alisds_provision(void);
void alisds_restore(void);

MX_WEAK void alisds_event_handler(alisds_event_e event);

#endif
