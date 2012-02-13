/*
* --------------------------------------------------------------------
* Copyright c                  Realtek Semiconductor Corporation, 2003
* All rights reserved.
*
* Program : Header file for all crypto sources
* Abstract : This file includes all common definitions of crypto engine.
* $Id: crypto.h,v 1.2 2004/12/22 08:48:08 yjlou Exp $
* $Log: crypto.h,v $
* Revision 1.2  2004/12/22 08:48:08  yjlou
* +: add 16 NOPs when polling AuthEngine
*
* Revision 1.1  2004/06/23 09:18:57  yjlou
* +: support 865xB CRYPTO Engine
*   +: CONFIG_RTL865XB_EXP_CRYPTOENGINE
*   +: basic encry/decry functions (DES/3DES/SHA1/MAC)
*   +: old-fashion API (should be removed in next version)
*   +: batch functions (should be removed in next version)
*
*
*
* --------------------------------------------------------------------
*/

#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#define UNCACHED_MALLOC(x)  (void *) (0xa0000000 | (uint32) rtlglue_malloc(x))
#define UNCACHED_FREE(x)  rtlglue_free( (void *) (~0x20000000 & (uint32) x ) )

#define printfByPolling rtlglue_printf

#define IDLE_CPU() \
	do {\
		asm( "\
			nop; nop; nop; nop; nop; nop; nop; nop; \
			nop; nop; nop; nop; nop; nop; nop; nop; \
			"); \
	} while (0) 

#endif// __CRYPTO_H__
