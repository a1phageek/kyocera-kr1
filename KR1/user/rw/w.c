#include <stdio.h>

int WritePhysicalMemory32(unsigned int addr, unsigned int value){
	char str[32];
	int ret=-1;
    sprintf(str,"0x%08x",addr);
	FILE *f=fopen("/proc/rtl865x/reg32","w+");
	FILE *f2=fopen("/proc/rtl865x/mem32","w+");	
	if(f &&f2){
	    fputs(str,f);
		memset(str,0,32);
	    sprintf(str,"0x%08x",(unsigned int)value);
	    fputs(str,f2);
		fclose(f);
		fclose(f2);
		ret=0;
	}
	return ret;
}

int WritePhysicalMemory16(unsigned int addr, unsigned short value){
	char str[32];
	int ret=-1;
    sprintf(str,"0x%08x",addr);
	FILE *f=fopen("/proc/rtl865x/reg16","w+");
	FILE *f2=fopen("/proc/rtl865x/mem16","w+");	
	if(f &&f2){
	    fputs(str,f);
		memset(str,0,32);
	    sprintf(str,"0x%04x",(unsigned short)value);
	    fputs(str,f2);
		fclose(f);
		fclose(f2);
		ret=0;
	}
	return ret;
}

int WritePhysicalMemory8(unsigned int addr, unsigned char value){
	char str[32];
	int ret=-1;
    sprintf(str,"0x%08x",addr);
	FILE *f=fopen("/proc/rtl865x/reg8","w+");
	FILE *f2=fopen("/proc/rtl865x/mem8","w+");	
	if(f &&f2){
	    fputs(str,f);
		memset(str,0,32);
	    sprintf(str,"0x%02x",(unsigned char)value);
	    fputs(str,f2);
		fclose(f);
		fclose(f2);
		ret=0;
	}
	return ret;
}

int main(int argc, char*argv[])
{
	unsigned int addr;
	unsigned int mode;
	int i,val;
	
	if (argc<4)
	{
		printf("usage: w  mode(1,2,4) addr(hex) value(hex) \n");
		return 0;	
	}
	//printf("input addr %s\n",argv[1]);
	//addr = strtol(argv[1],NULL,16);
	sscanf(argv[1],"%x",&mode);
	sscanf(argv[2],"%x",&addr);
	sscanf(argv[3],"%x",&val);
	switch(mode)
	{
		case 1:
			printf("Byte Write\n");
			WritePhysicalMemory8(addr, val);
			//*(unsigned char*)addr=(unsigned char)val;
			break;
		case 2:
			printf("2 Byte Write\n");
			WritePhysicalMemory16(addr, val);
			//*(unsigned short*)addr=(unsigned short)val;
			break;
		case 4:
			printf("4 byte Write\n");
			WritePhysicalMemory32(addr, val);			
			//*(unsigned int*)addr=(unsigned int)val;
			break;
		default:
			printf("Not Support\n");

	}
	return 0;
conti:
	return 0;
}
