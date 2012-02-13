#include <linux/config.h>
#include <stdio.h>
#include <stdarg.h>

static void config_dram(unsigned int base, unsigned int size)
{
    __asm__ __volatile__(
        ".set\tnoreorder\n\t"
        "mfc0\t$8,$12\n\t"
        "nop\n\t"
        "nop\n\t"
        "or\t$8,0x80000000\n\t"
        "mtc0\t$8,$12\n\t"
        "nop\n\t"
        "nop\n\t"
        "mtc0\t$0,$20\n\t"
        "nop\n\t"
        "nop\n\t"
        "li\t$8,0x3\n\t"
        "mtc0\t$8,$20\n\t"
        "nop\n\t"
        "nop\n\t"
        "move\t$8,$4\n\t"
        "la\t$9,0x0ffffc00\n\t"
        "and\t$8,$8,$9\n\t'"
        "mtc3\t$8,$4\n\t"
        "nop\n\t"
        "nop\n\t"
        "mtc3\t$8,$4\n\t" //writing again to make it work, do not know why???
        "nop\n\t"
        "nop\n\t"
        "add\t$8,$8,$5\n\t"
        "addiu\t$8,$8,-1\n\t"
        "mtc3\t$8,$5\n\t"
        "nop\n\t"
        "nop\n\t"
        "mtc0\t$0,$20\n\t"
        "nop\n\t"
        "nop\n\t"
        ".set\treorder\n\t");
}

int main(int argc, char **argv)
{
    unsigned int buf = 0x80600000;
    unsigned int ofst;
    unsigned int size;
    
    if (argc != 2)
    {
        printf("Usage: testdram SIZE\n");
        exit(0);
    }
    size = strtoul(argv[1], NULL, 16);

    config_dram(buf, size);

    usleep(100);

    for (ofst=0; ofst<size; ofst+=4)
        *(unsigned int*) (buf + ofst) = 0x55aa33cc + ofst;

    for (ofst=0; ofst<size; ofst+=4)
        if (*(unsigned int*) (buf + ofst) != 0x55aa33cc + ofst)
        {
            printf("FAIL!\n");
            break;
        }
    if (ofst >= size)
        printf("PASS!\n");

    exit(0);
}
