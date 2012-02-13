#include <linux/config.h>
#include <stdio.h>
#include <stdarg.h>

static void config_iram(unsigned int base, unsigned int size)
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
        "li\t$8,0x20\n\t"
        "mtc0\t$8,$20\n\t"
        "nop\n\t"
        "nop\n\t"
        "move\t$8,$4\n\t"
        "la\t$9,0x0ffffc00\n\t"
        "and\t$8,$8,$9\n\t'"
        "mtc3\t$8,$0\n\t"
        "nop\n\t"
        "nop\n\t"
        "mtc3\t$8,$0\n\t" //writing again to make it work, do not know why???
        "nop\n\t"
        "nop\n\t"
        "add\t$8,$8,$5\n\t"
        "addiu\t$8,$8,-1\n\t"
        "mtc3\t$8,$1\n\t"
        "nop\n\t"
        "nop\n\t"
        "li\t$8,0x10\n\t"
        "mtc0\t$8,$20\n\t"
        "nop\n\t"
        "nop\n\t"
        ".set\treorder\n\t");
}

int main(int argc, char **argv)
{
    int (*app_start)(void);
    unsigned int buf = 0x80600000;
    unsigned int ofst;
    unsigned int size;
    
    if (argc != 2)
    {
        printf("Usage: testiram SIZE\n");
        exit(0);
    }
    size = strtoul(argv[1], NULL, 16);

    for (ofst=0; ofst<size; ofst+=4)
        *(unsigned int*) (buf + ofst) = 0x20420001; /* opcode 'addi r2,r2,1' */
    *(unsigned int*) buf = 0x24020000; /* opcode 'addiu r2,zero,0x0000' */
    *(unsigned int*) (buf + size - 8) = 0x03e00008; /* opcode 'jr $ra' */

    config_iram(buf, size);

    usleep(100);

    app_start = (unsigned int (*)(void)) buf;
    if (app_start() == ((size / 4) - 2)) /* exclude 'addiu r2,zero,0' and 'jr ra'. */
        printf("PASS!\n");
    else
        printf("FAIL!\n");

    exit(0);
}
