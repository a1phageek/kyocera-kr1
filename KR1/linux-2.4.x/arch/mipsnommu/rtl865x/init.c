
#include <asm/io.h>
#include <linux/irq.h>
void lexra_setup()
{
#ifdef CONFIG_REMOTE_DEBUG
	rs_kgdb_hook(1);
#endif
#ifdef CONFIG_POLLED_CONSOLE
	register_console(&);
#endif
	return;
}
