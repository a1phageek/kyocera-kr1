#ifndef PCI_H
#define PCI_H

typedef struct BASE_ADDRESS_STRUCT{
  u32 Address;
  enum type_s {
    IO_SPACE=0x80,
    MEM_SPACE
  }Type;
  enum width_s {
    WIDTH32,
    WIDTH64
  }Width;
  u32 Prefetch;
  u32 Size;
}base_address_s;

typedef struct PCI_CONFIG_SPACE_STRUCT{
  u32 Config_Base;  //config space base address
  enum status_s{
    ENABLE,
    DISABLE
  }Status;  //device is enable or disable
  u32 Vendor_Device_ID;
  u8  Revision_ID;
  u32 Class_Code;
  u8  Header_Type;
  base_address_s BAR[6];
  u32 SubSystemVendor_ID;
}pci_config_s;

#define PCI_SLOT_NUMBER 1 
//Slot 0
#define PCI_SLOT0_CONFIG_BASE 	   0xBBD40000
//Slot 1
#define PCI_SLOT1_CONFIG_BASE 	   0xBBD80000
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
