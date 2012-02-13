/* vi: set sw=4 ts=4: */
/*
 *
 *
 * Copyright (C) 1995, 1996 by Bruce Perens <bruce@pixar.com>.
 * Copyright (C) 1999-2002 Erik Andersen <andersee@debian.org>
 * Adjusted by so many folks, it's impossible to keep track.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

/* Turn this on to disable all the dangerous 
   rebooting stuff when debugging.
#define DEBUG_INIT
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <paths.h>
#include <signal.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <limits.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/wait.h>
#ifdef BB_SYSLOGD
# include <sys/syslog.h>
#endif
#include <linux/config.h>

#define INIT_BUFFS_SIZE 256

/* From <linux/vt.h> */
struct vt_stat {
	unsigned short v_active;        /* active vt */
	unsigned short v_signal;        /* signal to send */
	unsigned short v_state;         /* vt bitmask */
};
static const int VT_GETSTATE = 0x5603;  /* get global vt state info */

/* From <linux/serial.h> */
struct serial_struct {
	int     type;
	int     line;
	int     port;
	int     irq;
	int     flags;
	int     xmit_fifo_size;
	int     custom_divisor;
	int     baud_base;
	unsigned short  close_delay;
	char    reserved_char[2];
	int     hub6;
	unsigned short  closing_wait; /* time to wait before closing */
	unsigned short  closing_wait2; /* no longer used... */
	int     reserved[4];
};


#if (__GNU_LIBRARY__ > 5) || defined(__dietlibc__) 
  #include <sys/reboot.h>
  #define init_reboot(magic) reboot(magic)
#else
  #define init_reboot(magic) reboot(0xfee1dead, 672274793, magic)
#endif

#ifndef _PATH_STDPATH
#define _PATH_STDPATH	"/usr/bin:/bin:/usr/sbin:/sbin"
#endif

#if defined BB_FEATURE_INIT_COREDUMPS
/*
 * When a file named CORE_ENABLE_FLAG_FILE exists, setrlimit is called 
 * before processes are spawned to set core file size as unlimited.
 * This is for debugging only.  Don't use this is production, unless
 * you want core dumps lying about....
 */
#define CORE_ENABLE_FLAG_FILE "/.init_enable_core"
#include <sys/resource.h>
#include <sys/time.h>
#endif

#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))

#if __GNU_LIBRARY__ > 5
	#include <sys/kdaemon.h>
#else
	extern int bdflush (int func, long int data);
#endif

#define _PATH_RC	"/etc/rc"
#define _PATH_REBOOT	"/bin/reboot"
#define SHELL        "/bin/sh"	     /* Default shell */
#define LOGIN_SHELL  "-" SHELL	     /* Default login shell */
#define INITTAB      "/etc/inittab"  /* inittab file location */
#ifndef INIT_SCRIPT
#define INIT_SCRIPT  "/etc/rc"   /* Default sysinit script. */
#endif

#define MAXENV	16		/* Number of env. vars */

/* Allowed init action types */
#define SYSINIT     0x001
#define RESPAWN     0x002
#define ASKFIRST    0x004
#define WAIT        0x008
#define ONCE        0x010
#define CTRLALTDEL  0x020
#define SHUTDOWN    0x040
#define RESTART     0x080

/* A mapping between "inittab" action name strings and action type codes. */
struct init_action_type {
	const char *name;
	int action;
};

static const struct init_action_type actions[] = {
	{"sysinit", SYSINIT},
	{"respawn", RESPAWN},
	{"askfirst", ASKFIRST},
	{"wait", WAIT},
	{"once", ONCE},
	{"ctrlaltdel", CTRLALTDEL},
	{"shutdown", SHUTDOWN},
	{"restart", RESTART},
	{0, 0}
};

/* Set up a linked list of init_actions, to be read from inittab */
struct init_action {
	pid_t pid;
	char command[INIT_BUFFS_SIZE];
	char terminal[INIT_BUFFS_SIZE];
	struct init_action *next;
	int action;
};

/* Static variables */
int run_sigint_processing = 0;
int stopped = 0;	/* are we stopped */

static char termType[32]   = "TERM=linux";
static char console[32]    = _PATH_CONSOLE;
#ifndef BB_SYSLOGD
//static char *log           = VC_5;
#endif
static sig_atomic_t got_cont = 0;
static const int LOG = 0x1;
static const int CONSOLE = 0x2;
#if defined BB_FEATURE_EXTRA_QUIET
static const int MAYBE_CONSOLE = 0x0;
#else
#define MAYBE_CONSOLE	CONSOLE
#endif
#ifndef RB_HALT_SYSTEM
static const int RB_HALT_SYSTEM = 0xcdef0123;
static const int RB_ENABLE_CAD = 0x89abcdef;
static const int RB_DISABLE_CAD = 0;
#define RB_POWER_OFF    0x4321fedc
static const int RB_AUTOBOOT = 0x01234567;
#endif

/* Function prototypes */
/* Print a message to the specified device.
 * Device may be bitwise-or'd from LOG | CONSOLE */
#ifdef DEBUG_INIT
static inline messageND(int device, char *fmt, ...) { }
#else 
#define messageND message
#endif
static void message(int device, char *fmt, ...) __attribute__ ((format (printf, 2, 3)));


static int do_command(const char *path, const char *filename, int dowait)
{
	pid_t pid, wpid;
	int stat, st;
	
	if((pid = vfork()) == 0) {
		/* the child */
		char *argv[3];
#ifdef INCLUDE_TIMEZONE
		char tz[CMDSIZ];
#endif
		char *env[3];

		close(0);
		argv[0] = (char *)path;
		argv[1] = (char *)filename;
		argv[2] = NULL;

		env[0] = "PATH=/bin:/usr/bin:/etc:/sbin:/usr/sbin";
#ifdef INCLUDE_TIMEZONE
		strcpy(tz, "TZ=");
		strcat(tz, tzone);
		env[1] = tz;
		env[2] = NULL;
#else
		env[1] = NULL;
#endif

		execve(path, argv, env);

		_exit(2);
	} else if(pid > 0) {
		if (!dowait)
			stat = 0;
		else {
			/* parent, wait till rc process dies before spawning */
			while ((wpid = wait(&stat)) != pid)
				if (wpid == -1 && errno == ECHILD) { /* see wait(2) manpage */
					stat = 0;
					break;
				}
		}
	} else if(pid < 0) {
		stat = -1;
	}
	st = WEXITSTATUS(stat);
	return st;
}




/* How much memory does this machine have?
   Units are kBytes to avoid overflow on 4GB machines */
	
/* Make sure there is enough memory to do something useful. *
 * Calls "swapon -a" if needed so be sure /etc/fstab is present... */
static void enter_single(void)
{
	pid_t pid;
	char *av[2];
	
    av[0] = _PATH_BSHELL;
    av[1] = NULL;
    if((pid = vfork()) == 0) {
    extern char **environ;
	/* the child */
	execve(_PATH_BSHELL, av, environ);
	_exit(0);
    } else if(pid > 0) {
    int i;
	while(wait(&i) != pid) /* nothing */;
    } else if(pid < 0) {
    }
}


void make_console(const char *tty)
{
	int j;
	char devname[32];

	close(0); close(1); close(2);
	if (!tty || !*tty) {
		if (open("/dev/null", O_RDWR|O_NONBLOCK) >= 0)
			dup(0), dup(0);
		return;
	}


#if LINUX_VERSION_CODE < 0x020100
/*
 *	until we get proper console support under 2.0
 */
	if (strcmp(tty, "console") == 0) {
		strcpy(devname, console_device);
	} else
#endif
	{
		strcpy(devname, "/dev/");
		strcat(devname, tty);
	}

#if 1 // DAVIDM_FIXME
	if (open(devname, O_RDWR|O_NONBLOCK) == -1) {
#if 1
		printf("console '%s' open failed: %d\n", devname, errno);
#endif
		return;
	}
#endif

	fcntl(0, F_SETFL, 0);
	dup(0); 
	dup(0);
	make_ascii_tty();
	j = ioctl(0, TIOCSCTTY, (char*)0);
}


static int do_rc(void)
{
	int rc;

	rc = do_command(_PATH_BSHELL, _PATH_RC, 1);
	if (rc)
		return(rc);
	return 0;
}





void make_ascii_tty(void)
{
	struct termios tty;

	tcgetattr(0, &tty);
	tty.c_iflag &= ~(INLCR|IGNCR|IUCLC);
	tty.c_iflag |= ICRNL;
	tty.c_oflag &= ~(OCRNL|OLCUC|ONOCR|ONLRET|OFILL);
	tty.c_oflag |= OPOST|ONLCR;
	tty.c_cflag |= CLOCAL;
	tty.c_lflag  = ISIG|ICANON|ECHO|ECHOE|ECHOK|ECHOCTL|ECHOKE;

#if LINUX_VERSION_CODE < 0x020100
	if (console_baud != -1)
		cfsetospeed(&tty, console_baud);
#endif

	tty.c_cc[VINTR]  = 3;
	tty.c_cc[VQUIT]  = 28;
	tty.c_cc[VERASE] = 8/*127*/;
	tty.c_cc[VKILL]  = 24;
	tty.c_cc[VEOF]   = 4;
	tty.c_cc[VTIME]  = 0;
	tty.c_cc[VMIN]   = 1;
	tty.c_cc[VSTART] = 17;
	tty.c_cc[VSTOP]  = 19;
	tty.c_cc[VSUSP]  = 26;

	tcsetattr(0, TCSANOW, &tty);
}

int boot_uart1=0;

int main(int argc, char **argv)
{
	
	chdir("/");
	setsid();

	/* Make sure PATH is set to something sane */
	putenv("PATH="_PATH_STDPATH);

	/* Make sure there is enough memory to do something useful. */
	do_rc();

#ifdef CONFIG_RTL865X
	for(i = 1; i < argc; i++) {
	    if(argv[i] && !strcmp(argv[i], "uart1"))
			boot_uart1=1;;
	}
#endif


#ifdef CONFIG_RTL865X
	if(boot_uart1)
		make_console("/dev/ttyS1");
	else
#endif
	make_console("/dev/console");
	for(;;)
	enter_single();



	
	//run_actions(SYSINIT);

}
/*
Local Variables:
c-file-style: "linux"
c-basic-offset: 4
tab-width: 4
End:
*/
