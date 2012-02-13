/*
 * Copyright c                Realtek Semiconductor Corporation, 2002
 * All rights reserved.                                                    
 * 
 * $Header: /home/cvsroot/uClinux-dist/user/init/rtl_bdinfo.h,v 1.1.1.1 2003/08/27 06:20:18 rupert Exp $
 *
 * $Author: rupert $
 *
 * Abstract: board info access module exported services
 *
 * $Log: rtl_bdinfo.h,v $
 * Revision 1.1.1.1  2003/08/27 06:20:18  rupert
 * uclinux initial
 *
 * Revision 1.1.1.1  2003/08/27 03:08:52  rupert
 *  initial version 
 *
 * Revision 1.6  2003/06/23 10:57:23  elvis
 * change include path of  rtl_types.h
 *
 * Revision 1.5  2003/05/20 08:44:49  elvis
 * change the include path of "rtl_types.h"
 *
 * Revision 1.4  2003/05/06 12:30:19  orlando
 * modify to work with new boot/loader.
 *
 * Revision 1.3  2003/05/05 07:18:20  orlando
 * add bdinfo_init() api.
 *
 * Revision 1.2  2003/05/02 09:46:17  orlando
 * move BDINFO_ADDR to rtl_bdinfo.h.
 *
 * Revision 1.4  2003/05/02 04:26:46  orlando
 * add bdinfo_setMac().
 *
 * Revision 1.3  2003/04/11 08:04:07  orlando
 * modify bdinfo_t definition to even-sized.
 *
 * Revision 1.2  2003/04/11 06:02:54  orlando
 * fix bug.
 *
 * Revision 1.1  2003/04/10 13:05:06  orlando
 * bdinfo module initial check-in.
 *
 */

#ifndef _RTL_BDINFO_H_
#define _RTL_BDINFO_H_

#include "rtl_types.h"
#include "flash_map.h"

/*
 * board info set to log image addr due to gdb.bix is larger than 64K,
 * which is therefore padded to 128K.
 */
/*
	#define BDINFO_ADDR FLASH_MAP_BOARD_INFO_ADDR  --> occupied by gdb (128K) now (2003/04/11)
	#define BDINFO_ADDR FLASH_MAP_LOG_IMAGE_ADDR --> access fail at this address ???
*/
#ifdef GDB_LOADER
	/* to be compatible with existing map */	
	#define BDINFO_ADDR 0xbfcd0000
#else
	/* new map since 2003.4.21 */
	//#define BDINFO_ADDR FLASH_MAP_BOARD_INFO_ADDR
	#define BDINFO_ADDR 0xbfc04000
#endif

typedef struct bdinfo_s {
	macaddr_t mac;           /* mac address */
    uint8 macNbr;            /* mac address number */
    uint8 rsv1;
} bdinfo_t;

/* exported function prototypes */
//int32 cmdBdinfo(uint32 channel, int32 argc, int8 ** argv);

/* function: bdinfo_init
 * decription: bdinfo module init function
 * return  none
 */
void bdinfo_init(void);

/* function: bdinfo_getMac
 * return  0  when successful
 *         -1 when fail
 * note: caller should allocate buffer for mac_P
 */
int32 bdinfo_getMac(macaddr_t *mac_P);

/* function: bdinfo_getMacNbr
 * return  0  when successful
 *         -1 when fail
 * note: caller should allocate buffer for pMacNbr
 */
int32 bdinfo_getMacNbr(uint32 * pMacNbr);

/* function: bdinfo_setMac
 * return  0  when successful
 *         -1 when fail
 * note: caller should allocate buffer for mac_P
 */
int32 bdinfo_setMac(macaddr_t *mac_P);

/* function: bdinfo_setMacNbr
 * return  0  when successful
 *         -1 when fail
 * note: macNbr >= 1 and macNbr <= 255
 */
int32 bdinfo_setMacNbr(uint32 macNbr);

#endif /* _RTL_BDINFO_H_ */
