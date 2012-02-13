/*
 * Copyright (c) 1997-1999 TOSHIBA Corporation
 *   
 * Abstract:
 *   TX3912/3922 processor internal register definitions
 * 
 * Notes:
 *
 * Revision History:
 *   Rev 1.0 : Initial release                  May.07,1999
 *   Rev 1.1 : Modify for TX39/Linux            Nov.01,1999
 */

#ifndef _TX39_H_
#define _TX39_H_

/*
 * TX39 Memory Map
 */

#define CS0_PHYSBASE            0x11000000
#define CS0_SIZE                0x0f000000
#define CS1_PHYSBASE            0x10000000
#define CS1_SIZE                0x00400000
#define CS2_PHYSBASE            0x10400000
#define CS2_SIZE                0x00400000
#define CS3_PHYSBASE            0x10800000
#define CS3_SIZE                0x00400000
#define MCS0_PHYSBASE           0x6c000000
#define MCS0_SIZE               0x04000000
#define MCS1_PHYSBASE           0x70000000
#define MCS1_SIZE               0x04000000
#define DRAMBANK0_PHYSBASE      0x00000000
#define DRAMBANK0_SIZE          0x02000000
#define DRAMBANK1_PHYSBASE      0x02000000
#define DRAMBANK1_SIZE          0x02000000

#define SDRAM0MODE_PHYSBASE     0x10e00000
#define SDRAM0MODE_SIZE         0x00100000
#define SDRAM1MODE_PHYSBASE     0x10f00000
#define SDRAM1MODE_SIZE         0x00100000

#define SDRAM0MODE_KSEG1	0xB0e00000
#define SDRAM1MODE_KSEG1	0xB0f00000

#define CARD2IOA_PHYSBASE       0x08000000
#define CARD2IOA_SIZE           0x04000000
#define CARD1IOA_PHYSBASE       0x0c000000
#define CARD1IOA_SIZE           0x04000000
#define CARD2MEM_PHYSBASE       0x68000000
#define CARD2MEM_SIZE           0x04000000
#define CARD1MEM_PHYSBASE       0x64000000
#define CARD1MEM_SIZE           0x04000000

#define INTERNAL_REG_PHYSBASE   0x10c00000

#define INTERNAL_REG_BASE	0xB0C00000	/* DJH */
/*
   BIU Register
*/

#define OFF_BIU_MEM_CONFIG0     0x000
#define BIU_ENDCLKOUTTRI        (1 << 30)
#define BIU_DISDQMINIT          (1 << 29)
#define BIU_ENSDRAMPD           (1 << 28)
#define BIU_SHOWDINO            (1 << 27)
#define BIU_ENRMAP2             (1 << 26)
#define BIU_ENRMAP1             (1 << 25)
#define BIU_ENWRINPAGE          (1 << 24)
#define BIU_ENCS3USER           (1 << 23)
#define BIU_ENCS2USER           (1 << 22)
#define BIU_ENCS1USER           (1 << 21)
#define BIU_ENCS1DRAM           (1 << 20)
#define BIU_BANK1CONFMASK       (3 << 18)
#define BIU_BANK1CONF_16SDRAM   (3 << 18)   /* Bank 1 as 16 bit SDRAM */
#define BIU_BANK1CONF_8SDRAM    (2 << 18)   /* Bank 1 as 8 bit SDRAM */
#define BIU_BANK1CONF_32DRAM    (1 << 18)   /* Bank 1 as 32 bit DRAM/HDRAM */
#define BIU_BANK1CONF_16DRAM    (0 << 18)   /* Bank 1 as 16 bit DRAM/HDRAM */
#define BIU_BANK0CONFMASK       (3 << 16)
#define BIU_BANK0CONF_16SDRAM   (3 << 16)   /* Bank 1 as 16 bit SDRAM */
#define BIU_BANK0CONF_8SDRAM    (2 << 16)   /* Bank 1 as 8 bit SDRAM */
#define BIU_BANK0CONF_32DRAM    (1 << 16)   /* Bank 1 as 32 bit DRAM/HDRAM */
#define BIU_BANK0CONF_16DRAM    (0 << 16)   /* Bank 1 as 16 bit DRAM/HDRAM */
#define BIU_ROWSEL1MASK         (3 << 14)
#define BIU_ROWSEL1_TYPE1       (0 << 14)   /* Row Address 18, 17:9 */
#define BIU_ROWSEL1_TYPE2       (1 << 14)   /* Row Address 22,18,20,19,17:9 */
#define BIU_ROWSEL1_TYPE3       (2 << 14)   /* Row Address 22,21,19,17:9 */
#define BIU_ROWSEL1_TYPE4       (3 << 14)   /* Row Address 23,21,19,17:9 */
#define BIU_ROWSEL0MASK         (3 << 12)
#define BIU_ROWSEL0_TYPE1       (0 << 12)   /* Row Address 18, 17:9 */
#define BIU_ROWSEL0_TYPE2       (1 << 12)   /* Row Address 22,18,20,19,17:9 */
#define BIU_ROWSEL0_TYPE3       (2 << 12)   /* Row Address 22,21,19,17:9 */
#define BIU_ROWSEL0_TYPE4       (3 << 12)   /* Row Address 23,21,19,17:9 */
#define BIU_COLSEL1MASK         (0xf << 8)
#define BIU_COLSEL1_TYPE1       (0 << 8)    /* Column Address 22,20,18,8:1 */
#define BIU_COLSEL1_TYPE2       (1 << 8)    /* Column Address 19,18,8:2 */
#define BIU_COLSEL1_TYPE3       (2 << 8)    /* Column Address 21,20,8:2 */
#define BIU_COLSEL1_TYPE4       (3 << 8)    /* Column Address 23,22,20,18,8:2 */
#define BIU_COLSEL1_TYPE5       (4 << 8)    /* Column Address 24,22,20,18,8:2 */
#define BIU_COLSEL1_TYPE6       (5 << 8)    /* Column Address 18,p,X,8:0 */
#define BIU_COLSEL1_TYPE7       (6 << 8)    /* Column Address 22,p,X,21,8:1 */
#define BIU_COLSEL1_TYPE8       (7 << 8)    /* Column Address 18,p,X,21,8:1 */
#define BIU_COLSEL1_TYPE9       (8 << 8)    /* Column Address 22,p,X,23,21,8:2 */
#define BIU_COLSEL1_TYPE10      (9 << 8)    /* Column Address 21,8:2 */

#define BIU_COLSEL0MASK         (0xf << 4)
#define BIU_COLSEL0_TYPE1       (0 << 4)    /* Column Address 22,20,18,8:1 */
#define BIU_COLSEL0_TYPE2       (1 << 4)    /* Column Address 19,18,8:2 */
#define BIU_COLSEL0_TYPE3       (2 << 4)    /* Column Address 21,20,8:2 */
#define BIU_COLSEL0_TYPE4       (3 << 4)    /* Column Address 23,22,20,18,8:2 */
#define BIU_COLSEL0_TYPE5       (4 << 4)    /* Column Address 24,22,20,18,8:2 */
#define BIU_COLSEL0_TYPE6       (5 << 4)    /* Column Address 18,p,X,8:0 */
#define BIU_COLSEL0_TYPE7       (6 << 4)    /* Column Address 22,p,X,21,8:1 */
#define BIU_COLSEL0_TYPE8       (7 << 4)    /* Column Address 18,p,X,21,8:1 */
#define BIU_COLSEL0_TYPE9       (8 << 4)    /* Column Address 22,p,X,23,21,8:2 */
#define BIU_COLSEL0_TYPE10      (9 << 4)    /* Column Address 21,8:2 */

#define BIU_CS3SIZE             (1 << 3)
#define BIU_CS2SIZE             (1 << 2)
#define BIU_CS1SIZE             (1 << 1)
#define BIU_CS0SIZE             1

#define OFF_BIU_MEM_CONFIG1     0x0004

/* TX3912 */
#define BIU_MCS3ACCVAL1MASK     (0xf << 28)
#define BIU_MCS3ACCVAL1(a)      (((a) << 28) & BIU_MCS3ACCVAL1MASK)
#define BIU_MCS3ACCVAL2MASK     (0xf << 24)
#define BIU_MCS3ACCVAL2(a)      (((a) << 24) & BIU_MCS3ACCVAL2MASK)
#define BIU_MCS2ACCVAL1MASK     (0xf << 20)
#define BIU_MCS2ACCVAL1(a)      (((a) << 20) & BIU_MCS2ACCVAL1MASK)
#define BIU_MCS2ACCVAL2MASK     (0xf << 16)
#define BIU_MCS2ACCVAL2(a)      (((a) << 16) & BIU_MCS2ACCVAL2MASK)

/* TX3922 */
#define BIU_C48MPLLON           (1 << 30)
#define BIU_ENMCS1BE            (1 << 29)
#define BIU_ENMCS0BE            (1 << 28)
#define BIU_ENMCS1ACC           (1 << 27)
#define BIU_ENMCS0ACC           (1 << 26)
#define BIU_BCLKDIVSHIFT        23
#define BIU_BCLKDIVMASK         (0x7 << 23)
#define BIU_BCLKDIV(a)          (((a) << 23) & BIU_BCLKDIVMASK)
#define BIU_ENBCLK              (1 << 22)
#define BIU_MCS1PAGE            (1 << 21)
#define BIU_MCS0PAGE            (1 << 20)
#define BIU_MCS1WAIT            (1 << 19)
#define BIU_MCS0WAIT            (1 << 18)
#define BIU_MCS1_32             (1 << 17)
#define BIU_MCS0_32             (1 << 16)

#define BIU_MCS1ACCVAL1MASK     (0xf << 12)
#define BIU_MCS1ACCVAL1(a)      (((a) << 12) & BIU_MCS1ACCVAL1MASK)
#define BIU_MCS1ACCVAL2MASK     (0xf << 8)
#define BIU_MCS1ACCVAL2(a)      (((a) << 8) & BIU_MCS1ACCVAL2MASK)
#define BIU_MCS0ACCVAL1MASK     (0xf << 4)
#define BIU_MCS0ACCVAL1(a)      (((a) << 4) & BIU_MCS0ACCVAL1MASK)
#define BIU_MCS0ACCVAL2MASK     0xf
#define BIU_MCS0ACCVAL2(a)      ((a) & BIU_MCS0ACCVAL2MASK)

#define OFF_BIU_MEM_CONFIG2     0x0008
#define BIU_CS3ACCVAL1MASK      (0xf << 28)
#define BIU_CS3ACCVAL1(a)       (((a) << 28) & BIU_CS3ACCVAL1MASK)
#define BIU_CS3ACCVAL2MASK      (0xf << 24)
#define BIU_CS3ACCVAL2(a)       (((a) << 24) & BIU_CS3ACCVAL2MASK)
#define BIU_CS2ACCVAL1MASK      (0xf << 20)
#define BIU_CS2ACCVAL1(a)       (((a) << 20) & BIU_CS2ACCVAL1MASK)
#define BIU_CS2ACCVAL2MASK      (0xf << 16)
#define BIU_CS2ACCVAL2(a)       (((a) << 16) & BIU_CS2ACCVAL2MASK)
#define BIU_CS1ACCVAL1MASK      (0xf << 12)
#define BIU_CS1ACCVAL1(a)       (((a) << 12) & BIU_CS1ACCVAL1MASK)
#define BIU_CS1ACCVAL2MASK      (0xf << 8)
#define BIU_CS1ACCVAL2(a)       (((a) << 8) & BIU_CS1ACCVAL2MASK)
#define BIU_CS0ACCVAL1MASK      (0xf << 4)
#define BIU_CS0ACCVAL1(a)       (((a) << 4) & BIU_CS0ACCVAL1MASK)
#define BIU_CS0ACCVAL2MASK      0xf
#define BIU_CS0ACCVAL2(a)       ((a) & BIU_CS0ACCVAL2MASK)

#define OFF_BIU_MEM_CONFIG3     0x00c
#define BIU_CARD2ACCVALMASK     (0xf << 28)
#define BIU_CARD2ACCVAL(a)      (((a) << 28) & BIU_CARD2ACCVALMASK)
#define BIU_CARD1ACCVALMASK     (0xf << 24)
#define BIU_CARD1ACCVAL(a)      (((a) << 24) & BIU_CARD1ACCVALMASK)
#define BIU_CARD2IOACCVALMASK   (0xf << 20)
#define BIU_CARD2IOACCVAL(a)    (((a) << 20) & BIU_CARD2IOACCVALMASK)
#define BIU_CARD1IOACCVALMASK   (0xf << 16)
#define BIU_CARD1IOACCVAL(a)    (((a) << 16) & BIU_CARD1IOACCVALMASK)

/* TX3912 */
#define BIU_ENMCS3PAGE          (1 << 15)
#define BIU_ENMCS2PAGE          (1 << 14)
#define BIU_ENMCS1PAGE          (1 << 13)
#define BIU_ENMCS0PAGE          (1 << 12)

#define BIU_ENCS3PAGE           (1 << 11)
#define BIU_ENCS2PAGE           (1 << 10)
#define BIU_ENCS1PAGE           (1 << 9)
#define BIU_ENCS0PAGE           (1 << 8)
#define BIU_CARD2WAITEN         (1 << 7)
#define BIU_CARD1WAITEN         (1 << 6)
#define BIU_CARD2IOEN           (1 << 5)
#define BIU_CARD1IOEN           (1 << 4)

/* TX3912 */
#define BIU_PORT8SEL            (1 << 3)

/* TX3922 */
#define BIU_CARD2_8SEL          (1 << 3)
#define BIU_CARD1_8SEL          (1 << 2)


#define OFF_BIU_MEM_CONFIG4     0x010
#define BIU_ENBANK1HDRAM        (1 << 31)
#define BIU_ENBANK1EDO          (1 << 31) 
#define BIU_ENBANK0HDRAM        (1 << 30)
#define BIU_ENBANK0EDO          (1 << 30)
#define BIU_ENARB               (1 << 29)
#define BIU_DISSNOP             (1 << 28)
#define BIU_CLRWRBUSERRINT      (1 << 27)
#define BIU_ENBANK1OPT          (1 << 26)
#define BIU_ENBANK0OPT          (1 << 25)
#define BIU_ENWATCH             (1 << 24)
#define BIU_WATCHTIMEVALMASK    (0xf << 20)
#define BIU_WATCHTIMEVAL(a)     (((a) << 20) & BIU_WATCHTIMEVALMASK)
#define BIU_MEMPOWERDOWN        (1 << 16)
#define BIU_ENRFSH1             (1 << 15)
#define BIU_ENRFSH0             (1 << 14)
#define BIU_RFSHVAL1MASK        (0x1f << 8)
#define BIU_RFSHVAL1(a)         (((a) << 8) & BIU_RFSHVAL1MASK)
#define BIU_RFSHVAL0MASK        0x1f
#define BIU_RFSHVAL0(a)         ((a) & BIU_RFSHVAL0MASK)

#define OFF_BIU_MEM_CONFIG5     0x014
#define BIU_STARTVAL2MASK       0xfffffe00
#define BIU_MASK2               0x0000000f

#define OFF_BIU_MEM_CONFIG6     0x018
#define BIU_STARTVAL1MASK       0xfffffe00
#define BIU_MASK1               0x0000000f

#define BIU_MASK_8K             0x00000000
#define BIU_MASK_4K             0x00000008
#define BIU_MASK_2K             0x0000000c
#define BIU_MASK_1K             0x0000000e
#define BIU_MASK_256            0x0000000f

#define OFF_BIU_MEM_CONFIG7     0x01c
#define BIU_RMAPADD2MASK        0xfffffe00

#define OFF_BIU_MEM_CONFIG8     0x020
#define BIU_RMAPADD1MASK        0xfffffe00

/*
   SIU Module
*/
#ifdef INTERNAL_TEST
#define OFF_SIU_TEST            0x1c8
#define SIU_ENDMATEST           (1 << 7)
#define SIU_ENNOTIMETEST        (1 << 6)
#define SIU_DMATESTWRMASK       (0x3f)
#endif /* INTERNAL_TEST */

/*
   TX3922 Status Register
 */
#define OFF_STATUS_REG          0x1cc
#define STSR_BUS_BUSY           (1 << 4)
#define STSR_CPU_CLOCK_MASK     (0x3)
#define STSR_CPU_CLOCK_133      (0)
#define STSR_CPU_CLOCK_150      (1)
#define STSR_CPU_CLOCK_166      (2)
#define STSR_CPU_CLOCK_others   (3)


/*
   Clock Module 
*/

#define OFF_CLOCK_CTL           0x1c0
#define CLOCK_CHICLKDIVMASK     0xff000000
#define CLOCK_CHICLKDIV(a)      ((a) << 24)
#define CLOCK_ENCLKTEST         (1 << 23)
#define CLOCK_TESTSELSIB        (1 << 22)
#define CLOCK_CHIMCLKSEL        (1 << 21)
#define CLOCK_CHICLKDIR         (1 << 20)
#define CLOCK_ENCHIMCLK         (1 << 19)

/* TX3912 */
#define CLOCK_ENVIDCLK          (1 << 18)
#define CLOCK_ENMSUBCLK         (1 << 17)
/* TX3922 */
#define CLOCK_SPICLKDIR         (1 << 18)
#define CLOCK_ENIRDACLK         (1 << 17)

#define CLOCK_ENSPICLK          (1 << 16)
#define CLOCK_ENTIMERCLK        (1 << 15)
#ifdef INTERNAL_TEST
#define CLOCK_FASTTIMERCLK      (1 << 14)
#endif /* INTERNAL_TEST */
#define CLOCK_SIBMCLKDIR        (1 << 13)

/* TX3922 */
#define CLOCK_ENC48MOUT         (1 << 12)

#define CLOCK_ENSIBMCLK         (1 << 11)
#define CLOCK_SIBMCLKDIVMASK    0x00000700
#define CLOCK_SIBMCLKDIV(a)     (((a) << 8) & CLOCK_SIBMCLKDIVMASK)
#define CLOCK_CSERSEL           (1 << 7)
#define CLOCK_CSERDIVMASK       0x00000070
#define CLOCK_CSERDIV(a)        (((a) << 4) & CLOCK_CSERDIVMASK)
#define CLOCK_ENCSERCLK         (1 << 3)
#define CLOCK_ENIRCLK           (1 << 2)
#define CLOCK_ENUARTACLK        (1 << 1)
#define CLOCK_ENUARTBCLK        1

/*
   CHI Module
*/
#define OFF_CHI_CTL             0x1d8
#define CHI_CHILOOP             (1 << 29)
#ifdef INTERNAL_TEST
#define CHI_CHIENTEST           (1 << 28)
#endif /* INTERNAL_TEST */
#define CHI_CHIFSDIR            (1 << 27)
#define CHI_CHIFSWIDEMASK       (3 << 25)
#define CHI_CHIFSWIDE_1BIT      0
#define CHI_CHIFSWIDE_2BIT      (1 << 25)
#define CHI_CHIFSWIDE_1BYTE     (2 << 25)
#define CHI_CHIFSWIDE_NCHAN     (3 << 25)
#define CHI_CHINCHANMASK        (0x1f << 20)
#define CHI_CHINCHAN(a)         (((a) << 20) & CHI_CHINCHANMASK)
#define CHI_CHITXBOFFMASK       (0xf << 16)
#define CHI_CHITXBOFF(a)        (((a) << 16) & CHI_CHITXBOFFMASK)
#define CHI_CHIRXBOFFMASK       (0xf << 12)
#define CHI_CHIRXBOFF(a)        (((a) << 12) & CHI_CHIRXBOFFMASK)
#define CHI_TXMSBFIRST          (1 << 11)
#define CHI_RXMSBFIRST          (1 << 10)
#define CHI_CHIRXFSPOL          (1 << 9)
#define CHI_CHITXFSPOL          (1 << 8)
#define CHI_CHIRXEDGE           (1 << 7)
#define CHI_CHITXEDGE           (1 << 6)
#define CHI_CHIFSEDGE           (1 << 5)
#define CHI_CHITXFSEDGE         (1 << 4)
#define CHI_CHICLK2XMODE        (1 << 3)
#define CHI_CHIRXEN             (1 << 2)
#define CHI_CHITXEN             (1 << 1)
#define CHI_ENCHI               1

#define OFF_CHI_PTREN           0x1dc
#define CHI_CHITXPTRB3EN        (1 << 31)
#define CHI_CHITXPTRB2EN        (1 << 30)
#define CHI_CHITXPTRB1EN        (1 << 29)
#define CHI_CHITXPTRB0EN        (1 << 28)
#define CHI_CHITXPTRA3EN        (1 << 27)
#define CHI_CHITXPTRA2EN        (1 << 26)
#define CHI_CHITXPTRA1EN        (1 << 25)
#define CHI_CHITXPTRA0EN        (1 << 24)
#define CHI_CHIRXPTRB3EN        (1 << 23)
#define CHI_CHIRXPTRB2EN        (1 << 22)
#define CHI_CHIRXPTRB1EN        (1 << 21)
#define CHI_CHIRXPTRB0EN        (1 << 20)
#define CHI_CHIRXPTRA3EN        (1 << 19)
#define CHI_CHIRXPTRA2EN        (1 << 18)
#define CHI_CHIRXPTRA1EN        (1 << 17)
#define CHI_CHIRXPTRA0EN        (1 << 16)

#define OFF_CHI_RCVPTRA         0x1e0
#define CHI_CHIRXPTRA3MASK      (0xf << 24)
#define CHI_CHIRXPTRA3(a)       (((a) << 24) & CHI_CHIRXPTRA3MASK)
#define CHI_CHIRXPTRA2MASK      (0xf << 16)
#define CHI_CHIRXPTRA2(a)       (((a) << 16) & CHI_CHIRXPTRA2MASK)
#define CHI_CHIRXPTRA1MASK      (0xf << 8)
#define CHI_CHIRXPTRA1(a)       (((a) << 8) & CHI_CHIRXPTRA1MASK)
#define CHI_CHIRXPTRA0MASK      0xf
#define CHI_CHIRXPTRA0(a)       ((a) & CHI_CHIRXPTRA0MASK)

#define OFF_CHI_RCVPTRB         0x1e4
#define CHI_CHIRXPTRB3MASK      (0xf << 24)
#define CHI_CHIRXPTRB3(a)       (((a) << 24) & CHI_CHIRXPTRB3MASK)
#define CHI_CHIRXPTRB2MASK      (0xf << 16)
#define CHI_CHIRXPTRB2(a)       (((a) << 16) & CHI_CHIRXPTRB2MASK)
#define CHI_CHIRXPTRB1MASK      (0xf << 8)
#define CHI_CHIRXPTRB1(a)       (((a) << 8) & CHI_CHIRXPTRB1MASK)
#define CHI_CHIRXPTRB0MASK      0xf
#define CHI_CHIRXPTRB0(a)       ((a) & CHI_CHIRXPTRB0MASK)

#define OFF_CHI_TXPTRA          0x1e8
#define CHI_CHITXPTRA3MASK      (0xf << 24)
#define CHI_CHITXPTRA3(a)       (((a) << 24) & CHI_CHITXPTRA3MASK)
#define CHI_CHITXPTRA2MASK      (0xf << 16)
#define CHI_CHITXPTRA2(a)       (((a) << 16) & CHI_CHITXPTRA2MASK)
#define CHI_CHITXPTRA1MASK      (0xf << 8)
#define CHI_CHITXPTRA1(a)       (((a) << 8) & CHI_CHITXPTRA1MASK)
#define CHI_CHITXPTRA0MASK      0xf
#define CHI_CHITXPTRA0(a)       ((a) & CHI_CHITXPTRA0MASK)

#define OFF_CHI_TXPTRB          0x1ec
#define CHI_CHITXPTRB3MASK      (0xf << 24)
#define CHI_CHITXPTRB3(a)       (((a) << 24) & CHI_CHITXPTRB3MASK)
#define CHI_CHITXPTRB2MASK      (0xf << 16)
#define CHI_CHITXPTRB2(a)       (((a) << 16) & CHI_CHITXPTRB2MASK)
#define CHI_CHITXPTRB1MASK      (0xf << 8)
#define CHI_CHITXPTRB1(a)       (((a) << 8) & CHI_CHITXPTRB1MASK)
#define CHI_CHITXPTRB0MASK      0xf
#define CHI_CHITXPTRB0(a)       ((a) & CHI_CHITXPTRB0MASK)

#define OFF_CHI_SIZE            0x1f0
#define CHI_CHIDMAPTRMASK       0x3ffc0000
#define CHI_CHIDMAPTR(a)        (((a) << 18) & CHI_CHIDMAPTR)
#define CHI_CHIBUFF1TIME        (1 << 15)
#define CHI_CHIMALOOP           (1 << 14)
#define CHI_CHISIZEMASK         0x3ffc
#define CHI_CHISIZE(a)          (((a) << 2) & CHI_HISIZEMASK
#define CHI_ENDMARXCHI          (1 <<2)
#define CHI_ENDMATXCHI          1

#define OFF_CHIRXSTART          0x1f4
#define CHI_RXSTARTMASK         0xfffffffc

#define OFF_CHITXSTART          0x1f8
#define CHI_TXSTARTMASK         0xfffffffc

#define OFF_CHITXHOLD           0x1fc
#define OFF_CHIRXHOLD           0x1fc

/*
   Interrupt Register
 */
#define OFF_INTR_STATUS1        0x100

/* TX3912 */
#define INTR_LCDINT             (1 << 31)
#define INTR_DFINT              (1 << 30)

#define INTR_CHI0_5INT          (1 << 29)
#define INTR_CHI1_0INT          (1 << 28)
#define INTR_CHIDMACNTINT       (1 << 27)
#define INTR_CHIININTA          (1 << 26)
#define INTR_CHIININTB          (1 << 25)
#define INTR_CHIACTINT          (1 << 24)
#define INTR_CHIERRINT          (1 << 23)
#define INTR_SND0_5INT          (1 << 22)
#define INTR_SND1_0INT          (1 << 21)
#define INTR_TEL0_5INT          (1 << 20)
#define INTR_TEL1_0INT          (1 << 19)
#define INTR_SNDDMACNTINT       (1 << 18)
#define INTR_TELDMACNTINT       (1 << 17)
#define INTR_LSNDCLIPINT        (1 << 16)
#define INTR_RSNDCLIPINT        (1 << 15)
#define INTR_VALSNDPOSINT       (1 << 14)
#define INTR_VALSNDNEGINT       (1 << 13)
#define INTR_VALTELPOSINT       (1 << 12)
#define INTR_VALTELNEGINT       (1 << 11)
#define INTR_SNDININT           (1 << 10)
#define INTR_TELININT           (1 << 9)
#define INTR_SIBSF0INT          (1 << 8)
#define INTR_SIBSF1INT          (1 << 7)
#define INTR_SIBIRQPOSINT       (1 << 6)
#define INTR_SIBIRQNEGINT       (1 << 5)

#define OFF_INTR_STATUS2        0x104
#define INTR_UARTARXINT         (1 << 31)
#define INTR_UARTARXOVERRUNINT  (1 << 30)
#define INTR_UARTAFRAMEERRINT   (1 << 29)
#define INTR_UARTABREAKINT      (1 << 28)
#define INTR_UARTAPARITYERRINT  (1 << 27)
#define INTR_UARTATXINT         (1 << 26)
#define INTR_UARTATXOVERRUNINT  (1 << 25)
#define INTR_UARTAEMPTYINT      (1 << 24)
#define INTR_UARTADMAFULLINT    (1 << 23)
#define INTR_UARTADMAHALFINT    (1 << 22)
#define INTR_UARTBRXINT         (1 << 21)
#define INTR_UARTBRXOVERRUNINT  (1 << 20)
#define INTR_UARTBFRAMEERRINT   (1 << 19)
#define INTR_UARTBBREAKINT      (1 << 18)
#define INTR_UARTBPARITYERRINT  (1 << 17)
#define INTR_UARTBTXINT         (1 << 16)
#define INTR_UARTBTXOVERRUNINT  (1 << 15)
#define INTR_UARTBEMPTYINT      (1 << 14)
#define INTR_UARTBDMAFULLINT    (1 << 13)
#define INTR_UARTBDMAHALFINT    (1 << 12)

/* TX3912 */
#define INTR_MBUSTXBUFAVAILINT  (1 << 11)
#define INTR_MBUSTXERRINT       (1 << 10)
#define INTR_MBUSEMPTYINT       (1 << 9)
#define INTR_MBUSRXBUFAVAILINT  (1 << 8)
#define INTR_MBUSRXERRINT       (1 << 7)
#define INTR_MBUSDETINT         (1 << 6)
#define INTR_MBUSDMAFULLINT     (1 << 5)
#define INTR_MBUSDMAHALFINT     (1 << 4)
#define INTR_MBUSPOSINT         (1 << 3)
#define INTR_MBUSNEGINT         (1 << 2)


#define OFF_INTR_STATUS3        0x108
#define INTR_MFIOPOSINT(a)      (1 << (a))

#define OFF_INTR_STATUS4        0x10c
#define INTR_MFIONEGINT(a)      (1 << (a))

#define OFF_INTR_STATUS5        0x110
#define INTR_RTCINT             (1 << 31)
#define INTR_ALARMINT           (1 << 30)
#define INTR_PERINT             (1 << 29)
#define INTR_STPTIMERINT        (1 << 28)
#define INTR_POSPWRINT          (1 << 27)       
#define INTR_NEGPWRINT          (1 << 26)       
#define INTR_POSPWROKINT        (1 << 25)       
#define INTR_NEGPWROKINT        (1 << 24)       
#define INTR_POSONBUTNINT       (1 << 23)       
#define INTR_NEGONBUTNINT       (1 << 22)       
#define INTR_SPIBUFAVAILINT     (1 << 21)
#define INTR_SPIERRINT          (1 << 20)
#define INTR_SPIRCVINT          (1 << 19)
#define INTR_SPIEMPTYINT        (1 << 18)
#define INTR_IRCONSMINT         (1 << 17)
#define INTR_CARSTINT           (1 << 16)
#define INTR_POSCARINT          (1 << 15)
#define INTR_NEGCARINT          (1 << 14)

/* TX3912 */
#define INTR_IOPOSINT_3912(a)   (1 << ((a) + 7))
#define INTR_IONEGINT_3912(a)   (1 << (a))

/* GVPDA */
//#define INTR_IOPOSINT_MASK	0x00000f80	// colin : 20010425 button
#define INTR_IOPOSINT_MASK      0x00001f80      // MM    : 20010611 button


#define OFF_INTR_STATUS6        0x114
#define INTR_IRQHIGH            (1 << 31)
#define INTR_IRQLOW             (1 << 30)
#define POWEROKNMI              (1 << 29)    // TX3912AU  only
#define INTR_INTVECTMASK        0x0000003c
#define INTR_INTVECT(a)         (((a) << 2) & INTR_INTVECTMASK)
#define INTR_INTPRIO(status6)   ((status6 & INTR_INTVECTMASK) >> 2)

/* TX3922 */
#define OFF_INTR_STATUS7        0x130
#define INTR_IRTXCINT           (1 << 20)
#define INTR_IRRXCINT           (1 << 19)
#define INTR_IRTXEINT           (1 << 18)
#define INTR_IRRXEINT           (1 << 17)
#define INTR_IRSIRPXINT         (1 << 16)

#define OFF_INTR_STATUS8        0x138
#define INTR_IOPOSINT_3922(a)   (1 << ((a) + 16))
#define INTR_IONEGINT_3922(a)   (1 << (a))

#define OFF_INTR_CLEAR1         0x100
#define OFF_INTR_CLEAR2         0x104
#define OFF_INTR_CLEAR3         0x108
#define OFF_INTR_CLEAR4         0x10c
#define OFF_INTR_CLEAR5         0x110

/* TX3922 */
#define OFF_INTR_CLEAR7         0x130
#define OFF_INTR_CLEAR8         0x138

#define OFF_INTR_ENABLE1        0x118
#define OFF_INTR_ENABLE2        0x11c
#define OFF_INTR_ENABLE3        0x120
#define OFF_INTR_ENABLE4        0x124
#define OFF_INTR_ENABLE5        0x128


#define OFF_INTR_ENABLE6        0x12c
#define INTR_GLOBALEN           (1 << 18)
#define INTR_IRQPRITEST         (1 << 17)
#define INTR_IRQTEST            (1 << 16)
#define INTR_PRIORITYMASKMASK   0x0000ffff
#define INTR_PRIORITYMASK(a)    ((1 << (a)) & INTR_PRIORITYMASKMASK)

/* TX3922 */
#define OFF_INTR_ENABLE7        0x134
#define OFF_INTR_ENABLE8        0x13c


/* 
  IO Module
*/
#define OFF_IO_CTL              0x180

/* GVPDA-Tx3911 */
#define SWITCH_3_0	0
#define	SWITCH_3_1	1
#define	SWITCH_3_2	2
#define	SWITCH_6	3
#define	SWITCH_7	4
#define	SWITCH_8	5
#define	SWITCH_9	6

/* TX3912 */
#define IO_IODEBSELMASK_3912    0x7f000000
#define IO_IODEBSEL_3912(a)     (1 << ((a) + 24))
#define IO_IODIRECMASK_3912     0x007f0000
#define IO_IODIREC_3912(a)      (1 << ((a) + 16))
#define IO_IODIRECOUT_3912(a)   (1 << ((a) + 16))
#define IO_IODIRECIN_3912(a)    0
#define IO_IODOUTMASK_3912      0x00007f00
#define IO_IODOUT_3912(a)       (1 << ((a) + 8))
#define IO_IODINMASK_3912       0x0000007f
#define IO_IODIN_3912(a)        (1 << (a))

/* GVPDA */
#define IO_IODEBSELMASK_GVPDA   0x3f000000
#define IO_IODINMASK_GVPDA	0x0000003f	// MM
#define IO_IODIREC_GVPDA	0x003f0000

/* TX3922 */
#define IO_IODEBSELMASK_3922    0xffff0000
#define IO_IODEBSEL_3922(a)     (1 << ((a) + 16))
#define IO_IODIRECMASK_3922     0x0000ffff
#define IO_IODIREC_3922(a)      (1 << (a))

#define OFF_IO_INOUT            0x19c
#define IO_IODOUTMASK_3922      0xffff0000
#define IO_IODOUT_3922(a)       (1 << ((a) + 16))
#define IO_IODINMASK_3922       0x0000ffff
#define IO_IODIN_3922(a)        (1 << (a))


#define OFF_IO_MFIOOUT          0x184
#define IO_MFIOOUT(a)           (1 << (a))

#define OFF_IO_MFIODIREC        0x188
#define IO_MFIODIRECOUT(a)      (1 << (a))
#define IO_MFIODIRECIN(a)       0

#define OFF_IO_MFIOIN           0x18c
#define IO_MFIOIN(a)            (1 << (a))
//#define IO_MFIOIN_SDKEY_MASK    0x00080000	/* colin : record side key mask */

#define OFF_IO_MFIOSEL          0x190
#define IO_MFIOSEL(a)           (1 << (a))

#define OFF_IO_IOPWRDN          0x194
#define IO_IOPDMASK_3912        0x0000007f /* 3912 */
#define IO_IOPDMASK_3922        0x0000ffff /* 3922 */

#define IO_IOPD_3912(a)         ((1 << (a)) & IO_IOPDMASK_3912)
#define IO_IOPD_3922(a)         ((1 << (a)) & IO_IOPDMASK_3922)

#define OFF_IO_MFIOPWRDN        0x198
#define IO_MFIOPD(a)            (1 << (a))

/*	GVPDA-TX3911 MIO pings definition */
#define	EL_SWITCH		2	/* el switch -- on : 1, off : 0 */
#define	FLASH_WP		3	/* speaker -- on : 1, off : 0 */
#define	FLASH_RB		4
#define	POWER_MEASURE		5	/* power measure : 1, normal : 0 */
#define	FLASH_ALE		7
#define	FLASH_CLE		8
#define	FLASH_CARDIORD		9
#define	FLASH_CARDIOWR		10
#define	FLASH_CE		11
#define	SWITCH_11		16
#define	SWITCH_10		17
#define	SWITCH_5		18
#define	SWITCH_4		19
#define	LED_SWITCH		21
#define	NOR_FLASH_SWITCH	22
#define	BUZZER_SWITCH		25

/*
  IR Module
*/

#define OFF_IR_CTL1             0x0a0
#define IR_CARDET               (1 << 24)
#define IR_BAUDVALMASK          0x00ff0000
#define IR_BAUD(a)              (((a) << 16) & IR_BAUDVALMASK)
#ifdef INTERNAL_TEST
#define IR_TESTIR               (1 << 4)
#endif
#define IR_DTINVERT             (1 << 3)
#define IR_RXPWR                (1 << 2)
#define IR_ENSTATE              (1 << 1)
#define IR_ENCONSM              1

#define OFF_IR_CTL2             0x0a4
#define IR_PERMASK              0xff000000
#define IR_PER(a)               (((a) << 24) & IR_PERMASK)
#define IR_ONTIMEMASK           0x00ff0000
#define IR_ONTIME(a)            (((a) << 16) & IR_ONTIMEMASK)
#define IR_DELAYVALMASK         0x0000ff00
#define IR_DELAYVAL(a)          (((a) << 8) & IR_DELAYVALMASK)
#define IR_WAITVALMASK          0x000000ff
#define IR_WAITVAL(a)           ((a) & IR_WAITVALMASK)

#define OFF_IR_HOLD             0x0a8
#define IR_TXHOLDMASK           0x0000ffff
#define IR_TXHOLD(a)            ((a) & IR_TXHOLDMASK)

/* TX3912 */
#define OFF_MAGICBUS_CTL1       0x0e0
#define MAGICBUS_MBUSON         (1 << 31)
#define MAGICBUS_EMPTY          (1 << 30)
#define MAGICBUS_MBUSINT        (1 << 29)
#define MAGICBUS_ENDMARX        (1 << 16)
#define MAGICBUS_ENDMATX        (1 << 15)
#ifdef INTERNAL_TEST
#define MAGICBUS_ENDMATEST      (1 << 14)
#endif
#define MAGICBUS_ENDMALOOP      (1 << 13)
#define MAGICBUS_LOOPBACK       (1 << 12)
#define MAGICBUS_RCVPHAPOL      (1 << 11)
#define MAGICBUS_DTIDLE         (1 << 10)
#define MAGICBUS_ENBYTEWAIT     (1 << 9)
#define MAGICBUS_ENWORDWAIT     (1 << 8)
#define MAGICBUS_DETPHAPOL      (1 << 7)
#define MAGICBUS_DETCLKPOL      (1 << 6)
#define MAGICBUS_PHAPOL         (1 << 5)
#define MAGICBUS_CLKPOL         (1 << 4)
#define MAGICBUS_SLAVE          (1 << 3)
#define MAGICBUS_FSLAVE         (1 << 2)
#define MAGICBUS_LONG           (1 << 1)
#define MAGICBUS_ENMBUS         1

#define OFF_MAGICBUS_CTL2       0x0e4
#define MAGICBUS_DELAYVALMASK   0xff000000
#define MAGICBUS_DELAYVAL(a)    (((a) << 24) & MAGICBUS_DELAYVALMASK)
#define MAGICBUS_BAUDRATEMASK   0x00ff0000
#define MAGICBUS_BAUDRATE(a)    (((a) << 16) & MAGICBUS_BAUDRATEMASK)
                                
#define OFF_MAGICBUS_DMACTL1    0x0e8
#define MAGICBUS_DMASTARTVALMASK 0xfffffffc
#define MAGICBUS_DMASTARTVAL(a) ((a) & MAGICBUS_DMASTARTVALMASK)

#define OFF_MAGICBUS_DMACTL2    0x0ec
#define MAGICBUS_DMALENGTHMASK  0x000ffffc
#define MAGICBUS_DMALENGTH(a)   ((a) & MAGICBUS_DMALENGTHMASK)
                                
#define OFF_MAGICBUS_DMACOUNT   0x0f0
#define MAGICBUS_DMACOUNTMASK   0x000ffffc

#define OFF_MAGICBUS_TXHOLD     0x0f8
#define OFF_MAGICBUS_TXHOLD_SWAP 0x0f4

#define OFF_MAGICBUS_RXHOLD     0x0f8

/*
  Power Management
*/
#define OFF_POWER_CTL           0x1c4
#define POWER_ONBUTN            (1 << 31)
#define POWER_PWRINT            (1 << 30)
#define POWER_PWROK             (1 << 29)

#define POWER_PWROKNMI          (1 << 28)       /* TX3922 */

#define POWER_VIDREFMASK        (3 << 27)       /* TX3912 */
#define POWER_VIDREF(a)         (((a) << 27) & POWER_VIDREFMASK)

#define POWER_SLOWBUS           (1 << 26)

#define POWER_DIVMOD            (1 << 25)       /* TX3912 */

#define POWER_STPTIMERVALMASK   0x0000f000
#define POWER_STPTIMERVAL(a)    (((a) << 12) & POWER_STPTIMERVALMASK)
#define POWER_ENSTPTIMER        (1 << 11)
#define POWER_ENFORCESHUTDOWN   (1 << 10)
#define POWER_FORCESHUTDOWN     (1 << 9)
#define POWER_FORCESHUTDOWNOCC  (1 << 8)
#define POWER_SELC2MS           (1 << 7)

#define POWER_WARMSTART         (1 << 6)        /* TX3922 */

#define POWER_BPDBVCC3          (1 << 5)
#define POWER_STOPCPU           (1 << 4)
#define POWER_DBNCONBUTN        (1 << 3)
#define POWER_COLDSTART         (1 << 2)
#define POWER_PWRCS             (1 << 1)
#define POWER_VCCON             (1 << 0)

/*
  SIB Module
*/
#define OFF_SIB_SIZE            0x60
#define SIB_SNDSIZEMASK         0x3ffc0000
#define SIB_SNDSIZE(a)          (((a) << 16) & SIB_SNDSIZEMASK)
#define SIB_TELSIZEMASK         0x00003ffc
#define SIB_TELSIZE(a)          ((a) & SIB_TELSIZEMASK)

#define OFF_SIB_SNDRXSTART      0x64
#define SIB_SNDRXSTARTMASK      0xfffffffc
#define SIB_SNDRXSTART(a)       ((a) & SIB_SNDRXSTARTMASK)

#define OFF_SIB_SNDTXSTART      0x68
#define SIB_SNDTXSTARTMASK      0xfffffffc
#define SIB_SNDTXSTART(a)       ((a) & SIB_SNDRXSTARTMASK)

#define OFF_SIB_TELRXSTART      0x6c
#define SIB_TELRXSTARTMASK      0xfffffffc
#define SIB_TELRXSTART(a)       ((a) & SIB_TELRXSTARTMASK)

#define OFF_SIB_TELTXSTART      0x70
#define SIB_TELTXSTARTMASK      0xfffffffc
#define SIB_TELTXSTART(a)       ((a) & SIB_TELRXSTARTMASK)

#define OFF_SIB_CTL             0x74
#define SIB_SIBIRQ              (1 << 31)
#ifdef INTERNAL_TEST
#define SIB_ENCNTTEST           (1 << 30)
#define SIB_ENDMATEST           (1 << 29)
#endif
#define SIB_SNDMONO             (1 << 28)
#define SIB_RMONOSNDIN          (1 << 27)
#define SIB_SCLKDIVMASK         (7 << 24)
#define SIB_SCLKDIV(a)          (((a) << 24) & SIB_SCLKDIVMASK)
#define SIB_TEL16               (1 << 23)
#define SIB_TELFSDIVMASK        0x007f0000
#define SIB_TELFSDIV(a)         (((a) << 16) & SIB_TELFSDIVMASK)
#define SIB_SND16               (1 << 15)
#define SIB_SNDFSDIVMASK        0x00007f00
#define SIB_SNDFSDIV(a)         (((a) << 8) & SIB_SNDFSDIVMASK)
#define SIB_SELTELSF1           (1 << 7)
#define SIB_SELSNDSF1           (1 << 6)
#define SIB_ENTEL               (1 << 5)
#define SIB_ENSND               (1 << 4)
#define SIB_SIBLOOP             (1 << 3)
#define SIB_ENSF1               (1 << 2)
#define SIB_ENSF0               (1 << 1)
#define SIB_ENSIB               1

#define OFF_SIB_SNDTXHOLD       0x078
#define OFF_SIB_SNDRXHOLD       0x078

#define OFF_SIB_TELTXHOLD       0x07c
#define OFF_SIB_TELRXHOLD       0x07c

#define OFF_SIB_SF0AUX          0x80
#define OFF_SIB_SF1AUX          0x84

#define OFF_SIB_SF0STATUS       0x88
#define OFF_SIB_SF1STATUS       0x8c

#define SIB_SF0_DATAMASK        0x0000ffff
#define SIB_SF0_ADDRMASK        0x78000000
#define SIB_SF0_ADDRSHIFT       27
#define SIB_SF0_WRITEBIT        (1 << 26)
#define SIB_SF0_AUDIOVALID_BIT  (1 << 17)
#define SIB_SF0_TELECOMVALID_BIT (1 << 16)

#define OFF_SIB_DMACTL          0x90
#define SIB_SNDBUFF1TIME        (1 << 31)
#define SIB_SNDDMALOOP          (1 << 30)
#define SIB_SNDDMAPTRMASK       0x3ffc0000
#define SIB_ENDMARXSND          (1 << 17)
#define SIB_ENDMATXSND          (1 << 16)
#define SIB_TELBUFF1TIME        (1 << 15)
#define SIB_TELDMALOOP          (1 << 14)
#define SIB_TELDMAPTRMASK       0x00003ffc
#define SIB_ENDMARXTEL          (1 << 1)
#define SIB_ENDMATXTEL          1

/*
  SPI Module
 */
#define OFF_SPI_CTL             0x160
#define SPI_SPION               (1 << 17)
#define SPI_EMPTY               (1 << 16)
#define SPI_DELAYVALMASK        0x0000f000
#define SPI_DELAYVAL(a)         (((a) << 12) & SPI_DELAYVALMASK)
#define SPI_BAUDRATEMASK        0x00000f00
#define SPI_BAUDRATE(a)         (((a) << 8) & SPI_BAUDRATEMASK)
#define SPI_PHAPOL              (1 << 5)
#define SPI_CHKPOL              (1 << 4)
#define SPI_WORD                (1 << 2)
#define SPI_LSB                 (1 << 1)
#define SPI_ENSPI               1

#define OFF_SPI_TXDATA          0x164
#define SPI_TXDATAMASK          0x0000ffff
#define OFF_SPI_RXDATA          0x164
#define SPI_RXDATAMASK          0x0000ffff

/*
   Timer
 */
/* RTC */
#define OFF_RTC_RTCHI           0x140

#define RTC_RTCHIMASK_3912      0x000000ff
#define RTC_BITNUM_3912         40
#define RTC_RTCHIMASK_3922      0x00000fff
#define RTC_BITNUM_3922         44

#define OFF_RTC_RTCLOW          0x144

#if 1
#define RTC_RTCHIMASK   RTC_RTCHIMASK_3912
#define RTC_BITNUM      RTC_BITNUM_3912
#else
#if TXTYPE==3912
#define RTC_RTCHIMASK   RTC_RTCHIMASK_3912
#define RTC_BITNUM      RTC_BITNUM_3912
#else
#define RTC_RTCHIMASK   RTC_RTCHIMASK_3922
#define RTC_BITNUM      RTC_BITNUM_3922
#endif
#endif
#define RTC_RTCMASK     ((DWORDLONG)RTC_RTCHIMASK << 32 | 0xffffffff)

/* Alarm */
#define OFF_ALARM_ALARMHI       0x148
#define ALARM_ALARMHIMASK       RTC_RTCHIMASK
#define OFF_ALARM_ALARMLOW      0x14c

/* Timer */
#define OFF_TIMER_CTL           0x150
#define TIMER_FREEZEPRE         (1 << 7)
#define TIMER_FREEZERTC         (1 << 6)
#define TIMER_FREEZETIMER       (1 << 5)
#define TIMER_ENPERTIMER        (1 << 4)
#define TIMER_RTCCLR            (1 << 3)
#define TIMER_TESTC8MS          (1 << 2)
#define TIMER_ENTESTCLK         (1 << 1)
#define TIMER_ENRTCTEST         1

#define OFF_PERIODIC_TIMER      0x154
#define PERIODIC_PERCNTSHIFT    16
#define PERIODIC_PERCNTMASK     0xffff0000
#define PERIODIC_PERCNT(a)      (((a) << PERIODIC_PERCNTSHIFT) & PERIODIC_PERCNTMASK)
#define PERIODIC_PERVALMASK     0x0000ffff
#define PERIODIC_PERVAL(a)      ((a) & PERIODIC_PERVALMASK)

/*
    Serial Definition
*/
#define OFF_UARTA_CTL1          0x0b0
#define OFF_UARTB_CTL1          0x0c8

#define UART_UARTON             (1 << 31)
#define UART_EMPTY              (1 << 30)
#define UART_PRXHOLDFULL        (1 << 29)
#define UART_RXHOLDFULL         (1 << 28)
#define UART_ENDMARX            (1 << 15)
#define UART_ENDMATX            (1 << 14)
#ifdef INTERNAL_TEST
#define UART_TESTMODE           (1 << 13)
#endif
#define UART_ENBREAKHALT        (1 << 12)
#define UART_ENDMATEST          (1 << 11)
#define UART_ENDMALOOP          (1 << 10)
#define UART_PULSEOPT2          (1 << 9)
#define UART_PULSEOPT1          (1 << 8)
#define UART_DTINVERT           (1 << 7)
#define UART_DISTXD             (1 << 6)
#define UART_TWOSTOP            (1 << 5)
#define UART_LOOPBACK           (1 << 4)
#define UART_BIT_7              (1 << 3)
#define UART_EVENPARITY         (1 << 2)
#define UART_ENPARITY           (1 << 1)
#define UART_ENUART             1

#define OFF_UARTA_CTL2          0x0b4
#define OFF_UARTB_CTL2          0x0cc
#define UART_BAUDMASK_18432     0x000003ff      // lower 10bits
#define UART_BAUDMASK_3686      0x000003ff      // lower 10bits
#define UART_BAUDMASK_3728      0x000003ff      // lower 10bits
#define UART_BAUDMASK_9216      0x000007ff      // lower 11bits

#if	1	/* DJH */
/*
	CLK2X = 6.912*8MHz = 55.296
	----->	SIBMCLKDIR = 1
		ENSIBMCLK = 1
		SIBMCLKDIV[2:0] = 1 : Divided by 3 ==> SIBMCLK = 18.432MHz
		CSERSEL = 0 : Select SIBMCLK as source
		CSERDIV[2:0] = 0 : Divided by 2 ==> fUARTCLK = 9.216Mhz
*/
#endif
/*
   Standart UART Values for base clock 1.8432MHz
   This comes from: baud rate = 1.8432 / ((BaudRate + 1) * 16)
*/
#define UART_B115200_18432	4
#define UART_B57600_18432	1
#define UART_B38400_18432	2
#define UART_B19200_18432	5
#define UART_B14400_18432	7
#define UART_B9600_18432	11
#define UART_B4800_18432	23
#define UART_B2400_18432	47
#define UART_B1200_18432	95
#define UART_B600_18432		191
#define UART_B300_18432		383

#if	1	/* DJH */
/*
	CLK2X = 7.3728*8MHz = 58.9824	 CSERCLKDIV = 4
	Uart Clk =  (CLK2X/4)/CSERCLKDIV
	TX3911-58 : (58.9824/4)/CSERCLKDIV = 3.6864
*/
#endif
/*
   Standart UART Values for base clock 3.6864MHz
   This comes from: baud rate = 3.6864MHz / ((BaudRate + 1) * 16)
*/
#if 0
#define UART_B115200_3686       1
#define UART_B57600_3686        3
#define UART_B38400_3686        5
#define UART_B19200_3686        11
#define UART_B14400_3686        15
#define UART_B9600_3686         23
#define UART_B4800_3686         47
#define UART_B2400_3686         95
#define UART_B1200_3686         191
#define UART_B600_3686          383
#define UART_B300_3686          767
#else		// colin 
#if 0
#define UART_B115200_3728       3
#define UART_B57600_3728        7
#define UART_B38400_3728        11
#define UART_B19200_3728        23
#define UART_B14400_3728        31
#define UART_B9600_3728         47
#define UART_B4800_3728         95
#define UART_B2400_3728         191
#define UART_B1200_3728         383
#define UART_B600_3728          767
#define UART_B300_3728          1535
#else
//DJH,20010920
#define UART_B115200_3728       1
#define UART_B57600_3728        3
#define UART_B38400_3728        5
#define UART_B19200_3728        11
#define UART_B14400_3728        15
#define UART_B9600_3728         23
#define UART_B4800_3728         47
#define UART_B2400_3728         95
#define UART_B1200_3728         191
#define UART_B600_3728          383
#define UART_B300_3728          767
#endif

#endif

/*
   Standart UART Values for base clock 9.216MHz
   This comes from: baud rate = 9.216MHz / ((BaudRate + 1) * 16)
*/
#define UART_B115200_9216       4
#define UART_B57600_9216        9
#define UART_B38400_9216        14
#define UART_B19200_9216        29
#define UART_B14400_9216        39
#define UART_B9600_9216         59
#define UART_B4800_9216         119
#define UART_B2400_9216         239
#define UART_B1200_9216         479
#define UART_B600_9216          959
#define UART_B300_9216          1919

/* TX3922 : UART clock 9.216MHz */
#define UART_B115200_3922       UART_B115200_9216
#define UART_B57600_3922        UART_B57600_9216
#define UART_B38400_3922        UART_B38400_9216
#define UART_B19200_3922        UART_B19200_9216
#define UART_B14400_3922        UART_B14400_9216
#define UART_B9600_3922         UART_B9600_9216
#define UART_B4800_3922         UART_B4800_9216
#define UART_B2400_3922         UART_B2400_9216
#define UART_B1200_3922         UART_B1200_9216
#define UART_B600_3922          UART_B600_9216
#define UART_B300_3922          UART_B300_9216

/* TX3912-75 : UART clock 3.6864MHz */
#define UART_B115200_3912_75    UART_B115200_3686
#define UART_B57600_3912_75     UART_B57600_3686
#define UART_B38400_3912_75     UART_B38400_3686
#define UART_B19200_3912_75     UART_B19200_3686
#define UART_B14400_3912_75     UART_B14400_3686
#define UART_B9600_3912_75      UART_B9600_3686
#define UART_B4800_3912_75      UART_B4800_3686
#define UART_B2400_3912_75      UART_B2400_3686
#define UART_B1200_3912_75      UART_B1200_3686
#define UART_B600_3912_75       UART_B600_3686
#define UART_B300_3912_75       UART_B300_3686

/* TX3912-92 : UART clock 9.216MHz */
#define UART_B115200_3912_92    UART_B115200_9216
#define UART_B57600_3912_92     UART_B57600_9216
#define UART_B38400_3912_92     UART_B38400_9216
#define UART_B19200_3912_92     UART_B19200_9216
#define UART_B14400_3912_92     UART_B14400_9216
#define UART_B9600_3912_92      UART_B9600_9216
#define UART_B4800_3912_92      UART_B4800_9216
#define UART_B2400_3912_92      UART_B2400_9216
#define UART_B1200_3912_92      UART_B1200_9216
#define UART_B600_3912_92       UART_B600_9216
#define UART_B300_3912_92       UART_B300_9216

#define OFF_UARTA_DMA_CTL1      0x0b8
#define OFF_UARTB_DMA_CTL1      0x0d0
#define UART_DMASTARTVALMASK    0xfffffffc

#define OFF_UARTA_DMA_CTL2      0x0bc
#define OFF_UARTB_DMA_CTL2      0x0d4
#define OFF_UARTA_DMA_COUNT     0x0c0
#define OFF_UARTB_DMA_COUNT     0x0d8
#define UART_DMALENGTHMASK      0x0000ffff

#define OFF_UARTA_TXDATA        0x0c4   /* Write Only */
#define OFF_UARTB_TXDATA        0x0dc   /* Write Only */
#define OFF_UARTA_RXDATA        0x0c4   /* Read Only */
#define OFF_UARTB_RXDATA        0x0dc   /* Read Only */
#define UART_BREAK              0x00000100  /* send/received break */
#define UART_TXDATAMASK         0x000000ff  /* send data mask */
#define UART_RXDATAMASK         0x000000ff  /* receive data mask */
#define UART_TXDATA(a)		((a) & UART_TXDATAMASK)

/*
   Video (TX3912)
 */
#define OFF_VIDEO_CTL1          0x28
#define VIDEO_LINECNTMASK       0xffc00000
#define VIDEO_LINECNT(a)        (((a) << 22) & VIDEO_LINECNTMASK)
#define VIDEO_LOADDLY           (1 << 21)
#define VIDEO_BAUDVALMASK       0x001f0000
#define VIDEO_BAUDVAL(a)        (((a) << 16) & VIDEO_BAUDVALMASK)
#define VIDEO_VIDDONEVALMASK    0x0000fe00
#define VIDEO_VIDDONEVAL(a)     (((a) << 9) & VIDEO_VIDDONEVALMASK)
#define VIDEO_ENFREEZEFRAME     (1 << 8)
#define VIDEO_BITSELMASK        0x000000c0
#define VIDEO_BITSEL_8BPP       0x000000c0
#define VIDEO_BITSEL_4BPP       0x00000080
#define VIDEO_BITSEL_2BPP       0x00000040
#define VIDEO_BITSEL_MONO       0x00000000
#define VIDEO_DISPSPLIT         (1 << 5)
#define VIDEO_DISP8             (1 << 4)
#define VIDEO_DFMODE            (1 << 3)
#define VIDEO_INVVID            (1 << 2)
#define VIDEO_DISPON            (1 << 1)
#define VIDEO_ENVID             1

#define OFF_VIDEO_CTL2          0x2c
#define VIDEO_VIDRATEMASK       0xffc00000
#define VIDEO_VIDRATE(a)        (((a) << 22) & VIDEO_VIDRATEMASK)
#define VIDEO_HORZVALMASK       0x001ff000
#define VIDEO_HORZVAL(a)        (((a) << 12) & VIDEO_HORZVALMASK)
#define VIDEO_LINEVALMASK       0x000003ff
#define VIDEO_LINEVAL(a)        ((a) & VIDEO_LINEVALMASK)

#define OFF_VIDEO_CTL3          0x30
#define VIDEO_VIDBANKMASK       0xfff00000
#define VIDEO_VIDBANK(a)        ((a) & VIDEO_VIDBANKMASK)
#define VIDEO_VIDBASEHIMASK     0x000ffff0
#define VIDEO_VIDBASEHI(a)      ((a) & VIDEO_VIDBASEHIMASK)

#define OFF_VIDEO_CTL4          0x34
#define VIDEO_DFVALMASK         0xff000000
#define VIDEO_DFVAL(a)          (((a) << 24) & VIDEO_DFVALMASK)
#define VIDEO_FRAMEMASKVALMASK  0x00f00000
#define VIDEO_FRAMEMASKVAL(a)   (((a) << 20) & VIDEO_FRAMEMASKVALMASK)
#define VIDEO_VIDBASELOMASK     0x000ffff0
#define VIDEO_VIDBASELO(a)      ((a) & VIDEO_VIDBASELOMASK)

#define OFF_VIDEO_CTL5          0x38
#define VIDEO_RSEL(a,b)         (((b) & 0x0000000f) << ((a) * 4))

#define OFF_VIDEO_CTL6          0x3c
#define VIDEO_GSEL(a,b)         (((b) & 0x0000000f) << ((a) * 4))

#define OFF_VIDEO_CTL7          0x40
#define VIDEO_BSEL(a,b)         (((b) & 0x0000000f) << ((a) * 4))
#define VIDEO_BLUESELMASK       0x0000ffff
#define VIDEO_BLUESEL(a)        ((a) & VIDEO_BLUESEL)

#define OFF_VIDEO_CTL8          0x44
#define VIDEO_PAT2_3MASK        0x00000fff
#define VIDEO_PAT2_3(a)         ((a) & VIDEO_PAT2_3MASK)
#define VIDEO_CTL8_DEFVAL       VIDEO_PAT2_3(0x7da)

#define OFF_VIDEO_CTL9          0x48
#define VIDEO_PAT3_4MASK        0xffff0000
#define VIDEO_PAT3_4(a)         (((a) << 16) & VIDEO_PAT3_4MASK)
#define VIDEO_PAT2_4MASK        0x0000ffff
#define VIDEO_PAT2_4(a)         ((a) & VIDEO_PAT2_4MASK)
#define VIDEO_CTL9_DEFVAL       (VIDEO_PAT3_4(0x7dbe) | VIDEO_PAT2_4(0xa5a5))

#define OFF_VIDEO_CTL10         0x4c
#define VIDEO_PAT4_5MASK        0x000fffff
#define VIDEO_PAT4_5(a)         ((a) & VIDEO_PAT4_5MASK)
#define VIDEO_CTL10_DEFVAL      (VIDEO_PAT4_5(0x7dfbe))

#define OFF_VIDEO_CTL11         0x50
#define VIDEO_PAT3_5MASK        0x000fffff
#define VIDEO_PAT3_5(a)         ((a) & VIDEO_PAT3_5MASK)
#define VIDEO_CTL11_DEFVAL      VIDEO_PAT3_5(0x7a5ad)

#define OFF_VIDEO_CTL12         0x54
#define VIDEO_PAT6_7MASK        0x0fffffff
#define VIDEO_PAT6_7(a)         ((a) & VIDEO_PAT6_7MASK)
#define VIDEO_CTL12_DEFVAL      VIDEO_PAT6_7(0xfbfdfe7)

#define OFF_VIDEO_CTL13         0x58
#define VIDEO_PAT5_7MASK        0x0fffffff
#define VIDEO_PAT5_7(a)         ((a) & VIDEO_PAT5_7MASK)
#define VIDEO_CTL13_DEFVAL      VIDEO_PAT5_7(0x7b5adef)
 
#define OFF_VIDEO_CTL14         0x5c
#define VIDEO_PAT4_7MASK        0x0fffffff
#define VIDEO_PAT4_7(a)         ((a) & VIDEO_PAT4_7MASK)
#define VIDEO_CTL14_DEFVAL      VIDEO_PAT4_7(0xb9dc663)

/*
  IrDA Module (3922)
 */

#define OFF_IRDA_RINGP          0x28
#define IRDA_TXPOINTMASK        0x00003f00
#define IRDA_TXPOINT(a)         (((a) << 8) & FIR_TXPOINTMASK)
#define IRDA_RXPOINTMASK        0x0000003f
#define IRDA_RXPOINT(a)         (((a) & FIR_RXPOINTMASK)

#define OFF_IRDA_BASE_ADDR      0x2c
#define IRDA_ADDRMASK           0xfffffc00

#define OFF_IRDA_RING_SIZE      0x30
#define IRDA_TXENTRYMASK        0x0000f000
#define IRDA_TXENTRY(a)         (((a) << 12) & IRDA_TXENTRYMASK)
#define IRDA_RXENTRYMASK        0x00000f00
#define IRDA_RXENTRY(a)         (((a) << 8) & IRDA_RXENTRYMASK)

/*
   Standart FIR Values  (3922)
 */
#define IRDA_RING_ENTRY_4       0x00
#define IRDA_RING_ENTRY_8       0x01
#define IRDA_RING_ENTRY_16      0x03
#define IRDA_RING_ENTRY_32      0x07
#define IRDA_RING_ENTRY_64      0x0f

#define OFF_IRDA_RING_PROMPT    0x34
                                
#define OFF_IRDA_CONFIG0        0x38
#define IRDA_TXONLOOP           (1 << 15)
#define IRDA_LOOPBACK           (1 << 14)
#define IRDA_ENTX               (1 << 12)
#define IRDA_ENRX               (1 << 11)
#define IRDA_ENDMAC             (1 << 10)
#define IRDA_RECVANY            (1 << 9)
#define IRDA_CRC16N32           (1 << 7)
#define IRDA_ENFIR              (1 << 6)
#define IRDA_ENMIR              (1 << 5)
#define IRDA_ENSIR              (1 << 4)
#define IRDA_ENSIRFILTER        (1 << 3)
#define IRDA_SIRTEST            (1 << 2)
#define IRDA_INVERTTX           (1 << 1)
#define IRDA_INVERTRX           1

#define OFF_IRDA_CONFIG1        0x3c
#define IRDA_ENCORE             (1 << 15)
#define IRDA_XCVRON             (1 << 11)
#define IRDA_CANRX              (1 << 9)
#define IRDA_CANSIRRX           (1 << 8)
#define IRDA_SIPINTR            (1 << 7)
#define IRDA_RXINPUT            (1 << 6)
#define IRDA_RXALWAYS           (1 << 5)

#define OFF_IRDA_SIRFLAG        0x40
#define IRDA_EOFMASK            0x0000ff00
#define IRDA_BOFMASK            0x000000ff

#define OFF_IRDA_ENABLE         0x44
#define IRDA_PHYANDCLOCK        (1 << 15)
#define IRDA_CONFIGERROR        (1 << 14)
#define IRDA_FIRON              (1 << 13)
#define IRDA_MIRON              (1 << 12)
#define IRDA_SIRON              (1 << 11)
#define IRDA_IE_ENTX            (1 << 10)
#define IRDA_IE_ENRX            (1 << 9)
#define IRDA_CRC16N32SIZE       (1 << 8)
#define IRDA_BROADCAST          1

#define OFF_IRDA_CONFIG2PHY     0x48
#define OFF_IRDA_PHYSCONFIG     0x4c
#define IRDA_BAUDRATEMASK       0x0000fc00
#define IRDA_BAUDRATE(a)        (((a) << 10) & IRDA_BAUDRATEMASK) 
#define IRDA_PULSEWIDTHMASK     0x000003e0
#define IRDA_PULSEWIDTH(a)      (((a) << 5) & IRDA_PULSEWIDTHMASK)
#define IRDA_PREAMBLEMASK       0x0000001f
#define IRDA_PREAMBLE(a)        ((a) & IRDA_PREAMBLEMASK)

/*
   Standart FIR Values  (3922)
*/
#define SIR_B115200             0
#define SIR_B57600              1
#define SIR_B38400              2
#define SIR_B19200              5
#define SIR_B9600               11
#define SIR_B2400               47
#define SIR_PULSEWIDTH_3_16     0       // 3/16 width
#define SIR_PULSEWIDTH_163      25      // 1.63us

#define MIR_B1152000            0       // 1.152Mbps
#define MIR_PULSEWIDTH          10
#define MIR_PREAMBLE_1M         1

#define FIR_B4000000            0       // 4Mbps (4PPM)
#define FIR_PREAMBLE_4M         15

#define OFF_IRDA_MAXPACKETLEN   0x50
#define IRDA_RXMASK             0x0000ffff

#define OFF_IRDA_RECV_BYTE_CNT  0x54
#define IRDA_RXCOUNTMASK        0x00000fff

#define OFF_IRDA_UNICAST_FILT   0x58
#define IRDA_ADDR4MASK          0xff000000
#define IRDA_ADDR4(a)           (((a) << 24) & IRDA_ADDR4MASK)
#define IRDA_ADDR3MASK          0x00ff0000
#define IRDA_ADDR3(a)           (((a) << 16) & IRDA_ADDR3MASK)
#define IRDA_ADDR2MASK          0x0000ff00
#define IRDA_ADDR2(a)           (((a) << 8) & IRDA_ADDR2MASK)
#define IRDA_ADDR1MASK          0x000000ff
#define IRDA_ADDR1(a)           ((a) & IRDA_ADDR1MASK)
#define IRDA_FIL_ADDRMASK       0x000000fe
#define IRDA_FIL_VALIDMASK      0x00000001

#define OFF_IRDA_STATUS_REG     0x5C
#define IRDA_RXMAXCOUNT         (1 << 11)
#define IRDA_TXUNDERRUN         (1 << 10)
#define IRDA_RXOVERRUN          (1 << 9)
#define IRDA_SLEEP              (1 << 8)
#define IRDA_RXOFFTIMEMASK      0x00000070
#define IRDA_RXOFFTIME(a)       (((a) < 4) & FIR_RXOFFTIMEMASK)
#define IRDA_RXBUSY             (1 << 3)
#define IRDA_FIRRX              (1 << 2)
#define IRDA_MIRRX              (1 << 1)
#define IRDA_SIRRX              1

/*
   Standart FIR Values  (3922)
*/
#define IRDA_OFFTIME_LT_10      0x0 // timeout < 10
#define IRDA_OFFTIME_GE_10      0x1 // Timeout >= 10 ms
#define IRDA_OFFTIME_GE_50      0x2 // Timeout >= 50 ms
#define IRDA_OFFTIME_GE_100     0x3 // Timeout >= 100 ms
#define IRDA_OFFTIME_GE_200     0x4 // Timeout >= 200 ms
#define IRDA_OFFTIME_GE_300     0x5 // Timeout >= 300 ms
#define IRDA_OFFTIME_GE_400     0x6 // Timeout >= 400 ms
#define IRDA_OFFTIME_GE_500     0x7 // Timeout >= 500 ms

                     
/*
   Revision register
   When 3912 Rev 1.1 and 3922 #1, this register is equal to offset 0x008.
 */

#define OFF_REVISION            0x208
#define REV_MASK                0xff

/*
  Register Access Macros..
*/

#define OFF_SIB_CONTROL         0x74
#define OFF_SIB_DMACONTROL      0x90

/*
 * Timer definitions
 */
#define TIMERCLK_1440	1440	/* 1.440MHz */
#define TIMERCLK_1297	1297	/* 1.297MHz */
#define TIMERCLK_1152	1152	/* 1.152MHz */
#define TIMERCLK_1008	1008	/* 1.008MHz */
#ifdef	CONFIG_GVPDA_TX3911
/* I want to use the precise value */
/*	((SYSCLK*PLL)/2) = CLK, CLK/32 = TimerMasterClock  -see P6-3 */
#define TIMERCLK_921600	921600	/* 921600 = ((7.3728*1000000*8)/2)/32 */
#define TIMERCLK_864000	864000	/* 864000  = ((6.912*1000000*8)/2)/32 */
#endif

#ifdef	CONFIG_GVPDA_TX3911
#define PERVAL_TX3911_55        TIMERCLK_864000
#define PERVAL_TX3911_58        TIMERCLK_921600
#endif
#define PERVAL_TX3912_75        TIMERCLK_1152
#define PERVAL_TX3912_92        TIMERCLK_1440
#define PERVAL_TX3922_166       TIMERCLK_1297
#define PERVAL_TX3922_148       TIMERCLK_1152
#define PERVAL_TX3922_129       TIMERCLK_1008

#ifdef	CONFIG_GVPDA_TX3911
#define BASECLK_6912            (6912)
#define BASECLK_7372            (7372)
#endif
#define BASECLK_8064            (8064)
#define BASECLK_9216            (9216)
#define BASECLK_10364           (10364)
#define BASECLK_11520           (11520)
#define BASECLK_UNKNOWN         (0)

#ifdef	CONFIG_GVPDA_TX3911
#define CLOCK_55_2Mhz           (55)
#define CLOCK_58_9Mhz           (59)
#endif
#define CLOCK_64_5Mhz           (64)
#define CLOCK_73_7Mhz           (75)
#define CLOCK_82_9Mhz           (83)
#define CLOCK_92_1Mhz           (92)
#define CLOCK_129_0Mhz          (129)
#define CLOCK_147_5Mhz          (148)
#define CLOCK_165_8Mhz          (166)
#define CLOCK_UNKNOWN           (0)

#endif /* _TX39_H_ */
