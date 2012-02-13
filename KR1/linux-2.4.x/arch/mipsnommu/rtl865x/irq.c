#include <linux/config.h>
#include <linux/init.h>
#include <asm/io.h>
#include <asm/ptrace.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/types.h>
#include <asm/irq.h>
#include "rtl865x.h"
#include <asm/rtl865x/interrupt.h>

/*#include "intdef.h"*/

extern asmlinkage unsigned int do_IRQ(int irq, struct pt_regs *regs);
extern asmlinkage void sim_handle_int(void);

extern irq_desc_t irq_desc[NR_IRQS];

void dummy_func(unsigned int irq)
{
}

static struct hw_interrupt_type  irq_no_type= {
	"none",
	dummy_func,
	dummy_func,
	dummy_func,
	dummy_func,
	dummy_func,
	dummy_func
};

#define RIRR	(*((volatile u32 *)0xbd012008))
#define RIMR	(*((volatile u32 *)0xbd012000))
#define RISR	(*((volatile u32 *)0xbd012004))
void __init simulator_irq_setup(void)
{
	int i;
	set_cp0_status(STATUSF_IP2);
	set_cp0_status(STATUSF_IP3);
	set_cp0_status(STATUSF_IP4);
	set_cp0_status(STATUSF_IP5);
	set_cp0_status(STATUSF_IP6);
	set_cp0_status(STATUSF_IP7);
	set_cp0_status(0x1);
	
	RIMR = 0;
	RISR = 0;
	RIRR = 0;
	set_except_vector(0, sim_handle_int);	
		
	for(i=0;i<NR_IRQS;i++)
	{
		irq_desc[i].handler	= &irq_no_type;
		irq_desc[i].status = 0;
		irq_desc[i].action	= 0;
		irq_desc[i].depth	= 1;
		//irq_desc[i].mask	= rtl8670_mask_irq;
		//irq_desc[i].unmask	= rtl8670_unmask_irq;
	}
}		

static char lastCheck[64]="";
static unsigned int lastLine=0;

int rtl8650_checkIsrStatus(char *funcName, int line, unsigned which){
	//printk("%s:%d\n",funcName, line);
	int ret=0;
	unsigned int isrChk=*((unsigned int *)0xbd012004)&which;
	if(isrChk){
		printk("=========>ISR%08x: %s:%d\n",*((unsigned int *)0xbd012004),funcName, line);
		printk("Last: %s:%d\n",lastCheck, lastLine);
		ret=1;
	}
	strcpy(lastCheck, funcName);
	lastLine=line;
	return ret;
}



void rtl8650_handle_interrupt(struct pt_regs *regs)
{
	unsigned short i;
	u32	isr;
	u32 	flag;

	isr = RISR & RIMR;
	clear_cp0_status( STATUSF_IP2| STATUSF_IP3| STATUSF_IP4| STATUSF_IP5|
			STATUSF_IP6| STATUSF_IP7);
	flag = RIMR;
	RIMR=0;
	for (i=0;i<12;i++)
	{
		if (isr & 0x80000000)
		{
			do_IRQ(i,regs);
		}
		isr = isr << 1;
	}
	set_cp0_status( STATUSF_IP2| STATUSF_IP3| STATUSF_IP4| STATUSF_IP5|
			STATUSF_IP6| STATUSF_IP7);
	RIMR=flag;
}

void enable_lx4180_irq(int irq)
{
	//ICU_MASK(irq);
	switch (irq)
	{
		case ICU_TMR:
			RIRR |= 3 << 30; 
			RIMR |= 1 << 31;
			break;
		case ICU_USB:
			RIRR |= 3 << 28;
			RIMR |= 1 << 30;
			break;
		case ICU_UART0:
			RIRR |= 3 << 22;
			RIMR |= 1 << 27;
			break;
		case ICU_UART1:
			RIRR |= 3 << 22;
			RIMR |= 1 << 28;
			break;
		case ICU_NIC:
			RIRR |= 3 << 18;
			RIMR |= 1 << 25;
			break;
		case ICU_PCI:
			RIRR |= 2 <<20; 
			RIMR |= 1 <<26;
		case ICU_LBCT:
			RIRR |= 3 << 10;
			RIMR |= 1 << 21;
		case ICU_GPIO:
			RIRR |= 3 << 16;
			RIMR |= 1 << 24;

		default:
			break;
		
	}
}

void __init init_IRQ (void)
{
#ifdef CONFIG_REMOTE_DEBUG
	console_init();

	printk("Breaking into debugger...\n");
	clear_cp0_status(STATUSF_IP2);
	clear_cp0_status(STATUSF_IP3);
	clear_cp0_status(STATUSF_IP4);
	clear_cp0_status(STATUSF_IP5);
	clear_cp0_status(STATUSF_IP6);
	clear_cp0_status(STATUSF_IP7);
	set_debug_traps();
	set_cp0_status(STATUSF_IP2);
	set_cp0_status(STATUSF_IP3);
	set_cp0_status(STATUSF_IP4);
	set_cp0_status(STATUSF_IP5);
	set_cp0_status(STATUSF_IP6);
	set_cp0_status(STATUSF_IP7);


#endif

}
int once;
void do_LBC(int irq, void *dev_id, struct pt_regs *regs)

{
	RISR =(1<<21); //clear LBC timeout status
	once++;
	if (once<=1)
	{
		printk("regs address %x\n",regs);

		printk("$0 : %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx\n",
	       	regs->regs[0], regs->regs[1], regs->regs[2], regs->regs[3],
               	regs->regs[4], regs->regs[5], regs->regs[6], regs->regs[7]);
		printk("$8 : %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx\n",
		       regs->regs[8], regs->regs[9], regs->regs[10], regs->regs[11],
               		regs->regs[12], regs->regs[13], regs->regs[14], regs->regs[15]);
		printk("$16: %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx\n",
		       regs->regs[16], regs->regs[17], regs->regs[18], regs->regs[19],
       	        regs->regs[20], regs->regs[21], regs->regs[22], regs->regs[23]);
		printk("$24: %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx\n",
		       regs->regs[24], regs->regs[25], regs->regs[26], regs->regs[27],
	       		regs->regs[28], regs->regs[29], regs->regs[30], regs->regs[31]);



		
		/*
		 * Saved cp0 registers
		 */
		printk("epc  : %08lx\nStatus: %08lx\nCause : %08lx\n",
		       regs->cp0_epc, regs->cp0_status, regs->cp0_cause);
		printk("Call stack:\n");
		traceStack(regs->cp0_epc, regs->regs[29],regs->regs[31] );
	}
	RIMR |= 1 << 21;

#ifdef CONFIG_REMOTE_DEBUG
    gdb_stub_backupRegs(regs);
    breakpoint();
#endif
}
#if 0

#include "lx4080exc.h"

#define ULONG unsigned int
#define UINT unsigned int

static int isJumpInstruction(char *pEXCdata);
static Branch_t emulateBranch(char *pEXCdata);
static UINT *computeBranchAddress (char *pEXCdata, Branch_t type);

/*
	globals:
*/
ULONG emulatedHI;
ULONG emulatedLO;

#ifdef INSTRUMENT_RI_TRAPS
countRI_t countRI;
int countRIEnabled;
#endif
#undef TRUE
#define TRUE (0 == 0)

#undef FALSE
#define FALSE (0 != 0)





/***************************************************************************
*
*/
#ifdef IN_PMON
int emulatelx4080RI(char *pEXCdata)
#endif
int emulatelx4080RI ( int vec,
                      char *pEXCdata,
                      struct pt_regs  *pRegs
                    )
{
    register ULONG rs;
    register ULONG rt;
    ULONG va;
    ULONG mem;
    ULONG newPC = 0;
    UINT inst;
    unsigned int byte;
    int status;
    int branchDelay;

    /*
      =========================================
      If this exception occurred in a branch
      delay slot (Cause(BD)) then the victim
      instruction is at EPC + 4:

      If it occurred in a jump (j, jr, jalr,
      or jal), then the instruction location
      must be adjusted according to the branch
      type (offset, target, register, or none):
      =========================================
    */

    branchDelay = ((0 != (CAUSE_BD & _STKOFFSET(EXC_DATA_CAUSE))) |
                   isJumpInstruction(pEXCdata));

    if (branchDelay)
    {
        newPC = (ULONG) computeBranchAddress (pEXCdata,
                                              emulateBranch(pEXCdata));
    }

    /*
      =====================================
      Get the instruction that caused this
      interrupt:
      =====================================
    */
    inst = *((UINT *)(_STKOFFSET(EXC_DATA_EPC)) + (branchDelay?1:0));

    /* In case the emulated zero register is somehow trashed. */
    _GPR_STKOFFSET(0) = 0;

    /*
      =================================
      Isolate the two source registers:
      =================================
    */
    rs = _GPR_STKOFFSET(_RS_(inst));
    rt = _GPR_STKOFFSET(_RT_(inst));

    /*
      =======================================
      Calculate the offset and alignment
      for lwl, lwr, swl, or swr instructions.
      For these instructions, 'rs' represents
      the base to which the offset is added:
      =======================================
    */
    va = rs + (ULONG)((short)_OFFSET_(inst));
    byte = va & 3;


    status = 0;

    /*
    =============================================================
    Three types of instructions deserve special consideration
    in the Lexra ESB:

    lwl, lwr, swl, and swr are unaligned load and store inst-
    ructions.  These four instructions are _always_ implement-
    ed in software.

    mult, multu, div, and divu instructions may be either soft-
    ware emulated or placed in the optional MAC-DIV module.
    When these instructions are implemented in the MAC-DIC module
    they do not generate Reserved Instruction (RI) traps.

    The mthi, mtlo, mfhi, and mflo instructions are implemented
    in hardware, no software emulation is necessary, EXCEPT for
    "Pass Zero" Test Chips.  The "Pass Zero" Test Chips require
    software emulation for these instructions.

    The 12 instructions described above are decoded by the foll-
    owing switch statement.  They may be uniquely identified by
    bits 26 through 31 (the primary opcode field) and bits 0
    through 5 (the subcode field).
    =============================================================
    */
    switch (_OP_(inst))
    {

        /*
          ===========================================================================
          Load Word Left:  lwl rt, offset + rs

          Add the sign-extended offset to base register 'rs'; this is the source
          address.  Copy the byte at this address to the leftmost unwritten byte
          in 'rt', proceeding from left to right.

          When the rightmost byte of the source is copied, the operation is complete.
          ===========================================================================
        */
    case 0x22:			/* lwl */
        SBD_DISPLAY ("RI22", 0);

        mem = *(ULONG *)(va - byte);
        mem = mem << byte*8;

        rt = (rt & ~(-1UL << byte*8)) | mem;

        _GPR_STKOFFSET(_RT_(inst)) = rt;
        status = 1;
#ifdef INSTRUMENT_RI_TRAPS
        if (countRIEnabled)
            ++countRI.lwl;
#endif
        break;

        /*
          ===========================================================================
          Load Word Right:  lwr rt, offset + rs

          Add the sign-extended offset to base register 'rs'; this is the source
          address.  Copy the byte at this address to the rightmost unwritten byte
          in 'rt', proceeding from right to left.

          When the leftmost byte of the source is copied, the operation is complete.
          ===========================================================================
        */
    case 0x26:			/* lwr */
        SBD_DISPLAY ("RI26", 0);

        mem = *(ULONG *)(va - byte);
        mem = mem >> (3-byte)*8;

        rt = (rt & ~(-1UL >> (3-byte)*8)) | mem;

        _GPR_STKOFFSET(_RT_(inst)) = rt;
        status = 1;
#ifdef INSTRUMENT_RI_TRAPS
        if (countRIEnabled)
            ++countRI.lwr;
#endif
        break;

        /*
          ===========================================================================
          Store Word Left:  swl rt, offset + rs

          Add the sign-extended offset to base register 'rs'; this is the destination
          address.  Proceeding from left to right, copy bytes from the register
          specified by 'rt' to bytes starting at the destination address.

          When the rightmost byte of the destination is written, the operation is
          complete.
          ===========================================================================
        */
    case 0x2A:			/* swl */
        SBD_DISPLAY ("RI2A", 0);

        mem = *(ULONG *)(va - byte);
        mem = mem & ~(-1UL >> byte*8);

        rt = (rt >> byte*8) | mem;

        *(ULONG *)(va - byte) = rt;
        status = 1;
#ifdef INSTRUMENT_RI_TRAPS
        if (countRIEnabled)
            ++countRI.swl;
#endif
        break;

        /*
          ===========================================================================
          Store Word Right:  swr rt, offset + rs

          Add the sign-extended offset to base register 'rs'; this is the destination
          address.  Proceeding from right to left, copy bytes from the register
          specified by 'rt' to bytes starting at the destination address.

          When the leftmost byte of the destination is written, the operation is
          complete.
          ===========================================================================
        */
    case 0x2E:			/* swr */
        SBD_DISPLAY ("RI2E", 0);

        mem = *(ULONG *)(va - byte);
        mem = mem & ~(-1UL << (3-byte)*8);

        rt = (rt << (3-byte)*8) | mem;

        *(ULONG *)(va - byte) = rt;
        status = 1;
#ifdef INSTRUMENT_RI_TRAPS
        if (countRIEnabled)
            ++countRI.swr;
#endif
        break;
    case 0x00:			/* Special */
        switch (_OPS_(inst))
        {

            /*
            	===================================
            	The move from HI-result instruction
            	must be emulated in software for
            	the Pass Zero Test Chip only.
            	Other LX4xxx devices implement mfhi
            	in hardware.
            	===================================
            */
        case 0x10:			/* mfhi */
            SBD_DISPLAY ("RI10", 0);

            _GPR_STKOFFSET(_RD_(inst))
            = _STKOFFSET(EXC_DATA_HI);

            status = 1;
#ifdef INSTRUMENT_RI_TRAPS
            if (countRIEnabled)
                ++countRI.mfhi;
#endif
            break;
            /*
            	===================================
            	The move to HI-result instruction
            	must be emulated in software for
            	the Pass Zero Test Chip only.
            	Other LX4xxx devices implement mthi
            	in hardware.
            	===================================
            */
        case 0x11:			/* mthi */
            SBD_DISPLAY ("RI11", 0);

            _STKOFFSET(EXC_DATA_HI)
            = _GPR_STKOFFSET(_RS_(inst));

            status = 1;
#ifdef INSTRUMENT_RI_TRAPS
            if (countRIEnabled)
                ++countRI.mthi;
#endif
            break;
            /*
            	===================================
            	The move from LO-result instruction
            	must be emulated in software for
            	the Pass Zero Test Chip only.
            	Other LX4xxx devices implement mflo
            	in hardware.
            	===================================
            */
        case 0x12:			/* mflo */
            SBD_DISPLAY ("RI12", 0);

            _GPR_STKOFFSET(_RD_(inst))
            = _STKOFFSET(EXC_DATA_LO);

            status = 1;
#ifdef INSTRUMENT_RI_TRAPS
            if (countRIEnabled)
                ++countRI.mflo;
#endif
            break;
            /*
            	===================================
            	The move to LO-result instruction
            	must be emulated in software for
            	the Pass Zero Test Chip only.
            	Other LX4xxx devices implement mtlo
            	in hardware.
            	===================================
            */
        case 0x13:			/* mtlo */
            SBD_DISPLAY ("RI13", 0);

            _STKOFFSET(EXC_DATA_LO)
            = _GPR_STKOFFSET(_RS_(inst));

            status = 1;
#ifdef INSTRUMENT_RI_TRAPS
            if (countRIEnabled)
                ++countRI.mtlo;
#endif
            break;
            /*
            	===================================
            	The signed multiply instruction
            	may be emulated in software or
            	implemented in the optional MAC-DIV
            	Module.
            	===================================
            */
        case 0x18:			/* mult */
            SBD_DISPLAY ("RI18", 0);
            {
                register unsigned int X = (unsigned int) rs;
                register unsigned int Y = (unsigned int) rt;
                register unsigned int i;
                register unsigned int Mask = 0x80000000;
                register unsigned int sign = (Mask & X) ^ (Mask & Y);
                register unsigned int HI = 0;
                register unsigned int LO = Mask&Y ? -Y : Y;
                register unsigned int CARRY;

                X = Mask&X ? -X : X;

                for (i=0; i<32; i++)
                {
                    CARRY = 0;
                    if (LO&1)
                    {
                        CARRY = HI;
                        HI += X;
                        CARRY = (HI < CARRY) | (HI < X);
                    }
                    LO >>= 1;
                    if (HI&1)
                    {
                        LO |= Mask;
                    }
                    HI >>= 1;
                    if (CARRY)
                    {
                        HI |= Mask;
                    }
                }

                if (sign)
                {
                    LO = ~LO;
                    HI = ~HI;
                    CARRY = LO;
                    LO += 1;
                    if (LO < CARRY)
                    {
                        HI += 1;
                    }
                }

                _STKOFFSET(EXC_DATA_HI) = HI;
                _STKOFFSET(EXC_DATA_LO) = LO;
            }

            status = 1;
#ifdef INSTRUMENT_RI_TRAPS
            if (countRIEnabled)
                ++countRI.mult;
#endif
            break;
            /*
            	===================================
            	The unsigned multiply instruction
            	may be emulated in software or
            	implemented in the optional MAC-DIV
            	Module.
            	===================================
            */
        case 0x19:			/* multu */
            SBD_DISPLAY ("RI19", 0);
            {
                register unsigned int X = (unsigned int) rs;
                register unsigned int i;
                register unsigned int Mask = 0x80000000;
                register unsigned int HI = 0;
                register unsigned int LO = (unsigned int) rt;
                register unsigned int CARRY;

                for (i=0; i<32; i++)
                {
                    CARRY = 0;
                    if (LO&1)
                    {
                        CARRY = HI;
                        HI += X;
                        CARRY = (HI < CARRY) | (HI < X);
                    }
                    LO >>= 1;
                    if (HI&1)
                    {
                        LO |= Mask;
                    }
                    HI >>= 1;
                    if (CARRY)
                    {
                        HI |= Mask;
                    }
                }

                _STKOFFSET(EXC_DATA_HI) = HI;
                _STKOFFSET(EXC_DATA_LO) = LO;
            }

            status = 1;
#ifdef INSTRUMENT_RI_TRAPS
            if (countRIEnabled)
                ++countRI.multu;
#endif
            break;
            /*
            	===================================
            	The signed divide instruction
            	may be emulated in software or
            	implemented in the optional MAC-DIV
            	Module.
            	===================================
            */
        case 0x1a:			/* div */
            SBD_DISPLAY ("RI1A", 0);
            {
                register unsigned int X = (unsigned int) rs;
                register unsigned int Y = (unsigned int) rt;
                register unsigned int i;
                register unsigned int Mask = 0x80000000;
                register unsigned int signHI = (Mask & X);
                register unsigned int sign = signHI ^ (Mask & Y);
                register unsigned int HI = 0;
                register unsigned int LO = Mask&X ? -X : X;

                Y = Mask&Y ? -Y : Y;

                for (i=0; i<32; i++)
                {
                    HI <<= 1;
                    if (LO&Mask)
                        HI |= 1;
                    LO <<= 1;

                    if (Y > HI)
                    {
                        LO &= ~1;
                    }
                    else
                    {
                        HI -= Y;
                        LO |= 1;
                    }
                }
                LO = sign ? -LO : LO;
                HI = signHI ? -HI : HI;

                _STKOFFSET(EXC_DATA_HI) = HI;
                _STKOFFSET(EXC_DATA_LO) = LO;
            }

            status = 1;
#ifdef INSTRUMENT_RI_TRAPS
            if (countRIEnabled)
                ++countRI.div;
#endif
            break;
            /*
            	===================================
            	The unsigned divide instruction
            	may be emulated in software or
            	implemented in the optional MAC-DIV
            	Module.
            	===================================
            */
        case 0x1b:			/* divu */
            SBD_DISPLAY ("RI1B", 0);
            {
                register unsigned int X = (unsigned int) rs;
                register unsigned int Y = (unsigned int) rt;
                register unsigned int i;
                register unsigned int Mask = 0x80000000;
                register unsigned int HI = 0;
                register unsigned int LO = X;

                for (i=0; i<32; i++)
                {
                    HI <<= 1;
                    if (LO&Mask)
                        HI |= 1;
                    LO <<= 1;

                    if (Y > HI)
                    {
                        LO &= ~1;
                    }
                    else
                    {
                        HI -= Y;
                        LO |= 1;
                    }
                }

                _STKOFFSET(EXC_DATA_HI) = HI;
                _STKOFFSET(EXC_DATA_LO) = LO;
            }

            status = 1;
#ifdef INSTRUMENT_RI_TRAPS
            if (countRIEnabled)
                ++countRI.divu;
#endif
            break;
        default:			/* special */
            SBD_DISPLAY ("RI00", 0);
        }
        break;
    default:
        SBD_DISPLAY ("RIXX", 0);
    }

#ifdef MINIMON
    minimondisplayhex(_STKOFFSET(EXC_DATA_EPC));
    minimondisplayhex(_STKOFFSET(EXC_DATA_CAUSE));
#endif

    if (status)
    {
        if (branchDelay)
        {
            _STKOFFSET(EXC_DATA_EPC) = newPC;
        }
        else
        {
            _STKOFFSET(EXC_DATA_EPC) += 4;
        }
    }

    return (status);
}

static int isJumpInstruction(char *pEXCdata)
{
    UINT *pc;
    UINT inst;

    pc = (UINT *)(_STKOFFSET(EXC_DATA_EPC));
    inst = *pc;

    switch (_OP_(inst))
    {
    case 0x00:			/* Special */
        if ((_OPS_(inst) == 0x08)	/* jr */
                | (_OPS_(inst) == 0x09))
        { /* jalr */
            return TRUE;
        }
        return FALSE;

    case 0x02:			/* j */
    case 0x03:			/* jal */
        return TRUE;

    default:
        return FALSE;
    }
}


static Branch_t emulateBranch(char *pEXCdata)
{
    register ULONG rs;
    register ULONG rt;
    UINT *pc;
    UINT inst;
    Branch_t branchStatus;

    pc = (UINT *)(_STKOFFSET(EXC_DATA_EPC));

    inst = *pc;

    rs = _GPR_STKOFFSET(_RS_(inst));
    rt = _GPR_STKOFFSET(_RT_(inst));

    branchStatus = BRANCH_T_NONE;
    switch (_OP_(inst))
    {
    case 0x00:			/* Special */
        if (_OPS_(inst) == 0x08)
        {	/* jr */
            branchStatus = BRANCH_T_REGISTER;
        }
        if (_OPS_(inst) == 0x09)
        {	/* jalr */
            _GPR_STKOFFSET(_RD_(inst)) = (ULONG) (pc + 2);
            branchStatus = BRANCH_T_REGISTER;
        }
        break;

    case 0x03:			/* jal */
        _STKOFFSET(EXC_DATA_RA) = (ULONG) (pc + 2);
    case 0x02:			/* j */
        branchStatus = BRANCH_T_TARGET;
        break;

    case 0x04:			/* beq */
        if (rs == rt) branchStatus = BRANCH_T_OFFSET;
        break;

    case 0x05:			/* bne */
        if (rs != rt) branchStatus = BRANCH_T_OFFSET;
        break;

    case 0x06:			/* blez */
        if ((signed long) rs <= (signed long) 0)
            branchStatus = BRANCH_T_OFFSET;
        break;

    case 0x07:			/* bgtz */
        if ((signed long) rs > (signed long) 0)
            branchStatus = BRANCH_T_OFFSET;
        break;

    case 0x01:			/* regimm */
        switch(_RT_(inst))
        {
        case 0x10:			/* bltzal */
            _STKOFFSET(EXC_DATA_RA) = (ULONG) (pc + 2);
        case 0x00:			/* bltz */
            if ((signed long) rs < (signed long) 0)
                branchStatus = BRANCH_T_OFFSET;
            break;

        case 0x11:			/* bgezal */
            _STKOFFSET(EXC_DATA_RA) = (ULONG) (pc + 2);
        case 0x01:			/* bgez */
            if ((signed long) rs >= (signed long) 0)
                branchStatus = BRANCH_T_OFFSET;
            break;
        }
        break;

        /* Todo: bcxf and bcxt */

    case 0x10:			/* cop0 */
        branchStatus = BRANCH_T_OFFSET;
        break;

    case 0x11:			/* cop1 */
        branchStatus = BRANCH_T_OFFSET;
        break;

    case 0x12:			/* cop2 */
        branchStatus = BRANCH_T_OFFSET;
        break;

    case 0x13:			/* cop3 */
        branchStatus = BRANCH_T_OFFSET;
        break;
    }

    return branchStatus;
}


static UINT *computeBranchAddress (char *pEXCdata, Branch_t type)
{
    register ULONG rs;
    UINT *pc;
    UINT inst;
    signed int offset;

    pc = (UINT *)(_STKOFFSET(EXC_DATA_EPC));

    inst = *pc;

    switch (type)
    {
    case BRANCH_T_OFFSET:
        offset = (signed short) (inst & 0xffff);
        return (pc + 1 + offset);

    case BRANCH_T_TARGET:
        offset = inst & 0x3ffffff;
        return (UINT *)(((ULONG)(pc + 1) & 0xf0000000) | (offset << 2));

    case BRANCH_T_REGISTER:
        rs = _GPR_STKOFFSET(_RS_(inst));
        return (UINT *)rs;

    case BRANCH_T_NONE:
        return (pc + 2);

    default:
        return (0);
    }
}





#endif
