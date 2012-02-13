/*
 * Copyright c                Realtek Semiconductor Corporation, 2002
 * All rights reserved.                                                    
 * 
 * $Header: /home/cvsroot/uClinux-dist/linux-2.4.x/arch/mipsnommu/rtl865x/traceStk.c,v 1.3 2004/05/17 12:06:34 cfliu Exp $
 *
 * $Author: cfliu $
 *
 * Abstract:
 *
 *   Lexra bus controller driver source code.
 *
 * $Log: traceStk.c,v $
 * Revision 1.3  2004/05/17 12:06:34  cfliu
 * no message
 *
 * Revision 1.2  2004/05/13 05:47:32  tony
 * new features: support PPTP/L2TP with WLAN on RTL865XB.
 *
 * Revision 1.1  2003/12/18 11:26:57  rupert
 * add Debug Utility
 *
 * Revision 1.2  2003/09/08 10:20:26  danwu
 * return to the instruction next to EPC (or delay slot if the instruction at EPC is
 * a branch or jump one) while restore from exception
 *
 * Revision 1.1  2003/09/08 09:22:44  danwu
 * init
 *
 * Revision 1.3  2003/06/16 03:29:24  danwu
 * add asicregs.h
 *
 * Revision 1.2  2003/03/20 14:22:36  danwu
 * no message
 *
 * Revision 1.1  2002/11/19 10:20:34  henryCho
 * Create.
 *
 */
#include <linux/config.h>
#include <linux/init.h>


/* LOCAL DEFINITIONS 
*/
#define JR_RA_INSTR         0x03e00008
#define ADDIU_SP_SP_INSTR   0x27bd0000
#define LW_RA_SP_INSTR      0x8fbf0000

#define OP_MASK        0xfc000000
#define SUBCODE_MASK   0x0000003f
#define RS_MASK        0x03e00000
#define RT_MASK        0x001f0000
#define BROFFSET_MASK  0x0000ffff
#define TARGET_MASK    0x03ffffff

#define JR_INSTR       0x00000008
#define JALR_INSTR     0x00000009
#define BREAK_INSTR    0x0005000d
#define BLTZ_INSTR     0x04000000
#define BGEZ_INSTR     0x04010000
#define BLTZAL_INSTR   0x04100000
#define BGEZAL_INSTR   0x04110000
#define J_INSTR        0x08000000
#define JAL_INSTR      0x0c000000
#define BEQ_INSTR      0x10000000
#define BNE_INSTR      0x14000000
#define BLEZ_INSTR     0x18000000
#define BGTZ_INSTR     0x1c000000



/* EXTERN VARIABLES
*/
extern void * pExceptionContext;

typedef struct {
  unsigned r[26];
  unsigned s8;
  unsigned ra;
  unsigned sr;
  unsigned pc;
  unsigned gp;
  unsigned sp;
} lx4180_regfile_t;


int isValidAddress(unsigned addr)
{
    if ( (addr >= 0x80000000 && addr < 0x80800000) || 
            (addr >= 0xa0000000 && addr < 0xa0800000) )
        if ( !(addr & 0x03) )   /*4-byte alignment. MIPS16 mode?*/
            return 1;
    
    return 0;
}

static void trace(const unsigned ra, const unsigned sp)
{
    unsigned* begin;
    unsigned* jr_ra;
    unsigned* lw_ra;
    unsigned parent_ra;
    unsigned parent_sp;
    
    if (!isValidAddress(ra) || !isValidAddress(sp))
        return;
    
    /* search upward for the begin of this subroutine */
    begin = (unsigned*)ra;
    while ( isValidAddress((unsigned)begin) )
    {
        /* instruction "addiu sp,XX" begins a subroutine */
        if ( (*begin & (OP_MASK | RS_MASK | RT_MASK)) == ADDIU_SP_SP_INSTR )
            break;
        /* instruction "jr ra" ends a subroutine */
        else if ( *begin == JR_RA_INSTR )
        {
            begin += 2;
            break;
        }
        begin--;
    }
    if ( !isValidAddress((unsigned)begin) )
        return;
    
    printk("Called by subroutine: %08x\n", begin);
    
    /* search downward for instruction "jr ra", which ends this subroutine */
    jr_ra = (unsigned*)ra;
    while ( isValidAddress((unsigned)jr_ra) )
    {
        if ( *jr_ra == JR_RA_INSTR )
            break;
        jr_ra++;
    }
    if ( !isValidAddress((unsigned)jr_ra) )
        return;
    
    /* look at previous or next instruction to find offset of stack deallocation
    and compute stack pointer of parent subroutine */
    if ( isValidAddress((unsigned)(jr_ra - 1)) && 
            ((*(jr_ra - 1) & (OP_MASK | RS_MASK | RT_MASK)) == ADDIU_SP_SP_INSTR) )
        parent_sp = sp + (*(jr_ra - 1) & BROFFSET_MASK);
    else if ( isValidAddress((unsigned)(jr_ra + 1)) && 
            ((*(jr_ra + 1) & (OP_MASK | RS_MASK | RT_MASK)) == ADDIU_SP_SP_INSTR) )
        parent_sp = sp + (*(jr_ra + 1) & BROFFSET_MASK);
    else
        return;
    if ( !isValidAddress(parent_sp) || (parent_sp == sp) )
        return;
    
    /* search for instruction "lw ra,XX(XX)" between ra and end of this subroutine */
    lw_ra = (unsigned*)ra;
    while ( lw_ra < jr_ra )
    {
        if ( (*lw_ra & (OP_MASK | RS_MASK | RT_MASK)) == LW_RA_SP_INSTR )
        {
            if ( isValidAddress(sp + (*lw_ra & BROFFSET_MASK)) )
                parent_ra = *(unsigned*)(sp + (*lw_ra & BROFFSET_MASK));
            else
                return;
            break;
        }
        lw_ra++;
    }
    if ( !isValidAddress(parent_ra) || (parent_ra == ra) )
        return;
        
    trace(parent_ra, parent_sp);
}

void traceStack(unsigned pc, unsigned sp, unsigned ra)
{
   // lx4180_regfile_t *regFile = pExceptionContext;
    unsigned* begin;
    unsigned opcode;
    
    /* search upward for the begin of this subroutine */
    begin = (unsigned*)pc;
    printk("EPC: %08x\n", begin);
#if 0
    while ( isValidAddress((unsigned)begin) )
    {
        /* instruction "addiu sp,XX" begins a subroutine */
        if ( (*begin & (OP_MASK | RS_MASK | RT_MASK)) == ADDIU_SP_SP_INSTR )
        {
            /* check if previous instruction is "jr ra" */
            if ( *(begin - 1) == JR_RA_INSTR )
                begin++;
            break;
        }
        /* instruction "jr ra" ends a subroutine */
        else if ( *begin == JR_RA_INSTR )
        {
            begin += 2;
            break;
        }
        begin--;
    }

    if ( !isValidAddress((unsigned)begin) ){
	    printk("Invalid addr %08x. Can't print stack\n", begin);
        return;
    }    
    printk("Subroutine: %08x\n", begin);
#endif    
    trace(ra, sp);
#if 0    
    opcode = *(unsigned*)pc;
    if ( ((opcode & OP_MASK) == JAL_INSTR) || 
            ((opcode & OP_MASK) == J_INSTR) || 
            ((opcode & (OP_MASK | SUBCODE_MASK)) == JR_INSTR) || 
            ((opcode & (OP_MASK | SUBCODE_MASK)) == JALR_INSTR) || 
            ((opcode & OP_MASK) == BEQ_INSTR) || 
            ((opcode & OP_MASK) == BNE_INSTR) || 
            ((opcode & (OP_MASK | RT_MASK)) == BLEZ_INSTR) || 
            ((opcode & (OP_MASK | RT_MASK)) == BGTZ_INSTR) || 
            ((opcode & (OP_MASK | RT_MASK)) == BLTZ_INSTR) || 
            ((opcode & (OP_MASK | RT_MASK)) == BLTZAL_INSTR) || 
            ((opcode & (OP_MASK | RT_MASK)) == BGEZ_INSTR) || 
            ((opcode & (OP_MASK | RT_MASK)) == BGEZAL_INSTR) )
        pc += 8;
    else
        pc+=4;
#endif    
}
