#include "uip.h"
#include "uip_arp.h"
#include "httpd.h"
#include "rtl_types.h"
#include "tftpnaive.h"
#include "swNic_poll.h"

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

#ifndef NULL
#define NULL (void *)0
#endif /* NULL */

int finishflag = 1;

/*-----------------------------------------------------------------------------------*/
int init_net (char * pMac, int * pSize, void * bufBase)
{
  u8_t i, arptimer;
  uint32 uip_pklen;
  uint8  *ppktData2;
  int ret;
  int counter=0;
  /* Initialize the uIP TCP/IP stack. */
  uip_init();
  memcpy(uip_ethaddr.addr, pMac, 6);
//  printf("Init uip_init()\n");
  /* Initialize the HTTP server. */
  httpd_init();
//  printf("Init httpd_init()\n");
  arptimer = 0;
  ppktData2 = uip_buf;
  while(1) {
    /* Let the tapdev network device driver read an entire IP packet
       into the uip_buf. If it must wait for more than 0.5 seconds, it
       will return with the return value 0. If so, we know that it is
       time to call upon the uip_periodic(). Otherwise, the tapdev has
       received an IP packet that is to be processed by uIP. */
    
        if(finishflag ==0)
          return 0;
        ret = swNic_receive_t((void**)&ppktData2,&uip_pklen);
        if ( ret == 0)
        {
     	memcpy(uip_buf,ppktData2,uip_pklen);
    	uip_len = uip_pklen;
 //   	printf("counter2 is %d\n",counter++);


//      if(++arptimer == 20) {	
//	uip_arp_timer();
//	arptimer = 0;
//       }
 //      printf("After Counter\n");
      
  

      if(BUF->type == htons(UIP_ETHTYPE_IP)) {
  //    	printf("Before uip_arp_ipin\n");
	uip_arp_ipin();
//	printf("After uip_arp_ipin\n");
	uip_input();
//	printf("After uip_input\n");
	/* If the above function invocation resulted in data that
	   should be sent out on the network, the global variable
	   uip_len is set to a value > 0. */
	if(uip_len > 0) {
	  uip_arp_out();
	  uip_pklen=uip_len;
	  ret = swNic_send_t(uip_buf,uip_pklen);
//	  printf("After swNic_send_t\n");
	}
      } else if(BUF->type == htons(UIP_ETHTYPE_ARP)) {

	uip_arp_arpin();
	/* If the above function invocation resulted in data that
	   should be sent out on the network, the global variable
	   uip_len is set to a value > 0. */	
	if(uip_len > 0) {
	  uip_pklen=uip_len;	
	  swNic_send_t(uip_buf,uip_pklen);
	}
      }
    }
  }
  printf("Leave init_net\n");
  return 0;
}
/*-----------------------------------------------------------------------------------*/
void
uip_log(char *m)
{
  printf("uIP log message: %s\n", m);
}
/*-----------------------------------------------------------------------------------*/
