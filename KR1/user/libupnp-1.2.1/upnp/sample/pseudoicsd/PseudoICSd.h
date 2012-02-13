#ifndef UPNP_PSEUDO_ICS_H
#define UPNP_PSEUDO_ICS_H

#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>


#include "upnp.h"
#include "sample_util.h"

#include "IPCon.h"
#include "IPConP.h"
#include "PortMap.h"
#include "PortMapP.h"


extern UpnpDevice_Handle device_handle;


#define PICS_INIT_PORT		52869
//// #define PICS_INIT_PORT          49152
#define PICS_INIT_DESC_DOC	"picsdesc"
#define PICS_INIT_CONF_DIR	"/etc/pseudoicsd/"
////#define PICS_INIT_CONF_DIR	"./web/"
#define PICS_PID_FILE "/var/run/pseudoicsd.pid"
#define PICS_PORTMAP_FILE "/var/run/pseudoicsd.port"

#define PICS_SERVICE_SERVCOUNT	3
#define PICS_SERVICE_OSINFO		0
#define PICS_SERVICE_CONFIG		1
#define PICS_SERVICE_CONNECT	2

#define PICS_OSINFO_VARCOUNT	4
#define PICS_OSINFO_MAJORVER	0
#define PICS_OSINFO_MINORVER	1
#define PICS_OSINFO_BUILDNUM	2
#define PICS_OSINFO_PCNAME		3

#define PICS_CONFIG_VARCOUNT	4
#define PICS_CONFIG_COLOR		0

#define PICS_MAX_VAL_LEN 32

/* This should be the maximum VARCOUNT from above */
#define PICS_MAXVARS PICS_OSINFO_VARCOUNT

#define IPCHAINS	"/sbin/ipchains"



/* Mutex for protecting the global state table data
   in a multi-threaded, asynchronous environment.
   All functions should lock this mutex before reading
   or writing the state table data. */
extern pthread_mutex_t PicsDevMutex;





void PicsDeviceShutdownHdlr(int);

int PicsDeviceStateTableInit(char*);
int PicsDeviceHandleSubscriptionRequest(struct Upnp_Subscription_Request *);
int PicsDeviceHandleGetVarRequest(struct Upnp_State_Var_Request *);
int PicsDeviceHandleActionRequest(struct Upnp_Action_Request *);
int PicsDeviceCallbackEventHandler(Upnp_EventType, void*, void*);

#endif
