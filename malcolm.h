#ifndef MALCOLM_H
# define MALCOLM_H

# include "libft/libft.h"
# include "errno.h"
# include <arpa/inet.h>
# include <string.h>
# include <ifaddrs.h>

typedef struct s_runtime {
	char**	err;
	char**	steps;
	in_addr_t ip_src;
	in_addr_t ip_trg;
} runtime;

typedef enum e_err {
	INV_PARAMS,
	INV_IP,
	INV_MAC,
	ERR_MAX
} err;

typedef enum e_steps {
	STEP_INTERFACE,
	STEP_BROADCAST,
	STEP_WAIT_REPLY,
	STEP_REPLY,
	STEP_EXIT,
	STEPS_MAX
} steps;

void	parse_params(int argc, char**argv, runtime* run);
void	err_exit(int err, runtime* run, char* err_var);
void	init_msgs(runtime* run);
bool	is_hexa(char* str, int size);
bool	is_character(char* str, int size, char c);

#endif