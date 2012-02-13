/*
* ----------------------------------------------------------------
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* $Header: /home/cvsroot/uClinux-dist/loader_srcroot/src/loader.c,v 1.30 2004/09/13 11:12:50 yjlou Exp $
*
* Abstract: Loader main source code.
*
* $Author: yjlou $
* 
* Revision 1.4  2003/08/29 08:56:15  danwu
* set mcr according to flash size
*
* $Log: loader.c,v $
* Revision 1.30  2004/09/13 11:12:50  yjlou
* *: Loader upgrades to "00.00.13".
* +: support S29GL032MR4/AM29LV320DB/AM29LV320DT flash
*
* Revision 1.29  2004/08/27 09:55:35  yjlou
* *: fixed the bug of 00.00.11: The old runtime code cannot read loader version.
*    New loader will be compatible to old runtime code (keep call-back function).
*
* Revision 1.28  2004/08/26 13:53:27  yjlou
* -: remove all warning messages!
* +: add compile flags "-Wno-implicit -Werror" in Makefile to treat warning as error!
*
* Revision 1.27  2004/08/26 13:20:00  yjlou
* *: Loader upgrades to "00.00.12".
* +: support "Loader Segment Descriptors Table"
* -: remove romcopystart/pause/resume
*
* Revision 1.26  2004/08/23 07:08:30  yjlou
* *: loader upgrades to "00.00.11"
* *: loader is moved from 0x80000000 to 0x80500000 (to reduce SDRAM consumption).
* *: communication section is cast to "string", not "function pointer".
*
* Revision 1.25  2004/08/20 10:32:43  yjlou
* +: loader version upgrades to 00.00.10
*    support kernel FS (MTD)
*
* Revision 1.24  2004/08/11 04:00:17  yjlou
* *: _flash_init() is renamed to flashdrv_init()
*
* Revision 1.23  2004/08/10 12:57:31  yjlou
* *: add flashdrv prefix for the following functions:
*      getBoardInfoAddr(void);
*      uint32 getCcfgImageAddr(void);
*      uint32 getRunImageAddr(void);
*
* Revision 1.22  2004/08/04 14:55:23  yjlou
* +: Loader version upgraded to '00.00.09'
* +: support booting from single 8MB/16MB flash (_SUPPORT_LARGE_FLASH_)
* -: merge rtl_bdinfo.h into flashdrv.h
*
* Revision 1.21  2004/07/14 02:16:09  yjlou
* +: add '#ifdef FAT_CODE' to remove un-used functions
*
* Revision 1.20  2004/06/14 09:09:56  yjlou
* *: seed is increased every 2-bytes.
*
* Revision 1.19  2004/05/28 05:54:12  yjlou
* +: Loader version migrates to 00.00.08
* +: support MX29LV320AB and MX29LV320AT
* *: fixed the bug of crash: detect over 4MB in 865x
* *: fixed for 5788A MCR bug.
*
* Revision 1.18  2004/05/26 06:51:49  yjlou
* *: use IS_865XB() instead of IS_REV_B()
* *: use IS_865XA() instead of IS_REV_A()
*
* Revision 1.17  2004/05/26 06:41:23  yjlou
* +: support infinite test flash.
*
* Revision 1.16  2004/05/25 11:28:00  yjlou
* *: fixed the bug : crash when writing default bdinfo with a key stroke.
*                    delay 0.5 sec to print out all messages before reboot.
* +: eraseFlash() and tesfFlash() in 50B also accept address between 0xbfc00000 and 0xc0000000.
*
* Revision 1.15  2004/05/21 11:43:17  yjlou
* +: support hub mode (still buggy) and erase flash function.
*
* Revision 1.14  2004/05/14 09:39:42  orlando
* check in CONFIG_RTL865X_BICOLOR_LED/DIAG_LED/INIT_BUTTON related code
*
* Revision 1.13  2004/05/13 13:27:01  yjlou
* +: loader version is migrated to "00.00.07".
* +: new architecture for INTEL flash (code is NOT verified).
* *: FLASH_BASE is decided by IS_REV_A()
* -: remove flash_map.h (content moved to flashdrv.h)
* -: remove un-necessary calling setIlev()
*
* Revision 1.12  2004/05/12 06:46:49  yjlou
* *: support testing FLASH memory: testFlash()
*
* Revision 1.11  2004/05/10 10:49:40  yjlou
* *: sharing flashdrv with goahead
*
* Revision 1.10  2004/05/10 02:13:03  yjlou
* -: remove the VT100 chars that reseting terminal at booting.
*
* Revision 1.9  2004/05/10 02:06:17  yjlou
* *: reset terminal at booting
*
* Revision 1.8  2004/05/06 03:52:04  yjlou
* *: correct the message of SysClkRate and MemoryclockRate in 8650A chip.
*
* Revision 1.7  2004/05/04 04:01:23  yjlou
* *: In run_without_update(), download file again if download failed. (origin: execute runtime if download failed)
* +: wait pressed key after initializing bdinfo.
* *: smartReboot() was changed (still buggy).
*
* Revision 1.6  2004/04/01 15:52:49  yjlou
* +: support detecting SDRAM size.
* *: MCR will be touched. Therefore, there is still a bug in 51B when detecting multiple flash chips.
*
* Revision 1.5  2004/03/30 11:34:38  yjlou
* *: commit for 80B IC back
*   +: system clock rate definitions have changed.
*   *: fixed the bug of BIST_READY_PATTERN
*   +: clean all ASIC table when init ASIC.
* -: define _L2_MODE_ to support L2 switch mode.
*
* Revision 1.4  2004/03/29 03:46:16  yjlou
* +: upgrade version number to "00.00.05"
*
* Revision 1.3  2004/03/26 10:10:01  yjlou
* *: change bdinfo, ccfg, and runtime offset from const to variable.
*    Therefore, this version of loader can autosense the flash type and address map.
*    Now, we support AMD, fujitsu, ST(for Buffalo) flashes.
* +: support ICE download mode
* *: fixed the bug: crash after setting default board info.
*
* Revision 1.2  2004/03/22 05:54:55  yjlou
* +: support two flash chips.
*
* Revision 1.1  2004/03/16 06:36:13  yjlou
* *** empty log message ***
*
* Revision 1.4  2004/03/16 06:04:04  yjlou
* +: support pure L2 switch mode (for hardware testing)
*
* Revision 1.3  2004/03/08 13:38:54  danwu
* shrink loader image size under 0xc000 and only flash block 0 & 3 occupied
* use default phy settings (100Mbps, instead of 10Mbps)
*
* Revision 1.2  2003/10/14 07:02:45  tony
* support auto config when start at first time.
*
* Revision 1.1.1.1  2003/09/25 08:16:55  tony
*  initial loader tree 
*
* Revision 1.3  2003/06/25 06:26:28  danwu
* add functionality - Load Run Image Without Updating Flash
* fill function pointer in commun section to let runtime read loader version
*
* Revision 1.2  2003/06/19 05:29:31  danwu
* add verification for MAC input
*
* Revision 1.1.1.1  2003/05/07 08:16:06  danwu
* no message
*
* ---------------------------------------------------------------
*/

#include <rtl_types.h>
#include <linux/autoconf.h>
#include "board.h"
#include <uart.h>
#include <rtl8650/swCore.h>
#include <rtl8650/swNic_poll.h>
#include <rtl8650/phy.h>
#include <flashdrv.h>
#include <rtl_image.h>
#include <rtl_flashdrv.h>
#include <semaphore.h>
#include <rtl8650/asicregs.h>
#include <rtl_errno.h>
#include "./gzip/gzip.h"
#include "./uip/uip.h"
#include "fw_ver.h"

const int8 disp_screenrest[]   =
{
    0x1B, '[', '?', '8', 'l',
    0x1B, '[', '?', '3', 'l',    
    0x1B, '[', '?', '5', 'l',0
};

const int8 home_cmd[] = /* Refer: http://www.fh-jena.de/~gmueller/Kurs_halle/esc_vt100.html */
{
	0x1B, '[', '2', 'J',						/* erase in display, complete display */
	0x1B, '[', '1', ';', '1', 'H', 0			/* Set cursor location to row 1, col 1 */
};

const int8 logo_msg[] = {
	"(c)Copyright Realtek, Inc. 2003" 
	"\nProject ROME LOADER\nVersion "
};

const int8 version_msg[]         =
{
    "00.00.13"
};

extern bdinfo_t	bdinfo;

static int _isBootFromROM;

extern void *   __ghsbegin_heap;
extern void *   __ghsend_stack;
extern uint8 __ghsbegin_commun[0x100]; /* 0x80000300~0x80000400 */

void _start();

void change_config(void);

/*
 *	This function will reboot system from RAM or ROM 
 *	  according to the previous rebooting.
 */
void SmartReboot( void )
{
	void (*app_start)(void);

	if ( _isBootFromROM )
		app_start = (void (*)(void)) flash_chip_info[0].BlockBase;
	else
		app_start = (void (*)(void)) _start;

#ifdef _DEBUG_
   	setIlev(UART0_ILEV);
	printf( "app_start=0x%08X\n\n\n\n", app_start );
	tick_Delay100ms(10);
#endif//_DEBUG_
           
	/* Far Jump Now */
   	setIlev(MAX_ILEV);
	app_start();    			
}


static uint32 download(char type)
{
    uint8 ch;
    uint32 ret;
    uint32 wait = 20; // 2 seconds
    uint32 image_len = 0;
    
/*    if((bdinfo.bootSequence==1)&&(type=='g'))
    {
    	ch = 'b';
    }
    else
    {
    	printf("\nPress 'b' for BOOTP, or 'x' for XMODEM ... (b)");
    	getchar(&ch);
    }

    if ( ch == 'x' )
    {
        printf("\nChange baudrate to 115200, and then press any key to continue ...");
        tick_Delay10ms(5);
        console_setBaudrate(115200);
        getchar(&ch);
        printf("\n");
        
        printf("\nXModem Download ...");
        
        while ( xmodem_Receive(&image_len, (int8 *) DRAM_MAP_DLOAD_BUF_ADDR) != 0 );
        tick_Delay100ms(5);
        printf("SUCCESS !");
        
        printf("\n\nChange baudrate back to 38400, and then press any key to continue ...");
        tick_Delay10ms(5);
        console_setBaudrate(38400);
        getchar(&ch);
        printf("\n");
    }
    else
*/

	/* If the defualt boot sequence is BOOT_FROM_BOOTP, we only wait user's input for 2 seconds. */
	printf("\nPress 'i' for ICE raw mode, or 'b' for BOOTP ... (b)");
	ch = 'b'; // default action
	while( wait-- )
	{
		if ( getch(&ch) ) break;
		
		tick_Delay100ms(1);
	}

	if ( ch == 'i' )
	{
		char szImgLen[10];

	 	printf( "\n\n! Stop MULTI, type: 'memload raw run.bix 0x%08X'\n", DRAM_MAP_DLOAD_BUF_ADDR );
		printf( "  Then, input the image length (in bytes): " );
 		getline( sizeof(szImgLen)-1, szImgLen, 0 );

		image_len = atoi( szImgLen );
		printf( "\n  Image Length: %d\n", image_len );
	}
	else if ( type == 'z')
	{

		printf( "\n\nEnter Backup Mode\n " );
		rtl_vlan_param_t vp;
        
        	if (bdinfo.mac[0]+bdinfo.mac[1]+bdinfo.mac[2]+bdinfo.mac[3]+bdinfo.mac[4]+bdinfo.mac[5]!=0)
            		printf("\nMAC: %02x-%02x-%02x-%02x-%02x-%02x.\n", 
                        	bdinfo.mac[0], bdinfo.mac[1], bdinfo.mac[2], bdinfo.mac[3], bdinfo.mac[4], bdinfo.mac[5]);
        	else
            		change_config();
    
        	/* Create vlan */
        	bzero((void *) &vp, sizeof(rtl_vlan_param_t));
        	memcpy(&vp.gMac, &bdinfo.mac[0], 6);
        	vp.egressUntag = 0x3f;
        	vp.mtu = 1522;
        	vp.memberPort = 0x1f;
        	ret = swCore_vlanCreate(8, &vp);
       		if ( ret != 0 )
        	{
            		printf( "\nCreating vlan fails:%d\n", ret );
            		while(1);
        	}
        	swCore_vlanSetSTPStatusOfAllPorts(8, RTL_STP_FORWARD);

		if ( init_net(&bdinfo.mac[0], &image_len, DRAM_MAP_DLOAD_BUF_ADDR) != 0 )
      			while(1);
      		
//        	if ( bootpReceive (&bdinfo.mac[0], &image_len, DRAM_MAP_DLOAD_BUF_ADDR) != 0 )
//            	while(1);
	        image_len = total_size;
	        printf("image_len is %d\n",image_len);
        	swCore_vlanDestroy(8);
        	while(1);
	}
    	else
    	{
    	    rtl_vlan_param_t vp;
    	    
    	    if (bdinfo.mac[0]+bdinfo.mac[1]+bdinfo.mac[2]+bdinfo.mac[3]+bdinfo.mac[4]+bdinfo.mac[5]!=0)
    	        printf("\nMAC: %02x-%02x-%02x-%02x-%02x-%02x.\n", 
    	                    bdinfo.mac[0], bdinfo.mac[1], bdinfo.mac[2], bdinfo.mac[3], bdinfo.mac[4], bdinfo.mac[5]);
    	    else
    	        change_config();
    	
    	    /* Create vlan */
    	    bzero((void *) &vp, sizeof(rtl_vlan_param_t));
    	    memcpy(&vp.gMac, &bdinfo.mac[0], 6);
    	    vp.egressUntag = 0x3f;
    	    vp.mtu = 1522;
    	    vp.memberPort = 0x1f;
    	    ret = swCore_vlanCreate(8, &vp);
    	    if ( ret != 0 )
    	    {
    	        printf( "\nCreating vlan fails:%d\n", ret );
    	        while(1);
    	    }
    	    swCore_vlanSetSTPStatusOfAllPorts(8, RTL_STP_FORWARD);
    	  
    	    if ( bootpReceive (&bdinfo.mac[0], &image_len, DRAM_MAP_DLOAD_BUF_ADDR) != 0 )
    	        while(1);
		  
    	    swCore_vlanDestroy(8);
    	}
    
    return image_len;
}

void input_address(char *s,uint32 def,uint32 *start)
{
		uint8 buf[11]={0}; 
		int i;
	 	printf("\nInput %s, default 0x%x (0x%x): ",s,def,*start);
 		getline(10, buf, 0);
 		
 		if((buf[0]=='0')&&(buf[1]=='x'))
 		{ 			
 			int len=strlen(buf),j;
 			*start=0;
 			for(i=len-1,j=0;i>=2;i--,j+=4)
 			{
 				if(buf[i]>='0'&&buf[i]<='9') *start+=(buf[i]-'0')<<j;
 				else if(buf[i]>='a'&&buf[i]<='f') *start+=(buf[i]-'a'+10)<<j;
 			}
 		}
 		else if(buf[0]==0)
 		{
 			//keep *start value
 		}
 		else *start=def;
}

void change_config(void)
{
        uint8 buf[18]; 
       	uint32	imask;
       	char mac[6];
       	uint8   ch;
       	void (*app_start)(void);
       	
        	
       	printf("\n!! Change configuration !!");
        	
        	
        while(1)
        {
           	printf("\nInput MAC address(%02x-%02x-%02x-%02x-%02x-%02x): ",bdinfo.mac[0],bdinfo.mac[1],bdinfo.mac[2],bdinfo.mac[3],bdinfo.mac[4],bdinfo.mac[5]);
       	
            getline(17, buf, 0);
            if(strToMac(mac, buf) == 0)
            {
              	memcpy(bdinfo.mac,mac,6);
               	break;
            }
			if(buf[0] == '\0')
				break;
        }

		input_address("run image start address in ram",DRAM_MAP_RUN_IMAGE_ADDR,&bdinfo.ramStartAddress);
		input_address("run image start offset in flash",0x20000,&bdinfo.rootStartOffset);
		input_address("run image max size",0x1e0000,&bdinfo.rootMaxSize);
		//input_address("config start offset",0x6000,&bdinfo.ccfgtStartOffset);
		//input_address("config max size",0x2000,&bdinfo.ccfgMaxSize);
		printf("\nInput Boot Sequence, 0:BOOT_FROM_FLASH 1:BOOT_FROM_BOOTP 2:L2 Switch(50A) \n3:L2 Switch(50B) 4:Hub Mode, default 0 (%d):",bdinfo.bootSequence);
		getchar(&ch); 
		if ( ch >= '0' && ch <= '4' )
			bdinfo.bootSequence = ch - '0';
		else if ( ch == '\r' || ch == '\n' )
			; // bdinfo.bootSequence = bdinfo.bootSequence; // the original value
		else
			bdinfo.bootSequence=0;
		printf("%d",bdinfo.bootSequence);			
		        		
 		//bdinfo.macNbr=1;
 		
		printf("\nWriting FlashROM...");
    	tick_Delay100ms(5);    
  		/* Disable all interrupts */
   		//imask = setIlev(MAX_ILEV);
    		
   		/* Remember to rip the header off */
		//if(flashdrv_updateImg((void *)&bdinfo, (void*)FLASH_MAP_BOARD_INFO_ADDR, sizeof(bdinfo_t)))
		if(flashdrv_updateImg((void *)&bdinfo, (void*)( flash_chip_info[0].BlockBase + flash_chip_info[0].BoardInfo ), sizeof(bdinfo_t)))
   		{
 			setIlev(imask);
 			printf("\nFAIL !\n\n");
 			change_config();
   		}
    
   		//setIlev(imask);
   		printf("\nSUCCESS");

		printf("\n\nPress any key to RESTART ..."); 
        getchar(&ch); 
        printf("\n"); 
        tick_Delay100ms(5);
            
        SmartReboot();    			

#if 0
        //app_start = (void (*)(void)) FLASH_MAP_BOOT_CODE_ADDR;
        app_start = (void (*)(void)) flash_chip_info[0].BlockBase;
           
        /* Far Jump Now */
    	setIlev(MAX_ILEV);
        app_start();    			
#endif//0
}


#ifdef _L2_MODE_
/*
 *  This function will make the whole system enter to pure layer 2 mode.
 *  This is for hareware team testing.
 */
void L2SwitchMode( uint32 for_RTL8650A )
{
    void * pPkt;
    uint32 len;

	// Config ASIC to L2 switch mode    
	if ( L2_swCore_config( &bdinfo.mac[0], for_RTL8650A ) == 0 )
		printf( "\n\nL2 switch mode is enabled ...\n" );
	else
	{
		printf( "\nInitialize L2 switch mode failed.\nSystem Halt.\n" );
		while( 1 );
	}

	// Polling packets ...
	while ( 1 )
	{
	    if ( swNic_receive(&pPkt, &len) == 0 )
	    {
			// silent discard ....
	    }
	}
}
#endif//_L2_MODE_


#ifdef _TEST_FLASH_
/*
 *  This function is used to test if the flash memory is ok.
 *  It will erase, program, and verify the flash memory.
 */
int32 testFlash()
{
	char ch;
	uint32 start, end, seed;
	uint32 cur;
	uint16 block[512];
	uint32 loopNum;
	
	printf( "\n\nTest flash memory ...\n\nIt will damage the flash memory, are you sure (y/N)? " );
	getchar( &ch ); 
	if ( ch != 'y' ) return;

	start = flashdrv_getRunImageAddr();
	input_address("start address",start,&start);
	if ( IS_865XB() && start >= 0xbfc00000 && start <= 0xc0000000 )
		start = start - 0xbfc00000 + 0xbe000000; // converting 0xbfc0-0000 to 0xbe00-0000

	end = (IS_865XB()?0xbe000000:0xbfc00000) + flashdrv_getDevSize();
	input_address("end address",end,&end);
	if ( IS_865XB() && end >= 0xbfc00000 && end <= 0xc0000000 )
		end = end - 0xbfc00000 + 0xbe000000; // converting 0xbfc0-0000 to 0xbe00-0000

	seed = 0x1234;
	input_address("pattern seed",seed,&seed);

	loopNum = 1;
	input_address("Loop times",loopNum, &loopNum);

	printf( "\n\nTest from 0x%08x to 0x%08x, pattern seed: 0x%04x, loop %d time(s).\nAre you sure (y/N)? ", start, end, seed, loopNum );
	getchar( &ch ); 
	if ( ch != 'y' ) return;
	printf( "y\n" );

	while( loopNum-- )
	{
		printf( "\n-=-=-= Loop Remain: %d =-=-=-\n", loopNum );
		
		for( cur = start;
		     cur < end;
		     cur += sizeof(block) )
		{
			int i;

			// generate pattern
			for( i = 0; i < sizeof(block)/2; i++ )
			{
				block[i] = seed++;
				seed &= 0xffff;
			}
		
			if ( 0 != flashdrv_updateImg( block, (void*)cur, sizeof(block) ) )
			{
				printf( "\n\nflash write(0x%08x) failed. HALTED.  Loop Remain:%d  seed:0x%04x\n", cur, loopNum, seed );
				while(1);
			}

			// compare pattern
			for( i = 0; i < sizeof(block)/2; i++ )
			{
				uint32 write, read;
				write =  block[i];
				read = *(volatile uint16*)(cur+(i<<1));
				if ( write != read )
				{
					printf( "\n\nflash compare error at 0x%08x:  write:0x%04x read:0x%04x. HALTED.  Loop Remain:%d  seed:0x%04x\n",
													cur+(i<<1), write, read, loopNum, seed );
					while(1);
				}
			}

			printf( "test %08x OK ....\n", cur );
			//tick_Delay100ms(30);
		}

 	}

	printf("\n\nflash test SUCCESS!\n\nany key to return to main menu ...");
    getchar(&ch);

	return 0;    			
}
#endif//_TEST_FLASH_


#ifdef _ERASE_FLASH_
/*
 *  This function is used to test if the flash memory is ok.
 *  It will erase, program, and verify the flash memory.
 */
int32 eraseFlash()
{
	char ch;
	uint32 start, end;
	uint32 cur;
    uint32 ChipSeq;
    uint32 BlockSeq;
	
	printf( "\n\nErase flash memory ...\n\nIt will damage the flash memory, are you sure (y/N)? " );
	getchar( &ch ); 
	if ( ch != 'y' ) return;

	start = flashdrv_getRunImageAddr();
	input_address("start address",start,&start);
	if ( IS_865XB() && start >= 0xbfc00000 && start <= 0xc0000000 )
		start = start - 0xbfc00000 + 0xbe000000; // converting 0xbfc0-0000 to 0xbe00-0000

	end = (IS_865XB()?0xbe000000:0xbfc00000) + flashdrv_getDevSize();
	input_address("end address",end,&end);
	if ( IS_865XB() && end >= 0xbfc00000 && end <= 0xc0000000 )
		end = end - 0xbfc00000 + 0xbe000000; // converting 0xbfc0-0000 to 0xbe00-0000

	printf( "\n\nErase from 0x%08x to 0x%08x\nAre you sure (y/N)? ", start, end );
	getchar( &ch ); 
	if ( ch != 'y' ) return;
	printf( "y\n" );
	
    /* Check if the total size is larger than flash boundary */
	if ( searchBlockSeq( ((uint32)start), &ChipSeq, &BlockSeq ) != 0 )
	{
		printf( "Size is larger than total flash size: 0x%08x\n", (uint32)start );
		return 1;
	}
	
	for( cur = start;
	     cur < end;
	     )
	{
		int i;
	
		if ( 0 != flashdrv_eraseBlock( ChipSeq, BlockSeq ) )
		{
			printf( "\n\nflash erase(0x%08x) failed. HALTED.\n", cur );
			return 1;
		}

		printf( "erase %08x OK ....\n", cur );
		//tick_Delay100ms(30);

		// Next Block
		BlockSeq++;
		if ( flashdrv_getBlockOffset( ChipSeq,  BlockSeq) == 0xffffffff )
		{
			// out of block, try next chip
			ChipSeq++;
			BlockSeq = 0;
			if ( flashdrv_getBlockOffset( ChipSeq,  BlockSeq) == 0xffffffff  )
				break; // no more chip and block
		}

		cur = flashdrv_getBlockBase( ChipSeq ) + flashdrv_getBlockOffset( ChipSeq,  BlockSeq );
	}

	printf("\n\nflash erase SUCCESS!\n\nany key to return to main menu..");
    getchar(&ch);

    return 0;    			
}
#endif//_ERASE_FLASH_


void start_ram_code()
{
    void (*app_start)(void);
    if((bdinfo.ramStartAddress&0xf0000000)!=0x80000000) 
    {
    		printf("\nCan't start image at address: 0x%x\n",bdinfo.ramStartAddress);
    		change_config();
    }            
    app_start = (void (*)(void)) bdinfo.ramStartAddress;
        
    /* Far Jump Now */
    setIlev(MAX_ILEV);
    app_start();
}
    
int run_without_update(void)
{
	fileImageHeader_t*	imghdr;
	flash_file			infile, outfile;     /* gunzip in/out file */    
	uint32  image_len;        

download_again:

    printf("\n!!Load Run Image Without Updating Flash !!");
	if ( (image_len = download('g')) == 0 ) goto download_again;
    		
	if((bdinfo.ramStartAddress&0xf0000000)!=0x80000000) 
    {
    	printf("\nCan't start image at address: 0x%x\n",bdinfo.ramStartAddress);
    	change_config();
    }
    		
    imghdr = (fileImageHeader_t *) DRAM_MAP_DLOAD_BUF_ADDR;
                
    /* unzip download buffer */
    infile.buffer = (char *) ((uint32) imghdr + sizeof(fileImageHeader_t));
    outfile.buffer = (char *)bdinfo.ramStartAddress;
    infile.size = imghdr->imageLen;
            
    if ( gzip_decompress(&infile, &outfile) )
    {
    	debug_printf("\nDecompress failed !");
    	goto download_again;
    }
    
	start_ram_code();
    return 0;
}


static uint32 getVer(void)
{
    int8 *ptr;
    uint32 val=0;
    uint32 ver=0;
    
    for(ptr=(int8*)version_msg;*ptr;ptr++)
        if( *ptr=='.' )
        {
            ver=(ver<<8)|val;
            val=0;
        }
        else if( ('0'<=*ptr) && (*ptr<='9') )
            val=(val<<3)+(val<<1)+(*ptr-'0');
    ver=(ver<<8)|val;
    
    return ver;
}


#if defined(CONFIG_RTL865X_DIAG_LED)

/* packet format:
 * src mac: 0x00,0x01,0x02,0x03,0x04,0x05
 * dst mac: 0xff,0xff,0xff,0xff,0xff,0xff
 * src    ip: 192.168.1.166 (0xc0-0xa8-0x01-0xa6)
 * target ip: 192.168.1.254 (0xc0-0xa8-0x01-0xfe)
 */
uint8 pktData1[2048]={
//0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x50, 0x02,0x03,0x04,0x05,0x08,0x06,0x00,0x01,
0x00,0x00,0x79,0x11,0x22,0x30,0x00,0x50, 0x02,0x03,0x04,0x05,0x08,0x06,0x00,0x01,
0x08,0x00,0x06,0x04,0x00,0x01,0x00,0x50, 0x02,0x03,0x04,0x05,0xc0,0xa8,0x01,0xa6,
0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xa8, 0x01,0xfe,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20, 0x20,0x20,0x20,0x20
};
uint8  pktData2[2048];
uint8  *ppktData2;
uint32 pktLen1=60;
uint32 pktLen2=60;
uint8  gwMac[6]={0x00,0x00,0x79,0x11,0x22,0x30};
/* set 6 port to PHY loopback mode */
void testPhyLoopback(void){
	uint32 i;
#define WRITE_MEM32(reg,val) REG32((reg))=(val)
#define READ_MEM32(reg)      REG32((reg))
#define SET_PHY_LOOPBACK(port)\
{\
    WRITE_MEM32(SWTAA, PORT0_PHY_CONTROL+(i*0x20));\
    WRITE_MEM32(TCR0, READ_MEM32(PORT0_PHY_CONTROL+(i*0x20)) & ~(1<<12));\
    WRITE_MEM32(SWTACR, ACTION_START|CMD_FORCE);\
    while((READ_MEM32(SWTACR)&ACTION_MASK)!=ACTION_DONE);\
    WRITE_MEM32(TCR0, READ_MEM32(PORT0_PHY_CONTROL+(i*0x20)) | (1<<8));\
    WRITE_MEM32(SWTACR, ACTION_START|CMD_FORCE);\
    while((READ_MEM32(SWTACR)&ACTION_MASK)!=ACTION_DONE);\
    WRITE_MEM32(TCR0, READ_MEM32(PORT0_PHY_CONTROL+(i*0x20)) | (1<<14));\
    WRITE_MEM32(SWTACR, ACTION_START|CMD_FORCE);\
    while((READ_MEM32(SWTACR)&ACTION_MASK)!=ACTION_DONE);\
}

    for(i=0;i<5;i++)
        SET_PHY_LOOPBACK(i);
	
    /* make sure link is up */
    for(i=0;i<5;i++){
        while (REG32(PORT0_PHY_STATUS+(i*0x20)) & (1<<2) == 0)
        tick_Delay10ms(1);
    }

    /* send a broadcast packet */
    {
        uint32 portCnt=0;
	int32 ret;
	uint32 loopCnt;

	ret = swNic_send((void *)pktData1, (uint32)pktLen1);
	if (ret!=0){
		printf("tx fail\n");
		gpio_led(9);
	}

	/* rx 5 times for 5 ports */
	for (portCnt=0;portCnt<5;portCnt++)
	{			
		memset(pktData2,0,2048);
		loopCnt=0;
		do{
			ret=swNic_receive((void**)&ppktData2,(uint32*)&pktLen2);
			if(ret==0)break;
			loopCnt++;
			tick_Delay10ms(1);
			//printf("\n%u",loopCnt);
		}while(loopCnt<0xff);
		if (loopCnt==0xff){
			printf("port [%d] rx fail\n",portCnt);
			gpio_led(9);		
		}else{
			if (pktLen1 != pktLen2){
				printf("port [%d] tx/rx length compared not same\n",portCnt);
				gpio_led(9);
			}
			memcpy(pktData2,ppktData2,pktLen2);
			//for(i=0;i<pktLen1;i++){if (i%0xf == 0) printf("\n");printf("%02x-",pktData2[i]);}
			if (memcmp(pktData1,pktData2,pktLen1)!=0){
				printf("port [%d] tx/rx content compared not same\n",portCnt);
				gpio_led(9);
			}
		} /* end if */
	} /* end portCnt */
    } /* end send broadcast pkt */
    printf("5 ports loopback test OK!\n");
} /* end testPhyLoopback */

/* cancel PHY loopback */
void cancelPhyLoopback(void)
{
	uint32 i;
	//swNic_installedProcessInputPacket(old_funcptr);
	for(i=0;i<5;i++){
		// suggest to backup old value before writing new vlaue
		REG32(TCR0)  =(REG32(PORT0_PHY_CONTROL+(i*0x20)) & ~(1<<14));
		REG32(SWTAA) =(PORT0_PHY_CONTROL+(i*0x20));
		REG32(SWTACR)=(ACTION_START|CMD_FORCE);
		while((REG32(SWTACR)&ACTION_MASK)!=ACTION_DONE);
		REG32(TCR0)  =(REG32(PORT0_PHY_CONTROL+(i*0x20)) | (1<<12));
		REG32(SWTAA) =(PORT0_PHY_CONTROL+(i*0x20));
		REG32(SWTACR)=(ACTION_START|CMD_FORCE);
		while((REG32(SWTACR)&ACTION_MASK)!=ACTION_DONE);
	}
} /* end cancelPhyLoopback */

#define DIAG_LED_ON()   {REG32(PABDAT) &= ~0x40000000;}
#define DIAG_LED_OFF()  {REG32(PABDAT) |=  0x40000000;}
#define DELAY_500_MSEC  tick_Delay10ms(50)
#define DELAY_3000_MSEC tick_Delay10ms(300)
void gpio_led(uint32 ledFreq){
	switch(ledFreq){
		case 1:
			while(1){
				DIAG_LED_OFF();DELAY_500_MSEC;/*1*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_3000_MSEC;
			}
			break;
		case 2:
			while(1){
				DIAG_LED_OFF();DELAY_500_MSEC;/*1*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*2*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_3000_MSEC;
			}
			break;
		case 3:
			while(1){
				DIAG_LED_OFF();DELAY_500_MSEC;/*1*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*2*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*3*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_3000_MSEC;
			}
			break;
		case 5:
			while(1){
				DIAG_LED_OFF();DELAY_500_MSEC;/*1*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*2*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*3*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*4*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*5*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_3000_MSEC;
			}
			break;
		case 9:
			while(1){
				DIAG_LED_OFF();DELAY_500_MSEC;/*1*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*2*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*3*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*4*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*5*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*6*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*7*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*8*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_500_MSEC;/*9*/
				DIAG_LED_ON();DELAY_500_MSEC;
				DIAG_LED_OFF();DELAY_3000_MSEC;
			}
			break;
		default:
			break;
	}
}/* end gpio_led */

#endif /* CONFIG_RTL865X_DIAG_LED */

void csp_main(uint32 HeapBegin, uint32 isBootFromROM )
{
    int32 port;
     uint32 gpio;
	void (*app_start)(void);
    //rootDirEntry_t *    dirEntry;
    rtl_auto_nego_ability_t ab;
	uint32 MCRsdram;
    
	/* save to global area. */
	_isBootFromROM = isBootFromROM; 

    /* Initialize malloc mechanism */
    i_alloc(&__ghsbegin_heap, &__ghsend_stack);
    
    /* Initialize interrupt */
    int_InitEnable();
    
    /* Initialize timer */
    tick_initTest();
    
    /* Initialize console */
    uart_InitTestConsl(ENABLE_UART0_INTERRUPT_MODE);

    printf( disp_screenrest );
    /* first show logo */
    printf( home_cmd );
    printf( logo_msg );
    printf( version_msg );
    printf("(uClinux) (%s %s)\n", __DATE__, __TIME__);

	printf( "[865x%s] ", (IS_865XA()?"A":"B" ) );

	// Read system clock rate and memory clock rate.
	uint32 scr = (REG32(SCLKCR) & 0xF0000000) >> 28;
	uint32 mcr = (REG32(SCLKCR) & 0x0F000000) >> 24;

	printf( "CPU Clock Rate: " );
	if ( IS_865XB() )
	{
		switch( scr )
		{
			case 0: printf( "200MHz" ); break;
			case 1: printf( "180MHz" ); break;
			case 2: printf( "170MHz" ); break;
			case 3: printf( "190MHz" ); break;
			case 4: printf( "160MHz" ); break;
			case 5: printf( "150MHz" ); break;
			case 6: printf( "140MHz" ); break;
			case 7: printf( "100MHz" ); break;
			default:printf( "unknown" ); break;
		}
	}
	else
	{
		// 50A
		printf("96MHz");
	}

	printf( ", Memory Clock Rate: " );
	if ( IS_865XB() )
	{
		switch( mcr )
		{
			case 0: printf( " 50MHz" ); break;
			case 1: printf( "100MHz" ); break;
			case 2: printf( "110MHz" ); break;
			case 3: printf( "120MHz" ); break;
			case 4: printf( "130MHz" ); break;
			case 5: printf( "140MHz" ); break;
			case 6: printf( "150MHz" ); break;
			case 7: printf( "160MHz" ); break;
			default:printf( "unknown" ); break;
		}
	}
	else
	{
		// 50A
		printf("48MHz");
	}
	printf("\n");
	//printf( " %08x\n",REG32(SCLKCR));

	/* detect flash chips */
	flashdrv_init();
	if ( flashdrv_getDevSize() > 0 )
	{
		printf("Detected flash size: total %dMB.\n", flashdrv_getDevSize() >> 20 );
	}
	else
	{
		printf("\nNo flash detected .... System halted!\n");
		while(1);
	}

	/* read SDRAM size */
	printf( "SDRAM size : ");
	switch ( MCRsdram = ( REG32( MCR ) & 0x30100800 ) ) // origional value 0x30000800
	{
		case 0x00000000: printf(" 2MB 16bits"); break;
		case 0x10000000: printf(" 8MB 16bits"); break;
		case 0x20000000: printf("16MB 16bits"); break;
		case 0x30000000: printf("32MB 16bits"); break;
		case 0x30000800: printf("64MB 16bits"); break;
		case 0x00100000: printf(" 4MB 32bits"); break;
		case 0x10100000: printf("16MB 32bits"); break;
		case 0x20100000: printf("32MB 32bits"); break;
		case 0x30100000: printf("64MB 32bits"); break;
		case 0x30100800: printf("128MB 32bits"); break;
		case 0x00000800: /* fall thru */
		case 0x10000800: /* fall thru */
		case 0x20000800: /* fall thru */
		default:
			printf( "unknown(0x%08X)", MCRsdram ); break;
	}
	printf( "\n" );
	
	// Display 0xbd013000 - 0xbd013008 REG setting
	printf( "(MCR = %lx):(MTCR0 = %lx):(MTCR1 = %lx)\n", REG32( MCR ), REG32( MTCR0 ), REG32( MTCR1 ) );
	
	printf("\n\n*********************************************\n");
    	printf("[GST]: Loader Firmware Version: %s\n", LDR_VER_FIRMWARE);	
    	printf("[GST]: Loader FW Version Date:  %s\n",LDR_VER_DATE);
    	printf("[GST]: Loader FW Version Build: %s\n", LDR_VER_BUILD);
    	printf("*********************************************\n\n\n");
	
#ifdef _DEBUG_
	printf( "_isBootFromROM:0x%X\n", _isBootFromROM );
#endif// _DEBUG_
	
    /* Initialize switch core */
    if ( swCore_init() ){
        printf("\nSwitch core initialization failed!");
        #if defined(CONFIG_RTL865X_DIAG_LED)
        gpio_led(9);
        #endif /* CONFIG_RTL865X_DIAG_LED */
    }

    /* Initialize NIC module */
    if ( swNic_init(8, 8, 4, MBUF_LEN) ){
        printf("\nSwitch nic initialization failed!");
        #if defined(CONFIG_RTL865X_DIAG_LED)
        gpio_led(9);
        #endif /* CONFIG_RTL865X_DIAG_LED */
    }

    #if defined(CONFIG_RTL865X_DIAG_LED)
    {
        int32 ret;
	uint32 i=0;
	rtl_vlan_param_t vp;
		
        /* Create vlan */
        bzero((void *) &vp, sizeof(rtl_vlan_param_t));
        memcpy(&vp.gMac, gwMac, 6);
        vp.egressUntag = 0x3f;
        vp.mtu = 1522;
        vp.memberPort = 0x1f;
        ret = swCore_vlanCreate(8, &vp);
        if ( ret != 0 )
        {
            printf( "\nCreating vlan fails:%d\n", ret );
            gpio_led(9);
        }
        swCore_vlanSetSTPStatusOfAllPorts(8, RTL_STP_FORWARD);
		testPhyLoopback();
		cancelPhyLoopback();
        swCore_vlanDestroy(8);
    }
    #endif /* CONFIG_RTL865X_DIAG_LED */

    #if defined(CONFIG_RTL865X_DIAG_LED)
    REG32(CPUIIMR) |= LINK_CHANG_IE; // enable cpu interface link change interrupt
    #endif /* CONFIG_RTL865X_DIAG_LED */

    /* Fill getVer function pointer into communication section */
    {
    	uint32 *pInst;
		uint32 ldrVer;

		/* This is a tiny function to return loader version for runtime code.
		 * It is used to compatible with old runtime code.
		 */
		ldrVer = getVer();
    	pInst = (uint32*)0x80000304;
    	pInst[0] = 0x34020000|(uint16)ldrVer; /* li $2,ldrVer */
    	pInst[1] = 0x34030000;                /* li $3,0      */
    	pInst[2] = 0x03E00008;                /* jr $31       */
    	pInst[3] = 0x00000000;                /* nop          */

		/* Set call-back function pointer */
	    *(volatile uint32*)__ghsbegin_commun = (uint32) pInst;
    }
    /* Copy version string into communication section */ 
	memcpy( &__ghsbegin_commun[0xF0], version_msg, sizeof(version_msg) );
    
    /* read board info */
    flashdrv_read( &bdinfo, (void*)(flash_chip_info[0].BlockBase + flash_chip_info[0].BoardInfo), sizeof(bdinfo_t) );
    
    if(bdinfo.ramStartAddress==0xffffffff)
    {
    	uint32 imask;
		char ch;

       	bdinfo.mac[0]=0x00;
       	bdinfo.mac[1]=0x00;
       	bdinfo.mac[2]=0x10;
       	bdinfo.mac[3]=0x11;
       	bdinfo.mac[4]=0x12;
       	bdinfo.mac[5]=0x00;
        bdinfo.ramStartAddress=DRAM_MAP_RUN_IMAGE_ADDR;
		bdinfo.rootStartOffset=0x20000;
		bdinfo.rootMaxSize=0x1e0000;
		bdinfo.bootSequence=0;
		printf("\nWriting FlashROM...");
    	tick_Delay100ms(5);    
  		/* Disable all interrupts */
   		//imask = setIlev(MAX_ILEV);    		
   		/* Remember to rip the header off */
		//if(flashdrv_updateImg((void *)&bdinfo, (void*)FLASH_MAP_BOARD_INFO_ADDR, sizeof(bdinfo_t)))
		if(flashdrv_updateImg((void *)&bdinfo, (void*)( flash_chip_info[0].BlockBase + flash_chip_info[0].BoardInfo ), sizeof(bdinfo_t)))
   		{
 			//setIlev(imask);
 			printf("\nFAIL !\n\n");
 			change_config();
   		}    
   		//setIlev(imask);
   		printf("\nSUCCESS\n\n");		            

		printf("any key to reboot..");
        getchar(&ch);
    	tick_Delay100ms(5);

    	//setIlev(MAX_ILEV);
        SmartReboot();    			

#if 0
       	//app_start = (void (*)(void)) FLASH_MAP_BOOT_CODE_ADDR;
        app_start = (void (*)(void)) flash_chip_info[0].BlockBase;
           
        /* Far Jump Now */
    	setIlev(MAX_ILEV);
        app_start();    			    	
#endif//0
    }

    while (1)
    {
        int32   result;
        uint32  image_len;
        uint32  wait;
        uint8   ch;

main_menu:
		wait = 5; // re-charge (0.5 second)
		
		// Boot Menu
        printf("\nPress 'r' to update run image, or 'a' to change config,\nor 'l' to update loader, or 'g' to load run image without updating Flash" );
#ifdef _L2_MODE_
        printf(",\nor '2' to enter L2 switch mode(50A), or '3' to enter L2 switch mode(50B)");
#endif//_L2_MODE_
#ifdef _HUB_MODE_
        printf(",\nor 'h' to enter hub mode");
#endif//_HUB_MODE_
#ifdef _TEST_FLASH_
		printf(",\nor 't' to test flash memory");
#endif//_TEST_FLASH_
#ifdef _ERASE_FLASH_
		printf(", or 'e' to erase flash memory");
#endif//_ERASE_FLASH_
		printf(" ...");

	//Add Backup Mode Botton
	{
		int counter = 30;
		int i;
		
		tick_Delay100ms(1);
		
		REG32(PABCNR) |= 0x00010000;
		gpio = REG32(PABDAT);
	
		// total check for 3 seconds to avoid restore to default caused by switch button pressed	
		while((counter >= 0) && ((gpio & 0x00010000) == 0))
		{
			gpio = REG32(PABDAT);
			if (((gpio & 0x00010000) == 0) && (counter == 0))
			{
				printf("\n\nSwitchButton [B0] = %X\n",(gpio & 0x00010000));
				//REG32(PABCNR) |= 0x00002000;
				REG32(PABDIR) |= 0x00002000;
				REG32(PCIMR) &=  ~0x0FC00000;
				into_backup();
				download('z');
			}
			tick_Delay100ms(1);
			counter--;
		}
		printf("\n\nSwitchButton [B0] = %X\n",(gpio & 0x00010000));
	}
	//Add Backup Mode Botton
	                        
        while (wait--)
        {
            if ( getch(&ch) )
            {
                if ( ch == 'r' )
                    goto update_run;                
                else if ( ch == 'z' )
                    goto update_run_backup;                
                else if ( ch == 'l' )
                    goto update_ldr;
                else if ( ch == 'g' )
                    run_without_update();
                else if ( ch == 'a' )
                    change_config();
#ifdef _TEST_FLASH_
                else if ( ch == 't' )
                	testFlash();
#endif//_TEST_FLASH_
#ifdef _ERASE_FLASH_
                else if ( ch == 'e' )
                	eraseFlash();
#endif//_ERASE_FLASH_
#ifdef _L2_MODE_
                else if ( ch == '2' )
                    L2SwitchMode( 1 ); // 8650A
                else if ( ch == '3' )
                    L2SwitchMode( 0 ); // 8650B
#endif//_L2_MODE_
#ifdef _HUB_MODE_
                else if ( ch == 'h' )
                    HubMode();
#endif//_HUB_MODE_
				else
					break; // un-supported key-stroke
					
				goto main_menu;
            }
            tick_Delay100ms(1);
        }

		if(bdinfo.bootSequence==1) run_without_update();
#ifdef _L2_MODE_
		if(bdinfo.bootSequence==2) L2SwitchMode( 1 );
		if(bdinfo.bootSequence==3) L2SwitchMode( 0 );
#else
		if ( bdinfo.bootSequence==2 || bdinfo.bootSequence==3 )
		{
			printf( "\n\n! NO L2 Switch mode!\nDefine _L2_MODE_, and re-compile the loader!\n" );
			while( 1 );
		}
#endif//_L2_MODE_

#ifdef _HUB_MODE_
		if(bdinfo.bootSequence==4) HubMode();
#else
		if ( bdinfo.bootSequence==4 )
		{
			printf( "\n\n! NO hub mode!\nDefine _HUB_MODE_, and re-compile the loader!\n" );
			while( 1 );
		}
#endif//_HUB_MODE_
		
        printf("\n\nLoading runtime image ...\n");
        
        
        
        // software reboot control.(Active Low)
        //======= Initial & Configure GPIO Registers ================================
	// Set Control Register
	REG32(PABCNR) &= ~0x00ff3800; // set to GPIO mode
	
	// Set Directory Register
	REG32(PABDIR) |=  0x00fe3000;  // set to output
	REG32(PABDIR) &=  ~0x00010800; // set to input
	
	// Disable all Interrupt
	REG32(PABIMR) &=  ~0x0000FFFF; // disable Port B interrupt
	REG32(PCIMR) &=  ~0x0FC00000;  // disable Port C pin3.4.5 interrupt
	
	REG32(PABDAT) |=  0x00800000; // Set SW_REBOOT to high
	
	REG32(PABDAT) |=  0x00800000; // Set SW_REBOOT to high
	REG32(PABDAT) |=  0x00040000; // Set DIAL UP LED to high
	REG32(PABDAT) |=  0x00002000; // Set STATUS LED to high
	
	
	REG32(PABDAT) &= ~0x00400000;  // Set FW_USE to Low for WDog disabled
	
	printf("SW_REBOOT [B7] = %lx\n", REG32(PABDAT) & 0x00800000);
	printf("FW_USE    [B6] = %lx\n", REG32(PABDAT) & 0x00400000);// Disabled at initiation stage
        //==================   End   =================================================	
	// add end
	
        if ( (result = fileSys_loadRuntimeImage()) == -1 )
        {
            printf("\n!!Update Root Directory !!");
			goto update_run;
        }
        else if ( result == -2 )
        {
update_run:
            printf("\n!!Update Run Image !!");
            
         
            if ( (image_len = download('r')) == 0 )
                continue;
 
            // image length - 24 for skipping Hardware ID verification
            image_len -= 24;
            
            /* update run image */
            if ( fileSys_updateRuntimeImage((void *) DRAM_MAP_DLOAD_BUF_ADDR, image_len) == 0 )
                continue;
            goto update_run;

update_run_backup:
            printf("\n!!Update Run Image !!");

            if ( (image_len = download('z')) == 0 )
                continue;
            
            // image length - 24 for supporting Hardware ID verification
            image_len -= 24;
            
            /* update run image */
            if ( fileSys_updateRuntimeImage((void *) DRAM_MAP_DLOAD_BUF_ADDR, image_len) == 0 )
                continue;
            goto update_run_backup;

        }
        else if ( result == -3 )
        {
			change_config();
        }
        
	start_ram_code();
        /* Never return here */
    

  


    
update_ldr:
        printf("\n!!Update Loader Image !!");
        printf("\nPress 'y' to confirm ... (n)");
        getchar(&ch);
        if ( ch != 'y' )
            continue;
        
        image_len = download('l');
        
        // image length - 24 for skipping Hardware ID verification
        image_len -= 24;
        
        /* update run image */
        if ( fileSys_updateBootImage((void *) DRAM_MAP_DLOAD_BUF_ADDR, image_len) == 0 )
        {
            printf("\n\nPress any key to RESTART ..."); 
            getchar(&ch); 
            printf("\n"); 
            tick_Delay100ms(5);
            //app_start = (void (*)(void)) FLASH_MAP_BOOT_CODE_ADDR;
            app_start = (void (*)(void)) flash_chip_info[0].BlockBase;
            
            /* Far Jump Now */
    	    setIlev(MAX_ILEV);
            app_start();
        }
        
        goto update_ldr;
        

    }

} /* end csp_main() */

/* The followings are to resolve sharing bsp in boot and various kernels */
void rtosTickIsr(void) { return; }
#ifdef FAT_CODE
void rtosIntEnter(void) { return; }
void rtosIntExit(void) { return; }

/* The followings are to resolve sharing uart driver in boot and runtime */
int32 sem_init(sem_t *sem, int32 flag, uint32 value) { return 0; }
int32 sem_trywait(sem_t *sem) { return 0; }
int32 sem_wait(sem_t *sem) { return 0; }
int32 sem_post(sem_t *sem) { return 0; }
#endif//FAT_CODE

void * pExceptionContext;
void gdb_exception(uint32 exception) {while(1);}

