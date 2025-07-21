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
	run->err = malloc(sizeof(char*) * (ERR_MAX + 1));
	if (!run->err)
	{
		ft_printf(strerror(errno));
		exit(1);
	}
	assign_msg(INV_PARAMS, "ft_malcolm: Invalid parameters: source_ip | source_mac_addr | target_ip | target_mac_addr\n", run->err);
	assign_msg(INV_IP, "t_malcolm: unknown host or invalid IP address: (%s)\n", run->err);
	assign_msg(INV_MAC, "t_malcolm: invalid mac address: (%s)\n", run->err);

	run->steps = malloc(sizeof(char*) * (STEPS_MAX + 1));
	if (!run->steps)
	{
		ft_printf(strerror(errno));
		exit(1);
	}
	assign_msg(STEP_INTERFACE, "Found available interface: %s\n", run->steps);
	assign_msg(STEP_BROADCAST, "An ARP request has been broadcast\n\tmac address of request: %s\n\tIP address of request: %s\n", run->steps);
	assign_msg(STEP_WAIT_REPLY, "Now sending an ARP reply to the target address with spoofed source, please wait.", run->steps);
	assign_msg(STEP_REPLY, "Sent an ARP reply packet, you may now check the arp table on the target", run->steps);
	assign_msg(STEP_EXIT, "Exiting program...", run->steps);
}

void	err_exit(int err, runtime* run, char* err_var)
{
	if (!err_var)
		ft_printf(run->err[err]);
	else
		ft_printf(run->err[err], err_var);
	exit(1);
}

bool	is_hexa(char* str, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F')))
			return false;
	}
	return true;
}

bool	is_character(char* str, int size, char c)
{
	for (int i = 0; i < size; i++)
	{
		if (str[i] != c)
			return false;
	}
	return true;
}