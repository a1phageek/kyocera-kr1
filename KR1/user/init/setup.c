#include <stdio.h>

int main(int argc, char*argv[])
{
	int i;
	printf("argv[1] %s\n",argv[1]);
	if (getenv("dns"))
		printf("dns  %s\n",getenv("dns"));
	if (getenv("ip"))
		printf("ip  %s\n",getenv("ip"));
	if (getenv("router"))
		printf("router %s\n",getenv("router"));
	if (getenv("subnet"))
		printf("subnet %s\n",getenv("dns"));
	if (getenv("broadcast"))
		printf("broadcast %s\n",getenv("broadcast"));
}
