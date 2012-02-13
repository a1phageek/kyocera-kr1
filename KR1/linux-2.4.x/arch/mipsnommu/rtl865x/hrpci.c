#include <linux/types.h>
#include  "rtl8650.h"
//#include <csp/netdriverif.h>
#include "pci.h"

int  (*pci_interrupt_handler[PCI_SLOT_NUMBER][8])(pci_config_s*);

void PCIBusIsr(void);

pci_config_s *pci_slot[PCI_SLOT_NUMBER][8];

#if 0 
extern int *init_RTL8139nic(int slot, int func, pci_config_s *pcfg);

//PCI Device Driver Table
struct
{
    u32 VendorDeviceID;
    int *(*pci_driver_init_entry)(int slot, int func, pci_config_s *pcfg);
} PCIDriverList[] =
{
#if defined(CONFIG_8139TOO) || defined(CONFIG_8139CP)
	0x813910EC, 			init_RTL8139nic,
#endif 	
	0
};


void pci_test(void)
{
	unsigned long io_data;
	int i;
	u32 tmp;

    tmp = REG32(0xbc803000)&0xFFFF8FFF;
    REG32(0xbc803000) = tmp|(4<<12);
	
	io_data = REG32(PCI_SLOT0_CONFIG_BASE+PCI_CONFIG_VENDER_DEV);
	if (io_data!=0xFFFFFFFF) {
	  printk("Slot 0 VENDER Device ID=%x\n", io_data);
	  for(i=0;i<0x30;i+=4)
	  {
		io_data = REG32(PCI_SLOT0_CONFIG_BASE+PCI_CONFIG_VENDER_DEV+i);
		printk("Slot 0 address %x=%x\n", i, io_data);
	  }
	};
	
	io_data = REG32(PCI_SLOT0_CONFIG_BASE+PCI_CONFIG_VENDER_DEV);
	if (io_data!=0xFFFFFFFF) {
	  printk("Slot 1 VENDER Device ID=%x\n", io_data);
	  for(i=0;i<0x30;i+=4)
	  {
		io_data = REG32(PCI_SLOT0_CONFIG_BASE+PCI_CONFIG_VENDER_DEV+i);
		printk("Slot 1 address %x=%x\n", i, io_data);
	  }
	}; 
}
#endif /* if 0 */

/*
scan the resource needed by PCI device
*/
void scan_resource(pci_config_s *pci_config, int slot_num, int dev_function, u32 config_base)
{
  int i;
  u32 BaseAddr, data;

    BaseAddr = config_base+PCI_CONFIG_BASE0;
    for (i=0;i<6;i++) {  //detect resource usage
    	REG32(BaseAddr) = 0xFFFFFFFF;
    	data = REG32(BaseAddr);
		
    	if (data!=0) {  //resource request exist
    	    int j;
    	    if (data&1) {  //IO space

    	    	pci_config->BAR[i].Type = IO_SPACE;
  	        //scan resource size
  	        for (j=2;j<32;j++)
  	            if (data&(1<<j)) break;
  	        if (j<32) pci_config->BAR[i].Size = 1<<j;
  	        else  pci_config->BAR[i].Size = 0;
    	    } else {  //Memory space

    	    	pci_config->BAR[i].Type = MEM_SPACE;
    	    	//bus width
    	    	if ((data&0x0006)==4) pci_config->BAR[i].Width = WIDTH64; //bus width 64
    	    	else pci_config->BAR[i].Width = WIDTH32;  //bus width 32
    	    	//prefetchable
    	    	if (data&0x0008) pci_config->BAR[i].Prefetch = 1; //prefetchable
    	    	else pci_config->BAR[i].Prefetch = 0;  //no prefetch
  	        //scan resource size
  	        if (pci_config->BAR[i].Width==WIDTH32) {
  	          for (j=4;j<32;j++)
  	            if (data&(1<<j)) break;
  	          if (j<32) pci_config->BAR[i].Size = 1<<j;
  	          else  pci_config->BAR[i].Size = 0;
  	        } else //width64 is not support
  	          pci_config->BAR[i].Size = 0;
    	    };
    	} else {  //no resource
    	    memset(&(pci_config->BAR[i]), 0, sizeof(base_address_s));    		
    	};
	BaseAddr += 4;  //next base address
    };
}


/*
scan the PCI bus, save config information into pci_slot,
return the number of PCI functions
*/
int pci_scan(void)
{
  u32 config_base, data;
  int i, dev_function, dev_num;

    dev_num = 0;
    for (i=0;i<PCI_SLOT_NUMBER;i++) {  //probe 2 pci slots
      //if (i==0) config_base = PCI_SLOT0_CONFIG_BASE;
      //else 
      config_base = PCI_SLOT0_CONFIG_BASE;
      dev_function=0;
      while (0!=REG32(config_base+PCI_CONFIG_VENDER_DEV)) {  //pci device exist
          pci_slot[i][dev_function] = kmalloc(sizeof(pci_config_s));
          pci_slot[i][dev_function]->Config_Base = config_base;
          pci_slot[i][dev_function]->Status = DISABLE;
          pci_slot[i][dev_function]->Vendor_Device_ID = REG32(config_base+PCI_CONFIG_VENDER_DEV);
          data = REG32(config_base+PCI_CONFIG_CLASS_REVISION);
          pci_slot[i][dev_function]->Revision_ID = data&0x00FF;
          pci_slot[i][dev_function]->Class_Code = data>>8;
          pci_slot[i][dev_function]->Header_Type = (REG32(config_base+PCI_CONFIG_CACHE)>>16)&0x000000FF;
          pci_slot[i][dev_function]->SubSystemVendor_ID = REG32(config_base+PCI_CONFIG_SUBSYSTEMVENDOR);
          scan_resource(pci_slot[i][dev_function], i, dev_function, config_base);  //probe resource request
          printk("\nPCI device exists: slot %d function %d VendorID %x DeviceID %x\n", i, dev_function, 
              pci_slot[i][dev_function]->Vendor_Device_ID&0x0000FFFF,
              pci_slot[i][dev_function]->Vendor_Device_ID>>16);
          dev_num++;
          dev_function++;
          config_base += dev_function*64*4;  //next function's config base
          if (!(pci_slot[i][0]->Header_Type&0x80)) goto stop_probe;  //single function card
          if (dev_function>=8) goto stop_probe;  //only 8 functions allow in a PCI card
      };
stop_probe:      
    };
    return dev_num;
}

/*
Check if vendor_device_id is in penable_list. If penable_list==NULL, all devices are enabled.
rtn -1:in list, 0:not in list
*/
int is_device_enable(u32 *penable_list, int list_num, u32 vendor_device_id)
{
  int i;
  
    if (vendor_device_id==0) return 0;  //this config space is null
    
    if (penable_list==NULL) return -1;  //no list, return in list
    
    for (i=0;i<list_num;i++)
      if (penable_list[i]==vendor_device_id) return -1;
    return 0;
}

/*
sort resource by its size
*/
void bubble_sort(int space_size[PCI_SLOT_NUMBER*8*6][2], int num)
{
  int i, j;
  int tmp_swap[2];
  
    for (i=0;i<num-1;i++) {
    	for (j=i;j<num-1;j++) {
          if (space_size[j][1]>space_size[j+1][1]) {
            tmp_swap[0] = space_size[j][0];
            tmp_swap[1] = space_size[j][1];
            space_size[j][0] = space_size[j+1][0];
            space_size[j][1] = space_size[j+1][1];
            space_size[j+1][0] = tmp_swap[0];
            space_size[j+1][1] = tmp_swap[1];
          };
        };
    };
}

/*
assign memory location to MEM & IO space
*penable_list: device resource assign list array of VENDOR_DEVICE number. Only devices 
        with vendor_device_id in plist will assign resource.
        All devices should assign resource if plist==NULL
list_num: number of item in list
rtn -1:OK, 0:fail
*/
int assign_resource(u32 *penable_list, int list_num)
{
  int i, slot, func, BARnum;
  int mem_space_size[PCI_SLOT_NUMBER*8*6][2], io_space_size[PCI_SLOT_NUMBER*8*6][2];  //[0]:store device index, [1]:store resource size
  int mem_idx, io_idx, total_size, tmp;
  u16 config_command[PCI_SLOT_NUMBER][8];  //record config space resource usage, 1:use IO, 2:use memory, 3:both
  
    memset(mem_space_size, 0, sizeof(mem_space_size));
    memset(io_space_size, 0, sizeof(io_space_size));
    memset(config_command, 0, sizeof(config_command));
    //collect resource
    mem_idx = io_idx =0;
    for (slot=0;slot<PCI_SLOT_NUMBER;slot++) {
      if (pci_slot[slot][0]==0) continue;  //this slot is null      
      if (pci_slot[slot][0]->Vendor_Device_ID==0) continue;  //this slot is null      
      for (func=0;func<8;func++) {
        if (pci_slot[slot][func]==0) break;
        pci_slot[slot][func]->Status = DISABLE;
        if (is_device_enable(penable_list, list_num, pci_slot[slot][func]->Vendor_Device_ID)) {   
            pci_slot[slot][func]->Status = ENABLE;
            for (BARnum=0;BARnum<6;BARnum++) {
      		if (pci_slot[slot][func]->BAR[BARnum].Type==MEM_SPACE){  //memory space
      		    config_command[slot][func] |= CMD_MEM_SPACE;  //this device use Memory
      		    mem_space_size[mem_idx][0] = (slot<<16)|(func<<8)|(BARnum<<0);
      		    mem_space_size[mem_idx][1] = pci_slot[slot][func]->BAR[BARnum].Size;
      		    mem_idx++;
      		} else if (pci_slot[slot][func]->BAR[BARnum].Type==IO_SPACE){  //io space
      		    config_command[slot][func] |= CMD_IO_SPACE;  //this device use IO
      		    io_space_size[io_idx][0] = (slot<<16)|(func<<8)|(BARnum<<0);
      		    io_space_size[io_idx][1] = pci_slot[slot][func]->BAR[BARnum].Size;
      		    io_idx++;
      		};
	    };  //for (BARnum=0;BARnum<6;BARnum++) 
        };  //if (is_device_enable(
      };  //for (func=0;func<8;func++)
    };  //for (slot=0;slot<PCI_SLOT_NUMBER;slot++)
    //sort by size
    if (mem_idx>1) bubble_sort(mem_space_size, mem_idx);
    if (io_idx>1)  bubble_sort(io_space_size, io_idx);
    
    //check mem total size
    total_size = 0;
    for (i=0;i<mem_idx;i++) {
    	tmp = mem_space_size[i][1]-1;
        total_size = (total_size+tmp)&(~tmp);
        total_size = total_size + mem_space_size[i][1];
    };
    if (total_size>PCI_MEM_SPACE_SIZE) return 0;  //lack of memory space resource

    //check io total size
    total_size = 0;
    for (i=0;i<io_idx;i++) {
    	tmp = io_space_size[i][1]-1;
        total_size = (total_size+tmp)&(~tmp);
        total_size = total_size + io_space_size[i][1];
    };
    if (total_size>PCI_IO_SPACE_SIZE) return 0;  //lack of io space resource
    
    //assign memory space
    total_size = 0;
    for (i=0;i<mem_idx;i++) {
    	tmp = mem_space_size[i][1]-1;
        total_size = (total_size+tmp)&(~tmp);
        tmp = mem_space_size[i][0];
        //assign to struct
        pci_slot[(tmp>>16)][(tmp>>8)&0x00FF]->BAR[tmp&0x00FF].Address = PCI_SLOT_MEM_BASE+total_size;
        //get BAR address and assign to PCI device
        tmp = ((tmp>>16)?PCI_SLOT0_CONFIG_BASE:PCI_SLOT0_CONFIG_BASE)   //SLOT
              +((tmp>>8)&0x00FF)*64*4					//function
              +((tmp&0x00FF)*4+PCI_CONFIG_BASE0);			//BAR bumber
        REG32(tmp) = (PCI_SLOT_MEM_BASE+total_size)&0x1FFFFFFF;  //map to physical address
        total_size = total_size + mem_space_size[i][1];  //next address
    };
    //assign IO space
    total_size = 0;
    for (i=0;i<io_idx;i++) {
    	tmp = io_space_size[i][1]-1;
        total_size = (total_size+tmp)&(~tmp);
        tmp = io_space_size[i][0];
        //assign to struct
        pci_slot[(tmp>>16)][(tmp>>8)&0x00FF]->BAR[tmp&0x00FF].Address = PCI_SLOT_IO_BASE+total_size;
        //get BAR address and assign to PCI device
        tmp = ((tmp>>16)?PCI_SLOT0_CONFIG_BASE:PCI_SLOT0_CONFIG_BASE)   //SLOT
              +((tmp>>8)&0x00FF)*64*4					//function
              +((tmp&0x00FF)*4+PCI_CONFIG_BASE0);			//BAR bumber
        REG32(tmp) = (PCI_SLOT_IO_BASE+total_size)&0x1FFFFFFF;  //map to physical address
        total_size = total_size + io_space_size[i][1];  //next address
    };
    //enable device
    for (slot=0;slot<PCI_SLOT_NUMBER;slot++) {
      if (pci_slot[slot][0]==0) continue;  //this slot is null      
      if (pci_slot[slot][0]->Vendor_Device_ID==0) continue;  //this slot is null      
      for (func=0;func<8;func++) {
        if (pci_slot[slot][func]==0) continue;  //this slot:function is null      
        if (pci_slot[slot][func]->Vendor_Device_ID==0) continue;  //this slot:function is null      
        //get config base address
        tmp = (slot?PCI_SLOT0_CONFIG_BASE:PCI_SLOT0_CONFIG_BASE)   //SLOT
              +func*64*4;					   //function
        //set Max_Lat & Min_Gnt, irq
	REG32(tmp+PCI_CONFIG_INT_LINE) = 0x40200100|(PCI_ILEV); //IRQ level

	//enable cache line size, lantancy
	REG32(tmp+PCI_CONFIG_CACHE) = (REG32(tmp+PCI_CONFIG_CACHE)&0xFFFF0000)|0x2004;  //32 byte cache, 20 latency
        //set command register
        //REG32(tmp+PCI_CONFIG_COMMAND) = (REG32(tmp+PCI_CONFIG_COMMAND)&0xFFFF0000)|config_command[slot][func]
        //					|CMD_BUS_MASTER|CMD_WRITE_AND_INVALIDATE|CMD_PARITY_ERROR_RESPONSE;
        REG16(tmp+PCI_CONFIG_COMMAND) = config_command[slot][func]
        					|CMD_BUS_MASTER|CMD_PARITY_ERROR_RESPONSE;//|CMD_WRITE_AND_INVALIDATE;
      };
    };
    
    return -1;
}

/*
scan pci bus, assign Memory & IO space to PCI card, and call init_XXXX to enable & register PCI device
rtn -1:ok, 0:fail
*/
int init_pci(void)
{
  int function_num;
  int i, j, idx;
  u32 tmp, divisor, scr;

   
  // REG32(GIMR) = REG32(GIMR)&(~PCIIE);  //disable int
    //set PCI bus divisor
    printk("Probing PCI Bus : You must plug a PCI device on Slot 0!! ");
    tmp = REG32(0xbc803000)&0xFFFF8FFF;
    REG32(0xbc803004)=0;
    scr=(REG32(0xbc803004)>>16)&7;
    if(scr==3)
	   divisor=2;
    else{
	u32 ratio[]={5,5,5,5,4,4,3,2};//check Datasheet SCR and MACCR.
	REG32(0xbc803000) = tmp|(1<<15);//reset PCI bridge
	divisor=ratio[scr];
    }
    printk("(SCR=0x%x,Div=%d)\n",scr, divisor &7 );
    REG32(0xbc803000) = tmp|(divisor<<12);
    memset(pci_slot, 0, 4*PCI_SLOT_NUMBER*8);
    function_num = pci_scan();
    if (function_num==0) {
    	printk("No PCI device exist!\n");
    	return 0;
    };
    //auto assign resource
    if (!assign_resource(NULL, 0)) {
    	printk("PCI Resource assignment failed!\n");
    	return 0;
    };
    
    printk("Find Total %d PCI function\n", function_num);
    return 0;
#if 0   
    //register PCI general ISR
    /*memset(pci_interrupt_handler, 0, sizeof(pci_interrupt_handler));
    REG32(IRR) = REG32(IRR) | ((PCI_ILEV) << PCIIRS_OFFSET);
    int_Register(PCI_ILEV, PCIBusIsr);*/
    
    //pci device init, do not put 2 same card in PCI slots!!!!!!!
    for (i=0;i<PCI_SLOT_NUMBER;i++) {
      if (pci_slot[i][0]==0) continue;  //this slot is null      
      if (pci_slot[i][0]->Vendor_Device_ID==0) continue;  //this slot is null      
      for (j=0;j<8;j++) {
        if (pci_slot[i][j]==0) continue;  //this function is null
        if (pci_slot[i][j]->Vendor_Device_ID==0) continue;  //this function is null
        if (pci_slot[i][j]->Status==DISABLE) continue;  //this function is disabled
        idx = 0;
        while (PCIDriverList[idx].VendorDeviceID!=0) {  //find match ID
          if (PCIDriverList[idx].VendorDeviceID==pci_slot[i][j]->Vendor_Device_ID)
              (*PCIDriverList[idx].pci_driver_init_entry)(i, j, pci_slot[i][j]);
          idx++;
        };
      };
    };

//code for 2 8139c+
    //two_pci();
    
//code for 1 8139c/8139c+
    //REG32(GIMR) = REG32(GIMR)|PCIIE;  //enable pci int
    //splx(0);
    while (1);
    
    return -1;
#endif /* 0 */    
}
#if 0 
/*
for PCI device to register ISR
*/
u32 pci_int_Register(int slot_num, int func_num, int (*pci_int_Handler)(pci_config_s*))
{
    if ((slot_num >= PCI_SLOT_NUMBER)||(slot_num<0)||(func_num>=8)||(func_num<0))
        return -1;
    
    pci_interrupt_handler[slot_num][func_num] = pci_int_Handler;

    return 0;
}

/*
main ISR for all PCI device
*/
void PCIBusIsr(void)
{
  int i, j, serviced;
  
    //if (serviced==1)
   // REG32(GISR)=PCIIP;  //PCI IRQ pending clear
    serviced = 0;
    //polling each device ISR for PCI IRQ
    for (i=0;i<PCI_SLOT_NUMBER;i++) 
      for (j=0;j<8;j++)
        if ( pci_interrupt_handler[i][j] != NULL ) {
          if ((*pci_interrupt_handler[i][j])(pci_slot[i][j]))  {  //IRQ is handled
                  serviced=1;
          }
        };
}
#endif /* if 0 */
