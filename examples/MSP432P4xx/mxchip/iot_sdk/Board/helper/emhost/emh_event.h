

#ifndef _EMH_EVENT_H_
#define _EMH_EVENT_H_

#include "mx_common.h"
#include "emh_api.h"

#ifdef __cplusplus
extern "C"
{
#endif




MX_WEAK void emh_ev_unknown(void);
MX_WEAK void emh_ev_ali_connection(emh_arg_ali_conn_e conn);
MX_WEAK void emh_ev_ali_get_local_atts(emh_ali_local_attrs_t *attrs);
MX_WEAK void emh_ev_ali_set_local_atts(emh_ali_local_attrs_t *attrs);
MX_WEAK void emh_ev_wlan(emh_arg_wlan_ev_e event);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
