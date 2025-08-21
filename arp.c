#include "malcolm.h"
/*
void	send_reply(runtime* run)
{
	arp_packet reply;

	memcpy(reply.eth_header.h_dest, run->, 6);
	memcpy(reply.eth_header.h_source, run->mac_src, 6);
	reply.eth_header.h_proto = htons(ETH_P_ARP);

	reply.arp_header.ar_hrd = run->request.arp_header.ar_hrd;
}*/



void	print_broadcast(runtime* run, arp_packet *request)
{
	char ifname[IF_NAMESIZE];

	if (!if_indextoname(run->interface.sll_ifindex, ifname))
		err_exit(ERR_MAX, run);
	print_step(STEP_INTERFACE, run, ifname);
	print_step(STEP_BROADCAST, run, request->sender_mac, request->sender_ip);
}

bool	listen_arp(runtime *run, int sock)
{
	arp_packet request;
	socklen_t interface_len = sizeof(run->interface);
	ssize_t len = recvfrom(sock, &request, sizeof(arp_packet), 0, (struct sockaddr*)&run->interface, &interface_len);

	if (len > 0 && request.arp_header.ar_op == htons(ARPOP_REQUEST))
	{
		print_broadcast(run, &request);
		return true;
	}
	return false;
}

void	arp(runtime* run)
{
	int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (sock < 0)
		err_exit(SOCK_ERR, run);
	while (!listen_arp(run, sock)) {}
}