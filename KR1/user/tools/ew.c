/* vi: set sw=4 ts=4: */
/*
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    unsigned int *addr, num, val, i=0;

    if ((argc != 4) && (argc != 6))
	{
        printf("Usage: ew ADDRESS [l NUMBER] = VALUE\n");
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

    if (argc == 4)
    {
        num = 1;
        val = strtoul(argv[3], NULL, 16);
    }
    else
    {
        num = strtoul(argv[3], NULL, 10);
        val = strtoul(argv[5], NULL, 16);
    }

    while (i < num)
    {
        *addr = val;
        i++; addr++;
    }

    exit(0);
}
