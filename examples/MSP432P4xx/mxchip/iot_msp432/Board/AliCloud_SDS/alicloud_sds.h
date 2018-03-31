
#ifndef _CLOUD_CHAT_H_
#define _CLOUD_CHAT_H_

#include "emh_api.h"
#include "alicloud_sds_db.h"

#define ALI_HANDLE_NONE  (-1)

#define SDS_ATTR_VAL_MAX_LEN  1024
#define SDS_ATTR_NAME_MAX_LEN 32


typedef enum
{
	eState_M1_initialize           = 1,
	eState_M2_provision            = 2,
	eState_M3_normal               = 3,
	eState_M4_disconnected         = 4,
	eState_M5_fault                = 5,
} cc_device_state_e;

typedef struct {
	cc_device_state_e device_state;
	emh_arg_ali_conn_e cloud_state;
	bool delay_prov;
	int num_handles;
} cc_context_t;

typedef int cc_device_handle;


mx_status alisds_init(int num_handles);
//void Process_InputData(uint8_t* data_buffer, uint16_t Nb_bytes);
mx_status alisds_loop(void);

mx_status alisds_attr_init(cc_device_handle handle, ali_dev_attr_t attr);


void alisds_attr_indicate_by_handle			(int handle);
void alisds_indicate_local_atts		(int attr_handles[], int num);

void alisds_provision(void);
void alisds_restore(void);

#endif
