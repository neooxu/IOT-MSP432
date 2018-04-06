

#ifndef _EMH_API_H_
#define _EMH_API_H_

#include "mx_common.h"
#include "mx_debug.h"
#include "emh_arg.h"
#include "emh_event.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Module controller */
mx_status	emh_module_init				(void);
void		emh_module_task				(void);

mx_status	ewh_module_reset			(void);
const char* emh_module_get_fw_version	(void);
uint32_t    emh_module_get_tick			(void);
mx_status	emh_module_restore_settings	(void);


/* wlan management */
mx_status			emh_wlan_get_para			(char ssid[33], char pwd[33]);
emh_arg_wlan_sta_e emh_wlan_get_sta_status		(void);

/* Ali SDS APIs */

typedef struct{
	struct{
		const char*				name;			/**< ALINKTEST */
		const char*				modle;			/**< ALINKTEST_LUA */
		const char*				key;			/**< LIVING */
		const char*				secret;			/**< LIGHT */
		emh_arg_ali_format_e	format;			/**< ALINKTEST_LIVING_LIGHT_ALINK_TEST_LUA */
	} product_info;
	struct{
		const char*				type;
		const char*				category;
		const char*				manufacture;
	} dev_info ;
} emh_ali_config_t;

mx_status				emh_ali_config			(const emh_ali_config_t *config);
mx_status				emh_ali_start_service	(void);
mx_status				emh_ali_provision		(bool on);
emh_arg_ali_status_e	emh_ali_get_status		(void);
mx_status				emh_ali_set_cloud_atts	(emh_arg_ali_format_e format, uint8_t *data, int32_t len);
mx_status				emh_ali_set_key         (const char *dev_key, const char *dev_sec);

mx_status				emh_ali_unbound			(void);



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
