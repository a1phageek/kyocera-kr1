
/*
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.                                                    
* 
* Program : Core types used in source tree.
* Abstract :  Types such as uint32 are not defined here. They are included via <sys/types.h>
* Author : David Chun-Feng Liu (cfliu@realtek.com.tw)               
* $Id: types.h,v 1.2 2004/02/26 10:04:26 cfliu Exp $
* $Log: types.h,v $
* Revision 1.2  2004/02/26 10:04:26  cfliu
* Remove NTOHS... macros
*
* Revision 1.1.1.1  2003/08/27 06:20:19  rupert
* uclinux initial
*
* Revision 1.1.1.1  2003/08/27 03:08:52  rupert
*  initial version 
*
* Revision 1.1  2003/07/23 01:01:03  small
* Add files
*
* Revision 1.1  2003/06/20 16:05:31  elvis
* Initial Version
*
* Revision 1.7  2003/05/19 10:06:53  cfliu
* Undo change to ISSET. For  ISSET( a, X) , any bit in X set returns TRUE
*
* Revision 1.6  2003/05/17 08:47:31  cfliu
* Change definition of ISSET()
*
* Revision 1.5  2003/04/30 15:34:18  cfliu
* add macros from rtl_types.h and add new macros to solve alignment, endian issues
*
* Revision 1.4  2003/04/02 15:39:39  cfliu
* Move ASSERT_ISP and ASSERT_ISR to rtl_depend.h
*
* Revision 1.3  2003/03/13 10:29:55  cfliu
* Remove unused symbols.
* Add ASSERT_CSP/ISR
*
* Revision 1.2  2002/04/29 10:04:28  hiwu
* remove ipaddr typedefine
*
* Revision 1.1  2002/04/29 05:45:25  cfliu
* Moved to new source tree
*
* Revision 1.5  2002/04/25 03:59:34  cfliu
* no message
*
* Revision 1.4  2002/04/04 10:47:29  cfliu
* Use 'indent' tool to unify coding style....
* The options used could be found on /product/cygtest/.indent.pro
*
* Revision 1.3  2002/03/28 06:46:54  cfliu
* no message
*
* Revision 1.2  2002/03/25 11:36:28  cfliu
* no message
*
* Revision 1.1  2002/03/21 05:44:07  cfliu
* Primitive data types changed to uint32, uint16, ...etc.
*
*/
#ifndef _TYPES_H
#define _TYPES_H

#include "rtl_types.h"

#ifndef CLEARBITS
#define CLEARBITS(a,b)	((a) &= ~(b))
#endif

#ifndef SETBITS
#define SETBITS(a,b)		((a) |= (b))
#endif

#ifndef ISSET
#define ISSET(a,b)		(((a) & (b))!=0)
#endif

#ifndef ISCLEARED
#define ISCLEARED(a,b)	(((a) & (b))==0)
#endif

#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif			   /* max */

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif			   /* min */

//round down x to multiple of y.  Ex: ROUNDDOWN(20, 7)=14
#ifndef ROUNDDOWN
#define	ROUNDDOWN(x, y)	(((x)/(y))*(y))
#endif

//round up x to multiple of y. Ex: ROUNDUP(11, 7) = 14
#ifndef ROUNDUP
#define	ROUNDUP(x, y)	((((x)+((y)-1))/(y))*(y))  /* to any y */
#endif

#ifndef ROUNDUP2
#define	ROUNDUP2(x, y)	(((x)+((y)-1))&(~((y)-1))) /* if y is powers of two */
#endif

#ifndef ROUNDUP4
#define	ROUNDUP4(x)		((1+(((x)-1)>>2))<<2)
#endif

#ifndef IS4BYTEALIGNED
#define IS4BYTEALIGNED(x)	 ((((x) & 0x3)==0)? 1 : 0)
#endif

#define swapl32(x)\
        ((((x) & 0xff000000U) >> 24) | \
         (((x) & 0x00ff0000U) >>  8) | \
         (((x) & 0x0000ff00U) <<  8) | \
         (((x) & 0x000000ffU) << 24))
#define swaps16(x)        \
        ((((x) & 0xff00) >> 8) | \
         (((x) & 0x00ff) << 8))


//Get/Set host-order value from/to little endian buffer. 
//first byte ptr points to Little endian system's LSB.
#define GET_UINT16_LITTLE_ENDIAN_UNALIGNED( ptr )  ((uint16)  (*(((uint8*)(ptr))+1)<<8) | *((uint8*)(ptr)))
#define SET_UINT16_LITTLE_ENDIAN_UNALIGNED(  u16value, ptr )	do{\
		*((uint8*)(ptr)) =  (uint8)((u16value));/* get and set LSB */\
		*(((uint8*)(ptr))+1) =  (uint8)((u16value)>>8);/*get and set MSB*/\
}while(0)
#define GET_UINT32_LITTLE_ENDIAN_UNALIGNED( ptr )  ((uint32)  (*(((uint8*)(ptr))+3)<<24)|(*(((uint8*)(ptr))+2)<<16)|(*(((uint8*)(ptr))+1)<<8) | *((uint8*)(ptr)))
#define SET_UINT32_LITTLE_ENDIAN_UNALIGNED(  u32value, ptr )	do{\
		*((uint8*)(ptr)) =  (uint8)((u32value));/* get and set LSB */\
		*(((uint8*)(ptr))+1) =  (uint8)((u32value)>>8);\
		*(((uint8*)(ptr))+2) =  (uint8)((u32value)>>16);\
		*(((uint8*)(ptr))+3) =  (uint8)((u32value)>>24);/*get and set MSB*/\
}while(0)


//Get/Set host-order value from/to big endian buffer. 
//first byte ptr points to Big endian system's the MSB
#define GET_UINT16_BIG_ENDIAN_UNALIGNED( ptr )  ((uint16)  (*((uint8*)(ptr))<<8) | *(((uint8*)(ptr))+1))
#define SET_UINT16_BIG_ENDIAN_UNALIGNED(  u16value, ptr )	do{\
	*((uint8*)(ptr)) = (uint8)(((uint16)u16value)>>8);  /*get and set MSB*/\
	*(((uint8*)(ptr))+1) = (uint8)(u16value); /* get and set LSB */\
}while(0)
#define GET_UINT32_BIG_ENDIAN_UNALIGNED( ptr )  ((uint32)  (*(((uint8*)(ptr))+3)<<24)|(*(((uint8*)(ptr))+2)<<16)|(*(((uint8*)(ptr))+1)<<8) | *((uint8*)(ptr)))
#define SET_UINT32_BIG_ENDIAN_UNALIGNED( u32value, ptr )	do{\
	*((uint8*)(ptr)) =  (uint8)((u32value)>>24);/* get and set MSB */\
	*(((uint8*)(ptr))+1) =  (uint8)((u32value)>>16);\
	*(((uint8*)(ptr))+2) =  (uint8)((u32value)>>8);\
	*(((uint8*)(ptr))+3) =  (uint8)((u32value));/*get and set LSB*/\
}while(0)


#if 0 /* _LITTLE_ENDIAN */
	#ifndef ntohs
	#define ntohs(x)   swaps16(x)
	#endif
	#ifndef ntohl
	#define ntohl(x)   swapl32(x)
	#endif
	#ifndef htons
	#define htons(x)   swaps16(x)
	#endif
	#ifndef htonl
	#define htonl(x)   swapl32(x)
	#endif
	#ifndef NTOHL
	#define NTOHL(d) ((d) = ntohl((d)))
	#endif
	#ifndef NTOHS
	#define NTOHS(d) ((d) = ntohs((uint16)(d)))
	#endif
	#ifndef HTONL
	#define HTONL(d) ((d) = htonl((d)))
	#endif
	#ifndef HTONS
	#define HTONS(d) ((d) = htons((uint16)(d)))
	#endif

	#define GET_UINT16_UNALIGNED( ptr )  				GET_UINT16_LITTLE_ENDIAN_UNALIGNED(ptr)
	#define SET_UINT16_UNALIGNED(  u16value, ptr )		SET_UINT16_LITTLE_ENDIAN_UNALIGNED(  u16value, ptr )
	#define GET_UINT32_UNALIGNED( ptr )  				GET_UINT32_LITTLE_ENDIAN_UNALIGNED(ptr)
	#define SET_UINT32_UNALIGNED(  u32value, ptr )		SET_UINT32_LITTLE_ENDIAN_UNALIGNED(  u32value,ptr)
	#define PKTGET_UINT16_UNALIGNED( ptr )  				GET_UINT16_BIG_ENDIAN_UNALIGNED(ptr)
	#define PKTSET_UINT16_UNALIGNED(  u16value, ptr 	)	SET_UINT16_BIG_ENDIAN_UNALIGNED(  u16value, ptr )
	#define PKTGET_UINT32_UNALIGNED( ptr )  				GET_UINT32_BIG_ENDIAN_UNALIGNED(ptr)
	#define PKTSET_UINT32_UNALIGNED(  u32value, ptr )	SET_UINT32_BIG_ENDIAN_UNALIGNED(  u32value,ptr)

#else /*Big endian system */

	#ifndef ntohs
	#define ntohs(x)   (x)
	#endif
	#ifndef ntohl
	#define ntohl(x)   (x) 
	#endif
	#ifndef htons
	#define htons(x)   (x)
	#endif
	#ifndef htonl
	#define htonl(x)   (x)
	#endif
	
	#define GET_UINT16_UNALIGNED( ptr )  			GET_UINT16_BIG_ENDIAN_UNALIGNED(ptr)
	#define SET_UINT16_UNALIGNED(  u16value, ptr )	SET_UINT16_BIG_ENDIAN_UNALIGNED(  u16value, ptr )
	#define GET_UINT32_UNALIGNED( ptr )  			GET_UINT32_BIG_ENDIAN_UNALIGNED(ptr)
	#define SET_UINT32_UNALIGNED(  u32value, ptr )	SET_UINT32_BIG_ENDIAN_UNALIGNED(  u32value,ptr)
	#define PKTGET_UINT16_UNALIGNED( ptr )  			GET_UINT16_BIG_ENDIAN_UNALIGNED(ptr)
	#define PKTSET_UINT16_UNALIGNED(  u16value, ptr)	SET_UINT16_BIG_ENDIAN_UNALIGNED(  u16value, ptr )
	#define PKTGET_UINT32_UNALIGNED( ptr )  			GET_UINT32_BIG_ENDIAN_UNALIGNED(ptr)
	#define PKTSET_UINT32_UNALIGNED(  u32value, ptr)	SET_UINT32_BIG_ENDIAN_UNALIGNED(  u32value,ptr)	


#endif


#ifndef __offsetof
#define __offsetof(type, field) ((size_t)(&((type *)0)->field))
#endif

#ifndef offsetof
#define offsetof(type, field) __offsetof(type, field)
#endif

#endif
