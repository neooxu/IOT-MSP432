#include "mx_debug.h"
#include "emh_api.h"
#include "alicloud_sds.h"

#include "json_generator.h"
#include "json_parser.h"

#include "drv_board.h"

#define APP_DEBUG MX_DEBUG_ON
#define sds_log(M, ...) MX_LOG(APP_DEBUG, "SDS", M, ##__VA_ARGS__)

static const emh_ali_config_t *ali_config = NULL;

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

//BBED
//const char *dev_key = "R41Qd1Rm9CMtWWmUIsTM";
//const char *dev_sec = "CnJ9c7DoRBgrAsVT9IHoYMz0LviLgWjw";

//BBEA
const char *dev_key = "eImkAqUT24v1VYO8fbmf";
const char *dev_sec = "7qSOxEu7HcbDDa0LJSc0sHLVnEjnn3qZ";

//BBE0
//const char *dev_key = "R41Qd1Rm9CMtWWmUIsTM";
//const char *dev_sec = "CnJ9c7DoRBgrAsVT9IHoYMz0LviLgWjw";


//microchip
//const char *dev_key = "7UXu5dclxk6Gja8LzelT";
//const char *dev_sec = "xXKxoAVPtTfeoP2DDwOnUB7ZxnIbuoaJ";


cc_context_t context;

static int *changed_handles = NULL;
static ali_dev_attr_t *alisds_attr_db = NULL;

static char incomming_val[SDS_ATTR_VAL_MAX_LEN];
static char incomming_name[SDS_ATTR_NAME_MAX_LEN];

extern const char oled_clear_line[OLED_DISPLAY_MAX_CHAR_PER_ROW];
		
mx_status alisds_init(const emh_ali_config_t *config, int num_handles)
{
	mx_status err = kNoErr;
	
	changed_handles = malloc(num_handles*sizeof(int));
	require_action(changed_handles, exit, err = kNoMemoryErr); 
	
	for (int i = 0; i < num_handles; i++) {
		changed_handles[i] = ALI_HANDLE_NONE;
	}
	
	alisds_attr_db = malloc(num_handles*sizeof(ali_dev_attr_t));
	require_action(alisds_attr_db, exit, err = kNoMemoryErr);
	memset(alisds_attr_db, 0x0, num_handles*sizeof(ali_dev_attr_t));
	
	context.device_state = eState_M1_initialize;
	context.cloud_state = EMH_ARG_ALI_CONN_DISCONNECTED;
	context.delay_prov = false;
	context.num_handles = num_handles;
	
	ali_config = config;
	
	err = emh_module_init();
	require_noerr(err, exit);
	
exit:
	if (err != kNoErr) {
		if (changed_handles) free(changed_handles);
		if (alisds_attr_db) free(alisds_attr_db);
	}
	return err;
}

mx_status alisds_attr_init(cc_device_handle handle, ali_dev_attr_t attr)
{
	alisds_attr_db[handle].name = attr.name;
	alisds_attr_db[handle].att_type = attr.att_type;
	alisds_attr_db[handle].read_func = attr.read_func;
	alisds_attr_db[handle].write_func = attr.write_func;
	
	alisds_attr_indicate_by_handle(handle);
	return kNoErr;
}

static mx_status _handle_state_initialize(void)
{
	mx_status err = kNoErr;
	char ssid[33], pwd[33];

	sds_log("FW version: %s", emh_module_get_fw_version());

	err = emh_ali_config(ali_config);
	require_noerr(err, exit);
	
	/* Set cloud access token */
	//err = emh_ali_set_key(dev_key, dev_sec);
	//require_noerr(err, exit);
	
	/* Start alisds daemon service*/
	err = emh_ali_start_service();
	require_noerr(err, exit);

	/* Check Wi-Fi configuration */
	err = emh_wlan_get_para(ssid, pwd);
	require_noerr(err, exit);

	if (strlen(ssid)) {
		sds_log("SSID: %s, PWD: %s", ssid, pwd);
		if (EMH_ARG_ALI_STATUS_CONNECTED == emh_ali_get_status()) {
			sds_log("Alicloud connected.");
			mx_hal_delay_ms(200);
			context.device_state = eState_M3_normal;
		}
		else {
			sds_log("Waiting for Alicloud connection");
			context.device_state = eState_M4_disconnected;
		}
	}
	else {
		sds_log("Wlan unconfigured, start provision mode");
		
		/* Start alisds Wi-Fi configuration */
		err = emh_ali_provision(true);
		require_noerr(err, exit);
		
		alisds_event_handler(ALISDS_EVENT_WLAN_CONFIG_STARTED);
		context.device_state = eState_M2_provision;
	}
	
exit:
	return err;
}


void alisds_attr_indicate_by_handle(int handle)
{
	int i;
	for (i = 0; i < context.num_handles && changed_handles[i] != ALI_HANDLE_NONE; i++) {
		
		if (changed_handles[i] == handle) return;
	}
	changed_handles[i]=handle;
}

mx_status alisds_loop(void)
{
	mx_status err = kNoErr;
	alisds_indicate_local_atts(changed_handles, context.num_handles);

	switch ( context.device_state ){
		case eState_M1_initialize:
		{
			err = _handle_state_initialize( );
			require_noerr_action(err, exit, context.device_state=eState_M5_fault);
		}
		break;
		
		case eState_M2_provision:
		{
		}
		break;
		
		case eState_M3_normal:
		case eState_M4_disconnected:
		{
		}
		break;
		case eState_M5_fault:
		{
		}
		break;

		default:
			sds_log("STATE ERROR");
			err = kNoErr;
	}
	
	emh_module_task();
exit:
	return err;
}



void emh_ev_wlan(emh_arg_wlan_ev_e event)
{
	sds_log("Wlan event: %s", emh_arg_for_type(EMH_ARG_WLAN_EV, event));
	if (event == EMH_ARG_WLAN_EV_STA_CONNECTED) {
		alisds_event_handler(ALISDS_EVENT_WLAN_CONNECTED);
	}
	else if (event == EMH_ARG_WLAN_EV_STA_DISCONNECTED) {
		alisds_event_handler(ALISDS_EVENT_WLAN_DISCONNECTED);
	}
}

void emh_ev_ali_connection(emh_arg_ali_conn_e conn)
{
	sds_log("AliCloud event: %s", emh_arg_for_type(EMH_ARG_ALI_CONN, conn));
	
	context.cloud_state = conn;
	
	if (conn == EMH_ARG_ALI_CONN_CONNECTED) {
		if (context.delay_prov == true) {
			alisds_provision();
			context.delay_prov = false;
		}

		
		mx_hal_delay_ms(1000);
		alisds_event_handler(ALISDS_EVENT_CLOUD_CONNECTED);
		
		context.device_state = eState_M3_normal;
		/* Alicloud get local value event do not trigger, so we trigger automatically */
		//for (int i = 0; i<context.num_handles; i++) {
		//	alisds_attr_indicate_by_handle(i);
		//}
	}

	if (conn == EMH_ARG_ALI_CONN_DISCONNECTED) {
		alisds_event_handler(ALISDS_EVENT_CLOUD_DISCONNECTED);
		context.device_state = eState_M4_disconnected;
	}
}


#define NUM_TOKENS	30


void emh_ev_ali_set_local_atts(emh_ali_local_attrs_t *attrs)
{
	jsontok_t json_tokens[NUM_TOKENS];
	jobj_t jobj;
	
	ali_att_val value;
	int attr_handles[50];
	int num;
	int handle;
	
	sds_log("Set local attrs event");
	require(attrs->format==EMH_ARG_ALI_FORMAT_JSON, exit);
	
	memset(attr_handles, 0, 50);
	mx_status err = json_init(&jobj, json_tokens, NUM_TOKENS, (char *)attrs->data, attrs->len);
	if (err != kNoErr) return;
	
	err = json_get_array_object(&jobj, "attrSet", &num);
	require_noerr(err, exit);
	
	for (int i = 0; i < num; i++) {
		err = json_array_get_str(&jobj, i, incomming_name, SDS_ATTR_NAME_MAX_LEN);
		require_noerr(err, exit);
		
		for( handle = 0; handle < context.num_handles; handle++) {
			if (strcmp(incomming_name, alisds_attr_db[handle].name)==0 ) {
				attr_handles[i] = handle;
				break;
			}
		}
	}
	
	err = json_release_array_object(&jobj);
	require_noerr(err, exit);
	
	for (int i = 0; i < num; i++) {
		handle = attr_handles[i];
		
		if (alisds_attr_db[handle].write_func == NULL) continue;
		if (kNoErr == json_get_composite_object(&jobj, alisds_attr_db[handle].name)) {
			if (kNoErr == json_get_val_str(&jobj, "value", incomming_val, SDS_ATTR_VAL_MAX_LEN)) {
				if (alisds_attr_db[handle].att_type == ALI_ATT_TYPE_BOOL
				||  alisds_attr_db[handle].att_type == ALI_ATT_TYPE_INT) {
					sscanf(incomming_val, "%d", (int *)&value);
				}
				else if (alisds_attr_db[handle].att_type == ALI_ATT_TYPE_FLOAT) {
					sscanf(incomming_val, "%f", (float *)&value);
				}
				else if (alisds_attr_db[handle].att_type == ALI_ATT_TYPE_STRING) {
					value.stringValue = incomming_val;
				}
				alisds_attr_db[handle].write_func(value);
				json_release_composite_object(&jobj);
				alisds_attr_indicate_by_handle(handle);
			}
		}
	}
		
	
	exit:
	return;
}


void alisds_indicate_local_atts(int attr_handles[], int num)
{
	if (context.device_state != eState_M3_normal ) return;
	
	struct json_str jstr;
	char buff[512] = {0};
	char val_str[20];
	mx_status err = kNoErr;
	ali_att_val value;
	int handle;
	int i;
	
	if(attr_handles[0]==ALI_HANDLE_NONE) return;
	
	sds_log("Indicate local attrs");
	
	json_str_init(&jstr, buff, sizeof(buff));
	
	err = json_start_object(&jstr);
	require_noerr(err, exit);
	
	for (i = 0; i < num && attr_handles[i] != ALI_HANDLE_NONE; i++) {
		handle = attr_handles[i];
		if (alisds_attr_db[handle].read_func == NULL ) continue;
				
		err = json_push_object(&jstr, alisds_attr_db[handle].name);
		require_noerr(err, exit);
		
		if (alisds_attr_db[handle].att_type == ALI_ATT_TYPE_BOOL) {
			err = alisds_attr_db[handle].read_func(&value);
			require_noerr(err, exit);
			snprintf(val_str, 20, "%d", value.boolValue);
			err = json_set_val_str(&jstr, "value", val_str);
			require_noerr(err, exit);
		}
		else if (alisds_attr_db[handle].att_type == ALI_ATT_TYPE_INT) {
			err = alisds_attr_db[handle].read_func(&value);
			require_noerr(err, exit);
			snprintf(val_str, 20, "%d", value.intValue);
			err = json_set_val_str(&jstr, "value", val_str);
			require_noerr(err, exit);
		}
		else if (alisds_attr_db[handle].att_type == ALI_ATT_TYPE_FLOAT) {
			err = alisds_attr_db[handle].read_func(&value);
			require_noerr(err, exit);
			snprintf(val_str, 20, "%.2f", value.floatValue);
			err = json_set_val_str(&jstr, "value", val_str);
			require_noerr(err, exit);
		}
		else if (alisds_attr_db[handle].att_type == ALI_ATT_TYPE_STRING) {
			err = alisds_attr_db[handle].read_func(&value);
			require_noerr(err, exit);
			//if (strlen(value.stringValue)) {
				err = json_set_val_str(&jstr, "value", value.stringValue);
			//}
			//else {
			//	err = json_set_val_null(&jstr, "value");
			//}
			require_noerr(err, exit);
		}
		err = json_pop_object(&jstr);
		require_noerr(err, exit);		
	}
	
	/* Create attrset */
	json_push_array_object(&jstr, "attrSet");
	for (int i = 0; i < num && attr_handles[i] != ALI_HANDLE_NONE; i++) {
		handle = attr_handles[i];
		err = json_set_array_str(&jstr, alisds_attr_db[handle].name);
		require_noerr(err, exit);
	}
	json_pop_array_object(&jstr);
	
	err = json_close_object(&jstr);
	require_noerr(err, exit);
	
	sds_log("Send to cloud %d bytes > %s", strlen(buff), buff);
	err = emh_ali_set_cloud_atts(EMH_ARG_ALI_FORMAT_JSON, (uint8_t *)buff, strlen(buff));
	require_noerr(err, exit);
	
	for (i = 0; i < num; i++)
	{
		attr_handles[i] = ALI_HANDLE_NONE;
	}
		
exit:
	return;	

}


void alisds_provision(void)
{
	mx_status err = kNoErr;
	char provision_ack[50];
	
	if (context.cloud_state == EMH_ARG_ALI_CONN_CONNECTED) {
		snprintf(provision_ack, 50, "{\"ErrorCode\":{\"value\":\"%d\"}}", 0);
		err = emh_ali_set_cloud_atts(EMH_ARG_ALI_FORMAT_JSON, (uint8_t *)provision_ack, strlen(provision_ack));
		require_noerr(err, exit);
		
		snprintf(provision_ack, 50, "{\"ErrorCode\":{\"value\":\"%d\"}}", 1);
		err = emh_ali_set_cloud_atts(EMH_ARG_ALI_FORMAT_JSON, (uint8_t *)provision_ack, strlen(provision_ack));
		require_noerr(err, exit);
		
		sds_log("Send provision acknowledgment to alisds");
	}
	else {
		context.delay_prov = true;
	}
	
	#if 0
	light_on = (light_on==true)? false:true;
	
	if (light_on == true) {
		hsb2rgb_led_open(hue, saturation, bright);
	}
	else {
		hsb2rgb_led_close();
	}
	
	alisds_attr_indicate_by_handle(ALI_HANDLE_LIGHT_SWITCH);
	#endif

	exit:
	return;
}

void alisds_restore(void)
{
	if (context.device_state == eState_M2_provision || context.device_state == eState_M3_normal) {
		emh_ali_unbound();
	}
	
	emh_module_restore_settings();
	context.device_state = eState_M1_initialize;
}


