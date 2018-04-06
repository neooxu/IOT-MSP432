
#ifndef _ATCMDPARSER_H_
#define _ATCMDPARSER_H_

#include "mx_toolchain.h"
#include "mx_common.h"

#ifdef __cplusplus
extern "C"
{
#endif


#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#define AT_BUFFER_SIZE 512
#define MAX_ARG_NUM    10

typedef void (*oob_callback)(void);

struct oob {
	unsigned len;
	const char *prefix;
	oob_callback cb;
	void *next;
};


void ATCmdParser_debug(bool on);
void ATCmdParser_init(const char *output_delimiter, const char *input_delimiter, int timeout, bool debug);
bool ATCmdParser_recv(const char *response, ...);
bool ATCmdParser_send(const char *command, ...);
void ATCmdParser_add_oob(const char *prefix, oob_callback cb);
void ATCmdParser_set_timeout(int timeout);
int ATCmdParser_read(char *data, int size);
bool ATCmdParser_process_oob(void);

int ATCmdParser_analyse_args(char args[], char *arg_list[], int list_size);
int ATCmdParser_write(const char *data, int size);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
