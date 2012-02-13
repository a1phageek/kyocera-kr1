/*
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* Program : Performance Profiling for ROME Driver
* Abstract : 
* Author : Yung-Chieh Lo (yjlou@realtek.com.tw)               
* $Id: romeperf.c,v 1.7 2005/01/18 07:46:07 yjlou Exp $
*/

#include "romeperf.h"
#include "rtl865x/rtl_glue.h"
#define KERNEL_SYSeALLS 
#include <asm/unistd.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include <asm/mipsregs.h>

/* Local variables */
static uint64 tempVariable64;
static uint32 tempVariable32;

/* Global variables */
uint64 cnt1, cnt2;
rtl8651_romeperf_stat_t romePerfStat[ROMEPERF_INDEX_MAX];
uint32 rtl8651_romeperf_inited = 0;
uint32 rtl8651_romeperf_enable = TRUE;

inline void CP3_COUNTER0_INIT( void )
{
__asm__ \
("  ;\
	mfc0	$8, $12			;\
	nop						;\
	la		$9, 0x80000000	;\
	or		$8, $9			;\
	mtc0	$8, $12			;\
	nop						;\
	nop						;\
");
}

inline uint32 CP3_COUNTER0_IS_INITED( void )
{
__asm__ \
("  ;\
	mfc0	$8, $12			;\
	la		$9, tempVariable32;\
	nop						;\
	sw		$8, 0($9)		;\
	nop						;\
");
	return tempVariable32;
}

inline void CP3_COUNTER0_START( void )
{
__asm__ \
("  ;\
	li		$8, 0x00000010	;\
	ctc3 	$8, $0			;\
	nop						;\
");
}

inline void CP3_COUNTER0_STOP( void )
{
__asm__ \
("	;\
	ctc3 	$0, $0			;\
	nop						;\
");
}

inline uint64 CP3_COUNTER0_GET( void )
{
__asm__ \
("	;\
	la		$8, tempVariable64;\
	nop						;\
	mfc3	$9, $9			;\
	nop						;\
	nop						;\
	sw		$9, 0($8)		;\
	nop						;\
	nop						;\
	mfc3	$9, $8			;\
	nop						;\
	nop						;\
	sw		$9, 4($8)		;\
	nop						;\
	nop						;\
");
	return tempVariable64;
}

int32 rtl8651_romeperfInit()
{
	CP3_COUNTER0_INIT();
	CP3_COUNTER0_START();

	rtl8651_romeperf_inited = TRUE;
	rtl8651_romeperf_enable = TRUE;
	memset( &romePerfStat, 0, sizeof( romePerfStat ) );

	romePerfStat[ROMEPERF_INDEX_NAPT_ADD].desc = "NAPT add_all";
	romePerfStat[ROMEPERF_INDEX_NAPT_ADD_1].desc = "NAPT add_checkIntIP";
	romePerfStat[ROMEPERF_INDEX_NAPT_ADD_2].desc = "NAPT add_localServer";
	romePerfStat[ROMEPERF_INDEX_NAPT_ADD_3].desc = "NAPT add_checkExtIp";
	romePerfStat[ROMEPERF_INDEX_NAPT_ADD_4].desc = "NAPT add_dupCheck1";
	romePerfStat[ROMEPERF_INDEX_NAPT_ADD_5].desc = "NAPT add_dupCheck2";
	romePerfStat[ROMEPERF_INDEX_NAPT_ADD_6].desc = "NAPT add_bPortReused";
	romePerfStat[ROMEPERF_INDEX_NAPT_ADD_7].desc = "NAPT add_routeCache";
	romePerfStat[ROMEPERF_INDEX_NAPT_ADD_8].desc = "NAPT add_tooManyConn";
	romePerfStat[ROMEPERF_INDEX_NAPT_ADD_9].desc = "NAPT add_initConn";
	romePerfStat[ROMEPERF_INDEX_NAPT_ADD_10].desc = "NAPT add_decisionFlo";
	romePerfStat[ROMEPERF_INDEX_NAPT_ADD_11].desc = "NAPT add_ambiguous";
	romePerfStat[ROMEPERF_INDEX_NAPT_DEL].desc = "NAPT del";
	romePerfStat[ROMEPERF_INDEX_NAPT_FIND_OUTBOUND].desc = "NATP outbound";
	romePerfStat[ROMEPERF_INDEX_NAPT_FIND_INBOUND].desc = "NAPT inbound";
	romePerfStat[ROMEPERF_INDEX_NAPT_UPDATE].desc = "NAPT update";
	romePerfStat[ROMEPERF_INDEX_UNTIL_RXTHREAD].desc = "IntDispatch-RxThread";
	romePerfStat[ROMEPERF_INDEX_RECVLOOP].desc = "RecvLoop-FwdInput";
	romePerfStat[ROMEPERF_INDEX_FWDENG_INPUT].desc = "FwdEng_Input()";
	romePerfStat[ROMEPERF_INDEX_BEFORE_CRYPTO_ENCAP].desc = "FwdInput-Crypto(En)";
	romePerfStat[ROMEPERF_INDEX_ENCAP].desc = "IPSEC Encap";
	romePerfStat[ROMEPERF_INDEX_ENCAP_CRYPTO_ENGINE].desc = "Encap Crypto";
	romePerfStat[ROMEPERF_INDEX_ENCAP_AUTH_ENGINE].desc = "Encap Authtication";
	romePerfStat[ROMEPERF_INDEX_BEFORE_CRYPTO_DECAP].desc = "FwdInput-Crypto(De)";
	romePerfStat[ROMEPERF_INDEX_DECAP].desc = "IPSEC Decap";
	romePerfStat[ROMEPERF_INDEX_DECAP_CRYPTO_ENGINE].desc = "Decap Crypto";
	romePerfStat[ROMEPERF_INDEX_DECAP_AUTH_ENGINE].desc = "Decap Authtication";
	romePerfStat[ROMEPERF_INDEX_FASTPATH].desc = "Fast Path";
	romePerfStat[ROMEPERF_INDEX_SLOWPATH].desc = "Slow Path";
	romePerfStat[ROMEPERF_INDEX_FWDENG_SEND].desc = "FwdEngSend()";
	romePerfStat[ROMEPERF_INDEX_UNTIL_ACLDB].desc = "FwdInput() Until ACLDB";
	romePerfStat[ROMEPERF_INDEX_GET_MTU_AND_SOURCE_MAC].desc = "L3Route_MTU_srcMAC";
	romePerfStat[ROMEPERF_INDEX_PPTPL2TP_1].desc = "L3Route_PPTPL2TP_1";
	romePerfStat[ROMEPERF_INDEX_PPPOE_ARP_CACHE].desc = "L3Route_PPPoE_ArpCache";
	romePerfStat[ROMEPERF_INDEX_PPTPL2TP_SEND].desc = "L3Route_PptpL2tpSend()";
	romePerfStat[ROMEPERF_INDEX_FRAG].desc = "L3Route_Fragment";
	romePerfStat[ROMEPERF_INDEX_EGRESS_ACL].desc = "FwdSend_EgressACL";
	romePerfStat[ROMEPERF_INDEX_PPTPL2TP_ENCAP].desc = "FwdSend_PPTP/L2TP_Encap";
	romePerfStat[ROMEPERF_INDEX_FROM_PS].desc = "FwdSend_FromPS";
	romePerfStat[ROMEPERF_INDEX_EXTDEV_SEND].desc = "FwdSend_ExtDevSend()";
	romePerfStat[ROMEPERF_INDEX_FRAG_2ND_HALF].desc = "FwdSend_Frag_2ndHalf()";
	romePerfStat[ROMEPERF_INDEX_12].desc = "FwdEng_temp_12";
	romePerfStat[ROMEPERF_INDEX_13].desc = "FwdEng_temp_13";
	romePerfStat[ROMEPERF_INDEX_14].desc = "FwdEng_temp_14";
	romePerfStat[ROMEPERF_INDEX_15].desc = "FwdEng_temp_15";
	romePerfStat[ROMEPERF_INDEX_16].desc = "FwdEng_temp_16";
	romePerfStat[ROMEPERF_INDEX_IRAM_1].desc = "IRAM Cached within IRAM";
	romePerfStat[ROMEPERF_INDEX_IRAM_2].desc = "IRAM Uncached within IRAM";
	romePerfStat[ROMEPERF_INDEX_IRAM_3].desc = "test ICACHE  (1024*100)";
	romePerfStat[ROMEPERF_INDEX_IRAM_4].desc = "test Uncached (1024*10)";
	romePerfStat[ROMEPERF_INDEX_DRAM_1].desc = "DRAM Cached within IRAM";
	romePerfStat[ROMEPERF_INDEX_DRAM_2].desc = "DRAM Uncached within IRAM";
	romePerfStat[ROMEPERF_INDEX_DRAM_3].desc = "test DCACHE  (1024*100)";
	romePerfStat[ROMEPERF_INDEX_DRAM_4].desc = "test Uncached (1024*10)";

	return SUCCESS;
}

int32 rtl8651_romeperfReset()
{
	rtl8651_romeperfInit();
	
	return SUCCESS;
}

#if 0/* old fashion function, for reference only. */
int32 rtl8651_romeperfStart()
{
	if ( rtl8651_romeperf_inited == FALSE ) rtl8651_romeperfInit();
	
	START_AND_GET_CP3_COUNTER0( cnt1 );

	return SUCCESS;
}

int32 rtl8651_romeperfStop( uint64 *pDiff )
{
	if ( rtl8651_romeperf_inited == FALSE ) rtl8651_romeperfInit();
	
	STOP_AND_GET_CP3_COUNTER0( cnt2 );

	*pDiff = cnt2 - cnt1;
	return SUCCESS;
}
#endif

int32 rtl8651_romeperfGet( uint64 *pGet )
{
	if ( rtl8651_romeperf_inited == FALSE ) return FAILED;

	/* Louis patch: someone will disable CP3 in somewhere. */
	CP3_COUNTER0_INIT();

	CP3_COUNTER0_STOP();
	*pGet = CP3_COUNTER0_GET();
	CP3_COUNTER0_START();
	
	return SUCCESS;
}

int32 rtl8651_romeperfPause( void )
{
	if ( rtl8651_romeperf_inited == FALSE ) return FAILED;

	rtl8651_romeperf_enable = FALSE;
	
	/* Louis patch: someone will disable CP3 in somewhere. */
	CP3_COUNTER0_INIT();

	CP3_COUNTER0_STOP();
	
	return SUCCESS;
}

int32 rtl8651_romeperfResume( void )
{
	if ( rtl8651_romeperf_inited == FALSE ) return FAILED;

	rtl8651_romeperf_enable = TRUE;
	
	/* Louis patch: someone will disable CP3 in somewhere. */
	CP3_COUNTER0_INIT();
 	
	CP3_COUNTER0_START();
	
	return SUCCESS;
}

int32 rtl8651_romeperfEnterPoint( uint32 index )
{
	if ( rtl8651_romeperf_inited == FALSE ||
	     rtl8651_romeperf_enable == FALSE ) return FAILED;
	if ( index >= (sizeof(romePerfStat)/sizeof(rtl8651_romeperf_stat_t)) )
		return FAILED;

	/* Louis patch: someone will disable CP3 in somewhere. */
	CP3_COUNTER0_INIT();

	CP3_COUNTER0_STOP();
	romePerfStat[index].tempCycle = CP3_COUNTER0_GET();
	romePerfStat[index].hasTempCycle = TRUE;
	CP3_COUNTER0_START();
	
	return SUCCESS;
}

int32 rtl8651_romeperfExitPoint( uint32 index )
{
	uint64 diff;
	
	if ( rtl8651_romeperf_inited == FALSE ||
	     rtl8651_romeperf_enable == FALSE ) return FAILED;
	if ( index >= (sizeof(romePerfStat)/sizeof(rtl8651_romeperf_stat_t)) )
		return FAILED;
	if ( romePerfStat[index].hasTempCycle == FALSE )
		return FAILED;

	/* Louis patch: someone will disable CP3 in somewhere. */
	CP3_COUNTER0_INIT();
	
	CP3_COUNTER0_STOP();
	diff = CP3_COUNTER0_GET() - romePerfStat[index].tempCycle;
	romePerfStat[index].hasTempCycle = FALSE;
	romePerfStat[index].accCycle += diff;
	romePerfStat[index].executedNum++;
	CP3_COUNTER0_START();
	
	return SUCCESS;
}

int32 rtl8651_romeperfDump()
{
	int i;

	rtlglue_printf( "index %30s %12s %8s %10s\n", "description", "accCycle", "totalNum", "Average" );
	for( i = 0; i < (sizeof(romePerfStat)/sizeof(rtl8651_romeperf_stat_t)); i++ )
	{
		if ( romePerfStat[i].executedNum == 0 )
		{
			rtlglue_printf( "[%3d] %30s %12s %8s %10s\n", i, romePerfStat[i].desc, "--", "--", "--" );
		}
		else
		{
			uint32 *pAccCycle = (uint32*)&romePerfStat[i].accCycle;
			rtlglue_printf( "[%3d] %30s %12lld %8d %10d\n", i, romePerfStat[i].desc, romePerfStat[i].accCycle, romePerfStat[i].executedNum, 
			                /* Hi-word */ (pAccCycle[0]*(0xffffffff/romePerfStat[i].executedNum)) +
			                /* Low-word */(pAccCycle[1]/romePerfStat[i].executedNum) );
		}
	}
	
	return SUCCESS;
}
