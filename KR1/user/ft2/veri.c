#include <linux/config.h>
#include <stdio.h>
#include <stdarg.h>
#include <linux/unistd.h>
#include <linux/errno.h>

static inline _syscall1(long, veri, void*, arg);

int main(int argc, char **argv)
{
    struct {int argc; char **argv;} arg = {argc, argv};
    //FILE *fp;
    //char cmd[2048];

    //fp = fopen("/home/ft2tab", "r");
    //while (fgets(cmd, 2048, fp) != NULL)
    //    veri(cmd);
    //fclos(fpt);

    veri((void*) &arg);

    exit(0);
}
