/* dhcpd.c
 *
 * udhcp Server
 * Copyright (C) 1999 Matthew Ramsay <matthewr@moreton.com.au>
 *			Chris Trew <ctrew@moreton.com.au>
 *
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/time.h>

#include "dhcpd.h"
#include "arpping.h"
#include "socket.h"
#include "options.h"
#include "files.h"
#include "serverpacket.h"
#include "common.h"


/* globals */
struct dhcpOfferedAddr *leases;
struct dhcpManualAddr manualPool[2];
struct server_config_t server_config;
unsigned int manualCount = 0;


int udhcpd_main(int argc, char *argv[])
{	
	fd_set rfds;
	struct timeval tv;
	int server_socket = -1;
	int bytes, retval;
	struct dhcpMessage packet;
	unsigned char *state;
	unsigned char *server_id, *requested;
	u_int32_t server_id_align, requested_align;
	unsigned long timeout_end;
	struct option_set *option;
	struct dhcpOfferedAddr *lease;
	int max_sock;
	int sig,i,mac14,mac56;
	unsigned long num_ips;
	FILE *fp;
	FILE *f;
	unsigned char p_chaddr[6];
	unsigned long p_yiaddr,p_tmp1, p_tmp2;
	
	start_log("server");

	memset(&server_config, 0, sizeof(struct server_config_t));
	
	if (argc < 2)
		read_config(DHCPD_CONF_FILE);
	else read_config(argv[1]);

	if ((option = find_option(server_config.options, DHCP_LEASE_TIME))) {
		memcpy(&server_config.lease, option->data + 2, 4);
		server_config.lease = ntohl(server_config.lease);
	}
	else server_config.lease = LEASE_TIME;
	
	/* Sanity check */
	num_ips = ntohl(server_config.end) - ntohl(server_config.start);
	if (server_config.max_leases > num_ips) {
		LOG(LOG_ERR, "max_leases value (%lu) not sane, "
			"setting to %lu instead",
			server_config.max_leases, num_ips);
		server_config.max_leases = num_ips;
	}

	leases = xcalloc(sizeof(struct dhcpOfferedAddr), server_config.max_leases);
	read_leases(server_config.lease_file);

	if (read_interface(server_config.interface, &server_config.ifindex,
			   &server_config.server, server_config.arp) < 0)
		return(1);
#if 1 
	background(server_config.pidfile);
#endif
	//udhcp_set_signal_pipe(0);/*rupert*/
	udhcp_set_signal_pipe(SIGUSR2);

	timeout_end = time(0) + server_config.auto_time;

	f=fopen("/var/dhcpd_client_static", "r");
	if(f != NULL ){
	for(i=0;i<2;i++){	
		fscanf(f, "%lx\n", &p_yiaddr);
		fscanf(f, "%lx\n", &p_tmp1);
		fscanf(f, "%lx\n", &p_tmp2);		
		printf("p_chaddr=%02x-%02x, p_yiaddr=%02x\n", p_tmp1, p_tmp2, p_yiaddr);
		p_chaddr[0] = (unsigned char)((p_tmp1 & 0xff000000) >> 24);
		p_chaddr[1] = (unsigned char)((p_tmp1 & 0x00ff0000) >>16);
		p_chaddr[2] = (unsigned char)((p_tmp1 & 0x0000ff00) >> 8);
		p_chaddr[3] = (unsigned char)(p_tmp1 & 0x000000ff);
		p_chaddr[4] = (unsigned char)((p_tmp2 & 0xff000000) >> 24);
		p_chaddr[5] = (unsigned char)((p_tmp2 & 0x00ff0000) >> 16);
	
		printf("[Penguin-dhcpd.c] p_chaddr[]=%02x-%02x-%02x-%02x-%02x-%02x\n", p_chaddr[0], p_chaddr[1], p_chaddr[2], p_chaddr[3], p_chaddr[4], p_chaddr[5]);
		add_lease(p_chaddr, p_yiaddr, 0x00093a80, "");		
	}
	 	//fscanf(f, "%lu %02x %02x %02x %02x %02x %02x \n", p_yiaddr, p_chaddr[0], p_chaddr[1], p_chaddr[2], p_chaddr[3], p_chaddr[4], p_chaddr[5]);
		fclose(f);
		//printf("[Penguin-dhcpd.c] udhcpd_main: p_chaddr=%02x-%02x-%02x-%02x-%02x-%02x, p_yiaddr=%02x\n", p_chaddr[0], p_chaddr[1], p_chaddr[2], p_chaddr[3], p_chaddr[4], p_chaddr[5], p_yiaddr);
	}

	while(1) { /* loop until universe collapses */

		if (server_socket < 0)
			if ((server_socket = listen_socket(INADDR_ANY, SERVER_PORT, server_config.interface)) < 0) {
				LOG(LOG_ERR, "FATAL: couldn't create server socket, %m");
				return(2);
			}			

		FD_ZERO(&rfds);
		FD_SET(server_socket, &rfds);
		FD_SET(udhcp_signal_pipe[0], &rfds);
		if (server_config.auto_time) {
			tv.tv_sec = timeout_end - time(0);
			tv.tv_usec = 0;
		}
		if (!server_config.auto_time || tv.tv_sec > 0) {
			max_sock = server_socket > udhcp_signal_pipe[0] ? server_socket : udhcp_signal_pipe[0];
			retval = select(max_sock + 1, &rfds, NULL, NULL, 
					server_config.auto_time ? &tv : NULL);
		} else retval = 0; /* If we already timed out, fall through */

		if (retval == 0) {
			write_leases();
			timeout_end = time(0) + server_config.auto_time;
			continue;
		} else if (retval < 0 && errno != EINTR) {
			DEBUG(LOG_INFO, "error on select");
			continue;
		}
		
		if (FD_ISSET(udhcp_signal_pipe[0], &rfds)) {
			if (read(udhcp_signal_pipe[0], &sig, sizeof(sig)) < 0)
				continue; /* probably just EINTR */
			switch (sig) {
			case SIGUSR1:
				LOG(LOG_INFO, "Received a SIGUSR1");
				write_leases();
				/* why not just reset the timeout, eh */
				timeout_end = time(0) + server_config.auto_time;
				continue;
			case SIGUSR2: /* rupert*/
				fp = fopen("/var/dhcps_client","w+");
				for (i = 0; i < server_config.max_leases; i++)
				{
				        if (leases[i].yiaddr) 
					{
						mac14=leases[i].chaddr[0];
						mac56=*(unsigned short*)&leases[i].chaddr[4];
						if (mac14==0 && mac56==0)
								continue;
						 fprintf(fp,"%02X:%02X:%02X:%02X:%02X:%02X ",
							leases[i].chaddr[0],leases[i].chaddr[1],
							leases[i].chaddr[2],leases[i].chaddr[3],
							leases[i].chaddr[4],leases[i].chaddr[5]);
						 fprintf(fp,"%u.%u.%u.%u ", 
								(leases[i].yiaddr>>24)&0xff,
								(leases[i].yiaddr>>16)&0xff,
								(leases[i].yiaddr>>8)&0xff,
								(leases[i].yiaddr)&0xff
										);
						if (leases[i].hostname)
						fprintf(fp,"%s\n", leases[i].hostname);
						else 
							fprintf(fp,"\n");
							
					}

				}
				fprintf(fp,"@@");
				fclose(fp);
				continue;


			case SIGTERM:
				LOG(LOG_INFO, "Received a SIGTERM");
				return(0);
			}
		}

		if ((bytes = get_packet(&packet, server_socket)) < 0) { /* this waits for a packet - idle */
			if (bytes == -1 && errno != EINTR) {
				DEBUG(LOG_INFO, "error on read, %m, reopening socket");
				close(server_socket);
				server_socket = -1;
			}
			continue;
		}

		if ((state = get_option(&packet, DHCP_MESSAGE_TYPE)) == NULL) {
			DEBUG(LOG_ERR, "couldn't get option from packet, ignoring");
			continue;
		}
		
		/* ADDME: look for a static lease */
		lease = find_lease_by_chaddr(packet.chaddr);
		switch (state[0]) {
		case DHCPDISCOVER:
			DEBUG(LOG_INFO,"received DISCOVER");
			
			if (sendOffer(&packet) < 0) {
				LOG(LOG_ERR, "send OFFER failed");
			}
			break;			
 		case DHCPREQUEST:
			DEBUG(LOG_INFO, "received REQUEST");

			requested = get_option(&packet, DHCP_REQUESTED_IP);
			server_id = get_option(&packet, DHCP_SERVER_ID);

			if (requested) memcpy(&requested_align, requested, 4);
			if (server_id) memcpy(&server_id_align, server_id, 4);
		
			if (lease) { /*ADDME: or static lease */
				if (server_id) {
					/* SELECTING State */
					DEBUG(LOG_INFO, "server_id = %08x", ntohl(server_id_align));
					if (server_id_align == server_config.server && requested && 
					    requested_align == lease->yiaddr) {
						sendACK(&packet, lease->yiaddr);
					}
				} else {
					if (requested) {
						/* INIT-REBOOT State */
						if (lease->yiaddr == requested_align)
							sendACK(&packet, lease->yiaddr);
						else sendNAK(&packet);
					} else {
						/* RENEWING or REBINDING State */
						if (lease->yiaddr == packet.ciaddr)
							sendACK(&packet, lease->yiaddr);
						else {
							/* don't know what to do!!!! */
							sendNAK(&packet);
						}
					}						
				}
			
			/* what to do if we have no record of the client */
			} else if (server_id) {
				/* SELECTING State */

			} else if (requested) {
				/* INIT-REBOOT State */
				if ((lease = find_lease_by_yiaddr(requested_align))) {
					if (lease_expired(lease)) {
						/* probably best if we drop this lease */
						memset(lease->chaddr, 0, 16);
					/* make some contention for this address */
					} else sendNAK(&packet);
				} else if (requested_align < server_config.start || 
					   requested_align > server_config.end) {
					sendNAK(&packet);
				} /* else remain silent */

			} else {
				 /* RENEWING or REBINDING State */
			}
			break;
		case DHCPDECLINE:
			DEBUG(LOG_INFO,"received DECLINE");
			if (lease) {
				memset(lease->chaddr, 0, 16);
				lease->expires = time(0) + server_config.decline_time;
			}			
			break;
		case DHCPRELEASE:
			DEBUG(LOG_INFO,"received RELEASE");
			if (lease) lease->expires = time(0);
			break;
		case DHCPINFORM:
			DEBUG(LOG_INFO,"received INFORM");
			send_inform(&packet);
			break;	
		default:
			LOG(LOG_WARNING, "unsupported DHCP message (%02x) -- ignoring", state[0]);
		}
	}

	return 0;
}
