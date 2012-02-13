#ifndef _PSEUDOICSD_PORTMAP_H_INCLUDED_
#define _PSEUDOICSD_PORTMAP_H_INCLUDED_

typedef struct _PortMapItem * PortMapItem;

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>

#include <linux/types.h>
#include <asm/types.h>
#include <sys/param.h>
#include <sys/socket.h>

//// #include <linux/ip_fw.h>
//// #include <linux/ip_masq.h>

#include "Obj.h"

typedef ObjList PortMap;

PortMap PortMap_New();
PortMap PortMap_Destroy(PortMap pmap);

int PortMap_Add(PortMap pmap,
						char *RemoteHost,
						int Proto,
						char *ExtIP,
						int ExtPort,
						char *IntIP,
						int IntPort,
						int Enabled,
						char *Desc,
						int LeaseDuration
						);

int PortMap_Delete(PortMap pmap,
						int Proto,
						int ExtPort
						);

#endif
