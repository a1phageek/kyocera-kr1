#ifndef _PSEUDOICSD_PORTMAPP_H_INCLUDED_
#define _PSEUDOICSD_PORTMAPP_H_INCLUDED_

#include "PortMap.h"

typedef struct _PortMapItem {
	//// int fd_socket;
	int Protocol;
	char *RemoteHost;
	char *ExternalIP;
	int ExternalPort;
	char *InternalIP;
	int InternalPort;
	int Enabled;
	char *PortMappingDescription;
	int LeaseDuration;
} _PortMapItem;

#endif
