/////////////////////////////////////////////////////////////////////////////
/*
    no-ip.com dynamic IP update client for Linux

   Copyright 2000-2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


			written June 2000
			by johna@onevista.com

	copyright transferred from 
		One Vista Associates 
	to 
		Free Software Foundation, Inc. 
	October 6, 2000 (johna)

	+	November 4, 2000
	+	updated noip.c and Makefile to build on solaris also
	+	collapsed expanded code into strerror()
	+	suggested by rob_nielson@mailandnews.com
	
	+	December 2, 2000
	+	updated noip.c to build on BSD also
	+	changed #include <linux/if.h> to #include <net/if.h>
	+	suggested by ramanan@users.sourceforge.net

	+	April 27, 2001 (Stephane Neveu stephane@skylord.org)
	+	changed the "SAVEDIPFILE" from /tmp/no-ip_save to 
		/var/run/no-ip_save
	+	added configuration default lookup into /usr/local/etc
		if /usr/local/lib doesn't have a configuration file
	+	fix output of contextual ourname[hostnum] in the function
		handle_dynup_error() instead of the "first" name

	+	August 27, 2001 (johna)
	+	added GROUP concept
	+	removed multiple host/domain name hack (use groups)
	+	changed SAVEDIPFILE back to /tmp 
			(must be root to write in /var/run)

	+	November 22, 2002 (johna)
	+	changed vsprintf to vsnprintf to avoid buffer overflow

	+	Version 2.0 December 2002 (johna -- major rewrite)
	+	using shared memory
	+	new config file format with autoconfig (-C)
	+	multiple instances supported (-M)
	+	status available for all instances (-S)
	+	can terminate an instance (-K)
	+	can toggle debugging for an instance (-D)

	+	March 2003	(johna)
	+	bumped MAX_NET_DEVS to 24
	+	drop root privs after acquiring conf (by Michal Ambroz)
	+	added -I interface_name flag (by Clifford Kite)
	
	+	April 2003	(johna)
	+	avoid listing IPV6 devices (robc at gmx.de)
	
	+	May 2003	(johna)
	+	replaced sleep(x) with select(1,0,0,0,timeout)
	+	added getifaddrs() for recent BSD systems (Peter Stromberg)
	+	added new SIOCGIFCONF for older BSD systems (Peter Stromberg)
*/			
/////////////////////////////////////////////////////////////////////////////                                            

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <termios.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pwd.h>
#include <time.h>

#ifdef bsd_with_getifaddrs
#include <ifaddrs.h>
#include <net/if_types.h>
#endif

#ifdef bsd
#include <sys/sockio.h>
#include <net/if_types.h>
#endif

#ifdef linux
 #ifndef SIOCGIFADDR
  #include <linux/sockios.h>
 #endif
#endif

#ifdef sun
 #include <sys/sockio.h>
#endif

#include "board.h" //Added by Nick HO

#define WRITE_TIMEOUT		60
#define CONNECT_TIMEOUT		60

//=== Added by Nick HO
#define DYNDNS_TYPE    0
#define EASYDNS_TYPE   1
#define NOIP_TYPE      2
//=== Added End 

#define CLIENT_IP_PORT		8245
#define IPLEN			16
#define LINELEN 	    256
//#define BIGBUFLEN		16384
#define BIGBUFLEN		1024

#define VERSION			"2.0.12"
#define NOIP_NAME		"dynupdate.no-ip.com"
#define USER_AGENT		"User-Agent: Linux DUC "VERSION
#define USTRNG			"username="
#define PWDSTRNG		"&pass="
#define GETIP_SCRIPT	"ip.php"
#if ENCRYPT
  #define REQUEST		"requestL="
#else
  #define REQUEST		""
#endif
  #define UPDATE_SCRIPT		"ducupdate.php"

#ifdef DEBUG
//#define OPTCHARS		"CYU:Fc:dD:hSMi:K:I:u:p:"  //Marked by Nick HO
  #define OPTCHARS		"CYU:Fc:dD:hSMi:K:I:u:p:R:T:"  //Added by Nick HO 
#else
//#define OPTCHARS		"CYU:Fc:hSMi:K:I:u:p:" //Marked by Nick HO
  #define OPTCHARS		"CYU:Fc:hSMi:K:I:u:p:R:T:" //Added by Nick HO
#endif
#define ARGC			1
#define ARGF			2
#define ARGY			4
#define ARGU			8
#define ARGI			16
#define ARGD			32
#define ARGS			64
#define ARGM			128
#define ARGK			256
#define ARGi			512
#define HOST			1
#define GROUP			2
#ifndef PREFIX
  #define PREFIX		""
#endif

#define CONFIG_FILENAME		PREFIX"/var/run/no-ip2.conf"
#define CONFIG_FILEPID		PREFIX"/var/run/no-ip2.pid"
#define CONFSTRLEN		1024
#define MAX_DEVLEN		16
#define MAX_INSTANCE		4
#define MAX_NET_DEVS		48
#define B64MOD			4
#define CONFIG_MAGIC		0x414a324c
#define NOIP_KEY		0x50494f4e
#define SHMEM_SIZE		(MAX_INSTANCE * sizeof(struct INSTANCE))
#define DEFAULT_NAT_INTERVAL	30

#define SPACE			' '
#define EQUALS			'='
#define COMMENT			'#'
#define COMMA                   ','

#define ALREADYSET               0
#define SUCCESS                  1
#define BADHOST                  2
#define BADPASSWD                3
#define BADUSER                  4
#define TOSVIOLATE               6
#define PRIVATEIP                7
#define HOSTDISABLED             8
#define HOSTISREDIRECT           9
#define BADGRP                  10
#define SUCCESSGRP              11
#define ALREADYSETGRP           12
#define RELEASEDISABLED         99

#define UNKNOWNERR		-1
#define FATALERR		-1
#define FAILURE         -1
#define NOHOSTLOOKUP	-2
#define SOCKETFAIL		-3
#define CONNTIMEOUT		-4
#define CONNFAIL		-5
#define READTIMEOUT		-6
#define READFAIL		-7
#define WRITETIMEOUT	-8
#define WRITEFAIL		-9
#define NOCONFIG		-10
#define BADCONFIG1		-11
#define BADCONFIG2		-12
#define BADCONFIG3		-13
#define BADCONFIG4		-14
#define BADCONFIG5		-15
#define BADCONFIG6		-16

#ifdef EMBED
#define Msg(...) 
#endif /* EMBED */

int	debug			= 	0;
int	timed_out		=	0;
int	background		=	1;	// new default, changed from 1 to 0 Nick HO
int	port_to_use		=	CLIENT_IP_PORT;
int	socket_fd		=	-1;
int	config_fd		=	-1;
int	nat			=	0;
int	interval		=	0;
int	log2syslog		= 	0;
int	connect_fail		=	0;
int offset                  =       0;
int	needs_conf 		=	0;
int	firewallbox		=	0;
int	forceyes		=	0;
int	update_cycle		=	0;
char    ddns_type		=	0;  //Added by Nick HO
int	show_config		=	0;
////int	shmid			=	0;
int	multiple_instances	=	0;
int	debug_toggle		=	0;
int	kill_proc		=	0;
int	reqnum			=	0;
void	*shmaddr		=	NULL;
char	*program		=	NULL;
char	*ourname		=	NULL;
char	tmp_filename[LINELEN]	=	"/tmp/NO-IPXXXXXX";
char	*config_filename	=	NULL;
char	*request		=	NULL;
char	*pid_path		=	NULL;
char	*execpath		=	NULL;
char	*config_username	=	NULL;
char	*config_password	=	NULL;
char	*config_Rhostname	=	NULL;   //Added by Nick HO
char	IPaddress[IPLEN];
char	iface[MAX_DEVLEN];

char	saved_args[LINELEN];
//char    buffer[BIGBUFLEN];
struct	termios argin, argout;

struct	sigaction sig;

///////////////////////////////////////////////////////////////////////////
void	process_options(int argc, char *argv[]);
void	alarm_handler(int signum);
void	exit_handler(int signum);
volatile void	wake_handler(int signum);

int 	get_our_visible_IPaddr(char *dest, char *ret_ip);

int		Read(int sock, char *buf, size_t count);
int		Write(int fd, char *buf, size_t count);
int		Connect(int port, char *dest);
int		converse_with_web_server(int fd, int d_type, char *r_host, char *send_buf);
int		converse_with_web_server_2(char *ret_ip, int socket_fd, char *send_buf);  //Added by Nick HO

void	dump_buffer(int x);
int		dynamic_update(int port, int d_type, unsigned long my_ip, char *buf);


void	autoconf();


void	Msg(char *fmt, ...);
int     encode_base64(const unsigned char *source, unsigned char *target, size_t source_size);


// Added by Nick HO
romeCfgParam_t *pRomeCfgParam;  

char *ddns_server[3] = {  //0=Dyndns, 1=eassydns, 2=noip
  "members.dyndns.org",
  "members.easydns.com",
  "dynupdate.no-ip.com"
};

char *get_request_line(char *request, int *request_len, int *request_idx);
// Added End 


///////////////////////////////////////////////////////////////////////////

//#define DDNS_TIMEOUT_SECOND		20*24*60*60		//20 days
#define DDNS_TIMEOUT_SECOND		5		//20 days
int main(int argc, char *argv[])
{
	char *p;
	struct passwd *nobody;
	char   tmp_IP[IPLEN]={0}, send_buf[1024];  //Added by Nick HO
	unsigned long old_ddns_ip=0, curr_wan_ip=0;
	int   j1, shmid;
	time_t	first_time, curr_time;

	printf("Entry noip2,nat=%u, background=%u\n",nat,background);  //Nick HO
	//exit(0); //Added by Nick HO

	port_to_use = CLIENT_IP_PORT;
    timed_out = 0;
    sig.sa_flags = 0;
    sigemptyset(&sig.sa_mask);
    sig.sa_handler = SIG_IGN;
    sigaction(SIGHUP,&sig,NULL);
    sigaction(SIGPIPE,&sig,NULL);
    sigaction(SIGUSR1,&sig,NULL);
    sigaction(SIGUSR2,&sig,NULL);
    sig.sa_handler = alarm_handler;
    sigaction(SIGALRM,&sig,NULL);
    p = strrchr(argv[0], '/');
    if (p)
       program = ++p;
    else
       program = argv[0];    

	
	sprintf(saved_args, "%s", p);
	*iface = 0;		// no supplied interface yet
	process_options(argc, argv);
	if (config_filename == NULL) 
	    config_filename = CONFIG_FILENAME;	
	
	nat = 1;
	background = 0;
	printf("ddns_type=%d, Rhostname=%s\n",ddns_type,config_Rhostname);
	printf("username=%s, passwd=%s \n",config_username,config_password);
	
	if (ddns_type != NOIP_TYPE)
		port_to_use = 80;	
	    
	printf("In noip2, nat=%u, bk=%u, IPaddress=%s \n", nat, background, IPaddress); //Added by Nick HO

	shmid=shmget(SHM_PROMECFGPARAM,sizeof(romeCfgParam_t),0666|IPC_CREAT);
	pRomeCfgParam=shmat(shmid,(void*)0,0);

	printf("WAN IP=%u.%u.%u.%u \n", pRomeCfgParam->ifCfgParam[0].ipAddr[0],  //192
	                                pRomeCfgParam->ifCfgParam[0].ipAddr[1],  //168
	                                pRomeCfgParam->ifCfgParam[0].ipAddr[2],  //52
	                                pRomeCfgParam->ifCfgParam[0].ipAddr[3]);   //92
	
	first_time = time(NULL);
	if (first_time == -1) {
		printf("time() fail \n");
		exit (1);
	}
	  
	printf("First_Time=%d \n",first_time);
	     
	for(j1=0; j1<2; j1++) {
		sleep(10);
		sprintf(tmp_IP,"%u.%u.%u.%u",pRomeCfgParam->ifCfgParam[0].ipAddr[0],
									 pRomeCfgParam->ifCfgParam[0].ipAddr[1],
									 pRomeCfgParam->ifCfgParam[0].ipAddr[2],
									 pRomeCfgParam->ifCfgParam[0].ipAddr[3]);
									 
		curr_wan_ip = inet_addr(tmp_IP); //tmp_ip=192.168.52.92 ==> curr_wan_ip=0x5c34a8c0
		printf("curr_wan_ip=%lx \n",curr_wan_ip);
		
		if ( (curr_wan_ip) && (curr_wan_ip != old_ddns_ip) ) {							 
			if ( get_our_visible_IPaddr(config_Rhostname, tmp_IP) != SUCCESS ) {
	           printf("Not connected to Net!");		
			   continue;				
			}
			old_ddns_ip = inet_addr(tmp_IP);  //Save DDNS Response IP
			
			if (old_ddns_ip == curr_wan_ip)  
				continue;

			if (dynamic_update(port_to_use, ddns_type, curr_wan_ip, send_buf) != SUCCESS) {
	    		printf("In noip2, dynamic_update() fail !!!\n");
	    		continue;				
			}
						
		}
		/*------------- 20 day to upgrade this DDNS website-------------*/
		else {
			curr_time = time(NULL);
			
			printf("Curr_Time=%d \n",curr_time);
			
			if ( ((curr_time - first_time)>DDNS_TIMEOUT_SECOND) && (curr_wan_ip) ) {
				if (dynamic_update(port_to_use, ddns_type, curr_wan_ip, send_buf) != SUCCESS) {
	    			printf("In noip2, dynamic_update() fail !!!\n");
	    			continue;					
				}
			}
		}		
	}	
	printf("Terminate the noip2 program !!\n");
	return 0;
}
///////////////////////////////////////////////////////////////////////////
void process_options(int argc, char *argv[])
{
	extern  int     optind, opterr;
	extern  char    *optarg;
	int     c, have_args = 0;

	printf("Entry process_options() !!\n");

	while ((c = getopt(argc, argv, OPTCHARS)) != EOF)	{
		switch (c) {
		case 'C':
			needs_conf++;
			log2syslog = -1;
			have_args |= ARGC;
			break;
		case 'F':
			firewallbox++;
			have_args |= ARGF;
			break;
		case 'Y':
			forceyes++;
			have_args |= ARGY;
			break;
		case 'U':
			update_cycle = atoi(optarg);
			have_args |= ARGU;
			break;
		case 'c':
			config_filename = optarg;
			strcat(saved_args, " -c ");
			strcat(saved_args, optarg);
			break;		
		case 'S':
			show_config++;
			log2syslog = -1;
			have_args |= ARGS;
			break;
		case 'M':
			multiple_instances++;
			have_args |= ARGM;
			break;
                case 'K':
                        kill_proc = atoi(optarg);
			have_args |= ARGK;
                        break;
		case 'i':
			strcpy(IPaddress, optarg);
			strcat(saved_args, " -i ");
			strcat(saved_args, optarg);
			have_args |= ARGi;
			break;
		case 'I':
			strcpy(iface, optarg);
			strcat(saved_args, " -I ");
			strcat(saved_args, optarg);
			have_args |= ARGI;
			break;
			break;
		case 'u': /* user name */			
			config_username = optarg;
			////strcat(saved_args, " -u ");
			////strcat(saved_args, optarg);
			break;
		case 'p': /* password name */
			config_password = optarg;
			//// strcat(saved_args, " -p ");
			//// strcat(saved_args, optarg);
			break;			
		// Added by Nick HO Start
		case 'R': /* password name */
			config_Rhostname = optarg;
			printf("Rhostname=%s \n",config_Rhostname); //Added by Nick HO
			break;
		case 'T':
		  printf("In process_options(), A.type=%s \n",optarg); 
			ddns_type = atoi(optarg);
			printf("In process_options(), B.type=%u \n",ddns_type); 
			have_args |= ARGU;
			break;
		// Added End
		default:
			//Usage();
			exit(0);
		}
	}
	if (needs_conf && (have_args > (ARGC|ARGF|ARGY|ARGU|ARGI))){
	    Msg("Error! -C option can only be used with -F -Y -U -I -c options.");
	    exit(1);
	}
	if (debug_toggle && (have_args != ARGD)){
	    Msg("Error! -D option can't be used with any other options.");
	    exit(1);
	}
	if (kill_proc && (have_args != ARGK)){
	    Msg("Error! -K option can't be used with any other options.");
	    exit(1);
	}
	if (show_config && (have_args != ARGS)){
	    Msg("Error! -S option can't be used with any other options.");
	    exit(1);
	}
	if (argc - optind) {
	    //Usage();
	    exit(1);
	}
	return;
}
///////////////////////////////////////////////////////////////////////////
void alarm_handler(int signum)	// entered on SIGALRM
{
	timed_out = 1;
}
///////////////////////////////////////////////////////////////////////////
void exit_handler(int signum)	//entered on SIGINT,  SIGTERM & SIGQUIT
{	
	background = 0;
	// don't reset handler -- exit on second signal
}
///////////////////////////////////////////////////////////////////////////
volatile void wake_handler(int signum)	//entered on SIGUSR1
{			// used to wake sleeping process only
}

/////////////////////////////////////////////////////////////////////////
int Read(int sock, char *buf, size_t count)
{
	size_t bytes_read = 0;
	int i;
	
	
	
	bytes_read = read(sock, buf, count);
		
	if (bytes_read <= 0)  
		return READFAIL;
	else 
		return bytes_read;
}
///////////////////////////////////////////////////////////////////////////
int Write(int fd, char *buf, size_t count)
{
	size_t bytes_sent = 0;
	int i;

	
	if (write(fd, buf, count)==-1) {
		printf("Write Failure !!!!\n");
		return FAILURE;
	}
	
	return SUCCESS;
}
///////////////////////////////////////////////////////////////////////////
//== return socket no.
//== return -1 fail
int Connect(int port, char *dest)
{
	int	fd, i;
	struct	in_addr saddr;
	struct	sockaddr_in addr;
	struct	hostent *host;

	//host = gethostbyname(NOIP_NAME); //Marked by Nick HO
	host = gethostbyname(dest); //Added by Nick HO
	if (!host) {
		printf("In Connect(), gethostbyname() fail !!!!\n");
		return NOHOSTLOOKUP;
	}
	memcpy(&saddr.s_addr, host->h_addr_list[0], 4);
	memset((char *) &addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = saddr.s_addr;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		printf("In Connect, socket() fail !!!\n");
		return SOCKETFAIL;
	}
		
	//timed_out = 0;
	//alarm(CONNECT_TIMEOUT);
	i = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
	//alarm(0);
	if (i < 0)  {
		printf("In Connect(), connect() fail !!!\n");
	    if (timed_out) 
			i = CONNTIMEOUT;			
	    else
			i = CONNFAIL;
	    close(fd);		// remove old socket
	    //connect_fail = 1;
	    return CONNTIMEOUT;
	}
	//socket_fd = fd;
	//connect_fail = 0;
	return fd;
}
/////////////////////////////////////////////////////////////////////////
// Return 0 ==> Successful, Nick HO
//        -1 ==> Fail, Nick HO
int converse_with_web_server(int fd, int d_type, char *r_host, char *send_buf)
{	
	int  x, ret, len, idx;  
	char *cptr, *line;
	char recv_buf[1024];
	//=== Added End


	printf("Entyr converse_with_web_server() \n"); //Added by Nick HO
	printf("buffer=%s\n",send_buf);

	if ((x = Write(fd, send_buf, strlen(send_buf))) != SUCCESS) {
		printf("Write() fail, x=%x !!\n", x);
	    close(fd);
	    return x;
	}
	memset(recv_buf,0,sizeof(recv_buf));
	if ((x = Read(fd, recv_buf, sizeof(recv_buf) - 2)) <= 0) { //Read return bytes to read
		printf("Read() fail x=%x!!\n, x");
	    close(fd);
	    return x;
	}
	
	printf("In converse_with_web_server(), receive bytes=%d \n",x);
	
	//======== Added by Nick HO Start ==========
	if (d_type != NOIP_TYPE) {
		
		cptr = strstr(recv_buf, "good");
		if (cptr==NULL) {
			if ( strstr(recv_buf, "nochg")==NULL) {
			   printf("Cannot find good char. in recv_buf \n");
			   return FATALERR;
		    }
		    else
		       return SUCCESS;		
		}
		else
			return SUCCESS;
		
		/**************************
		
		idx = 0;
		while ( ( line = get_request_line(recv_buf, &len, &idx) ) != (char*) 0 )	{
			if ( line[0] == '\0' )
				break;
		}	
		close(fd);
		
		printf("After get_request_line(), len=%d\n",len);
		recv_buf[len] = 0;
		printf("DDNS: %s", &recv_buf[idx]);
		if ( strncasecmp( &recv_buf[idx], "good", 4 ) == 0 )
			return SUCCESS;  //Successful
		else
			return FATALERR;  //Fail	
		**************************/
	}
	else { //=== Is NO-IP
		cptr = strstr(recv_buf, r_host);
		if (cptr==NULL) {
			printf("Cannot find %s in recv_buf \n", r_host);
			return FATALERR;
		}
		
		printf("cptr=%s \n",cptr);
		cptr = cptr+strlen(r_host);
		if (*cptr !=':') return FAILURE;
		cptr++;
		
		ret = atoi(cptr);
		switch (ret) {
			case 0:
				printf("DNS is Current, no update done \n");
				return SUCCESS;
				break;
			case 1:
				printf("DNS update successful \n");
				return SUCCESS;
				break;
			default:
				printf("DDNS update Failure, ret=%d !!!\n",ret);
				return FAILURE;
				break;
		}		
	}
	//======== Added End  ======================
		
	return SUCCESS;
}


int converse_with_web_server_2(char *ret_ip, int fd, char *send_buf)
{
	int	x;
	char *p;
	char rcv_buf[1024];

	printf("Entry converse_with_web_server_2() \n");
	printf("send_buf=%s\n", send_buf);
	
	if ((x = Write(fd, send_buf, strlen(send_buf))) != SUCCESS) {
		printf("In converse_with_web_server_2(), write fail !!\n");
	    //close(fd);
	    return x;
	}
	if ((x = Read(fd, rcv_buf, sizeof(rcv_buf))) < 0) {
		printf("In converse_with_web_server_2(), read fail !!\n");
	   // close(fd);
	    return x;
	}
	rcv_buf[x++] = 0;		// terminate the response
	if ((p = strrchr(rcv_buf,'\n')))// get end of penultimate line
	   strcpy(ret_ip, ++p);  	// address on next line
    else 
    	return FATALERR;


	return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////

int get_our_visible_IPaddr(char *dest, char *ret_ip)
{
	int	x, fd;
	char	*p = NULL;
	char send_buf[512];
	
    //======  Added by Nick HO   ======
    printf("Entry get_our_visible_IPaddr, ddns_type=%u !\n",ddns_type);
    
    if (ddns_type != NOIP_TYPE)  { //=== Send DNS to quesry     
        struct hostent *name_ptr;
        struct in_addr inetaddr;
        char *ip_ptr;
    	
  		name_ptr = gethostbyname(dest);

  		if (name_ptr==NULL) {
    		printf("Get Name Fail  !!\n");
    		return FAILURE;
    	}
  		else {
    		//printf("name=%s, h_length=%d \n",ptr->h_name, ptr->h_length);
    		inetaddr.s_addr = *((unsigned long *) name_ptr->h_addr_list[0]);
    		ip_ptr = inet_ntoa(inetaddr);
    		strcpy(ret_ip, ip_ptr);
    		printf("IPaddress=%s \n", ret_ip);
  		}    	
    	return SUCCESS;
    }
    //=====  Added End  =====	
	//*dest = 0;				// remove old address
	if ((fd = Connect(CLIENT_IP_PORT, ddns_server[ddns_type])) < 0)  {
	   printf("Connect(%s) fail \n",ddns_server[ddns_type]);
	   return FAILURE;
	} else {
	    // set up text for web page query
	    sprintf(send_buf, 
		"GET http://%s/ip.php HTTP/1.0\n%s\n\n", NOIP_NAME,USER_AGENT);
	    if ((x = converse_with_web_server_2(ret_ip, fd, send_buf)) != SUCCESS) {  //Call different function, Nick HO
	        printf("converse_with_web_server_2() fail, x=%x \n",x);
	        close(socket_fd);
			return FAILURE;
	    } else {
	    	printf("converse_with_web_server_2() successful, ret_ip=%s \n",ret_ip);
			close(socket_fd);
			return SUCCESS;				// NORMAL EXIT
	    } 
	}	
}

///////////////////////////////////////////////////////////////////////////
int dynamic_update(int port, int d_type, unsigned long my_ip, char *send_buf)
{
	int	fd, i, x, is_group, retval, response;
	char	*p, *pos, tbuf[LINELEN], gname[LINELEN];
	struct in_addr inetaddr;	
	char tmp_buf[101], tmp_buf1[101];
	

	printf("Entry dynamic_update() \n");  //Added by Nick HO
	
	inetaddr.s_addr = my_ip;

	retval = SUCCESS;
	
	//=== Connect will create a new socket no.
	if ((fd = Connect(port_to_use, ddns_server[d_type])) <0) {
		printf("Connect(%s) fail, fd=[%u] \n",ddns_server[d_type],fd);	    
	    return fd;
	}		
	
	//=== Modified by Nick HO Start
	switch (d_type) {
		case DYNDNS_TYPE:
	   	   	sprintf(send_buf, "GET /nic/update?system=dyndns&hostname=%s",config_Rhostname);
      		sprintf(tmp_buf, "&myip=%s", inet_ntoa(inetaddr));
      		strcat(send_buf, tmp_buf);
      		sprintf(tmp_buf, "&wildcard=OFF");
      		strcat(send_buf, tmp_buf);
      		sprintf(tmp_buf, "&mx=");
      		strcat(send_buf, tmp_buf);
      		sprintf(tmp_buf, "&backmx=NO");
      		strcat(send_buf, tmp_buf);
      		sprintf(tmp_buf, "&offline=NO ");
      		strcat(send_buf, tmp_buf);
      		strcpy(tmp_buf, "HTTP/1.0\r\nUser-Agent: client/1.0\r\n");
      		strcat(send_buf, tmp_buf);      
      		sprintf(tmp_buf, "Host: %s\r\nConnection: close\r\n", ddns_server[d_type]);
      		strcat(send_buf, tmp_buf);
      
      		strcpy(tmp_buf, "Authorization: Basic ");
      		strcat(send_buf, tmp_buf);
      		sprintf(tmp_buf1,"%s:%s",config_username, config_password);
      		encode_base64(tmp_buf1, tmp_buf, strlen(tmp_buf1));
      		strcat(send_buf, tmp_buf);
      		strcpy(tmp_buf, "\r\n\r\n");
      		strcat(send_buf, tmp_buf);
			printf("In dynamic_update(), DYNDNS TYPE combine http finish \n");
      		break;
      		
        case EASYDNS_TYPE:    
      		sprintf(send_buf, "GET /dyn/dyndns.php?&hostname=%s", config_Rhostname);
      		sprintf(tmp_buf, "&myip=%s",inet_ntoa(inetaddr));
      		strcat(send_buf, tmp_buf);
      		sprintf(tmp_buf, "&wildcard=OFF");
      		strcat(send_buf, tmp_buf);
      		sprintf(tmp_buf, "&mx=");
      		strcat(send_buf, tmp_buf);
      		sprintf(tmp_buf, "&backmx=NO ");
      		strcat(send_buf, tmp_buf);
      
      		strcpy(tmp_buf, "HTTP/1.0\r\nUser-Agent: client/1.0\r\n");
      		strcat(send_buf, tmp_buf);      
      		sprintf(tmp_buf, "Host: %s\r\nConnection: close\r\n", ddns_server[d_type]);
      		strcat(send_buf, tmp_buf);
      
      		strcpy(tmp_buf, "Authorization: Basic ");
      		strcat(send_buf, tmp_buf);
      		sprintf(tmp_buf1,"%s:%s",config_username, config_password);
      		encode_base64(tmp_buf1, tmp_buf, strlen(tmp_buf1));
      		strcat(send_buf, tmp_buf);
      		strcpy(tmp_buf, "\r\n\r\n");
      		strcat(send_buf, tmp_buf); 
      		break;
      		
	    case NOIP_TYPE:
	    	sprintf(send_buf, "GET http://dynupdate.no-ip.com/ducupdate.php?username=%s&pass=%s&h[]=%s&ip=%s ",config_username,config_password,config_Rhostname ,inet_ntoa(inetaddr));            
      		strcpy(tmp_buf, "HTTP/1.0\r\nUser-Agent: Linux DUC 2.0.12");
      		strcat(send_buf, tmp_buf);
      		//sprintf(tmp_buf, "Host: %s\r\nConnection: close\r\n", ddns_server[d_type]);
      		sprintf(tmp_buf, "Host: %s\r\nConnection: close\r\n", ddns_server[d_type]);
      		strcat(send_buf, tmp_buf);      
      		strcpy(tmp_buf, "\r\n");
      		strcat(send_buf, tmp_buf);
      		break;
      	default:
      		printf("DDNS Type:%d not support !! \n",ddns_type);
      		return -1;
    }

	
	x = converse_with_web_server(fd, d_type, config_Rhostname, send_buf);
	if (x !=SUCCESS)	{		
	    printf("converse_with_web_server() fail !!!\n"); //Added by Nick HO	    
	    return x;
	}
	else
	   printf("Dynamic update IP Successful !!!!\n");
	close(fd);	
	return x;
}

//================  Added by Nick HO Start  =====================
char *get_request_line(char *request, int *request_len, int *request_idx)
{
    int i;
    char c;

    for ( i = *request_idx; *request_idx < *request_len; ++(*request_idx) )
	{
    	c = request[*request_idx];
    	if ( c == '\n' || c == '\r' )
    	{
    	    request[*request_idx] = '\0';
    	    ++(*request_idx);
    	    if ( c == '\r' && *request_idx < *request_len &&
    		 request[*request_idx] == '\n' )
    		{
        		request[*request_idx] = '\0';
        		++(*request_idx);
    		}
    	    return &(request[i]);
    	}
	}
    return (char*) 0;
}


static int tables_initialised = 0;
static char base64_to_char [64];

static void
init_conversion_tables (void)
{
    unsigned char value; /* Value to store in table */
    unsigned char offset;
    unsigned char index; /* Index in table */

    /* Reset the tables */
    memset (base64_to_char, 0, sizeof (base64_to_char));
    
    value = 'A';
    offset = 0;
    
    for (index = 0; index < 62; index++)
    {
        if (index == 26)
        {
            value = 'a';
            offset = 26;
        }
        else
            if (index == 52)
            {
                value = '0';
                offset = 52;
            }
        base64_to_char [index] = value + index - offset;
    }
    base64_to_char [62] = '+';
    base64_to_char [63] = '/';
    
    tables_initialised = 1;
}

int
encode_base64(const unsigned char *source, unsigned char *target, size_t source_size)
{
    int target_size = 0;                /*  Length of target buffer          */
    int nb_block;                       /*  Total number of blocks           */
    unsigned char       *p_source;      /*  Pointer to source buffer         */
    unsigned char       *p_target;      /*  Pointer to target buffer         */
    unsigned char       value;          /*  Value of Base64 byte             */

    if (source_size == 0)
        return (0);

    if (!tables_initialised)
        init_conversion_tables();
    

    nb_block = (int) (source_size / 3);

    /*  Check if we have a partially-filled block                            */
    if (nb_block * 3 != (int) source_size)
        nb_block++;
    target_size = (size_t) nb_block * 4;
    target [target_size] = '\0';

    p_source = (unsigned char *) source;         /*  Point to start of buffers        */
    p_target = target;

    while (nb_block--)
    {
        /*  Byte 1                                                           */
        value = *p_source >> 2;
        *p_target++ = base64_to_char [value];

        /*  Byte 2                                                           */
        value = (*p_source++ & 0x03) << 4;
        if ((size_t) (p_source - source) < source_size)
            value |= (*p_source & 0xF0) >> 4;
        *p_target++ = base64_to_char [value];

        /*  Byte 3 - pad the buffer with '=' if block not completed          */
        if ((size_t) (p_source - source) < source_size)
        {
            value = (*p_source++ & 0x0F) << 2;
            if ((size_t) (p_source - source) < source_size)
                value |= (*p_source & 0xC0) >> 6;
            *p_target++ = base64_to_char [value];
        }
        else
            *p_target++ = '=';

        /*  Byte 4 - pad the buffer with '=' if block not completed          */
        if ((size_t) (p_source - source) < source_size)
        {
            value       = *p_source++ & 0x3F;
            *p_target++ = base64_to_char [value];
        }
        else
            *p_target++ = '=';
   }
   return (target_size);
}

//===============   Added End   ===============================

/////////////////////////////////////////////////////////////////////////////
#ifndef EMBED
void Msg(char *fmt, ...)
{
	va_list ap;
	char	errmsg[LINELEN];

	va_start(ap, fmt);
	vsnprintf(errmsg, LINELEN-1, fmt, ap);
	va_end(ap);
	if (log2syslog > 0)
		syslog(LOG_ERR, "%s\n", errmsg);
	else
		fprintf(stderr, "%s\n", errmsg);
}
#endif /* EMBED */
/////////////////////////////////////////////////////////////////////////////

