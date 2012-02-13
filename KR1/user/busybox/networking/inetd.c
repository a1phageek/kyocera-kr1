/*
 * Copyright (c) 1983,1991 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * David A. Holland.
 *
 * Busybox port by Vladimir Oleynik (C) 2001-2003 <dzo@simtreas.ru>
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

/*
 * Inetd - Internet super-server
 *
 * This program invokes all internet services as needed.
 * connection-oriented services are invoked each time a
 * connection is made, by creating a process.  This process
 * is passed the connection as file descriptor 0 and is
 * expected to do a getpeername to find out the source host
 * and port.
 *
 * Datagram oriented services are invoked when a datagram
 * arrives; a process is created and passed a pending message
 * on file descriptor 0.  Datagram servers may either connect
 * to their peer, freeing up the original socket for inetd
 * to receive further messages on, or ``take over the socket'',
 * processing all arriving datagrams and, eventually, timing
 * out.  The first type of server is said to be ``multi-threaded'';
 * the second type of server ``single-threaded''.
 *
 * Inetd uses a configuration file which is read at startup
 * and, possibly, at some later time in response to a hangup signal.
 * The configuration file is ``free format'' with fields given in the
 * order shown below.  Continuation lines for an entry must being with
 * a space or tab.  All fields must be present in each entry.
 *
 *      service name                    must be in /etc/services
 *      socket type                     stream/dgram/raw/rdm/seqpacket
 *      protocol                        must be in /etc/protocols
 *      wait/nowait[.max]               single-threaded/multi-threaded, max #
 *      user[.group]                    user/group to run daemon as
 *      server program                  full path name
 *      server program arguments        maximum of MAXARGS (20)
 *
 * RPC services unsuported
 *
 * Comment lines are indicated by a `#' in column 1.
 */

/*
 * Here's the scoop concerning the user.group feature:
 *
 * 1) No group listed.
 *
 *      a) for root:    NO setuid() or setgid() is done
 *
 *      b) nonroot:     setuid()
 *                      setgid(primary group as found in passwd)
 *                      initgroups(name, primary group)
 *
 * 2) set-group-option on.
 *
 *      a) for root:    NO setuid()
 *                      setgid(specified group)
 *                      setgroups(1, specified group)
 *
 *      b) nonroot:     setuid()
 *                      setgid(specified group)
 *                      initgroups(name, specified group)
 *
 * All supplementary groups are discarded at startup in case inetd was
 * run manually.
 */

#define __USE_BSD_SIGNAL

#include "busybox.h"

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#ifndef __linux__
#ifndef RLIMIT_NOFILE
#define RLIMIT_NOFILE   RLIMIT_OFILE
#endif
#endif

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include <errno.h>
#include <signal.h>
#include <netdb.h>
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>

#define _PATH_INETDCONF "/etc/inetd.conf"
#define _PATH_INETDPID  "/var/run/inetd.pid"

#ifndef MIN
#define MIN(a, b)       ((a) < (b) ? (a) : (b))
#endif

#define TOOMANY         40              /* don't start more than TOOMANY */
#define CNT_INTVL       60              /* servers in CNT_INTVL sec. */
#define RETRYTIME       (60*10)         /* retry after bind or server fail */

#ifndef OPEN_MAX
#define OPEN_MAX        64
#endif


/* Reserve some descriptors, 3 stdio + at least: 1 log, 1 conf. file */
#define FD_MARGIN       (8)
static int     rlim_ofile_cur = OPEN_MAX;

#ifdef RLIMIT_NOFILE
static struct rlimit   rlim_ofile;
#endif

#define INETD_UNSUPPORT_BILTIN 1

/* Check unsupporting builtin */
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_ECHO
#undef INETD_UNSUPPORT_BILTIN
#endif
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_DISCARD
#undef INETD_UNSUPPORT_BILTIN
#endif
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_TIME
#undef INETD_UNSUPPORT_BILTIN
#endif
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_DAYTIME
#undef INETD_UNSUPPORT_BILTIN
#endif
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_CHARGEN
#undef INETD_UNSUPPORT_BILTIN
#endif

static struct  servtab {
	char    *se_service;            /* name of service */
	int     se_socktype;            /* type of socket to use */
	int     se_family;              /* address family */
	char    *se_proto;              /* protocol used */
	short   se_wait;                /* single threaded server */
	short   se_checked;             /* looked at during merge */
	char    *se_user;               /* user name to run as */
	char    *se_group;              /* group name to run as */
#ifndef INETD_UNSUPPORT_BILTIN
	const struct  biltin *se_bi;    /* if built-in, description */
#endif
	char    *se_server;             /* server program */
#define MAXARGV 20
	char    *se_argv[MAXARGV+1];    /* program arguments */
	int     se_fd;                  /* open descriptor */
	union {
		struct  sockaddr se_un_ctrladdr;
		struct  sockaddr_in se_un_ctrladdr_in;
		struct  sockaddr_un se_un_ctrladdr_un;
	} se_un;                        /* bound address */
#define se_ctrladdr     se_un.se_un_ctrladdr
#define se_ctrladdr_in  se_un.se_un_ctrladdr_in
#define se_ctrladdr_un  se_un.se_un_ctrladdr_un
	int     se_ctrladdr_size;
	int     se_max;                 /* max # of instances of this service */
	int     se_count;               /* number started since se_time */
	struct  timeval se_time;        /* start of se_count */
	struct  servtab *se_next;
} *servtab;

/* Length of socket listen queue. Should be per-service probably. */
static int      global_queuelen = 128;

static int      nsock, maxsock;
static fd_set   allsock;
static int      timingout;
static sigset_t blockmask, emptymask;


       /* Echo received data */
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_ECHO
static void echo_stream(int, struct servtab *);
static void echo_dg(int, struct servtab *);
#endif
	/* Internet /dev/null */
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_DISCARD
static void discard_stream(int, struct servtab *);
static void discard_dg(int, struct servtab *);
#endif
	/* Return 32 bit time since 1900 */
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_TIME
static void machtime_stream(int, struct servtab *);
static void machtime_dg(int, struct servtab *);
#endif
	/* Return human-readable time */
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_DAYTIME
static void daytime_stream(int, struct servtab *);
static void daytime_dg(int, struct servtab *);
#endif
	/* Familiar character generator */
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_CHARGEN
static void chargen_stream(int, struct servtab *);
static void chargen_dg(int, struct servtab *);
#endif

#ifndef INETD_UNSUPPORT_BILTIN
struct biltin {
	const char *bi_service;         /* internally provided service name */
	int bi_socktype;                /* type of socket supported */
	short bi_fork;          /* 1 if should fork before call */
	short bi_wait;          /* 1 if should wait for child */
	void (*bi_fn)(int, struct servtab *); /* fn which performs it */
};

static const struct biltin biltins[] = {
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_ECHO
	/* Echo received data */
	{ "echo",               SOCK_STREAM,    1, 0,   echo_stream, },
	{ "echo",               SOCK_DGRAM,     0, 0,   echo_dg, },
#endif
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_DISCARD
	/* Internet /dev/null */
	{ "discard",    SOCK_STREAM,    1, 0,   discard_stream, },
	{ "discard",    SOCK_DGRAM,     0, 0,   discard_dg, },
#endif
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_TIME
	/* Return 32 bit time since 1900 */
	{ "time",               SOCK_STREAM,    0, 0,   machtime_stream, },
	{ "time",               SOCK_DGRAM,     0, 0,   machtime_dg,     },
#endif
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_DAYTIME
	/* Return human-readable time */
	{ "daytime",    SOCK_STREAM,    0, 0,   daytime_stream, },
	{ "daytime",    SOCK_DGRAM,     0, 0,   daytime_dg,     },
#endif
#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_CHARGEN
	/* Familiar character generator */
	{ "chargen",    SOCK_STREAM,    1, 0,   chargen_stream, },
	{ "chargen",    SOCK_DGRAM,     0, 0,   chargen_dg,     },
#endif
	{ NULL, 0, 0, 0, NULL }
};
#endif  /* INETD_UNSUPPORT_BILTIN */

#define NUMINT  (sizeof(intab) / sizeof(struct inent))
static const char *CONFIG = _PATH_INETDCONF;

#define BCOPY(s, d, z) memcpy(d, s, z)

static void
syslog_err_and_discard_dg(int se_socktype, const char *msg, ...)
	__attribute__ ((noreturn, format (printf, 2, 3)));

static void
syslog_err_and_discard_dg(int se_socktype, const char *msg, ...)
{
	char buf[50];
	va_list p;

	va_start(p, msg);
	vsyslog(LOG_ERR, msg, p);
	if (se_socktype != SOCK_STREAM)
		recv(0, buf, sizeof (buf), 0);
	_exit(1);
}

static FILE *fconfig;
static char line[256];

static FILE *
setconfig(void)
{
	FILE *f = fconfig;

	if (f != NULL) {
		fseek(f, 0L, L_SET);
	} else {
		f = fconfig = fopen(CONFIG, "r");
		if(f == NULL)
			syslog(LOG_ERR, "%s: %m", CONFIG);
	}
	return f;
}

static char *
nextline(void)
{
	char *cp;
	FILE *fd = fconfig;

	if (fgets(line, sizeof (line), fd) == NULL)
		return ((char *)0);
	cp = strchr(line, '\n');
	if (cp)
		*cp = '\0';
	return (line);
}

static char *
skip(char **cpp)
{
	char *cp = *cpp;
	char *start;

	if (*cpp == NULL)
		return ((char *)0);

again:
	while (*cp == ' ' || *cp == '\t')
		cp++;
	if (*cp == '\0') {
		int c;

		c = getc(fconfig);
		(void) ungetc(c, fconfig);
		if (c == ' ' || c == '\t')
			if ((cp = nextline()) != NULL)
				goto again;
		*cpp = NULL;
		return NULL;
	}
	start = cp;
	while (*cp && *cp != ' ' && *cp != '\t')
		cp++;
	if (*cp != '\0')
		*cp++ = '\0';
	*cpp = cp;
	return (start);
}

static char *
newstr(char *cp)
{
	cp = strdup(cp ? cp : "");
	if (cp)
		return(cp);

	syslog_err_and_discard_dg(SOCK_STREAM, "strdup: %m");
}


static struct servtab *
getconfigent(void)
{
	static struct servtab serv;
	struct servtab *sep = &serv;
	int argc;
	char *cp, *arg;

more:
	while ((cp = nextline()) && *cp == '#')
		;
	if (cp == NULL)
		return ((struct servtab *)0);
	memset((char *)sep, 0, sizeof *sep);
	sep->se_service = newstr(skip(&cp));
	arg = skip(&cp);
	if (arg == NULL)
		goto more;

	if (strcmp(arg, "stream") == 0)
		sep->se_socktype = SOCK_STREAM;
	else if (strcmp(arg, "dgram") == 0)
		sep->se_socktype = SOCK_DGRAM;
	else if (strcmp(arg, "rdm") == 0)
		sep->se_socktype = SOCK_RDM;
	else if (strcmp(arg, "seqpacket") == 0)
		sep->se_socktype = SOCK_SEQPACKET;
	else if (strcmp(arg, "raw") == 0)
		sep->se_socktype = SOCK_RAW;
	else
		sep->se_socktype = -1;

	sep->se_proto = newstr(skip(&cp));
	if (strcmp(sep->se_proto, "unix") == 0) {
		sep->se_family = AF_UNIX;
	} else {
		sep->se_family = AF_INET;
		if (strncmp(sep->se_proto, "rpc/", 4) == 0) {
			syslog(LOG_ERR, "%s: rpc services not suported",
			    sep->se_service);
			goto more;
		}
	}
	arg = skip(&cp);
	if (arg == NULL)
		goto more;
	{
		char    *s = strchr(arg, '.');
		if (s) {
			*s++ = '\0';
			sep->se_max = atoi(s);
		} else
			sep->se_max = TOOMANY;
	}
	sep->se_wait = strcmp(arg, "wait") == 0;
	sep->se_user = newstr(skip(&cp));
	sep->se_group = strchr(sep->se_user, '.');
	if (sep->se_group) {
		*sep->se_group++ = '\0';
	}
	sep->se_server = newstr(skip(&cp));
	if (strcmp(sep->se_server, "internal") == 0) {
#ifndef INETD_UNSUPPORT_BILTIN
		const struct biltin *bi;

		for (bi = biltins; bi->bi_service; bi++)
			if (bi->bi_socktype == sep->se_socktype &&
			    strcmp(bi->bi_service, sep->se_service) == 0)
				break;
		if (bi->bi_service == 0) {
			syslog(LOG_ERR, "internal service %s unknown",
				sep->se_service);
			goto more;
		}
		sep->se_bi = bi;
		sep->se_wait = bi->bi_wait;
#else
		syslog(LOG_ERR, "internal service %s unknown",
				sep->se_service);
			goto more;
#endif
	} else
#ifndef INETD_UNSUPPORT_BILTIN
		sep->se_bi = NULL
#endif
		;
	argc = 0;
	for (arg = skip(&cp); cp; arg = skip(&cp)) {
		if (argc < MAXARGV)
			sep->se_argv[argc++] = newstr(arg);
	}
	while (argc <= MAXARGV)
		sep->se_argv[argc++] = NULL;
	return (sep);
}

static void
freeconfig(struct servtab *cp)
{
	int i;

	free(cp->se_service);
	free(cp->se_proto);
	free(cp->se_user);
	/* Note: se_group is part of the newstr'ed se_user */
	free(cp->se_server);
	for (i = 0; i < MAXARGV; i++)
		free(cp->se_argv[i]);
}

#ifndef INETD_UNSUPPORT_BILTIN
static char **Argv;
static char *LastArg;

static void
setproctitle(char *a, int s)
{
	size_t size;
	char *cp;
	struct sockaddr_in sn;
	char buf[80];

	cp = Argv[0];
	size = sizeof(sn);
	if (getpeername(s, (struct sockaddr *)&sn, &size) == 0)
		(void) sprintf(buf, "-%s [%s]", a, inet_ntoa(sn.sin_addr));
	else
		(void) sprintf(buf, "-%s", a);
	strncpy(cp, buf, LastArg - cp);
	cp += strlen(cp);
	while (cp < LastArg)
		*cp++ = ' ';
}
#endif  /* INETD_UNSUPPORT_BILTIN */

static struct servtab *
enter(struct servtab *cp)
{
	struct servtab *sep;
	sigset_t oldmask;

	sep = (struct servtab *)malloc(sizeof (*sep));
	if (sep == NULL) {
		syslog_err_and_discard_dg(SOCK_STREAM, bb_msg_memory_exhausted);
	}
	*sep = *cp;
	sep->se_fd = -1;
	sigprocmask(SIG_BLOCK, &blockmask, &oldmask);
	sep->se_next = servtab;
	servtab = sep;
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
	return (sep);
}

static int
bump_nofile(void)
{
#ifdef RLIMIT_NOFILE

#define FD_CHUNK        32

	struct rlimit rl;

	if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
		syslog(LOG_ERR, "getrlimit: %m");
		return -1;
	}
	rl.rlim_cur = MIN(rl.rlim_max, rl.rlim_cur + FD_CHUNK);
	if (rl.rlim_cur <= rlim_ofile_cur) {
		syslog(LOG_ERR,
#if _FILE_OFFSET_BITS == 64
			"bump_nofile: cannot extend file limit, max = %lld",
#else
			"bump_nofile: cannot extend file limit, max = %ld",
#endif
			rl.rlim_cur);
		return -1;
	}

	if (setrlimit(RLIMIT_NOFILE, &rl) < 0) {
		syslog(LOG_ERR, "setrlimit: %m");
		return -1;
	}

	rlim_ofile_cur = rl.rlim_cur;
	return 0;

#else
	syslog(LOG_ERR, "bump_nofile: cannot extend file limit");
	return -1;
#endif
}


static void
setup(struct servtab *sep)
{
	int on = 1;

	if ((sep->se_fd = socket(sep->se_family, sep->se_socktype, 0)) < 0) {
		syslog(LOG_ERR, "%s/%s: socket: %m",
		    sep->se_service, sep->se_proto);
		return;
	}
	if (setsockopt(sep->se_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&on,
			    sizeof(on)) < 0)
		syslog(LOG_ERR, "setsockopt (SO_REUSEADDR): %m");
	if (bind(sep->se_fd, &sep->se_ctrladdr, sep->se_ctrladdr_size) < 0) {
		syslog(LOG_ERR, "%s/%s: bind: %m",
		    sep->se_service, sep->se_proto);
		(void) close(sep->se_fd);
		sep->se_fd = -1;
		if (!timingout) {
			timingout = 1;
			alarm(RETRYTIME);
		}
		return;
	}
	if (sep->se_socktype == SOCK_STREAM)
		listen(sep->se_fd, global_queuelen);

	FD_SET(sep->se_fd, &allsock);
	nsock++;
	if (sep->se_fd > maxsock) {
		maxsock = sep->se_fd;
		if (maxsock > rlim_ofile_cur - FD_MARGIN)
			bump_nofile();
	}
}

static void
config(int signum)
{
	struct servtab *sep, *cp, **sepp;
	sigset_t oldmask;
	unsigned n;

	(void)signum;
	if (setconfig() == NULL)
		return;

	for (sep = servtab; sep; sep = sep->se_next)
		sep->se_checked = 0;
	while ((cp = getconfigent()) != NULL) {
		for (sep = servtab; sep; sep = sep->se_next)
			if (strcmp(sep->se_service, cp->se_service) == 0 &&
			    strcmp(sep->se_proto, cp->se_proto) == 0)
				break;
		if (sep != 0) {
			int i;

#define SWAP(type, a, b) {type c=(type)a; (type)a=(type)b; (type)b=(type)c;}

			sigprocmask(SIG_BLOCK, &emptymask, &oldmask);
			/*
			 * sep->se_wait may be holding the pid of a daemon
			 * that we're waiting for.  If so, don't overwrite
			 * it unless the config file explicitly says don't
			 * wait.
			 */
			if (
#ifndef INETD_UNSUPPORT_BILTIN
			    cp->se_bi == 0 &&
#endif
			    (sep->se_wait == 1 || cp->se_wait == 0))
				sep->se_wait = cp->se_wait;
			if (cp->se_max != sep->se_max)
				SWAP(int, cp->se_max, sep->se_max);
			if (cp->se_user)
				SWAP(char *, sep->se_user, cp->se_user);
			if (cp->se_group)
				SWAP(char *, sep->se_group, cp->se_group);
			if (cp->se_server)
				SWAP(char *, sep->se_server, cp->se_server);
			for (i = 0; i < MAXARGV; i++)
				SWAP(char *, sep->se_argv[i], cp->se_argv[i]);
#undef SWAP
			sigprocmask(SIG_SETMASK, &oldmask, NULL);
			freeconfig(cp);
		} else {
			sep = enter(cp);
		}
		sep->se_checked = 1;

		switch (sep->se_family) {
		case AF_UNIX:
			if (sep->se_fd != -1)
				break;
			(void)unlink(sep->se_service);
			n = strlen(sep->se_service);
			if (n > sizeof(sep->se_ctrladdr_un.sun_path) - 1)
				n = sizeof(sep->se_ctrladdr_un.sun_path) - 1;
			strncpy(sep->se_ctrladdr_un.sun_path, sep->se_service, n);
			sep->se_ctrladdr_un.sun_family = AF_UNIX;
			sep->se_ctrladdr_size = n +
					sizeof sep->se_ctrladdr_un.sun_family;
			setup(sep);
			break;
		case AF_INET:
			sep->se_ctrladdr_in.sin_family = AF_INET;
			sep->se_ctrladdr_size = sizeof sep->se_ctrladdr_in;
			{
				u_short port = htons(atoi(sep->se_service));

				if (!port) {
					struct servent *sp;
					sp = getservbyname(sep->se_service,
								sep->se_proto);
					if (sp == 0) {
						syslog(LOG_ERR,
						    "%s/%s: unknown service",
						    sep->se_service, sep->se_proto);
						continue;
					}
					port = sp->s_port;
				}
				if (port != sep->se_ctrladdr_in.sin_port) {
					sep->se_ctrladdr_in.sin_port = port;
					if (sep->se_fd != -1) {
						FD_CLR(sep->se_fd, &allsock);
						nsock--;
						(void) close(sep->se_fd);
					}
					sep->se_fd = -1;
				}
				if (sep->se_fd == -1)
					setup(sep);
			}
		}
	}
	if (fconfig) {
		(void) fclose(fconfig);
		fconfig = NULL;
	}
	/*
	 * Purge anything not looked at above.
	 */
	sigprocmask(SIG_SETMASK, &blockmask, &oldmask);
	sepp = &servtab;
	while ((sep = *sepp) != NULL) {
		if (sep->se_checked) {
			sepp = &sep->se_next;
			continue;
		}
		*sepp = sep->se_next;
		if (sep->se_fd != -1) {
			FD_CLR(sep->se_fd, &allsock);
			nsock--;
			(void) close(sep->se_fd);
		}
		if (sep->se_family == AF_UNIX)
			(void)unlink(sep->se_service);
		freeconfig(sep);
		free((char *)sep);
	}
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
}



static void
reapchild(int signum)
{
	int status;
	int pid;
	struct servtab *sep;

	(void)signum;
	for (;;) {
		pid = wait3(&status, WNOHANG, (struct rusage *)0);
		if (pid <= 0)
			break;
		for (sep = servtab; sep; sep = sep->se_next)
			if (sep->se_wait == pid) {
				if (WIFEXITED(status) && WEXITSTATUS(status))
					syslog(LOG_WARNING,
					    "%s: exit status 0x%x",
					    sep->se_server, WEXITSTATUS(status));
				else if (WIFSIGNALED(status))
					syslog(LOG_WARNING,
					    "%s: exit signal 0x%x",
					    sep->se_server, WTERMSIG(status));
				sep->se_wait = 1;
				FD_SET(sep->se_fd, &allsock);
				nsock++;
			}
	}
}

static void
retry(int signum)
{
	struct servtab *sep;

	(void)signum;
	timingout = 0;
	for (sep = servtab; sep; sep = sep->se_next) {
		if (sep->se_fd == -1) {
			switch (sep->se_family) {
			case AF_UNIX:
			case AF_INET:
				setup(sep);
				break;
			}
		}
	}
}

static void
goaway(int signum)
{
	struct servtab *sep;

	(void)signum;
	for (sep = servtab; sep; sep = sep->se_next)
		if (sep->se_fd != -1 && sep->se_family == AF_UNIX)
			(void)unlink(sep->se_service);
	(void)unlink(_PATH_INETDPID);
	exit(0);
}



extern int
inetd_main(int argc, char *argv[])
{
	struct servtab *sep;
	struct passwd *pwd;
	struct group *grp = NULL;
	struct sigaction sa;
	int pid;
	unsigned long opt;
	char *sq;
	gid_t gid;

#ifdef INETD_UNSUPPORT_BILTIN
# define dofork 1
#else
	int dofork;
	extern char **environ;
#endif

	gid = getgid();
	setgroups(1, &gid);

#ifndef INETD_UNSUPPORT_BILTIN
	Argv = argv;
	if (environ == 0 || *environ == 0)
		environ = argv;
	while (*environ)
		environ++;
	LastArg = environ[-1] + strlen(environ[-1]);
#endif

#if defined(__uClinux__)
	opt = bb_getopt_ulflags(argc, argv, "q:f", &sq);
	if (!(opt & 2)) {
	    daemon(0, 0);
	    /* reexec for vfork() do continue parent */
	    vfork_daemon_rexec(argc, argv, "-f");
	}
#else
	opt = bb_getopt_ulflags(argc, argv, "q:", &sq);
	daemon(0, 0);
#endif /* uClinux */

	if(opt & 1) {
			global_queuelen = atoi(optarg);
			if (global_queuelen < 8) global_queuelen=8;
		}
	argc -= optind;
	argv += optind;

	if (argc > 0)
		CONFIG = argv[0];

	openlog(bb_applet_name, LOG_PID | LOG_NOWAIT, LOG_DAEMON);
	{
		FILE *fp;

		if ((fp = fopen(_PATH_INETDPID, "w")) != NULL) {
			fprintf(fp, "%u\n", getpid());
			(void)fclose(fp);
		}
	}

#ifdef RLIMIT_NOFILE
	if (getrlimit(RLIMIT_NOFILE, &rlim_ofile) < 0) {
		syslog(LOG_ERR, "getrlimit: %m");
	} else {
		rlim_ofile_cur = rlim_ofile.rlim_cur;
		if (rlim_ofile_cur == RLIM_INFINITY)    /* ! */
			rlim_ofile_cur = OPEN_MAX;
	}
#endif

	config(0);

	sigemptyset(&emptymask);
	sigemptyset(&blockmask);
	sigaddset(&blockmask, SIGCHLD);
	sigaddset(&blockmask, SIGHUP);
	sigaddset(&blockmask, SIGALRM);

	memset(&sa, 0, sizeof(sa));
	sa.sa_mask = blockmask;
	sa.sa_handler = retry;
	sigaction(SIGALRM, &sa, NULL);
	sa.sa_handler = config;
	sigaction(SIGHUP, &sa, NULL);
	sa.sa_handler = reapchild;
	sigaction(SIGCHLD, &sa, NULL);
	sa.sa_handler = goaway;
	sigaction(SIGTERM, &sa, NULL);
	sa.sa_handler = goaway;
	sigaction(SIGINT, &sa,  NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &sa, NULL);

	{
		/* space for daemons to overwrite environment for ps */
#define DUMMYSIZE       100
		char dummy[DUMMYSIZE];

		(void)memset(dummy, 'x', DUMMYSIZE - 1);
		dummy[DUMMYSIZE - 1] = '\0';

		(void)setenv("inetd_dummy", dummy, 1);
	}

	for (;;) {
	    int n, ctrl;
	    fd_set readable;

	    if (nsock == 0) {
		sigprocmask(SIG_BLOCK, &blockmask, NULL);
		while (nsock == 0)
		    sigsuspend(&emptymask);
		sigprocmask(SIG_SETMASK, &emptymask, NULL);
	    }
	    readable = allsock;
	    if ((n = select(maxsock + 1, &readable, (fd_set *)0,
		(fd_set *)0, (struct timeval *)0)) <= 0) {
		    if (n < 0 && errno != EINTR)
			syslog(LOG_WARNING, "select: %m");
		    sleep(1);
		    continue;
	    }
	    for (sep = servtab; n && sep; sep = sep->se_next)
	    if (sep->se_fd != -1 && FD_ISSET(sep->se_fd, &readable)) {
		n--;
		if (!sep->se_wait && sep->se_socktype == SOCK_STREAM) {
			/* Fixed AGC */
			fcntl(sep->se_fd, F_SETFL, O_NDELAY);
			/* --------- */
			ctrl = accept(sep->se_fd, NULL, NULL);
			fcntl(sep->se_fd, F_SETFL, 0);
			if (ctrl < 0) {
				if (errno == EINTR || errno == EWOULDBLOCK)
					continue;
				syslog(LOG_WARNING, "accept (for %s): %m",
					sep->se_service);
				continue;
			}
		} else
			ctrl = sep->se_fd;
		sigprocmask(SIG_BLOCK, &blockmask, NULL);
		pid = 0;
#ifndef INETD_UNSUPPORT_BILTIN
		dofork = (sep->se_bi == 0 || sep->se_bi->bi_fork);
#endif
		if (dofork) {
			if (sep->se_count++ == 0)
			    (void)gettimeofday(&sep->se_time,
				(struct timezone *)0);
			else if (sep->se_count >= sep->se_max) {
				struct timeval now;

				(void)gettimeofday(&now, (struct timezone *)0);
				if (now.tv_sec - sep->se_time.tv_sec >
				    CNT_INTVL) {
					sep->se_time = now;
					sep->se_count = 1;
				} else {
					syslog(LOG_ERR,
			"%s/%s server failing (looping), service terminated",
					    sep->se_service, sep->se_proto);
					FD_CLR(sep->se_fd, &allsock);
					(void) close(sep->se_fd);
					sep->se_fd = -1;
					sep->se_count = 0;
					nsock--;
					sigprocmask(SIG_SETMASK, &emptymask,
						    NULL);
					if (!timingout) {
						timingout = 1;
						alarm(RETRYTIME);
					}
					continue;
				}
			}
			pid = fork();
		}
		if (pid < 0) {
			syslog(LOG_ERR, "fork: %m");
			if (sep->se_socktype == SOCK_STREAM)
				close(ctrl);
			sigprocmask(SIG_SETMASK, &emptymask, NULL);
			sleep(1);
			continue;
		}
		if (pid && sep->se_wait) {
			sep->se_wait = pid;
			FD_CLR(sep->se_fd, &allsock);
			nsock--;
		}
		sigprocmask(SIG_SETMASK, &emptymask, NULL);
		if (pid == 0) {
#ifndef INETD_UNSUPPORT_BILTIN
			if (sep->se_bi)
				(*sep->se_bi->bi_fn)(ctrl, sep);
			else
#endif
			      {
				if ((pwd = getpwnam(sep->se_user)) == NULL) {
					syslog_err_and_discard_dg(
						sep->se_socktype,
						"getpwnam: %s: No such user",
						sep->se_user);
				}
				if (sep->se_group &&
				    (grp = getgrnam(sep->se_group)) == NULL) {
					syslog_err_and_discard_dg(
						sep->se_socktype,
						"getgrnam: %s: No such group",
						sep->se_group);
				}
				/*
				 * Ok. There are four cases here:
				 *   1. nonroot user, no group specified
				 *   2. nonroot user, some group specified
				 *   3. root user, no group specified
				 *   4. root user, some group specified
				 * In cases 2 and 4 we setgid to the specified
				 * group. In cases 1 and 2 we run initgroups
				 * to run with the groups of the given user.
				 * In case 4 we do setgroups to run with the
				 * given group. In case 3 we do nothing.
				 */
				if (pwd->pw_uid) {
					if (sep->se_group)
						pwd->pw_gid = grp->gr_gid;
					setgid((gid_t)pwd->pw_gid);
					initgroups(pwd->pw_name, pwd->pw_gid);
					setuid((uid_t)pwd->pw_uid);
				} else if (sep->se_group) {
					setgid((gid_t)grp->gr_gid);
					setgroups(1, &grp->gr_gid);
				}
				dup2(ctrl, 0);
				close(ctrl);
				dup2(0, 1);
				dup2(0, 2);
#ifdef RLIMIT_NOFILE
				if (rlim_ofile.rlim_cur != rlim_ofile_cur) {
					if (setrlimit(RLIMIT_NOFILE,
							&rlim_ofile) < 0)
						syslog(LOG_ERR,"setrlimit: %m");
				}
#endif
				for (ctrl = rlim_ofile_cur-1; --ctrl > 2; )
					(void)close(ctrl);

				memset(&sa, 0, sizeof(sa));
				sa.sa_handler = SIG_DFL;
				sigaction(SIGPIPE, &sa, NULL);

				execv(sep->se_server, sep->se_argv);
				syslog_err_and_discard_dg(sep->se_socktype,
					"execv %s: %m", sep->se_server);
			}
		}
		if (!sep->se_wait && sep->se_socktype == SOCK_STREAM)
			close(ctrl);
	    }
	}
}


/*
 * Internet services provided internally by inetd:
 */
#define BUFSIZE 4096

#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_ECHO
/* Echo service -- echo data back */
static void
echo_stream(int s, struct servtab *sep)
{
	char buffer[BUFSIZE];
	int i;

	setproctitle(sep->se_service, s);
	while ((i = read(s, buffer, sizeof(buffer))) > 0 &&
	    write(s, buffer, i) > 0)
		;
	exit(0);
}

/* Echo service -- echo data back */
static void
echo_dg(int s, struct servtab *sep)
{
	char buffer[BUFSIZE];
	int i;
	size_t size;
	struct sockaddr sa;

	(void)sep;

	size = sizeof(sa);
	if ((i = recvfrom(s, buffer, sizeof(buffer), 0, &sa, &size)) < 0)
		return;
	(void) sendto(s, buffer, i, 0, &sa, sizeof(sa));
}
#endif /* CONFIG_FEATURE_INETD_SUPPORT_BILTIN_ECHO */


#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_DISCARD
/* Discard service -- ignore data */
static void
discard_stream(int s, struct servtab *sep)
{
	char buffer[BUFSIZE];

	setproctitle(sep->se_service, s);
	while ((errno = 0, read(s, buffer, sizeof(buffer)) > 0) ||
			errno == EINTR)
		;
	exit(0);
}

/* Discard service -- ignore data */
static void
discard_dg(int s, struct servtab *sep)
{
	char buffer[BUFSIZE];
	(void)sep;
	read(s, buffer, sizeof(buffer));
}
#endif  /* CONFIG_FEATURE_INETD_SUPPORT_BILTIN_DISCARD */


#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_CHARGEN
#include <ctype.h>
#define LINESIZ 72
static char ring[128];
static char *endring;

static void
initring(void)
{
	int i;

	endring = ring;

	for (i = 0; i <= 128; ++i)
		if (isprint(i))
			*endring++ = i;
}

/* Character generator */
static void
chargen_stream(int s, struct servtab *sep)
{
	char *rs;
	int len;
	char text[LINESIZ+2];

	setproctitle(sep->se_service, s);

	if (!endring) {
		initring();
		rs = ring;
	}

	text[LINESIZ] = '\r';
	text[LINESIZ + 1] = '\n';
	for (rs = ring;;) {
		if ((len = endring - rs) >= LINESIZ)
			BCOPY(rs, text, LINESIZ);
		else {
			BCOPY(rs, text, len);
			BCOPY(ring, text + len, LINESIZ - len);
		}
		if (++rs == endring)
			rs = ring;
		if (write(s, text, sizeof(text)) != sizeof(text))
			break;
	}
	exit(0);
}

/* Character generator */
static void
chargen_dg(int s, struct servtab *sep)
{
	struct sockaddr sa;
	static char *rs;
	size_t len, size;
	char text[LINESIZ+2];

	(void)sep;

	if (endring == 0) {
		initring();
		rs = ring;
	}

	size = sizeof(sa);
	if (recvfrom(s, text, sizeof(text), 0, &sa, &size) < 0)
		return;

	if ((len = endring - rs) >= LINESIZ)
		BCOPY(rs, text, LINESIZ);
	else {
		BCOPY(rs, text, len);
		BCOPY(ring, text + len, LINESIZ - len);
	}
	if (++rs == endring)
		rs = ring;
	text[LINESIZ] = '\r';
	text[LINESIZ + 1] = '\n';
	(void) sendto(s, text, sizeof(text), 0, &sa, sizeof(sa));
}
#endif /* CONFIG_FEATURE_INETD_SUPPORT_BILTIN_CHARGEN */


#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_TIME
/*
 * Return a machine readable date and time, in the form of the
 * number of seconds since midnight, Jan 1, 1900.  Since gettimeofday
 * returns the number of seconds since midnight, Jan 1, 1970,
 * we must add 2208988800 seconds to this figure to make up for
 * some seventy years Bell Labs was asleep.
 */

static long
machtime(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, (struct timezone *)0) < 0) {
		fprintf(stderr, "Unable to get time of day\n");
		return (0L);
	}
	return (htonl((long)tv.tv_sec + 2208988800UL));
}

static void
machtime_stream(int s, struct servtab *sep)
{
	long result;
	(void)sep;

	result = machtime();
	write(s, (char *) &result, sizeof(result));
}

static void
machtime_dg(int s, struct servtab *sep)
{
	long result;
	struct sockaddr sa;
	size_t size;
	(void)sep;

	size = sizeof(sa);
	if (recvfrom(s, (char *)&result, sizeof(result), 0, &sa, &size) < 0)
		return;
	result = machtime();
	(void) sendto(s, (char *) &result, sizeof(result), 0, &sa, sizeof(sa));
}
#endif /* CONFIG_FEATURE_INETD_SUPPORT_BILTIN_TIME */


#ifdef CONFIG_FEATURE_INETD_SUPPORT_BILTIN_DAYTIME
/* Return human-readable time of day */
static int
human_readable_time_sprintf(char *buffer)
{
	time_t clocc = time(NULL);

	return sprintf(buffer, "%.24s\r\n", ctime(&clocc));
}

static void
daytime_stream(int s, struct servtab *sep)
{
	char buffer[256];
	size_t st = human_readable_time_sprintf(buffer);

	(void)sep;

	write(s, buffer, st);
}

/* Return human-readable time of day */
static void
daytime_dg(int s, struct servtab *sep)
{
	char buffer[256];
	struct sockaddr sa;
	size_t size;

	(void)sep;

	size = sizeof(sa);
	if (recvfrom(s, buffer, sizeof(buffer), 0, &sa, &size) < 0)
		return;
	size = human_readable_time_sprintf(buffer);
	sendto(s, buffer, size, 0, &sa, sizeof(sa));
}
#endif /* CONFIG_FEATURE_INETD_SUPPORT_BILTIN_DAYTIME */
