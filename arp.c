#include "malcolm.h"

void send_arp_reply(struct arp_packet *request, struct sockaddr_ll *addr)
{
	(void)request;
	(void)addr;
}

void handle_arp_request(struct arp_packet *request, struct sockaddr_ll *addr) {
    // Check if the target IP matches your interface IP
	unsigned char your_ip_address[4] = {11, 11, 11, 11};
    if (memcmp(request->target_ip, &your_ip_address, 4) == 0) {
        send_arp_reply(request, addr);
    }
}

void	arp(runtime* run)
{
	int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (sock < 0) {
		err_exit(SOCK_ERR, run);
	}

	struct arp_packet packet;
	struct sockaddr_ll addr;
	socklen_t addr_len = sizeof(addr);

	ssize_t len = recvfrom(sock, &packet, sizeof(packet), 0, (struct sockaddr*)&addr, &addr_len);

	if (len > 0 && packet.arp_header.ar_op == htons(ARPOP_REQUEST)) {
		char ifname[IF_NAMESIZE];

		if (!if_indextoname(addr.sll_ifindex, ifname))
			err_exit(ERR_MAX, run);
		print_step(STEP_INTERFACE, run, ifname);
		handle_arp_request(&packet, &addr);
	}
}