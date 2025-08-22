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

	if (sendto(sock, &reply, sizeof(arp_packet), 0, NULL, 0) == -1)
		err_exit(ERR_MAX, run);
	print_step(STEP_REPLY, run);
}

bool ip_owned_by_ifa(in_addr_t ip, in_addr_t ifa_ip, in_addr_t ifa_mask)
{
	if ((ip & ifa_mask) == (ifa_ip & ifa_mask))
		return true;
	return false;
}

void	set_target_interface(runtime* run)
{
	struct ifaddrs *ifa_begin, *ifa_it;

	if (getifaddrs(&ifa_begin))
		err_exit(ERR_MAX, run);
	
	for (ifa_it = ifa_begin; ifa_it; ifa_it = ifa_it->ifa_next)
	{
		if (ifa_it->ifa_addr == NULL || ifa_it->ifa_netmask == NULL)
			continue;
		if (ip_owned_by_ifa(run->ip_trg, ((struct sockaddr_in*)ifa_it->ifa_addr)->sin_addr.s_addr, ((struct sockaddr_in*)ifa_it->ifa_netmask)->sin_addr.s_addr))
			break;
	}
	if (!ifa_it)
	{
		char ip_str[INET_ADDRSTRLEN];
		if (!inet_ntop(AF_INET, &run->ip_trg, ip_str, INET_ADDRSTRLEN))
			err_exit(ERR_MAX, run);
		err_exit(NO_INTERFACE, run, ip_str);
	}
	if ((run->trg_interface_index = if_nametoindex(ifa_it->ifa_name)) == 0)
		err_exit(ERR_MAX, run);
	freeifaddrs(ifa_begin);
}

void	print_broadcast(runtime* run, arp_packet *request)
{
	char ip_str[INET_ADDRSTRLEN];

	if (!inet_ntop(AF_INET, &request->sender_ip, ip_str, INET_ADDRSTRLEN))
		err_exit(ERR_MAX, run);
	print_step(STEP_BROADCAST, run, request->sender_mac[0], request->sender_mac[1], request->sender_mac[2], request->sender_mac[3],
		request->sender_mac[4], request->sender_mac[5], ip_str);
	print_step(STEP_WAIT_REPLY, run);
}

bool	listen_arp(runtime *run, int sock)
{
	arp_packet request;
	ssize_t len = recvfrom(sock, &request, sizeof(arp_packet), 0, NULL, 0);

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
	set_target_interface(run);
	while (!listen_arp(run, sock)) {}
	send_reply(run, sock);
}