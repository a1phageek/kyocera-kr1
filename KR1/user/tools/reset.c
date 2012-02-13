/* vi: set sw=4 ts=4: */
/*
 */

#include <stdio.h>
#include <stdarg.h>

int main(int argc, char **argv)
{
	void (*app_start)(void);

    printf("Reset after 1 second ...\n"); 
    sleep(1);

	/* disable all interrupts */
	__asm__ __volatile__(" \
			.set	noreorder\
			nop\
			mtc0	$0,$12\
			nop\
			.set	reorder\
	");
    
    /* setup watchdog for reset */
    *(unsigned long*) 0xbd01203c = 0xff<<24;
    while(1);
}
