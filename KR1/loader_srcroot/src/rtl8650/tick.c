/*
* ----------------------------------------------------------------
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* $Header: /home/cvsroot/uClinux-dist/loader_srcroot/src/rtl8650/tick.c,v 1.8 2004/08/26 13:53:27 yjlou Exp $
*
* Abstract: Tick timer driver.
*
* $Author: yjlou $
*
* $Log: tick.c,v $
* Revision 1.8  2004/08/26 13:53:27  yjlou
* -: remove all warning messages!
* +: add compile flags "-Wno-implicit -Werror" in Makefile to treat warning as error!
*
* Revision 1.7  2004/07/14 02:16:09  yjlou
* +: add '#ifdef FAT_CODE' to remove un-used functions
*
* Revision 1.6  2004/05/26 06:51:49  yjlou
* *: use IS_865XB() instead of IS_REV_B()
* *: use IS_865XA() instead of IS_REV_A()
*
* Revision 1.5  2004/05/12 06:36:16  yjlou
* *: tick_Delay10ms() and tick_Delay100ms() use polling mode instead.
*
* Revision 1.4  2004/05/06 03:51:09  yjlou
* *: use IS_REV_B() to set CDBR
*
* Revision 1.3  2004/03/31 01:49:20  yjlou
* *: all text files are converted to UNIX format.
*
* Revision 1.2  2004/03/30 11:34:38  yjlou
* *: commit for 80B IC back
*   +: system clock rate definitions have changed.
*   *: fixed the bug of BIST_READY_PATTERN
*   +: clean all ASIC table when init ASIC.
* -: define _L2_MODE_ to support L2 switch mode.
*
* Revision 1.1  2004/03/16 06:36:13  yjlou
* *** empty log message ***
*
* Revision 1.2  2004/03/09 00:45:38  danwu
* remove unused code to shrink loader image size under 0xc000 and only flash block 0 & 3 occupied
*
* Revision 1.1.1.1  2003/09/25 08:16:55  tony
*  initial loader tree 
*
* Revision 1.2  2003/06/25 06:31:00  danwu
* add tick_pollStart() and tick_pollGet10MS() to provide timing while
* interrupts are disabled
*
* Revision 1.1.1.1  2003/05/07 08:16:06  danwu
* no message
*
* ---------------------------------------------------------------
*/

#include <rtl_types.h>
#include "board.h"
#include <rtl8650/asicregs.h>
#include <tick.h>



/* STATIC VARIABLE DECLARATIONS
 */
static uint32      SysClkRate;
static uint64      SysUpTime;
static uint32	   TimerCounter;
#ifdef FAT_CODE
static uint32      isWatchdogReset;
#endif// FAT_CODE
static uint32      timer_test;



/* LOCAL SUBPROGRAM SPECIFICATIONS
 */
static void tick_IntHandler();
static void tick_testIntHandler();
static void tick_watchdogInit(void);



/*****************************************************************************/
/* initialization function                                                   */
/*****************************************************************************/
int32 tick_initTest()
{
#define DIVISOR     0xE
    int32   result = 0;
    
    REG32(MACMR) = 0;
    switch ( REG32(MACMR) & SYS_CLK_MASK )
    {
        case SYS_CLK_100M:
            SysClkRate = 100000000;
            break;
        case SYS_CLK_90M:
            SysClkRate = 90000000;
            break;
        case SYS_CLK_85M:
            SysClkRate = 85000000;
            break;
        case SYS_CLK_96M:
            SysClkRate = 96000000;
            break;
        case SYS_CLK_80M:
            SysClkRate = 80000000;
            break;
        case SYS_CLK_75M:
            SysClkRate = 75000000;
            break;
        case SYS_CLK_70M:
            SysClkRate = 70000000;
            break;
        case SYS_CLK_50M:
            SysClkRate = 50000000;
            break;
        default:
            while(1);
    }
    
    /* Set clock division base */
	if ( IS_865XB() )
		REG32(CDBR) = DIVISOR << DIVF_OFFSET; //50B
	else
		REG32(CDBR) = (DIVISOR - 1) << DIVF_OFFSET; //51
    
#if 0
    tick_watchdogInit();
    
    /* Test counter 0/1 */
    timer_test = 0;
    int_Register(TICK_ILEV, TCIE, TCIRS_OFFSET, tick_testIntHandler);
    REG32(TCCNR) = TC0EN | TC0MODE_COUNTER | TC1EN | TC1MODE_COUNTER;
    REG32(TCIR) = TC0IE | TC1IE;
    setIlev(TICK_ILEV);
    REG32(TC0DATA) = 2 << TCD_OFFSET;
    REG32(TC1DATA) = 2 << TCD_OFFSET;
    for (i=0; i<0xffff; i++)
    {
        imask = setIlev(TICK_ILEV + 1);
        if ( timer_test == 3 )
            break;
        setIlev(imask);
    }
    result = ~timer_test & 3;
    int_unRegister(TICK_ILEV, TCIE, TCIRS_OFFSET, tick_testIntHandler);
#endif
    
    /* Set timer mode and Enable timer */
    REG32(TCCNR) = TC0EN | TC0MODE_TIMER;
    
    /* Set timeout per msec */
    REG32(TC0DATA) = (((SysClkRate / DIVISOR) / TICK_FREQ) + 1) << TCD_OFFSET;

    /* Install interrupt handler */
    int_Register(TICK_ILEV, TCIE, TCIRS_OFFSET, tick_IntHandler);
    
    /* Initialize system up time */
    SysUpTime = 0;
    
    /* Enable timer interrupt */
    REG32(TCIR) = TC0IE;
    if ( TICK_ILEV < getIlev() )
    	setIlev(TICK_ILEV);
    
    return result;
}


uint32 tick_getSysClkRate(void)
{
    return SysClkRate;
}
static int sys_led=0;

static int backup=0;
int32 into_backup()
{
	backup = 1;
}


static void tick_IntHandler()
{

    if ( REG32(TCIR) & TC0IP )
    {
        REG32(TCIR) = TC0IP | TC0IE;
        SysUpTime++;
        if (backup)
	{
        	TimerCounter++;
        	if( TimerCounter > 25) // use this interger to define system status LED frequency
        	{
        		
        		if(sys_led)
        		{
        			
        			REG32(PABDAT) |= 0x00002000;
        			sys_led = 0;
        		}
        		else
        		{
        			
        			REG32(PABDAT) &= (~0x00002000);
        			sys_led = 1;
        		}
        		TimerCounter = 0;
		}
	}	
        rtosTickIsr();
    }
}


static void tick_testIntHandler()
{
    if ( REG32(TCIR) & TC0IP )
        timer_test |= 1;
    if ( REG32(TCIR) & TC1IP )
        timer_test |= 2;
    REG32(TCIR) = TC0IP | TC1IP;
}

uint64 tick_GetSysUpTime(uint64	*systime)
{
    uint32 	  imask;
    
    imask = setIlev(TICK_ILEV + 1);
	*systime = SysUpTime;
    setIlev(imask);
    
	return (*systime);
}

void tick_Delay10ms(uint32 ten_msec)
{
#if 0
    uint64     stime;
    uint64     ctime;
    uint64     etime;

    tick_GetSysUpTime(&stime);
    etime = (100 / TICK_FREQ) * ten_msec;
    
    while (TRUE)
    {
        if (tick_GetSysUpTime(&ctime) < stime)
        {  /* wrapped */
        	if ((stime - ctime) >= etime)
         	break;
        }
        else
        {
        	if (ctime >= (stime + etime))
            break;
      }
   }
#else//0
	// polling mode
	int i;
	for(i=0;i<ten_msec;i++)
	{
		tick_pollStart();
		while(tick_pollGet10MS()==0);
	}
#endif//0
}

void tick_Delay100ms(uint32 hundred_msec)
{
#if 0
    uint64     stime;
    uint64     ctime;
    uint64     etime;

    tick_GetSysUpTime(&stime);
    etime = (1000 / TICK_FREQ) * hundred_msec;
    
    while (TRUE)
    {
        if (tick_GetSysUpTime(&ctime) < stime)
        {  /* wrapped */
        	if ((stime - ctime) >= etime)
         	break;
        }
        else
        {
        	if (ctime >= (stime + etime))
            break;
      }
   }
#else//0
	// polling mode
	int i;
	for(i=0;i<(hundred_msec<<3)+(hundred_msec<<1);i++)
	{
		tick_pollStart();
		while(tick_pollGet10MS()==0);
	}
#endif//0
}

/* This function is used when interrupts must be disabled */
static uint32 pstime;
static uint32 p10ms;
static uint32 ptoggle;

void tick_pollStart(void)
{
    pstime = REG32(TC0CNT);
    p10ms = 0;
    ptoggle = 0;
}

uint32 tick_pollGet10MS(void)
{
    if (REG32(TC0CNT) > pstime)
        ptoggle = 1;
    else if (ptoggle == 1)
    {
        p10ms++;
        ptoggle = 0;
    }
    
    return p10ms;
}


#if 0
static void tick_watchdogInit(void)
{
    isWatchdogReset = REG32(WDTCNR) & WDTIND;
    REG32(WDTCNR) = (WDSTOP_PATTERN << WDTE_OFFSET) | WDTCLR | OVSEL_18;
}

void tick_watchdogStart(void)
{
    REG32(WDTCNR) &= ~(0xff << WDTE_OFFSET);
}

void tick_watchdogStop(void)
{
    REG32(WDTCNR) |= (WDSTOP_PATTERN << WDTE_OFFSET);
}

void tick_watchdogClear(void)
{
    REG32(WDTCNR) |= WDTCLR;
}

int32 tick_isWatchdogReset(void)
{
    return (int32) isWatchdogReset;
}
    
void tick_cmdTestWdt(uint32 channel, int32 argc, int8 ** argv)
{
    tick_watchdogStart();
    printf("\nWatchdog will reset the system!");
}
#endif
