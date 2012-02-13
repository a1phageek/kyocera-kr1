/**
 * \addtogroup httpd
 * @{
 */

/**
 * \file
 * HTTP server script language C functions file.
 * \author Adam Dunkels <adam@dunkels.com>
 *
 * This file contains functions that are called by the web server
 * scripts. The functions takes one argument, and the return value is
 * interpreted as follows. A zero means that the function did not
 * complete and should be invoked for the next packet as well. A
 * non-zero value indicates that the function has completed and that
 * the web server should move along to the next script line.
 *
 */

/*
 * Copyright (c) 2001, Adam Dunkels.
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.  
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: cgi.c,v 1.23.2.4 2003/10/07 13:22:27 adam Exp $
 *
 */


#include <rtl_types.h>
#include <board.h>
#include <flashdrv.h>
#include <rtl_image.h>
#include <rtl_flashdrv.h>

#include "uip.h"
#include "cgi.h"
#include "httpd.h"
#include "fs.h"
#include "fsdata.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static u8_t upgrade_cgi(u8_t next);

extern const struct fsdata_file file_error_html;

cgifunction cgitab[] = {
  upgrade_cgi   /* CGI function "a" */
};

static const char closed[] =   /*  "CLOSED",*/
{0x43, 0x4c, 0x4f, 0x53, 0x45, 0x44, 0};
static const char syn_rcvd[] = /*  "SYN-RCVD",*/
{0x53, 0x59, 0x4e, 0x2d, 0x52, 0x43, 0x56, 
 0x44,  0};
static const char syn_sent[] = /*  "SYN-SENT",*/
{0x53, 0x59, 0x4e, 0x2d, 0x53, 0x45, 0x4e, 
 0x54,  0};
static const char established[] = /*  "ESTABLISHED",*/
{0x45, 0x53, 0x54, 0x41, 0x42, 0x4c, 0x49, 0x53, 0x48, 
 0x45, 0x44, 0};
static const char fin_wait_1[] = /*  "FIN-WAIT-1",*/
{0x46, 0x49, 0x4e, 0x2d, 0x57, 0x41, 0x49, 
 0x54, 0x2d, 0x31, 0};
static const char fin_wait_2[] = /*  "FIN-WAIT-2",*/
{0x46, 0x49, 0x4e, 0x2d, 0x57, 0x41, 0x49, 
 0x54, 0x2d, 0x32, 0};
static const char closing[] = /*  "CLOSING",*/
{0x43, 0x4c, 0x4f, 0x53, 0x49, 
 0x4e, 0x47, 0};
static const char time_wait[] = /*  "TIME-WAIT,"*/
{0x54, 0x49, 0x4d, 0x45, 0x2d, 0x57, 0x41, 
 0x49, 0x54, 0};
static const char last_ack[] = /*  "LAST-ACK"*/
{0x4c, 0x41, 0x53, 0x54, 0x2d, 0x41, 0x43, 
 0x4b, 0};

static const char *states[] = {
  closed,
  syn_rcvd,
  syn_sent,
  established,
  fin_wait_1,
  fin_wait_2,
  closing,
  time_wait,
  last_ack};
  

/*-----------------------------------------------------------------------------------*/
unsigned long total_size=0;
unsigned long total_filesize=0;
unsigned long total_packets=0;
int content_length;

// HardWare ID, 22 bytes only not include Model ID.
#define HWID_LEN 22
extern const char HWID[];

char *get_request_line(int *request_len, int *request_idx)
{
    int i;
    char c;
    
    //printf("uip_appdata[%d] is %X\n",*request_idx,uip_appdata[*request_idx]);
    //printf("request_len is %d\n",*request_len);
    //printf("request_idx is %d\n",*request_idx);
    
    for ( i = *request_idx; *request_idx < *request_len; ++(*request_idx) )
	{
    	c = uip_appdata[*request_idx];
    	if ( c == '\n' || c == '\r' )
    	{
    	    uip_appdata[*request_idx] = '\0';
    	    ++(*request_idx);
    	    if ( c == '\r' && *request_idx < *request_len &&
    		 uip_appdata[*request_idx] == '\n' )
    		{
        		uip_appdata[*request_idx] = '\0';
        		++(*request_idx);
    		}
    	    return (char *)&(uip_appdata[i]);
    	}
	}
    return (char*) 0;
}

int parse_header(void)
{
	int request_len,request_idx;
	char *line;
	char *cp;
	
	printf("Start Backup Mode Upgrading\n");
  	
  	request_len = uip_datalen()-54;
  	request_idx = 0;
  	while ( ( line = get_request_line(&request_len, &request_idx) ) != (char*) 0 )
  	{
  	    if ( line[0] == '\0' )
    	    	break;	
  	    else if ( strncmp( line, "Content-Length:", 15 ) == 0 )
	    {
	    	//printf("Got it\n");
	   	//printf("line is %s\n",line);
    	    	cp = &line[16];
    	    	//printf("cp is %s\n",cp);

	    	total_filesize= strtol(cp, NULL, 0);
    	  
	    }	   	
  	}
  	
  	total_packets ++;
}

static u8_t upgrade_cgi(u8_t next)
{
  /* We use sprintf() to print the number of file accesses to a
     particular file (given as an argument to the function in the
     script). We then use uip_send() to actually send the data. */

  char *line;
  char *cp;
  struct fs_file fsfile; 
   
  int request_len,request_idx;
  int len;
  unsigned long temp;
  int count;
  int update_loader = 0;
  int update_runtime = 0;
  
  
  if(next) return 1;
	
  if (total_packets == 0)
  {
  	parse_header();
  	return 1;
  }
  if (total_packets == 2 )
  {
	request_len = uip_datalen();
	request_idx = 0;
	//printf("11-request_idx is %d\n",request_idx);
	while ( ( line = get_request_line(&request_len, &request_idx) ) != (char*) 0 )
  	{
  	    //printf("line[0] is %X\n",line[0]);	
  	    if ( line[0] == '\0' )
    	    	break;
    	    else if ( strncmp( line, "-----------------------------", 29 ) == 0 )
    	    {
    	    	content_length = strlen(line);	
    	    }
  	    else if ( strncmp( line, "Content-Type:", 13 ) == 0 )
	    {
	   //printf("Got it\n");
	   //printf("line is %s\n",line);
    	    	cp = (char *)&uip_appdata[request_idx+2];
    	   
    	    //printf("uip_appdata[%d+2] is %X\n",request_idx,uip_appdata[request_idx+2]);
    	    	if(uip_datalen() != 1446)
    	    	{
    	    		total_size = uip_datalen()-(request_idx+2)-content_length-6;
    	    		len = uip_datalen()-(request_idx+2)-content_length-6;
    	    	}
    	    	else
    	    	{
    	    		total_size = uip_datalen()-(request_idx+2);
    	    		len = uip_datalen()-(request_idx+2);
    	    	}
    	    
    	     	memcpy ((void *) (DRAM_MAP_DLOAD_BUF_ADDR),cp, len);
    	    
    	     	//printf("DRAM_MAP_DLOAD_BUF_ADDR is %X\n",REG32(DRAM_MAP_DLOAD_BUF_ADDR));
    	     	len = uip_datalen() - content_length - 6;
	     	break;
	    }	   	
  	}
  	total_packets++;
	return 0;
  }
//  printf("Incoming packet is %d\n",uip_datalen());
// 
//  printf("uip_appdata[0] is %X\n",uip_appdata[0]);
//   printf("uip_appdata[1] is %X\n",uip_appdata[1]);
//    printf("uip_appdata[2] is %X\n",uip_appdata[2]);
//     printf("uip_appdata[3] is %X\n",uip_appdata[3]); 
//     printf("uip_appdata[4] is %X\n",uip_appdata[4]);
//      printf("uip_appdata[5] is %X\n",uip_appdata[5]);
//       printf("uip_appdata[6] is %X\n",uip_appdata[6]);
//        printf("uip_appdata[7] is %X\n",uip_appdata[7]);
     
 if ( total_size != 0 )
 {
 //	printf("uip_datalen() is %d\n",uip_datalen());
  if (uip_datalen() != 1446 )
  {

  	if (uip_datalen()!= 962)
  	{
  		//printf("Last Packets\n");
  		len = uip_datalen() - content_length - 6;
  		if ( len  > 0)
  		{
  			cp = (char *)&uip_appdata[0];
  			memcpy ((void *) (DRAM_MAP_DLOAD_BUF_ADDR + total_size),cp, len);
  			total_size += uip_datalen() - content_length - 6;
  			
  		}
  		else
  		{
  			temp = total_size;
  			total_size +=len;
  			len= temp - total_size;
  			//memset((void *) (DRAM_MAP_DLOAD_BUF_ADDR + total_size),0,len);
  		}
  		printf("total upgraded size is %d\n",total_size);
  		
  		
  		// Verify type of received file
  		{
  			fileImageHeader_t * imghdr;
  			char tmpHWID[HWID_LEN];
  			char *hw_id_ptr;
  			
  			hw_id_ptr = (void *) (DRAM_MAP_DLOAD_BUF_ADDR + total_size - 24);
  			
  			memcpy(tmpHWID, hw_id_ptr, HWID_LEN);
  			tmpHWID[HWID_LEN] = '\0';
  			
		        // Hardware ID check
			if(strncmp(tmpHWID, HWID, HWID_LEN) != 0) 
			{
				printf("\nHardware ID Error.\n");
				return 2;
  			}
  			printf("HW ID matched\n");
  			
  			imghdr = (fileImageHeader_t *) DRAM_MAP_DLOAD_BUF_ADDR;
  			
  			// Check type magic number
    			if (imghdr->imageType == RTL_IMAGE_TYPE_BOOT ) 
    			{
        			printf("Loader Image Type Matched\n");
        			update_loader = 1;
    				goto update_LOADERIMAGE;
    			}
    			
    			if((imghdr->imageType == RTL_IMAGE_TYPE_RUN) || (imghdr->imageType == RTL_IMAGE_TYPE_KFS))
    			{
    				printf("RunTime Image Type Matched\n");
    				update_runtime = 1;
    				goto update_RUNIMAGE;
    			}
    			
    			printf("\nImage type error!");
        		return 2;
    			
update_LOADERIMAGE:  		
  			if ((update_loader == 1) && ( fileSys_updateBootImage((void *) DRAM_MAP_DLOAD_BUF_ADDR, total_size - 24) == 0 ))
        		{
        	    		printf("Backup Mode Loader Upgrad Success\n");
          			SmartReboot();
        		}
        		else
         		{
         			printf("Backup Mode Loader Not ok\n");
         			return 2;
         		}    
  			finishflag = 0;
  			return 1;
  		}

update_RUNIMAGE:  		
         	if ((update_runtime == 1) && ( fileSys_updateRuntimeImage((void *) DRAM_MAP_DLOAD_BUF_ADDR, total_size - 24) == 0 ))
         	{
          		printf("Backup Mode RunTime Upgrad Success\n");
          		SmartReboot();
         	}
         	else
         	{
         		printf("Backup Mode Runtime Not ok\n");
         		return 2;
         	}    
  		finishflag = 0;
  		return 1;
        }
        else
        {
        	len = uip_datalen();
        	cp = (char *)&uip_appdata[0];
        	memcpy ((void *) (DRAM_MAP_DLOAD_BUF_ADDR + total_size),cp, len);
        	total_size += uip_datalen();        	
  		//	printf("total_size is %d\n",total_size);
        }
  }
  else
  {
  	len = uip_datalen();
  	cp = (char *)&uip_appdata[0];
	memcpy ((void *) (DRAM_MAP_DLOAD_BUF_ADDR + total_size),cp, len);
  	total_size += uip_datalen();
  //	printf("total_size is %d\n",total_size);	
  }
 }
	//  uip_send(uip_appdata, sprintf((char *)uip_appdata, "%5u", fs_count(&hs->script[4])));  
  total_packets++;
  return 0;
}
/*-----------------------------------------------------------------------------------*/

