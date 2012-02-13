#include <stdio.h>


int ReadPhysicalMemory32(unsigned int addr, unsigned char *tmp){
	int ret=-1;
	FILE *f=fopen("/proc/rtl865x/reg32","w+");
	if(f){
		_ReadPhysicalMemory32(f, addr, tmp);
		ret=0;
	}
	fclose(f);

	return ret;
}

int _ReadPhysicalMemory32(FILE *f,unsigned int addr, unsigned char *tmp){
	char str[16];
    sprintf(tmp,"0x%08x",addr);
    fputs(tmp,f);
	fgets(tmp,11,f);
}

int ReadPhysicalMemory16(unsigned int addr, unsigned char *tmp){
	int ret=-1;
	FILE *f=fopen("/proc/rtl865x/reg16","w+");
	if(f){
		_ReadPhysicalMemory16(f, addr, tmp);
		ret=0;
	}
	fclose(f);
	return ret;
}

int _ReadPhysicalMemory16(FILE *f,unsigned int addr, unsigned char *tmp){
    sprintf(tmp,"0x%08x",addr);
    fputs(tmp,f);
	fgets(tmp,7,f);
}

int ReadPhysicalMemory8(unsigned int addr, unsigned char *tmp){
	int ret=-1;
	FILE *f=fopen("/proc/rtl865x/reg8","w+");
	if(f){
		_ReadPhysicalMemory8(f, addr, tmp);
		ret=0;
	}
	fclose(f);

	return ret;
}

int _ReadPhysicalMemory8(FILE *f,unsigned int addr, unsigned char *tmp){
    sprintf(tmp,"0x%08x",addr);
    fputs(tmp,f);
	fgets(tmp,5,f);
}


int main(int argc, char*argv[])
{
	unsigned int addr,dst;
	unsigned int count;
	unsigned int i,val;
	unsigned char str[20];
	FILE *f=NULL;
	
	if (argc==2)
	{

		if (strlen(argv[1])!=8)
		{
			printf("Invalid address\n");
			return 0;
		}
		printf("address  : value (hex)\n");
		sscanf(argv[1],"%x",&addr);
	 	val = *(unsigned int*)addr;
		printf("%08X : %08X\n",addr,val);
		return 0;

	}

	if (argc<3)
	{
		printf("usage: x addr byte_count\n");
		return 0;	
	}
	//printf("input addr %s\n",argv[1]);
	//addr = strtol(argv[1],NULL,16);
	sscanf(argv[1],"%x",&addr);
	sscanf(argv[2],"%d",&count);

	printf("address  : value (hex)\n");
	switch(count)
	{	
		case 1:
			ReadPhysicalMemory8(addr, str);
			printf("%08X : %s\n",addr,str);
			break;
		case 2:
			ReadPhysicalMemory16(addr, str);
			printf("%08X : %s\n",addr,str);
			break;
		case 4:
			ReadPhysicalMemory32(addr, str);
			printf("%08X : %s\n",addr,str);
			break;

	}
	dst = addr+count;
	if (count>4){
		while (1)
		{
			printf("\n%08X: ",addr);
			for (i=0;i<16;i+=4)
			{
				ReadPhysicalMemory32(addr, str);
			   printf("%s ",str);
			   addr+=4;
		           if (addr>=dst) goto conti;
			}
		        if (addr>=dst) goto conti;

		}	
	}
	return 0;
conti:
	printf("\n");
	return 0;
}
