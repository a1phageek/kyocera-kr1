/*
 * Copyright c                Realtek Semiconductor Corporation, 2002
 * All rights reserved.                                                    
 * 
 * $Header: /home/cvsroot/uClinux-dist/user/init/rtl_flashdrv.h,v 1.1.1.1 2003/08/27 06:20:19 rupert Exp $
 *
 * $Author: rupert $
 *
 * Abstract:
 *
 *   Flash driver header file for export include.
 *
 * $Log: rtl_flashdrv.h,v $
 * Revision 1.1.1.1  2003/08/27 06:20:19  rupert
 * uclinux initial
 *
 * Revision 1.1.1.1  2003/08/27 03:08:52  rupert
 *  initial version 
 *
 * Revision 1.3  2003/06/23 11:03:39  elvis
 * change include path of  rtl_types.h
 *
 * Revision 1.2  2003/05/20 08:45:36  elvis
 * change the include path of "rtl_types.h"
 *
 * Revision 1.1  2003/04/29 14:18:51  orlando
 * flashdrv module initial check-in (used by cfgmgr, ported from srcroot).
 *
 * Revision 1.1  2002/07/19 08:39:14  danwu
 * Create file.
 *
 *
 * 
 */

#ifndef _RTL_FLASHDRV_H_
#define _RTL_FLASHDRV_H_



#include "rtl_types.h"

typedef struct flashdriver_obj_s {
	uint32  flashSize;
	uint32  flashBaseAddress;
	uint32 *blockBaseArray_P;
	uint32  blockBaseArrayCapacity;
	uint32  blockNumber;
} flashdriver_obj_t;



/*
 * FUNCTION PROTOTYPES
 */
uint32 flashdrv_init(flashdriver_obj_t * const drvObj_P);
uint32 flashdrv_eraseBlock(void *startAddr_P);
uint32 flashdrv_read (void *dstAddr_P, void *srcAddr_P, uint32 size);
uint32 flashdrv_write(void *dstAddr_P, void *srcAddr_P, uint32 size);
uint32 flashdrv_updateImg(void *srcAddr_P, void *dstAddr_P, uint32 size);

#endif  /* _RTL_FLASHDRV_H_ */

