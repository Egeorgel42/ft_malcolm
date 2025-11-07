#ifndef MALCOLM_H
# define MALCOLM_H

# include "libft/libft.h"
# include <errno.h>
# include <arpa/inet.h>
# include <string.h>
# include <ifaddrs.h>
# include <stdio.h>
# include <stdarg.h>
# include <ifaddrs.h>
# include <sys/socket.h>
# include <netinet/if_ether.h>
# include <netpacket/packet.h>
# include <net/if.h>
# include <net/if_arp.h>
# include <signal.h>

struct arp_packet {
    struct ethhdr	eth_header;
    struct arphdr	arp_header;
    unsigned char	sender_mac[6];
    in_addr_t		sender_ip;
    unsigned char	target_mac[6];
    in_addr_t		target_ip;
} __attribute__((packed));

typedef struct s_runtime {
	char**				err;
	char**				steps;
	in_addr_t			ip_src;
	in_addr_t			ip_trg;
    unsigned char		mac_src[6];
    unsigned char		mac_trg[6];
	struct arp_packet	response;
	unsigned int		trg_interface_index;
	bool				flood_flag;
} runtime;

typedef enum e_err {
	INV_PARAMS,
	INV_IP,
	INV_MAC,
	PERM_MAC_WILDCARD,
	SOCK_ERR,
	NO_INTERFACE,
	ERR_MAX
} err;

typedef enum e_steps {
	STEP_INTERFACE,
	STEP_BROADCAST,
	STEP_WAIT_REPLY,
	STEP_REPLY,
	STEP_EXIT,
	STEP_FLOOD,
	STEP_FLOOD_STOP,
	STEPS_MAX
} steps;

void	parse_params(int argc, char**argv, runtime* run);
void	print_step(int step, runtime* run, ...);
void	err_exit(int err, runtime* run, ...);
void	init_msgs(runtime* run);
bool	is_hexa(char* str, int size);
bool	is_character(char* str, int size, char c);
void	arp(runtime* run);
void	free_runtime(runtime *run);
void	check_signal_exit(runtime *run);

#endif