/* 
 */

#include <linux/config.h>
#include <stdio.h>
#include <stdarg.h>
#include <linux/unistd.h>
#include <linux/errno.h>

static inline _syscall1(long, tabparse, void*, arg);

int main(int argc, char **argv)
{
    struct {int argc; char **argv;} arg = {argc, argv};

    tabparse((void*) &arg);

    exit(0);
}
