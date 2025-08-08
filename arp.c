#include "malcolm.h"

bool	listen_arp(runtime *run, int sock)
{
	socklen_t interface_len = sizeof(run->interface);

	ssize_t len = recvfrom(sock, &run->request, sizeof(run->request), 0, (struct sockaddr*)&run->interface, &interface_len);

	if (len > 0 && run->request.arp_header.ar_op == htons(ARPOP_REQUEST) && (run->ip_trg == 0 || run->request.target_ip == run->ip_trg)) {
		char ifname[IF_NAMESIZE];

		if (!if_indextoname(run->interface.sll_ifindex, ifname))
			err_exit(ERR_MAX, run);
		print_step(STEP_INTERFACE, run, ifname);
		return true;
	}
	return false;
}

void	arp(runtime* run)
{
	int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (sock < 0) {
		err_exit(SOCK_ERR, run);
	}
	while (!listen_arp(run, sock)) {}
}