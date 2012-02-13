#include "common.h"
#include "PseudoICSd.h"
#ifdef _UPNP_MM_
#include "upnp_mm.h"
#endif


char *PicsDeviceType[] = {"urn:schemas-upnp-org:device:InternetGatewayDevice:1"
						 ,"urn:schemas-upnp-org:device:WANDevice:1"
						 ,"urn:schemas-upnp-org:device:WANConnectionDevice:1"};

char *PicsServiceType[] = {"urn:schemas-microsoft-com:service:OSInfo:1"
							   ,"urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1"
							   ,"urn:schemas-upnp-org:service:WANIPConnection:1"};

char *PicsServiceId[] = {"urn:microsoft-com:serviceId:OSInfo1"
							 ,"urn:upnp-org:serviceId:WANCommonIFC1"
							 ,"urn:upnp-org:serviceId:WANIPConn1"};
char *GateServiceId[] = {"urn:microsoft-com:serviceId:OSInfo1"
	                         ,"urn:upnp-org:serviceId:WANCommonIFC1"
    	                     ,"urn:upnp-org:serviceId:WANIPConn1"};


/* The amount of time before advertisements
   will expire */
int default_advr_expire=1800;

char *pics_udn = NULL;
IPCon ipcon;
PortMap pmap;

int pm_cnt = 0;

long int startup_time = 0;

UpnpDevice_Handle device_handle=-1;

int debug_on = 0;

/* Mutex for protecting the global state table data
   in a multi-threaded, asynchronous environment.
   All functions should lock this mutex before reading
   or writing the state table data. */
pthread_mutex_t PicsDevMutex= PTHREAD_MUTEX_INITIALIZER;

#ifdef USERAPP_NOMMU
// ICSd status variable
static int upnp_ICSd_enable	= TRUE;		// daemon enable ?
static int upnp_ICSd_ready	= FALSE;		// service ready ?
#endif /* USERAPP_NOMMU */

/********************************************************************************
 * PicsDeviceStateTableInit
 *
 * Description: 
 *       Initialize the device state table for 
 * 	 this TvDevice, pulling identifier info
 *       from the description Document.  Note that 
 *       knowledge of the service description is
 *       assumed.  State table variables and default
 *       values are currently hard coded in this file
 *       rather than being read from service description
 *       documents.
 *
 * Parameters:
 *   DescDocURL -- The description document URL
 *
 ********************************************************************************/
int PicsDeviceStateTableInit(char* DescDocURL) 
{
    IXML_Document * DescDoc=NULL;
    int ret = UPNP_E_SUCCESS; 

    if (UpnpDownloadXmlDoc(DescDocURL, &DescDoc) != UPNP_E_SUCCESS) {
		//// printf("PicsDeviceStateTableInit -- Error Parsing %s\n", DescDocURL);
		ret = UPNP_E_INVALID_DESC;
    }

    pics_udn = SampleUtil_GetFirstDocumentItem(DescDoc, "UDN");

    return(ret);
}




/********************************************************************************
 * PicsDeviceHandleSubscriptionRequest
 *
 * Description: 
 *       Called during a subscription request callback.  If the
 *       subscription request is for this device and either its
 *       control service or picture service, then accept it.
 *
 * Parameters:
 *   sr_event -- The subscription request event structure
 *
 ********************************************************************************/
int PicsDeviceHandleSubscriptionRequest(struct Upnp_Subscription_Request *sr_event) 
{

    IXML_Document * PropSet;
	PropSet = NULL;

    pthread_mutex_lock(&PicsDevMutex);


	if (strcmp(sr_event->UDN,pics_udn) == 0) {

		if (strcmp(sr_event->ServiceId,PicsServiceId[PICS_SERVICE_OSINFO]) == 0) {

			UpnpAddToPropertySet(&PropSet, "OSMajorVersion", "5");
			UpnpAddToPropertySet(&PropSet, "OSMinorVersion", "1");
			UpnpAddToPropertySet(&PropSet, "OSBuildNumber", "2600");
			UpnpAddToPropertySet(&PropSet, "OSMachineName", "RTL865x");
            UpnpAcceptSubscriptionExt(device_handle, sr_event->UDN, sr_event->ServiceId,PropSet,sr_event->Sid);
            ixmlDocument_free(PropSet);

		} else if (strcmp(sr_event->ServiceId,PicsServiceId[PICS_SERVICE_CONFIG]) == 0) {
			UpnpAddToPropertySet(&PropSet, "PhysicalLinkStatus", IPCon_GetIfStatByStr(ipcon));
            UpnpAcceptSubscriptionExt(device_handle, sr_event->UDN, sr_event->ServiceId,PropSet,sr_event->Sid);
            ixmlDocument_free(PropSet);

		} else if (strcmp(sr_event->ServiceId,PicsServiceId[PICS_SERVICE_CONNECT]) == 0) {

			UpnpAddToPropertySet(&PropSet, "PossibleConnectionTypes", "IP_Routed");
			UpnpAddToPropertySet(&PropSet, "ConnectionStatus", "Connected");
			UpnpAddToPropertySet(&PropSet, "X_Name", "Local Area Connection");
			UpnpAddToPropertySet(&PropSet, "ExternalIPAddress", IPCon_GetIpAddrByStr(ipcon));
			UpnpAddToPropertySet(&PropSet, "PortMappingNumberOfEntries", "0");
			UpnpAcceptSubscriptionExt(device_handle, sr_event->UDN, sr_event->ServiceId,PropSet,sr_event->Sid);
			ixmlDocument_free(PropSet);
                                  
		}

    }

    pthread_mutex_unlock(&PicsDevMutex);	

    return(1);
}




/********************************************************************************
 * PicsDeviceHandleGetVarRequest
 *
 * Description: 
 *       Called during a get variable request callback.  If the
 *       request is for this device and either its control service
 *       or picture service, then respond with the variable value.
 *
 * Parameters:
 *   cgv_event -- The control get variable request event structure
 *
 ********************************************************************************/
int PicsDeviceHandleGetVarRequest(struct Upnp_State_Var_Request *cgv_event) 
{

    int getvar_succeeded = 0;

    cgv_event->CurrentVal = NULL;
  
    pthread_mutex_lock(&PicsDevMutex);
  
    if (getvar_succeeded) {
	cgv_event->ErrCode = UPNP_E_SUCCESS;
    } else {
	//// printf("Error in UPNP_CONTROL_GET_VAR_REQUEST callback:\n"); 
	//// printf("   Unknown variable name = %s\n",  cgv_event->StateVarName); 
	cgv_event->ErrCode = 404;
	strcpy(cgv_event->ErrStr, "Invalid Variable");
    }

    pthread_mutex_unlock(&PicsDevMutex);
  
    return(cgv_event->ErrCode == UPNP_E_SUCCESS);
}


int getProtoNum(char * proto) {
	if (strcmp(proto,"TCP")==0) return 6;
	else if (strcmp(proto,"UDP")==0) return 17;
	else return 0;
}

int chkIPADDRstring(char * addr) {
	struct in_addr dmy;
	return inet_aton(addr, &dmy); 
}

/********************************************************************************
 * PicsDeviceCallbackEventHandler
 *
 * Description: 
 *       The callback handler registered with the SDK while registering
 *       root device or sending a search request.  Detects the type of 
 *       callback, and passes the request on to the appropriate procedure.
 *
 * Parameters:
 *   EventType -- The type of callback event
 *   Event -- Data structure containing event data
 *   Cookie -- Optional data specified during callback registration
 *
 ********************************************************************************/
int PicsDeviceCallbackEventHandler(Upnp_EventType EventType, 
			 void *Event, 
			 void *Cookie)
{

    SampleUtil_PrintEvent(EventType, Event);
  
    switch ( EventType) {

	    case UPNP_EVENT_SUBSCRIPTION_REQUEST:
			PicsDeviceHandleSubscriptionRequest((struct Upnp_Subscription_Request *) Event);
			break;

	    case UPNP_CONTROL_GET_VAR_REQUEST:
//			PicsDeviceHandleGetVarRequest((struct Upnp_State_Var_Request *) Event);
			break;

	    case UPNP_CONTROL_ACTION_REQUEST:
			PicsDeviceHandleActionRequest((struct Upnp_Action_Request *) Event);
			break;

		/* ignore these cases, since this is not a control point */
	    case UPNP_DISCOVERY_ADVERTISEMENT_ALIVE:
			break;

	    case UPNP_DISCOVERY_SEARCH_RESULT:
			break;

	    case UPNP_DISCOVERY_SEARCH_TIMEOUT:
			break;

	    case UPNP_DISCOVERY_ADVERTISEMENT_BYEBYE:
			break;

	    case UPNP_CONTROL_ACTION_COMPLETE:
			break;

    	case UPNP_CONTROL_GET_VAR_COMPLETE:
			break;

    	case UPNP_EVENT_RECEIVED:
			break;

    	case UPNP_EVENT_RENEWAL_COMPLETE:
			break;

    	case UPNP_EVENT_SUBSCRIBE_COMPLETE:
			break;

    	case UPNP_EVENT_UNSUBSCRIBE_COMPLETE:
			break;
	    
    	default:
	    	//// syslog(LOG_DEBUG, "Error in PicsDeviceCallbackEventHandler: unknown event type %d\n", EventType);
    }

    /* Print a summary of the event received */
    //// SampleUtil_PrintEvent(EventType, Event);

    return(0);
}




/********************************************************************************
 * PicsDeviceCommandLoop
 *
 * Description: 
 *       Function that receives commands from the user at the command prompt
 *       during the lifetime of the device, and calls the appropriate
 *       functions for those commands.
 *
 * Parameters:
 *    None
 *
 ********************************************************************************/
void* PicsDeviceCommandLoop(void *args)
{
    int stoploop=0;
    char cmdline[100];
    char cmd[100];
#ifdef USERAPP_NOMMU
    while (!stoploop && (upnp_ICSd_enable == TRUE)) {
#else
    while (!stoploop) {
#endif /* USERAPP_NOMMU */
		sprintf(cmdline, " ");
		sprintf(cmd, " ");

		printf("\n>> ");

		// Get a command line
		fgets(cmdline, 100, stdin);

		sscanf(cmdline, "%s", cmd);

		if (strcasecmp(cmd, "exit") == 0) {
		    printf("Shutting down...\n");
		    UpnpUnRegisterRootDevice(device_handle);
		    UpnpFinish();
		    SampleUtil_Finish(  );
#ifdef USERAPP_NOMMU
		    upnp_ICSd_enable = FALSE;
#else
	#ifdef _UPNP_MM_
		    upnp_mm_destroy();
	#endif
		    exit(0);
#endif /* USERAPP_NOMMU */
		} else {
		    printf("\n   Unknown command: %s\n\n", cmd);
		    printf("   Valid Commands:\n");
		    printf("     Exit\n\n");
		}

    }

    return NULL;
}


int substr(char *docpath, char *infile, char *outfile, char *str_from, char *str_to)
{
	FILE *fpi, *fpo;
	char pathi[256], patho[256];
	char buffi[4096], buffo[4096];
	int len_buff, len_from, len_to;
	int i, j;

	sprintf(pathi, "%s%s", docpath, infile);
	if ((fpi = fopen(pathi,"r")) == NULL) {
		//// printf("input file can not open\n");
		return (-1);
	}

	sprintf(patho, "%s%s", docpath, outfile);
	if ((fpo = fopen(patho,"w")) == NULL) {
		//// printf("output file can not open\n");
		fclose(fpi);
		return (-1);
	}

	len_from = strlen(str_from);
	len_to   = strlen(str_to);

	while (fgets(buffi, 4096, fpi) != NULL) {
		len_buff = strlen(buffi);
		for (i=0, j=0; i <= len_buff-len_from; i++, j++) {
			if (strncmp(buffi+i, str_from, len_from)==0) {
				strcpy (buffo+j, str_to);
				i += len_from - 1;
				j += len_to - 1;
			} else
				*(buffo + j) = *(buffi + i);
		}
		strcpy(buffo + j, buffi + i);
		fputs(buffo, fpo);
	}

	fclose(fpo);
	fclose(fpi);
	return (0);
}


void
linux_print( const char *string )
{
	if(debug_on)		
    	printf( "%s", string );
}

static void
create_pidfile()
{
    FILE *pidfile;
    
    //// syslog(LOG_NOTICE, "pseudoicsd create pidfile %s", PICS_PID_FILE);
    
    if ((pidfile = fopen(PICS_PID_FILE, "w")) != NULL) {
		fprintf(pidfile, "%d\n", getpid());
		(void) fclose(pidfile);
    } else {
		error("Failed to create pid file %s\n", PICS_PID_FILE);
    }
}
#ifdef USERAPP_NOMMU
static void
remove_pid()
{
	FILE *in			= NULL;

	in = fopen(PICS_PID_FILE, "r");
	if (in)
	{
		unlink(PICS_PID_FILE);
	}
}
#endif /* USERAPP_NOMMU */
static void
cleanup_pid()
{
   	FILE *in;
	char pidnumber[32];
	
	in = fopen(PICS_PID_FILE, "r");
	if (in) {
		if (fscanf(in, "%s", pidnumber) == 1) {
		    fclose(in);
		    if (atoi(pidnumber) != getpid()){
				char cmd[64];
				sprintf(cmd,"kill -15 %s",pidnumber);
				system(cmd);
		    }
		} else
			fclose(in);
	}	
}


static void
cleanup_portmap()
{
   	FILE *in;
	char command[255];
	
	in = fopen(PICS_PORTMAP_FILE, "r");
	if (in) {
		 while(fscanf(in, "%s\n", command) != EOF) {
		 	system(command);		    
		};
		fclose(in);
#ifdef USERAPP_NOMMU
		/* chenyl : remove portmap file */
		unlink(PICS_PORTMAP_FILE);
#endif /* USERAPP_NOMMU */
	}
#ifndef USERAPP_NOMMU
	sprintf(command,"rm %s",PICS_PORTMAP_FILE);
	system(command);
#endif /* USERAPP_NOMMU */
}

#ifdef USERAPP_NOMMU
/*
 * sig_handler()
 *
 * handle signals
 * In:       signo - the type of signal that has been recieved.
 *
 */
void sig_handler(int signo)
{
	switch(signo)
	{
		case SIGHUP:
			// re-init ICSd and enable service
			//printf("get SIGHUP: reinit and restart upnp ICSd\n");
			upnp_ICSd_ready = FALSE;
			upnp_ICSd_enable = TRUE;
			break;
	  	case SIGTERM:
			// stop ICSd
			//printf("get SIGTERM: stop upnp ICSd\n");
			upnp_ICSd_enable = FALSE;
			break;
		default:
			// terminate program
			//printf("get %d: terminate upnp ICSd\n", signo);
			PortMap_Destroy(pmap);
			IPCon_Destroy(ipcon);
			UpnpUnRegisterRootDevice(device_handle);
			UpnpFinish();
			SampleUtil_Finish(  );
			upnp_ICSd_enable = FALSE;
			upnp_ICSd_ready = FALSE;
#ifdef _UPNP_MM_
			upnp_mm_destroy();
#endif
			remove_pid();			// remove pid file
			exit(0);
	}
}
#endif /* USERAPP_NOMMU */

#ifdef USERAPP_NOMMU
#define UPNP_MAX_ARGC		5
#endif /* USERAPP_NOMMU */
int main(int argc, char** argv)
{

    int ret=1;
    int port;
    char lan_ip_address[16];
	char *desc_doc_name=NULL, *conf_dir_path=NULL;
    char desc_doc_url[200];
#ifdef USERAPP_NOMMU
	char arg[UPNP_MAX_ARGC][20] = {0};
#endif /* USERAPP_NOMMU */
//    pthread_t cmdloop_thread;
//    int code;
#ifndef USERAPP_NOMMU
    int sig;
    sigset_t sigs_to_catch;
#endif /* USERAPP_NOMMU */

#ifdef _UPNP_MM_
	{
				  //{4, 8, 16, 64, 128, 256, 512, 1024, 2048, 4096}
		//int cnt[10] = {1024, 1024, 512, 1024, 128, 128, 32, 16, 16, 16, 32};
		int cnt[10] = {1024, 1024, 1024, 1024, 128, 128, 32, 16, 16, 32, 16};
		upnp_mm_init(cnt);
	}
#endif

#ifdef USERAPP_NOMMU
	// set status bit
	upnp_ICSd_enable = TRUE;

	/* register signals to handle */

	signal(SIGHUP, sig_handler);		// re-read configurate file
	signal(SIGTERM, sig_handler);		// stop program
	// ignore child-signal
	signal(SIGCHLD, SIG_IGN);
#endif /* USERAPP_NOMMU */

	//// syslog(LOG_INFO, "UPnP IGD (Pseudo ICS daemon) Ver.0.1 by WILDLAB (lamb-info@wildlab.com)");
	//// syslog(LOG_INFO, "UPnP library loaded, Thanks to developper in Intel !");
#ifndef USERAPP_NOMMU
	if(argc == 4 && (strcmp(argv[3],"debug") == 0) ){
		debug_on = TRUE;
	} else if (argc != 3)
	{
		//// syslog(LOG_ERR, "wrong number of arguments\n Usage: %s ifname(internal) ifname(external)\n", argv[0]);
#ifdef _UPNP_MM_
		upnp_mm_destroy();
#endif
		exit(1);
	}else{
		debug_on = FALSE;
	}
#else /* USERAPP_NOMMU */

	if (argc > 2)
	{
#ifdef _UPNP_MM_
		upnp_mm_destroy();
#endif
		exit(1);
	}else if ((argc == 2) && (strncmp(argv[1], "debug", 5) == 0))
	{
		debug_on = TRUE;
	}else
	{
		debug_on = FALSE;
	}

ICSd_restart:

	assert(upnp_ICSd_enable == TRUE);
	assert(upnp_ICSd_ready == FALSE);

	// reset arg
	bzero(arg, sizeof(arg));
	// read configuration file and set arguments
	{
		FILE *cfg_file = NULL;
		int idx = 0;

		cfg_file = fopen("/var/run/pseudoicsd.arg", "r");
		if (!cfg_file)
		{
			printf("uPnP daemon ERR: cannot open configuration file!\n");
			upnp_ICSd_enable = FALSE;
			goto exit_state;
		}

		while (fscanf(cfg_file, "%s", &arg[idx][0]) != EOF)
		{
			idx ++;
			if (idx >= UPNP_MAX_ARGC)
			{
				printf("uPnP daemon WARNING: configuration variables too much: get first %d variables only!\n", UPNP_MAX_ARGC);
				break;
			}
		}
		fclose(cfg_file);

		if (idx < 2)
		{	// argc is not enough
			printf("uPnP daemon ERR: configuration variables set error!\n");
			upnp_ICSd_enable = FALSE;
			goto exit_state;
		}
	}
#endif /* USERAPP_NOMMU */
	startup_time = time(NULL);
	
	SampleUtil_Initialize( linux_print );

#ifdef USERAPP_NOMMU
	ipcon = IPCon_New(arg[0]);
#else
	ipcon = IPCon_New(argv[1]);
#endif /* USERAPP_NOMMU */

	strcpy(lan_ip_address, IPCon_GetIpAddrByStr(ipcon));  
	IPCon_Destroy(ipcon);

	port = PICS_INIT_PORT;
	desc_doc_name=PICS_INIT_DESC_DOC;  
	conf_dir_path=PICS_INIT_CONF_DIR;  

	sprintf(desc_doc_url, "http://%s:%d/%s.xml", lan_ip_address, port, desc_doc_name);

    //// syslog(LOG_DEBUG, "Intializing UPnP \n\twith desc_doc_url=%s\n\t", desc_doc_url);
    //// syslog(LOG_DEBUG, "\t     ipaddress=%s port=%d\n", lan_ip_address, port);
    //// syslog(LOG_DEBUG, "\t     conf_dir_path=%s\n", conf_dir_path);

	// clean pid and port mapping file
#ifndef USERAPP_NOMMU
	cleanup_pid();
#endif /* USERAPP_NOMMU */
	cleanup_portmap();
	create_pidfile();

	// replace ip-addr & port in desc.xml
	substr(conf_dir_path, "picsdesc.skl", "picsdesc.xml", "!ADDR!", lan_ip_address);

	sleep(10);

	if ((ret = UpnpInit(lan_ip_address, port)) != UPNP_E_SUCCESS) {
		//// syslog(LOG_ERR, "Error with UpnpInit -- %d\n", ret);
		UpnpFinish();
		SampleUtil_Finish();
#ifdef USERAPP_NOMMU
		goto exit_state;
#else
	#ifdef _UPNP_MM_
		upnp_mm_destroy();
	#endif
		exit(1);
#endif /* USERAPP_NOMMU */
	}
    //// syslog(LOG_DEBUG, "UPnP Initialized\n");
   
    //// syslog(LOG_DEBUG, "Specifying the webserver root directory -- %s\n", conf_dir_path);
    if ((ret = UpnpSetWebServerRootDir(conf_dir_path)) != UPNP_E_SUCCESS) {
		//// syslog(LOG_ERR, "Error specifying webserver root directory -- %s: %d\n", conf_dir_path, ret);
		UpnpFinish();
		SampleUtil_Finish(  );
#ifdef USERAPP_NOMMU
		goto exit_state;
#else
	#ifdef _UPNP_MM_
		upnp_mm_destroy();
	#endif
		exit(1);
#endif /* USERAPP_NOMMU */
    }
#ifdef USERAPP_NOMMU
	ipcon = IPCon_New(arg[1]);
#else
	ipcon = IPCon_New(argv[2]);
#endif /* USERAPP_NOMMU */
	pmap = PortMap_New();

	/* ftp port */
	PortMap_Add(pmap, "", 6, "", 21,
				"", 21, 0, "FTP", 0);					
	/* telet port */
	PortMap_Add(pmap, "", 6, "", 23,
				"", 23, 0, "TELET", 0);				
	/* smtp port */
	PortMap_Add(pmap, "", 6, "", 25,
				"", 25, 0, "SMTP", 0);					
	/* http port */
	PortMap_Add(pmap, "", 6, "", 80,
				"", 80, 0, "HTTP", 0);
	/* pop3 port */
	PortMap_Add(pmap, "", 6, "", 110,
				"", 110, 0, "POP3", 0);			
	/* imap4 port */
	PortMap_Add(pmap, "", 6, "", 143,
				"", 143, 0, "IMAP4", 0);					
					
    //// syslog(LOG_DEBUG, "Registering the RootDevice\n");

 
    if ((ret = UpnpRegisterRootDevice(desc_doc_url, PicsDeviceCallbackEventHandler, &device_handle, &device_handle)) != UPNP_E_SUCCESS) {
    	//// printf("%s %d\n",__FILE__,__LINE__);
		//// syslog(LOG_ERR, "Error registering the rootdevice : %d\n", ret);	
#ifdef USERAPP_NOMMU
		PortMap_Destroy(pmap);
		IPCon_Destroy(ipcon);
#endif /* USERAPP_NOMMU */
		UpnpFinish();
		SampleUtil_Finish(  );
#ifdef USERAPP_NOMMU
		goto exit_state;
#else
	#ifdef _UPNP_MM_
		upnp_mm_destroy();
	#endif
		exit(1);
#endif /* USERAPP_NOMMU */
    } else {
		//// syslog(LOG_DEBUG, "RootDevice Registered\n");
		//// syslog(LOG_DEBUG, "Initializing State Table\n");
		PicsDeviceStateTableInit(desc_doc_url);
		//// syslog(LOG_DEBUG, "State Table Initialized\n");
		if ((ret = UpnpSendAdvertisement(device_handle, default_advr_expire)) != UPNP_E_SUCCESS) {
		    //// syslog(LOG_ERR, "Error sending advertisements : %d\n", ret);
#ifdef USERAPP_NOMMU
			PortMap_Destroy(pmap);
			IPCon_Destroy(ipcon);
			UpnpUnRegisterRootDevice(device_handle);
#endif /* USERAPP_NOMMU */
		        UpnpFinish();
		        SampleUtil_Finish(  );
#ifdef USERAPP_NOMMU
			goto exit_state;
#else
	#ifdef _UPNP_MM_
			upnp_mm_destroy();
	#endif
		        exit(1);
#endif /* USERAPP_NOMMU */
		}
		//// syslog(LOG_DEBUG, "Advertisements Sent\n");
    }

#ifdef USERAPP_NOMMU
	upnp_ICSd_ready = TRUE;		// ready to get signal and serve
#endif /* USERAPP_NOMMU */
	/* start a command loop thread */
//    code = pthread_create( &cmdloop_thread, NULL, PicsDeviceCommandLoop, NULL );

#ifndef USERAPP_NOMMU
	/* Catch Ctrl-C and properly shutdown */
	sigemptyset(&sigs_to_catch);
	//// sigaddset(&sigs_to_catch, SIGINT);
	sigaddset(&sigs_to_catch, SIGTERM);
	sigwait(&sigs_to_catch, &sig);
	//// syslog(LOG_INFO, "Shutting down on signal %d...\n", sig);
#else
	while ((upnp_ICSd_enable == TRUE) && (upnp_ICSd_ready == TRUE))
	{	// infinite loop
		sleep(1);
	}

#endif /* USERAPP_NOMMU */

	PortMap_Destroy(pmap);
	IPCon_Destroy(ipcon);

	UpnpUnRegisterRootDevice(device_handle);
	UpnpFinish();
	SampleUtil_Finish();
#ifdef USERAPP_NOMMU
	upnp_ICSd_ready = FALSE;

exit_state:

#ifdef _UPNP_MM_
	upnp_mm_reinit();
#endif
	while (upnp_ICSd_enable == FALSE)
	{
		sleep(1);
	}
	// upnp_ICSd_enable == TRUE, re-init
	goto ICSd_restart;

#else

#ifdef _UPNP_MM_
	upnp_mm_destroy();
#endif
 	exit(0);
#endif /* USERAPP_NOMMU */
}

/********************************************************************************
 * PicsDeviceHandleActionRequest
 *
 * Description: 
 *       Called during an action request callback.  If the
 *       request is for this device and either its control service
 *       or picture service, then perform the action and respond.
 *
 * Parameters:
 *   ca_event -- The control action request event structure
 *
 ********************************************************************************/
int PicsDeviceHandleActionRequest(struct Upnp_Action_Request *ca_event)
{
	int result = 0;
	
	pthread_mutex_lock(&PicsDevMutex);
	if (strcmp(ca_event->DevUDN, pics_udn) == 0)
	{
		if (strcmp(ca_event->ServiceID,PicsServiceId[PICS_SERVICE_CONNECT]) ==0)
		{
				if (strcmp(ca_event->ActionName,"SetConnectionType") == 0)
				     	result = GateDeviceSetConnectionType(ca_event);
				else if (strcmp(ca_event->ActionName,"GetConnectionTypeInfo") == 0)
					result = GateDeviceGetConnectionTypeInfo(ca_event);
				else if (strcmp(ca_event->ActionName,"RequestConnection") == 0)
					result = GateDeviceRequestConnection(ca_event);
				else if (strcmp(ca_event->ActionName,"RequestTermination") == 0)
					result = GateDeviceRequestTermination(ca_event);
				else if (strcmp(ca_event->ActionName,"ForceTermination") == 0)
					result = GateDeviceForceTermination(ca_event);
				else if (strcmp(ca_event->ActionName,"SetAutoDisconnectTime") == 0)
					result = GateDeviceSetAutoDisconnectTime(ca_event);
				else if (strcmp(ca_event->ActionName,"SetIdleDisconnectTime") == 0)
					result = GateDeviceSetIdleDisconnectTime(ca_event);
				else if (strcmp(ca_event->ActionName,"SetWarnDisconnectDelay") == 0)
					result = GateDeviceSetWarnDisconnectDelay(ca_event);
				else if (strcmp(ca_event->ActionName,"GetStatusInfo") == 0)
					result = GateDeviceGetStatusInfo(ca_event);
				else if (strcmp(ca_event->ActionName,"GetAutoDisconnectTime") == 0)
					result = GateDeviceGetAutoDisconnectTime(ca_event);
				else if (strcmp(ca_event->ActionName,"GetIdleDisconnectTime") == 0)
					result = GateDeviceGetIdleDisconnectTime(ca_event);
				else if (strcmp(ca_event->ActionName,"GetWarnDisconnectDelay") == 0)
					result = GateDeviceGetWarnDisconnectDelay(ca_event);
				else if (strcmp(ca_event->ActionName,"GetNATRSIPStatus") == 0)
					result = GateDeviceGetNATRSIPStatus(ca_event);
				else if (strcmp(ca_event->ActionName,"GetGenericPortMappingEntry") == 0)
					result = GateDeviceGetGenericPortMappingEntry(ca_event);
				else if (strcmp(ca_event->ActionName,"GetSpecificPortMappingEntry") == 0)
					result = GateDeviceGetSpecificPortMappingEntry(ca_event);
				else if (strcmp(ca_event->ActionName,"GetExternalIPAddress") == 0)
					result = GateDeviceGetExternalIPAddress(ca_event);
				else if (strcmp(ca_event->ActionName,"AddPortMapping") == 0)
					result = GateDeviceAddPortMapping(ca_event);
				else if (strcmp(ca_event->ActionName,"DeletePortMapping") == 0)
					result = GateDeviceDeletePortMapping(ca_event);
				else result = 0;
		}
		else if (strcmp(ca_event->ServiceID,PicsServiceId[PICS_SERVICE_CONFIG]) == 0)
		{
			if (strcmp(ca_event->ActionName,"GetCommonLinkProperties") == 0)
				result = GateDeviceGetCommonLinkProperties(ca_event);
			else if (strcmp(ca_event->ActionName, "X") == 0)
				result = GateDeviceX(ca_event);
			else if (strcmp(ca_event->ActionName, "X_GetICSStatistics") == 0)
				result = GateDeviceX(ca_event);				
			else if (strcmp(ca_event->ActionName,"GetTotalBytesSent") == 0)
				result = GateDeviceGetTotalBytesSent(ca_event);
			else if (strcmp(ca_event->ActionName,"GetTotalBytesReceived") == 0)
				result = GateDeviceGetTotalBytesReceived(ca_event);
			else if (strcmp(ca_event->ActionName,"GetTotalPacketsSent") == 0)
				result = GateDeviceGetTotalPacketsSent(ca_event);
			else if (strcmp(ca_event->ActionName,"GetTotalPacketsReceived") == 0)
				result = GateDeviceGetTotalPacketsReceived(ca_event);
			else result = 0;
		}
	}
	
	pthread_mutex_unlock(&PicsDevMutex);

	return(result);
}

int GateDeviceX(struct Upnp_Action_Request *ca_event)
{
	ca_event->ErrCode = 401;
	strcpy(ca_event->ErrStr, "Invalid Action");
	ca_event->ActionResult = NULL;
	return (ca_event->ErrCode);
}

int GateDeviceGetCommonLinkProperties(struct Upnp_Action_Request *ca_event)
{
	char result_str[500];

        ca_event->ErrCode = UPNP_E_SUCCESS;
        sprintf(result_str, "<u:%sResponse xmlns:u=\"%s\">\n%s\n</u:%sResponse>", ca_event->ActionName,
                "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1",
                "<NewWANAccessType>Cable</NewWANAccessType><NewLayer1UpstreamMaxBitRate>131072</NewLayer1UpstreamMaxBitRate><NewLayer1DownstreamMaxBitRate>614400</NewLayer1DownstreamMaxBitRate><NewPhysicalLinkStatus>Up</NewPhysicalLinkStatus>",
                ca_event->ActionName);
        ca_event->ActionResult = ixmlParseBuffer(result_str);

        return(ca_event->ErrCode);


}

int GateDeviceGetTotalBytesSent(struct Upnp_Action_Request *ca_event)
{
	char result_str[500];
	char dev[15];
	char *iface;
	FILE *stream;
	unsigned long bytes=0, total=0;

	/* Read sent from /proc */
	stream = fopen ( "/proc/net/dev", "r" );
	if ( stream != NULL )
	{
		iface=ipcon->ifname;
		while ( getc ( stream ) != '\n' );
		while ( getc ( stream ) != '\n' );

		while ( !feof( stream ) )
		{
			fscanf ( stream, "%[^:]:%*u %*u %*u %*u %*u %*u %*u %*u %lu %*u %*u %*u %*u %*u %*u %*u\n", dev, &bytes );
			if ( strcmp ( dev, iface )==0 )
				total += bytes;
		}
		fclose ( stream );
	}
	else
	{
		total=1;
	}

        ca_event->ErrCode = UPNP_E_SUCCESS;
        sprintf(result_str, "<u:%sResponse xmlns:u=\"%s\">\n<NewTotalBytesSent>%lu</NewTotalBytesSent>\n</u:%sResponse>",
		ca_event->ActionName,
                "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1",
                total, ca_event->ActionName);
        ca_event->ActionResult = ixmlParseBuffer(result_str);

        return(ca_event->ErrCode);

}
int GateDeviceGetTotalBytesReceived(struct Upnp_Action_Request *ca_event)
{
	char result_str[500];
	char dev[15];
        char *iface;
        FILE *stream;
        unsigned long bytes=0,total=0;

	/* Read received from /proc */
        stream = fopen ( "/proc/net/dev", "r" );
        if ( stream != NULL )
        {
                iface=ipcon->ifname;

                while ( getc ( stream ) != '\n' );
                while ( getc ( stream ) != '\n' );

                while ( !feof( stream ) )
                {
                        fscanf ( stream, "%[^:]:%lu %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u\n", dev, &bytes );
                        if ( strcmp ( dev, iface )==0 )
                                total += bytes;
                }
		fclose ( stream );
        }
	else
	{
                total=1;
        }

        ca_event->ErrCode = UPNP_E_SUCCESS;
        sprintf(result_str, "<u:%sResponse xmlns:u=\"%s\">\n<NewTotalBytesReceived>%lu</NewTotalBytesReceived>\n</u:%sResponse>",
		ca_event->ActionName,
                "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1",
                total, ca_event->ActionName );
        ca_event->ActionResult = ixmlParseBuffer(result_str);

        return(ca_event->ErrCode);

}
int GateDeviceGetTotalPacketsSent(struct Upnp_Action_Request *ca_event)
{
	char result_str[500];
	char dev[15];
        char *iface;
        FILE *stream;
        unsigned long pkt=0, total=0;

        /* Read sent from /proc */
        stream = fopen ( "/proc/net/dev", "r" );
        if ( stream != NULL )
        {
                iface=ipcon->ifname;
                while ( getc ( stream ) != '\n' );
                while ( getc ( stream ) != '\n' );

                while ( !feof( stream ) )
                {
                        fscanf ( stream, "%[^:]:%*u %*u %*u %*u %*u %*u %*u %*u %*u %lu %*u %*u %*u %*u %*u %*u\n", dev, &pkt );
                        if ( strcmp ( dev, iface )==0 )
                                total += pkt;
		}
                fclose ( stream );
        }
        else
        {
                total=1;
        }

        ca_event->ErrCode = UPNP_E_SUCCESS;
        sprintf(result_str, "<u:%sResponse xmlns:u=\"%s\">\n<NewTotalPacketsSent>%lu</NewTotalPacketsSent>\n</u:%sResponse>", ca_event->ActionName,
                "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1",
                total, ca_event->ActionName);
        ca_event->ActionResult = ixmlParseBuffer(result_str);

        return(ca_event->ErrCode);

}
int GateDeviceGetTotalPacketsReceived(struct Upnp_Action_Request *ca_event)
{
	char result_str[500];
	char dev[15];
        char *iface;
        FILE *stream;
        unsigned long pkt=0, total=0;

        /* Read sent from /proc */
        stream = fopen ( "/proc/net/dev", "r" );
        if ( stream != NULL )
        {
                iface=ipcon->ifname;
                while ( getc ( stream ) != '\n' );
                while ( getc ( stream ) != '\n' );

                while ( !feof( stream ) )
                {
                        fscanf ( stream, "%[^:]:%*u %lu %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u\n", dev, &pkt );
                        if ( strcmp ( dev, iface )==0 )
                                total += pkt;
                }
                fclose ( stream );
        }
        else
	{
                total=1;
        }

        ca_event->ErrCode = UPNP_E_SUCCESS;
        sprintf(result_str, "<u:%sResponse xmlns:u=\"%s\">\n<NewTotalPacketsReceived>%lu</NewTotalPacketsReceived>\n</u:%sResponse>", ca_event->ActionName,
                "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1",
                total, ca_event->ActionName);
        ca_event->ActionResult = ixmlParseBuffer(result_str);

        return(ca_event->ErrCode);

}

int GateDeviceSetConnectionType(struct Upnp_Action_Request *ca_event)
{
	return(UPNP_E_SUCCESS);
}

int GateDeviceGetConnectionTypeInfo(struct Upnp_Action_Request *ca_event)
{
	char result_str[500];

	ca_event->ErrCode = UPNP_E_SUCCESS;
	sprintf(result_str, "<u:%sResponse xmlns:u=\"%s\">\n%s\n</u:%sResponse>", ca_event->ActionName,
		"urn:schemas-upnp-org:service:WANIPConnection:1",
		"<NewConnectionType>IP_Routed</NewConnectionType>\n<NewPossibleConnectionTypes>IP_Routed</NewPossibleConnectionTypes>",
		ca_event->ActionName);
	ca_event->ActionResult = ixmlParseBuffer(result_str);

	return(ca_event->ErrCode);
}

int GateDeviceRequestConnection(struct Upnp_Action_Request *ca_event)
{
	return(UPNP_E_SUCCESS);
}

int GateDeviceRequestTermination(struct Upnp_Action_Request *ca_event)
{
	return(UPNP_E_SUCCESS);
}

int GateDeviceForceTermination(struct Upnp_Action_Request *ca_event)
{
	return(UPNP_E_SUCCESS);
}

int GateDeviceSetAutoDisconnectTime(struct Upnp_Action_Request *ca_event)
{
	return(UPNP_E_SUCCESS);
}

int GateDeviceSetIdleDisconnectTime(struct Upnp_Action_Request *ca_event)
{
	return(UPNP_E_SUCCESS);
}

int GateDeviceSetWarnDisconnectDelay(struct Upnp_Action_Request *ca_event)
{
	return(UPNP_E_SUCCESS);
}

int GateDeviceGetStatusInfo(struct Upnp_Action_Request *ca_event)
{

	long int uptime;
	char result_str[500];
        
	uptime = (time(NULL) - startup_time);
	ca_event->ErrCode = UPNP_E_SUCCESS;
        sprintf(result_str, "<u:%sResponse xmlns:u=\"%s\">\n<NewConnectionStatus>%s</NewConnectionStatus><NewLastConnectionError>ERROR_NONE</NewLastConnectionError><NewUptime>%li</NewUptime>\n</u:%sResponse>", ca_event->ActionName,
	                "urn:schemas-upnp-org:service:WANIPConnection:1",
	                "Connected",uptime,
	                ca_event->ActionName);
        ca_event->ActionResult = ixmlParseBuffer(result_str);
        return(ca_event->ErrCode);
}

int GateDeviceGetAutoDisconnectTime(struct Upnp_Action_Request *ca_event)
{
	return(UPNP_E_SUCCESS);
}

int GateDeviceGetIdleDisconnectTime(struct Upnp_Action_Request *ca_event)
{
	return(UPNP_E_SUCCESS);
}

int GateDeviceGetWarnDisconnectDelay(struct Upnp_Action_Request *ca_event)
{
	return(UPNP_E_SUCCESS);
}

int GateDeviceGetNATRSIPStatus(struct Upnp_Action_Request *ca_event)
{
	char result_str[500];

	ca_event->ErrCode = UPNP_E_SUCCESS;
	sprintf(result_str, "<u:%sResponse xmlns:u=\"%s\">\n%s\n</u:%sResponse>", ca_event->ActionName,
		"urn:schemas-upnp-org:service:WANIPConnection:1",
		"<NewRSIPAvailable>0</NewRSIPAvailable>\n<NewNATEnabled>1</NewNATEnabled>",
		ca_event->ActionName);
	ca_event->ActionResult = ixmlParseBuffer(result_str);

	return(ca_event->ErrCode);
}

int GateDeviceGetExternalIPAddress(struct Upnp_Action_Request *ca_event)
{
	char result_str[500];
	char result_parm[500];
	char ip_address[16];

	strcpy(ip_address, IPCon_GetIpAddrByStr(ipcon));  

	ca_event->ErrCode = UPNP_E_SUCCESS;
	sprintf(result_parm,"<NewExternalIPAddress>%s</NewExternalIPAddress>", ip_address);
	sprintf(result_str, "<u:%sResponse xmlns:u=\"%s\">\n%s\n</u:%sResponse>", ca_event->ActionName,
		"urn:schemas-upnp-org:service:WANIPConnection:1",result_parm, ca_event->ActionName);
	ca_event->ActionResult = ixmlParseBuffer(result_str);
	
	return(ca_event->ErrCode);
}

int GateDeviceGetGenericPortMappingEntry(struct Upnp_Action_Request *ca_event)
{
	int i = 0;
	int index = 0;
	char *mapindex = NULL;
	int action_succeeded = 0;
	char result_parm[500];
	char result_str[500];
	
	ObjListData data;
	PortMapItem pitem=NULL;	

	if ((mapindex = SampleUtil_GetFirstDocumentItem(ca_event->ActionRequest, "NewPortMappingIndex")))
	{
		index = atoi(mapindex);
		data = ObjList_GetStartEdge((ObjList)pmap);
		for (i=0; i < ObjList_GetLength(pmap); i++)
		{
			data = ObjListData_GetNext(data);
			pitem = (PortMapItem)ObjListData_GetObj(data);			
			
			if (i == index)
			{
				char *strp1;
							
				if(pitem->Protocol == 17)
					strp1 = "UDP";
				else
					strp1 = "TCP";

				if(pitem->Enabled)
					pitem->Enabled = 1;
				else
					pitem->Enabled = 0;								
														
				sprintf(result_parm,"<NewRemoteHost>%s</NewRemoteHost><NewExternalPort>%d</NewExternalPort><NewProtocol>%s</NewProtocol><NewInternalPort>%d</NewInternalPort><NewInternalClient>%s</NewInternalClient><NewEnabled>%d</NewEnabled><NewPortMappingDescription>%s</NewPortMappingDescription><NewLeaseDuration>%u</NewLeaseDuration>",
						"",pitem->ExternalPort, strp1, pitem->InternalPort,pitem->InternalIP, pitem->Enabled, pitem->PortMappingDescription, pitem->LeaseDuration);
				action_succeeded = 1;
				break;
			}
		}
		if (action_succeeded)
		{
			ca_event->ErrCode = UPNP_E_SUCCESS;
	                sprintf(result_str, "<u:%sResponse xmlns:u=\"%s\">\n%s\n</u:%sResponse>", ca_event->ActionName,
	                        "urn:schemas-upnp-org:service:WANIPConnection:1",result_parm, ca_event->ActionName);
	                ca_event->ActionResult = ixmlParseBuffer(result_str);
		}
		else
		{
			ca_event->ErrCode = 713;
                        strcpy(ca_event->ErrStr, "SpecifiedArrayIndexInvalid");
                        ca_event->ActionResult = NULL;
		}

	}
	else
	{
	         //// syslog(LOG_DEBUG, "Failure in GateDeviceGetGenericortMappingEntry: Invalid Args");
	         ca_event->ErrCode = 402;
                 strcpy(ca_event->ErrStr, "Invalid Args");
                 ca_event->ActionResult = NULL;
	}

#ifdef _UPNP_MM_
	if (mapindex) upnp_free(mapindex);
#else
	if (mapindex) free(mapindex);
#endif
	return(ca_event->ErrCode);
}

int GateDeviceGetSpecificPortMappingEntry(struct Upnp_Action_Request *ca_event)
{
	char *ext_port=NULL;
	char *proto=NULL;
	char result_parm[500];
	char result_str[500];
	int prt;
	int action_succeeded = 0;
	
	ObjListData data;
	PortMapItem pitem=NULL;
	int i;					

	if ((ext_port = SampleUtil_GetFirstDocumentItem(ca_event->ActionRequest, "NewExternalPort")) 
		&& (proto = SampleUtil_GetFirstDocumentItem(ca_event->ActionRequest,"NewProtocol")))
	{
		data = ObjList_GetStartEdge((ObjList)pmap);
		prt = getProtoNum(proto);
		if ((prt == 17) || (prt == 6))
		{
			for (i=0; i < ObjList_GetLength(pmap); i++)
			{
				data = ObjListData_GetNext(data);
				pitem = (PortMapItem)ObjListData_GetObj(data);
				
							
				if (((pitem->ExternalPort == atoi(ext_port)) && (pitem->Protocol == prt )))
				{
					sprintf(result_parm,"<NewInternalPort>%d</NewInternalPort><NewInternalClient>%s</NewInternalClient><NewEnabled>%d</NewEnabled><NewPortMappingDescription>%s</NewPortMappingDescription><NewLeaseDuration>%u</NewLeaseDuration>",
					pitem->InternalPort, 
					pitem->InternalIP,
					pitem->Enabled,
					pitem->PortMappingDescription,
					pitem->LeaseDuration);
					action_succeeded = 1;
					break;
				}
			}
			if (action_succeeded)
			{
				ca_event->ErrCode = UPNP_E_SUCCESS;
		                sprintf(result_str, "<u:%sResponse xmlns:u=\"%s\">\n%s\n</u:%sResponse>", ca_event->ActionName,
		                        "urn:schemas-upnp-org:service:WANIPConnection:1",result_parm, ca_event->ActionName);
		                ca_event->ActionResult = ixmlParseBuffer(result_str);
			}
			else
			{
				//// syslog(LOG_DEBUG, "Failure in GateDeviceGetSpecificPortMappingEntry: PortMapping Doesn't Exist...");
		                ca_event->ErrCode = 714;
		                strcpy(ca_event->ErrStr, "NoSuchEntryInArray");
		                ca_event->ActionResult = NULL;
			}
		}
		else
		{
		        //// syslog(LOG_DEBUG, "Failure in GateDeviceGetSpecificPortMappingEntry: Invalid NewProtocol=%s\n",proto);
                        ca_event->ErrCode = 402;
                        strcpy(ca_event->ErrStr, "Invalid Args");
                        ca_event->ActionResult = NULL;
		}
	}
	else
	{
		//// syslog(LOG_DEBUG, "Failure in GateDeviceGetSpecificPortMappingEntry: Invalid Args");
		ca_event->ErrCode = 402;
		strcpy(ca_event->ErrStr, "Invalid Args");
		ca_event->ActionResult = NULL;
	}
	
	return (ca_event->ErrCode);
}

int GateDeviceAddPortMapping(struct Upnp_Action_Request *ca_event)
{
	char *remote_host=NULL;
	char *ext_port=NULL;
	char *proto=NULL; 
	char *int_port=NULL;
	char *int_ip=NULL;
	char *desc=NULL;
	char *enabled=NULL;	
	int prt,result=0;
	char num[5];
	char result_str[500];
	char address[16];
	IXML_Document *PropSet= NULL;	
	int action_succeeded = 0;
	
	strcpy(address, IPCon_GetIpAddrByStr(ipcon));  		
		
	if (((ext_port = SampleUtil_GetFirstDocumentItem(ca_event->ActionRequest, "NewExternalPort"))
		&& (proto    = SampleUtil_GetFirstDocumentItem(ca_event->ActionRequest,"NewProtocol"))
		&& (int_port = SampleUtil_GetFirstDocumentItem(ca_event->ActionRequest, "NewInternalPort"))
		&& (int_ip   = SampleUtil_GetFirstDocumentItem(ca_event->ActionRequest, "NewInternalClient"))
		&& (enabled   = SampleUtil_GetFirstDocumentItem(ca_event->ActionRequest, "NewEnabled"))
		&& (desc     = SampleUtil_GetFirstDocumentItem(ca_event->ActionRequest, "NewPortMappingDescription"))))
	{
		prt = getProtoNum(proto);
		if ((prt == 6) || (prt ==17) )
		{
				
			if (chkIPADDRstring(int_ip)!=0)
			{
			
				result=PortMap_Add(pmap, NULL, prt, IPCon_GetIpAddrByStr(ipcon), atoi(ext_port),
										int_ip,	atoi(int_port), atoi(enabled), desc, 0);
				if (result==1)
				{
					sprintf(num,"%d",ObjList_GetLength(pmap));
					PropSet= UpnpCreatePropertySet(1,"PortMappingNumberOfEntries", num);
					UpnpNotifyExt(device_handle, ca_event->DevUDN,ca_event->ServiceID,PropSet);
					ixmlDocument_free(PropSet);
					//// syslog(LOG_DEBUG, "AddPortMap: RemoteHost: %s Prot: %d ExtPort: %d Int: %s.%d\n", remote_host, prt, atoi(ext_port), int_ip, atoi(int_port));
					action_succeeded = 1;
					
				}
				else
				{
					if (result==718)
					{
						//// syslog(LOG_DEBUG,"Failure in GateDeviceAddPortMapping: RemoteHost: %s Prot:%d ExtPort: %d Int: %s.%d\n",remote_host,prt, atoi(ext_port),int_ip, atoi(int_port));
						ca_event->ErrCode = 718;
						strcpy(ca_event->ErrStr, "ConflictInMappingEntry");
						ca_event->ActionResult = NULL;
					}
				}
			}
			else
			{
		                 //// syslog(LOG_DEBUG, "Failure in GateDeviceAddPortMapping: Invalid NewInternalClient=%s\n",int_ip);
                                 ca_event->ErrCode = 402;
                                 strcpy(ca_event->ErrStr, "Invalid Args");
                                 ca_event->ActionResult = NULL;
			}
		}
		else
		{
		      //// syslog(LOG_DEBUG, "Failure in GateDeviceAddPortMapping: Invalid NewProtocol=%s\n",proto);
                      ca_event->ErrCode = 402;
                      strcpy(ca_event->ErrStr, "Invalid Args");
                      ca_event->ActionResult = NULL;
		}
	}
	else
	{
			
		//// syslog(LOG_DEBUG, "Failiure in GateDeviceAddPortMapping: Invalid Arguments!");
                ca_event->ErrCode = 402;
                strcpy(ca_event->ErrStr, "Invalid Args");
                ca_event->ActionResult = NULL;
	}
	
	if (action_succeeded)
	{

		ca_event->ErrCode = UPNP_E_SUCCESS;
		sprintf(result_str, "<u:%sResponse xmlns:u=\"%s\">\n%s\n</u:%sResponse>",
			ca_event->ActionName, "urn:schemas-upnp-org:service:WANIPConnection:1", "", ca_event->ActionName);
		ca_event->ActionResult = ixmlParseBuffer(result_str);
	}
	
#ifdef _UPNP_MM_
    if (ext_port)	upnp_free(ext_port);
    if (int_port)	upnp_free(int_port);
    if (proto)		upnp_free(proto);
    if (int_ip)		upnp_free(int_ip);
    if (desc)		upnp_free(desc);
    if (enabled)	upnp_free(enabled);
#else
    if (ext_port)	free(ext_port);
    if (int_port)	free(int_port);
    if (proto)		free(proto);
    if (int_ip)		free(int_ip);
    if (desc)		free(desc);
    if (enabled)	free(enabled);
#endif

	return(ca_event->ErrCode);
}

int GateDeviceDeletePortMapping(struct Upnp_Action_Request *ca_event)
{
	char *ext_port=NULL;
	char *proto=NULL;
	int prt,result=0;
	char num[5];
	char result_str[500];
	IXML_Document *PropSet= NULL;
	int action_succeeded = 0;

	if (((ext_port = SampleUtil_GetFirstDocumentItem(ca_event->ActionRequest, "NewExternalPort")) &&
		(proto = SampleUtil_GetFirstDocumentItem(ca_event->ActionRequest, "NewProtocol"))))
	{

		prt = getProtoNum(proto);
		if ((prt == 6) || (prt == 17))
		{			
			result = PortMap_Delete(pmap, prt, atoi(ext_port));
			if (result==1)
			{
				//// syslog(LOG_DEBUG, "DeletePortMap: Proto:%s Port:%s\n",proto, ext_port);
				sprintf(num,"%d",ObjList_GetLength(pmap));
				PropSet= UpnpCreatePropertySet(1,"PortMappingNumberOfEntries", num);
				UpnpNotifyExt(device_handle, ca_event->DevUDN,ca_event->ServiceID,PropSet);
				ixmlDocument_free(PropSet);
				action_succeeded = 1;
			}
			else
			{
				//// syslog(LOG_DEBUG, "Failure in GateDeviceDeletePortMapping: DeletePortMap: Proto:%s Port:%s\n",proto, ext_port);
				ca_event->ErrCode = 714;
				strcpy(ca_event->ErrStr, "NoSuchEntryInArray");
				ca_event->ActionResult = NULL;
			}
		}
		else
		{
			//// syslog(LOG_DEBUG, "Failure in GateDeviceDeletePortMapping: Invalid NewProtocol=%s\n",proto);
			ca_event->ErrCode = 402;
                        strcpy(ca_event->ErrStr, "Invalid Args");
                        ca_event->ActionResult = NULL;
		}
	}
	else
	{
	        //// syslog(LOG_DEBUG, "Failiure in GateDeviceDeletePortMapping: Invalid Arguments!");
       	        ca_event->ErrCode = 402;
       	        strcpy(ca_event->ErrStr, "Invalid Args");
	        ca_event->ActionResult = NULL;
	}

	if (action_succeeded)
	{
		ca_event->ErrCode = UPNP_E_SUCCESS;
		sprintf(result_str, "<u:%sResponse xmlns:u=\"%s\">\n%s\n</u:%sResponse>",
			ca_event->ActionName, "urn:schemas-upnp-org:service:WANIPConnection:1", "", ca_event->ActionName);
		ca_event->ActionResult = ixmlParseBuffer(result_str);
	}

#ifdef _UPNP_MM_
	if (ext_port)upnp_free(ext_port);
	if (proto) upnp_free(proto);
#else
	if (ext_port)free(ext_port);
	if (proto) free(proto);
#endif
	return(ca_event->ErrCode);
}

