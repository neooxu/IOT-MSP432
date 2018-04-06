

#ifndef _EMH_ALICLOUD_DB_H_
#define _EMH_ALICLOUD_DB_H_

#include "mx_common.h"
#include "mx_debug.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum _ali_att_type_e{
	ALI_ATT_TYPE_BOOL,
	ALI_ATT_TYPE_INT,
	ALI_ATT_TYPE_FLOAT,
	ALI_ATT_TYPE_STRING,
	ALI_ATT_TYPE_DATA,
	ALI_ATT_TYPE_NULL,
} ali_att_type_e;

typedef union {
	bool		boolValue;
	int			intValue;
	double		floatValue;
	char*		stringValue;
	char*		dateValue;
} ali_att_val;

typedef mx_status (*alisds_read_attr)(ali_att_val *value);
typedef mx_status (*alisds_write_attr)(ali_att_val value);

typedef struct _ali_dev_attr_t {
	char*					name;
	ali_att_type_e			att_type;
	alisds_read_attr		read_func;
	alisds_write_attr		write_func;
} ali_dev_attr_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
