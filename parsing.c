#include "malcolm.h"

static void	hex_to_uchar(unsigned char* dest, char *src)
{
	*dest = src[0];
	*dest <<= 4;
	*dest &= src[1];
}

static void	parse_mac(char* src, unsigned char* dest, runtime* run, bool has_star)
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
		hex_to_uchar(&dest[k], &src[j]);
		k++;
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
	parse_mac(argv[2], run->mac_src, run, false);
	run->ip_trg = parse_ip(argv[3], run);
	parse_mac(argv[4], run->mac_trg, run, true);
}