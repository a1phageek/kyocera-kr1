#ifndef PCI_H
#define PCI_H



struct pci_slot_baseaddr_s {
	u32 slotNum;
	u32 addr;
};

#define PCI_SLOT0_CONFIG_BASE 	   0xBBD40000
#define PCI_SLOT1_CONFIG_BASE 	   0xBBD80000
#define PCI_SLOT2_CONFIG_BASE 	   0xBBD10000
#define PCI_SLOT3_CONFIG_BASE 	   0xBBD20000

#define PCI_SLOT_MEM_BASE 0xBBF00000
#define PCI_SLOT_IO_BASE  0xBBE00000
#define PCI_MEM_SPACE_SIZE 0x00100000
#define PCI_IO_SPACE_SIZE  0x00100000

//PCI configuration space
#define PCI_CONFIG_VENDER_DEV 	0x0000
#define PCI_CONFIG_COMMAND 	0x0004
#define PCI_CONFIG_STATUS  	0x0006
#define PCI_CONFIG_CLASS_REVISION  	0x0008
#define PCI_CONFIG_CACHE  	0x000c
#define PCI_CONFIG_LATENCY 	0x000d
#define PCI_CONFIG_HEADER_TYPE 	0x000e
#define PCI_CONFIG_BASE0	0x0010
#define PCI_CONFIG_BASE1	0x0014
#define PCI_CONFIG_BASE2	0x0018
#define PCI_CONFIG_BASE3	0x001c
#define PCI_CONFIG_BASE4	0x0020
#define PCI_CONFIG_BASE5	0x0024
#define PCI_CONFIG_SUBSYSTEMVENDOR 	0x002c
#define PCI_CONFIG_INT_LINE	0x003c
#define PCI_CONFIG_INT_PIN	0x003d
#define PCI_CONFIG_MAXLAN 	0x003f
#define PCI_CONFIG_MINGNT 	0x003e

//PCI command register flag
#define CMD_FAST_BACK_TO_BACK	 	(1<<9)
#define CMD_SERR		 	(1<<8)
#define CMD_STEP_CONTROL	 	(1<<7)
#define CMD_PARITY_ERROR_RESPONSE 	(1<<6)
#define CMD_VGA_PALLETE_SNOOP	 	(1<<5)
#define CMD_WRITE_AND_INVALIDATE 	(1<<4)
#define CMD_SPECIAL_CYCLE	 	(1<<3)
#define CMD_BUS_MASTER			(1<<2)
#define CMD_MEM_SPACE			(1<<1)
#define CMD_IO_SPACE			(1<<0)

#endif
