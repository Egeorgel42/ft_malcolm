#include "malcolm.h"

void	send_reply(runtime* run, int sock)
{
	arp_packet reply;

	memcpy(reply.eth_header.h_dest, run->mac_trg, 6);
	memcpy(reply.eth_header.h_source, run->mac_src, 6);
	reply.eth_header.h_proto = htons(ETH_P_ARP);

	reply.arp_header.ar_hrd = htons(ARPHRD_ETHER);
	reply.arp_header.ar_pro = htons(ETH_P_IP);
	reply.arp_header.ar_hln = 6;
	reply.arp_header.ar_pln = 4;
	reply.arp_header.ar_op = htons(ARPOP_REPLY);

	memcpy(&reply.sender_ip, &run->ip_src, 4);
	memcpy(&reply.target_ip, &run->ip_trg, 4);
	memcpy(reply.sender_mac, run->mac_src, 6);
	memcpy(reply.target_mac, run->mac_trg, 6);

	sendto(sock, &reply, sizeof(arp_packet), 0, (struct sockaddr *)&run->interface, sizeof(run->interface));
	print_step(STEP_REPLY, run);
}

void	print_broadcast(runtime* run, arp_packet *request)
{
	char ifname[IF_NAMESIZE];
	char ip_str[INET_ADDRSTRLEN];

	if (!if_indextoname(run->interface.sll_ifindex, ifname))
		err_exit(ERR_MAX, run);
	print_step(STEP_INTERFACE, run, ifname);
	if (!inet_ntop(AF_INET, &request->sender_ip, ip_str, INET_ADDRSTRLEN))
		err_exit(ERR_MAX, run);
	print_step(STEP_BROADCAST, run, request->sender_mac[0], request->sender_mac[1], request->sender_mac[2], request->sender_mac[3],
		request->sender_mac[4], request->sender_mac[5], ip_str);
	print_step(STEP_WAIT_REPLY, run);
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
	send_reply(run, sock);
}