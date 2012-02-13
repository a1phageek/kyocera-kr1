#include "common.h"
#include "IPConP.h"

#ifdef _UPNP_MM_
#include "upnp_mm.h"
#endif

/*****		*****/





/*****		*****/

int get_sockfd()
{
	static int sockfd = -1;

	if (sockfd == -1) {
		if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) == -1) {
			perror("user: socket creation failed");
			return(-1);
		}
	}
	return sockfd;
}

IPCon IPCon_New(char * ifname)
{
	IPCon ipcon;
#ifdef _UPNP_MM_
	ipcon = (IPCon)upnp_malloc(sizeof(_IPCon));
#else
	ipcon = (IPCon)malloc(sizeof(_IPCon));
#endif
	if (!ipcon) printf("Error in IPCon_New:Cannot allocate memory\n");

	/* WAN側デバイス名初期化 */
	ipcon->ifname = ifname;
	return (ipcon);
}


IPCon IPCon_Destroy(IPCon this)
{
	if (!this) return (NULL);

#ifdef _UPNP_MM_
	upnp_free(this);
#else
	free(this);
#endif
	return (NULL);
}


/*****		*****/

int IPCon_SetIfName(IPCon this, char * ifname)
{
	this->ifname = ifname;
	return (0);
}


char * IPCon_GetIfName(IPCon this)
{
	return (this->ifname);
}


/*****		*****/

struct in_addr * IPCon_GetIpAddr(IPCon this)
{
    static struct ifreq ifr;
	struct sockaddr_in *saddr;
    int fd;

    fd = get_sockfd();
    if (fd >= 0) {
	    strcpy(ifr.ifr_name, this->ifname);
		ifr.ifr_addr.sa_family = AF_INET;
		if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) {
			saddr = &ifr.ifr_addr;
			return &saddr->sin_addr;
		} else {
			return NULL;
		}
		close(fd);
	}
	return NULL;
}


char * IPCon_GetIpAddrByStr(IPCon this)
{
	struct in_addr *adr;

	adr = IPCon_GetIpAddr(this);
	if (adr == NULL) {
		return NULL;
	} else {
		return inet_ntoa(*adr);
	}
}



int IPCon_IsIfUp(IPCon this)	// UP=0
{
    struct ifreq ifr;
    int fd;

    fd = get_sockfd();
    if (fd >= 0) {
	    strcpy(ifr.ifr_name, this->ifname);
		ifr.ifr_addr.sa_family = AF_INET;

	    if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0)
			return (-1);

	    if (ifr.ifr_flags & IFF_UP) {
			return (1);
		} else {
			return (0);
		}
		close(fd);
	}
	return (-1);
}

char * IPCon_GetIfStatByStr(IPCon this)
{
	if (IPCon_IsIfUp(this))
		return ("UP");
	else
		return (NULL);
}
