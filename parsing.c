#include "malcolm.h"

static unsigned char hex_to_4bit(char hex)
{
	unsigned char res;
	if (hex >= '0' && hex <= '9')
		res = hex - '0';
	else if (hex >= 'a' && hex <= 'f')
		res = hex - 'a' + 10;
	else
		res = hex - 'A' + 10;
	return res;
}

static void	hex_to_uchar(unsigned char* dest, char *src)
{
	*dest = hex_to_4bit(src[0]);
	*dest <<= 4;
	*dest |= hex_to_4bit(src[1]);
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
	int	param_nbr = 1;

	run->flood_flag = false;
	if (argc == 6 && ft_strcmp(argv[1], "-f") == 0)
	{
		param_nbr++;
		run->flood_flag = true;
	}
	else if (argc != 5)
		err_exit(INV_PARAMS, run);
	run->ip_src = parse_ip(argv[param_nbr++], run);
	parse_mac(argv[param_nbr++], run->mac_src, run, false);
	run->ip_trg = parse_ip(argv[param_nbr++], run);
	parse_mac(argv[param_nbr], run->mac_trg, run, true);
}