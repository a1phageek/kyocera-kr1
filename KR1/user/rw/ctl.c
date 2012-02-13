#include <stdio.h>

int main(int argc, char*argv[])
{
	int mode;
	char str[80];
	FILE *f;
	if (argc<3)
	{
		printf("Simple tool to write kernel sysctl variables\n");
		printf("usage: wctl filename {value}\n");
		return 0;	
	}

	f=fopen(argv[1],"w+");
	sscanf(argv[2],"%x",&mode);
	sprintf(str,"%u",mode);
	fputs(str,f);
	fclose(f);
				
	return 0;
}
