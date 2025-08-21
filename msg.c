#include "malcolm.h"

static void	assign_msg(int nbr, char* msg, char** msg_arr)
{
	msg_arr[nbr] = ft_strdup(msg);
	if (!msg_arr[nbr])
	{
		ft_printf(strerror(errno));
		exit(1);
	}
}

void	init_msgs(runtime* run)
{
	run->err = malloc(sizeof(char*) * ERR_MAX);
	if (!run->err)
	{
		ft_printf(strerror(errno));
		exit(1);
	}
	assign_msg(INV_PARAMS, "ft_malcolm: Invalid parameters: source_ip | source_mac_addr | target_ip | target_mac_addr\n", run->err);
	assign_msg(INV_IP, "ft_malcolm: unknown host or invalid IP address: (%s)\n", run->err);
	assign_msg(INV_MAC, "ft_malcolm: invalid mac address: (%s)\n", run->err);
	assign_msg(PERM_MAC_WILDCARD, "ft_malcolm: wildcard '*' not allowed for source MAC address\n", run->err);
	assign_msg(SOCK_ERR, "ft_malcolm: Socket creation failed\n", run->err);

	run->steps = malloc(sizeof(char*) * STEPS_MAX);
	if (!run->steps)
	{
		ft_printf(strerror(errno));
		exit(1);
	}
	assign_msg(STEP_INTERFACE, "Found available interface: %s\n", run->steps);
	assign_msg(STEP_BROADCAST, "An ARP request has been broadcast\n\tmac address of request: %02x:%02x:%02x:%02x:%02x:%02x\n\tIP address of request: %s\n", run->steps);
	assign_msg(STEP_WAIT_REPLY, "Now sending an ARP reply to the target address with spoofed source, please wait.", run->steps);
	assign_msg(STEP_REPLY, "Sent an ARP reply packet, you may now check the arp table on the target", run->steps);
	assign_msg(STEP_EXIT, "Exiting program...", run->steps);
}

void	print_step(int step, runtime* run, ...)
{
	va_list args;
	va_start(args, run);
	vprintf(run->steps[step], args);
	va_end(args);
}

void	err_exit(int err, runtime* run, ...)
{
	if (err == ERR_MAX)
	{
		printf(strerror(errno));
		exit(1);
	}
	va_list args;
	va_start(args, run);
	vprintf(run->err[err], args);
	va_end(args);
	exit(1);
}
