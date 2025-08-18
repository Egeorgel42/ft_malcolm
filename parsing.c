#include "malcolm.h"

static void	parse_mac(char* src, char* dest, runtime* run, bool has_star)
{
	bool star = false;
	int j = 0;
	int k = 0;
	for (int i = 0;; i++)
	{
		if (is_character(&src[j], 2, '*'))
		{
			if (!has_star)
				err_exit(PERM_MAC_WILDCARD, run);
			star = true;
		}
		else if (star)
			err_exit(INV_MAC, run, src);
		else if (!is_hexa(&src[j], 2))
			err_exit(INV_MAC, run, src);
		ft_strlcpy(&dest[k], &src[j], 2);
		k += 2;
		j += 2;
		if (!(i < 5))
			break;
		if (src[j] != ':')
			err_exit(INV_MAC, run, src);
		j++;
	}
	if (src[j])
		err_exit(INV_MAC, run, src);
}

static in_addr_t	parse_ip(char* str, runtime* run)
{
	in_addr_t ip = inet_addr(str);
	if (ip == (in_addr_t)(-1))
		err_exit(INV_IP, run, str);
	return ip;
}

void	parse_params(int argc, char**argv, runtime* run)
{
	if (argc != 5)
		err_exit(INV_PARAMS, run);
	run->ip_src = parse_ip(argv[1], run);
	parse_mac((char*)run->mac_src, argv[2], run, false);
	run->ip_trg = parse_ip(argv[3], run);
	parse_mac((char*)run->mac_trg, argv[4], run, true);
}