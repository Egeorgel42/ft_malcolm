#include "malcolm.h"

static void	parse_mac(char* str, runtime* run)
{
	bool star = false;
	int j = 0;
	for (int i = 0;; i++)
	{
		if (!star && is_character(&str[j], 2, '*'))
			star = true;
		else if (star && is_hexa(&str[j], 2))
			err_exit(INV_MAC, run, str);
		else if (!is_character(&str[j], 2, '*') && !is_hexa(&str[j], 2))
			err_exit(INV_MAC, run, str);
		j += 2;
		if (!(i < 5))
			break;
		if (str[j] != ':')
			err_exit(INV_MAC, run, str);
		j++;
	}
	if (str[j])
		err_exit(INV_MAC, run, str);
}

static in_addr_t	parse_ip(char* str, runtime* run)
{
	in_addr_t ip = inet_addr(str);
	if (ip == (in_addr_t)(-1))
		err_exit(INV_IP, run, str);
	return htonl(ip);
}

void	parse_params(int argc, char**argv, runtime* run)
{
	if (argc != 5)
		err_exit(INV_PARAMS, run);
	run->ip_src = parse_ip(argv[1], run);
	parse_mac(argv[2], run);
	run->ip_trg = parse_ip(argv[3], run);
	parse_mac(argv[4], run);
}