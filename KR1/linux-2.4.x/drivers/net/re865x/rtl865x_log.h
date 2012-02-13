/*
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* Program : rtl8651_log.h
* Abstract : 
* Creator : 
* Author :  
*/
#ifndef _RTL8651_LOG_H
#define _RTL8651_LOG_H

#include "rtl865x/rtl8651_tblDrv.h"

#ifdef _RTL_NEW_LOGGING_MODEL

/**********************************************************
	Global variables
**********************************************************/
#define RTL8651_LOGPROC_FILE			"log_module"
#define RTL8651_MAX_LOG_MODULES		9
#define RTL8651_MAX_LOG_MSGSIZE		256
#define RTL8651_MAX_LOG_BUFFERSIZE	16000

/**********************************************************
	Extern variable
**********************************************************/
extern int32 timezone_diff;	/* for re_core to set time zone */
/**********************************************************
	Data structure
**********************************************************/
//for circular buffer
struct circ_buf 
{	
	int size;	
	int head;	
	int tail;	
	char data[1];
};
/***************************************************************************
			External Functions for forwarding engine declaration
***************************************************************************/
int32 myLoggingFunction(uint32 moduleId, uint32 logNo, rtl8651_logInfo_t *info);


#endif /* _RTL_NEW_LOGGING_MODEL */
#endif /* _RTL8651_LOG_H */
