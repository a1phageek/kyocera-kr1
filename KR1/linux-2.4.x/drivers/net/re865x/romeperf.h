/*
 *	romereal.h
 */

#ifndef _ROMEPERF_H_
#define _ROMEPERF_H_

#include "rtl865x/rtl_types.h"
#include <asm/mipsregs.h>


struct rtl8651_romeperf_stat_s {
	char *desc;
	uint64 accCycle;
	uint64 tempCycle;
	uint32 executedNum;
	uint32 hasTempCycle:1; /* true if tempCycle is valid. */
};
typedef struct rtl8651_romeperf_stat_s rtl8651_romeperf_stat_t;


/* for rtl8651_romeperfEnterPoint() and rtl8651_romeperfExitPoint() */
#define ROMEPERF_INDEX_NAPT_ADD 0
#define ROMEPERF_INDEX_NAPT_ADD_1 1
#define ROMEPERF_INDEX_NAPT_ADD_2 2
#define ROMEPERF_INDEX_NAPT_ADD_3 3
#define ROMEPERF_INDEX_NAPT_ADD_4 4
#define ROMEPERF_INDEX_NAPT_ADD_5 5
#define ROMEPERF_INDEX_NAPT_ADD_6 6
#define ROMEPERF_INDEX_NAPT_ADD_7 7
#define ROMEPERF_INDEX_NAPT_ADD_8 8
#define ROMEPERF_INDEX_NAPT_ADD_9 9
#define ROMEPERF_INDEX_NAPT_ADD_10 10
#define ROMEPERF_INDEX_NAPT_ADD_11 11
#define ROMEPERF_INDEX_NAPT_DEL 12
#define ROMEPERF_INDEX_NAPT_FIND_OUTBOUND 13
#define ROMEPERF_INDEX_NAPT_FIND_INBOUND 14
#define ROMEPERF_INDEX_NAPT_UPDATE 15
#define ROMEPERF_INDEX_UNTIL_RXTHREAD 20
#define ROMEPERF_INDEX_RECVLOOP 21
#define ROMEPERF_INDEX_FWDENG_INPUT 22
#define ROMEPERF_INDEX_BEFORE_CRYPTO_ENCAP 23
#define ROMEPERF_INDEX_ENCAP 24
#define ROMEPERF_INDEX_ENCAP_CRYPTO_ENGINE 25
#define ROMEPERF_INDEX_ENCAP_AUTH_ENGINE 26
#define ROMEPERF_INDEX_BEFORE_CRYPTO_DECAP 27
#define ROMEPERF_INDEX_DECAP 28
#define ROMEPERF_INDEX_DECAP_CRYPTO_ENGINE 29
#define ROMEPERF_INDEX_DECAP_AUTH_ENGINE 30
#define ROMEPERF_INDEX_FASTPATH 31
#define ROMEPERF_INDEX_SLOWPATH 32
#define ROMEPERF_INDEX_UNTIL_ACLDB 33
#define ROMEPERF_INDEX_GET_MTU_AND_SOURCE_MAC 34
#define ROMEPERF_INDEX_PPTPL2TP_1 35
#define ROMEPERF_INDEX_PPPOE_ARP_CACHE 36
#define ROMEPERF_INDEX_PPTPL2TP_SEND 37
#define ROMEPERF_INDEX_FRAG 38
#define ROMEPERF_INDEX_FWDENG_SEND 39
#define ROMEPERF_INDEX_EGRESS_ACL 40
#define ROMEPERF_INDEX_PPTPL2TP_ENCAP 41
#define ROMEPERF_INDEX_FROM_PS 42
#define ROMEPERF_INDEX_EXTDEV_SEND 43
#define ROMEPERF_INDEX_FRAG_2ND_HALF 44
#define ROMEPERF_INDEX_12 45
#define ROMEPERF_INDEX_13 46
#define ROMEPERF_INDEX_14 47
#define ROMEPERF_INDEX_15 48
#define ROMEPERF_INDEX_16 49
#define ROMEPERF_INDEX_IRAM_1 50
#define ROMEPERF_INDEX_IRAM_2 51
#define ROMEPERF_INDEX_IRAM_3 52
#define ROMEPERF_INDEX_IRAM_4 53
#define ROMEPERF_INDEX_DRAM_1 54
#define ROMEPERF_INDEX_DRAM_2 55
#define ROMEPERF_INDEX_DRAM_3 56
#define ROMEPERF_INDEX_DRAM_4 57
#define ROMEPERF_INDEX_MAX 58

int32 rtl8651_romeperfInit( void );
inline int32 rtl8651_romeperfStart(void);
inline int32 rtl8651_romeperfStop(void);
int32 rtl8651_romeperfReset( void );
int32 rtl8651_romeperfEnterPoint( uint32 index );
int32 rtl8651_romeperfExitPoint( uint32 index );
int32 rtl8651_romeperfDump( void );
int32 rtl8651_romeperfPause( void );
int32 rtl8651_romeperfResume( void );
int32 rtl8651_romeperfGet( uint64 *pGet );

extern rtl8651_romeperf_stat_t romePerfStat[ROMEPERF_INDEX_MAX];



#endif/* _ROMEPERF_H_ */
