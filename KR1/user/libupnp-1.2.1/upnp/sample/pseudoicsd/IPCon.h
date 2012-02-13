#ifndef _PSEUDOICSD_IPCON_H_INCLUDED_
#define _PSEUDOICSD_IPCON_H_INCLUDED_

typedef struct _IPCon * IPCon;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

/*****		*****/

IPCon IPCon_New(char * ifname);
IPCon IPCon_Destroy(IPCon this);


/*****		*****/

int IPCon_SetIfName(IPCon this, char * ifname);
char * IPCon_GetIfName(IPCon this);


/*****		*****/

struct in_addr * IPCon_GetIpAddr(IPCon this);
char * IPCon_GetIpAddrByStr(IPCon this);

int IPCon_IsIfUp(IPCon this);	// UP=0
char * IPCon_GetIfStatByStr(IPCon this);

#endif /* _PSEUDOICSD_IPCON_H_INCLUDED_ */
