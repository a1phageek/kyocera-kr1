#include "common.h"

#define RTL865X_ONLY 1

#include "PortMapP.h"
#include "PseudoICSd.h"

#ifdef RTL865X_ONLY
#include <re865x.h>
#endif /* RTL865X_ONLY */

#ifdef _UPNP_MM_
#include "upnp_mm.h"
#endif

PortMapItem PortMapItem_Destroy(PortMapItem pitem);
int flushPacketFilter();
int addPortForward(int Proto, char *ExtIP, int ExtPort, char *IntIP, int IntPort, int Enabled, char *Desc);
int delPortForward(int Proto, char *ExtIP, int ExtPort, char *IntIP, int IntPort);
ObjListData findMap(PortMap pmap, int Proto, int ExtPort);

#ifdef RTL865X_ONLY
/*******************************/
/* RTL8651 specific function   */
static int device_ioctl(char *name,unsigned long arg0, unsigned long arg1, unsigned long arg2, unsigned long arg3)
{
	unsigned long args[4];
	struct ifreq ifr;
	int sockfd;

	args[0] = arg0;
	args[1] = arg1;
	args[2] = arg2;
	args[3] = arg3;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		return -3;
	}
	//memset((char*)&ifr,0,sizeof(ifr));
	strcpy((char*)&ifr.ifr_name, name);
	((unsigned long *)(&ifr.ifr_data))[0] = (unsigned long)args;
	if (ioctl(sockfd, SIOCDEVPRIVATE, &ifr)<0)
	{
		perror("device ioctl:");
		close(sockfd);
		return -1;
	}
	close(sockfd);
	return 0;
}

static int rtl8651_addNaptUpnpPortMapping(unsigned char isTcp, unsigned int extIpAddr, unsigned short extPort, unsigned int intIpAddr, unsigned short intPort)
{
	unsigned long ret;
	unsigned int ptr[5];	
	ptr[0]=&isTcp;
	ptr[1]=&extIpAddr;
	ptr[2]=&extPort;
	ptr[3]=&intIpAddr;
	ptr[4]=&intPort;
	device_ioctl("eth0",RTL8651_ADDNAPTUPNPPORTMAPPING,(unsigned int)(ptr),0,&ret);
	return ret;	
}

static int rtl8651_delNaptUpnpPortMapping(unsigned char isTcp, unsigned int extIpAddr, unsigned short extPort, unsigned int intIpAddr, unsigned short intPort)
{
	unsigned long ret;
	unsigned int ptr[5];	
	ptr[0]=(unsigned int)&isTcp;
	ptr[1]=(unsigned int)&extIpAddr;
	ptr[2]=(unsigned int)&extPort;
	ptr[3]=(unsigned int)&intIpAddr;
	ptr[4]=(unsigned int)&intPort;
	device_ioctl("eth0",RTL8651_DELNAPTUPNPPORTMAPPING,(unsigned int)(ptr),0,&ret);
	return ret;		
}

#endif /* RTL865X_ONLY */ 

PortMap PortMap_New()
{
	PortMap pmap;

	pmap = ObjList_Create();
	if (pmap == NULL) return (NULL);

	flushPacketFilter();

	return (pmap);
}


PortMap PortMap_Destroy(PortMap pmap)
{
	if (pmap == NULL) return (NULL);

	ObjList_Destroy(pmap);

	flushPacketFilter();

	return (NULL);
}


int PortMap_Add(PortMap pmap, char *RemoteHost, int Proto, char *ExtIP, int ExtPort,
				char *IntIP, int IntPort, int Enabled, char *Desc, int LeaseDuration) {

	PortMapItem pitem;	

	/* ALL CHECK CLEARED !! */

#ifdef _UPNP_MM_
	pitem = (PortMapItem)upnp_calloc(1,sizeof(_PortMapItem));
#else
	pitem = (PortMapItem)calloc(1,sizeof(_PortMapItem));
#endif
	if (pitem == NULL) {
		return (-104); // OUT_OF_MEMORY
	}

	if (RemoteHost != NULL) {
#ifdef _UPNP_MM_
		pitem->RemoteHost = upnp_malloc(sizeof(char) * (strlen(RemoteHost)+1));
#else
		pitem->RemoteHost = malloc(sizeof(char) * (strlen(RemoteHost)+1));
#endif
		strcpy(pitem->RemoteHost, RemoteHost);
	} else pitem->RemoteHost = NULL;

	pitem->Protocol = Proto;

	if (ExtIP != NULL) {
#ifdef _UPNP_MM_
		pitem->ExternalIP = upnp_malloc(sizeof(char) * (strlen(ExtIP)+1));
#else
		pitem->ExternalIP = malloc(sizeof(char) * (strlen(ExtIP)+1));
#endif
		strcpy(pitem->ExternalIP, ExtIP);
	} else pitem->ExternalIP = NULL;

	pitem->ExternalPort = ExtPort;

	if (IntIP != NULL) {
#ifdef _UPNP_MM_
		pitem->InternalIP = upnp_malloc(sizeof(char) * (strlen(IntIP)+1));
#else
		pitem->InternalIP = malloc(sizeof(char) * (strlen(IntIP)+1));
#endif
		strcpy(pitem->InternalIP, IntIP);
	} else pitem->InternalIP = NULL;

	pitem->InternalPort = IntPort;

	pitem->Enabled = Enabled;

	if (Desc != NULL) {
#ifdef _UPNP_MM_
		pitem->PortMappingDescription = upnp_malloc(sizeof(char) * (strlen(Desc)+1));
#else
		pitem->PortMappingDescription = malloc(sizeof(char) * (strlen(Desc)+1));
#endif
		strcpy(pitem->PortMappingDescription, Desc);
	} else pitem->PortMappingDescription = NULL;

	pitem->LeaseDuration = LeaseDuration;

	/* delete old entry first */
	PortMap_Delete(pmap,Proto,ExtPort);

	ObjList_InsertObjToEnd(pmap, pitem, (ObjDestructor)PortMapItem_Destroy);

	addPortForward(Proto, ExtIP, ExtPort, IntIP, IntPort, Enabled, Desc);

	return (1);
}


int PortMap_Delete(PortMap pmap, int Proto, int ExtPort) {

	ObjListData data;
	
	data = findMap(pmap, Proto, ExtPort);

	if (data != NULL) {
	
		ObjList_DeleteObjToNext((ObjList)pmap, data);
		
		return (1);
	}	

	return (0);
}


PortMapItem PortMapItem_Destroy(PortMapItem pitem)
{

	if (pitem == NULL) return (NULL);

	delPortForward(pitem->Protocol, pitem->ExternalIP, pitem->ExternalPort, pitem->InternalIP, pitem->InternalPort);

	return (NULL);
}


int doCommand(char *cmd) {
	system(cmd);	
	return (1);
}

int flushPacketFilter() 
{
    return(1);
}


int addPortForward(int Proto, char *ExtIP, int ExtPort, char *IntIP, int IntPort, int Enabled, char *Desc) 
{

    char command[255];
	char prt[4];
	FILE *portfile;
	unsigned char isTCP = 0;
	struct in_addr extIp, intIp;

	if (Proto==6) {
		strcpy(prt, "tcp");
		isTCP = 1;
	}
	else {
		strcpy(prt, "udp");
	}
	
	if(Enabled){
#ifndef RTL865X_ONLY		
		sprintf(command,"/sbin/iptables -t nat -A PREROUTING -p %s -d %s --dport %d -j DNAT --to %s:%d"
					, prt, ExtIP, ExtPort, IntIP, IntPort);					
	    doCommand(command);
#else		
		/* add to ASIC */	
		inet_aton(ExtIP, &extIp); 	
		inet_aton(IntIP, &intIp); 	
		rtl8651_addNaptUpnpPortMapping( isTCP, extIp.s_addr, ExtPort, intIp.s_addr, IntPort);
#endif /* RTL865X_ONLY */		
    
	    /* keep port open file */
    	//// syslog(LOG_NOTICE, "pseudoicsd create pidfile %s", PICS_PORTMAP_FILE);
    
    	if ((portfile = fopen(PICS_PORTMAP_FILE, "a")) != NULL) {
    		
			sprintf(command,"/sbin/iptables -t nat -D PREROUTING -p %s -d %s --dport %d -j DNAT --to %s:%d"
					, prt, ExtIP, ExtPort, IntIP, IntPort);		
    		
			fprintf(portfile, "%s\n", command);
			(void) fclose(portfile);
    	} else {
			//// syslog(LOG_NOTICE,"Failed to create pid file %s\n", PICS_PORTMAP_FILE);
    	}	    
   	
	}

    return 1;
}


int delPortForward(int Proto, char *ExtIP, int ExtPort, char *IntIP, int IntPort) {

    char command[255];
	char prt[4];
	unsigned char isTCP = 0;
	struct in_addr extIp, intIp;

	if (Proto==6) {
		strcpy(prt, "tcp");
		isTCP = 1;
	}
	else {
		strcpy(prt, "udp");
	}
#ifndef RTL865X_ONLY		
	sprintf(command, "/sbin/iptables -t nat -D PREROUTING -p %s -d %s --dport %d -j DNAT --to %s:%d"
				,prt, ExtIP, ExtPort, IntIP, IntPort);

	doCommand(command);
#else	
	/* remove from ASIC */	
	inet_aton(ExtIP, &extIp); 	
	inet_aton(IntIP, &intIp); 	

 	rtl8651_delNaptUpnpPortMapping( isTCP, extIp.s_addr, ExtPort, intIp.s_addr, IntPort);	
#endif /* RTL865X_ONLY */
    return 1;
}


ObjListData findMap(PortMap pmap, int Proto, int ExtPort) {

	ObjListData data;
	PortMapItem pitem=NULL;
	int i;

	data = ObjList_GetStartEdge((ObjList)pmap);

	for( i=0; i < ObjList_GetLength(pmap); i++) {
		data = ObjListData_GetNext(data);
		pitem = (PortMapItem)ObjListData_GetObj(data);
		if ((pitem->ExternalPort == ExtPort) && (pitem->Protocol == Proto)) {
			return (data);
		}	
	}

	return (NULL);

}
