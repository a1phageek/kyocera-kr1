/* vi: set sw=4 ts=4: */
/*
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    unsigned int *addr, num, i=0;

    if ((argc != 2) && (argc != 4))
	{
        printf("Usage: dw ADDRESS [l NUMBER]\n");
        exit(0);
    }

    if ((strlen(argv[1]) != 8) && (strlen(argv[1]) != 10))
    {
        printf("Invalid address!\n");
        exit(0);
    }
    addr = (unsigned int*) strtoul(argv[1], NULL, 16);
    if ((unsigned int) addr & 3)
    {
        printf("Invalid address!\n");
        exit(0);
    }

    if (argc == 2)
        num = 1;
    else
        num = strtoul(argv[3], NULL, 10);

    while (i < num)
    {
        if (i % 4 == 0)
			printf("%08x: ", (unsigned int) addr);
        printf("%08x ", *addr);
        if (i % 4 == 3)
			printf("\n");
        i++; addr++;
    }
	if (num % 4 != 0)
		printf("\n");

    exit(0);
}
