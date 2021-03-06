/*  Copyright (C) 2002     Manuel Novoa III
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*  ATTENTION!   ATTENTION!   ATTENTION!   ATTENTION!   ATTENTION!
 *
 *  Besides uClibc, I'm using this code in my libc for elks, which is
 *  a 16-bit environment with a fairly limited compiler.  It would make
 *  things much easier for me if this file isn't modified unnecessarily.
 *  In particular, please put any new or replacement functions somewhere
 *  else, and modify the makefile to use your version instead.
 *  Thanks.  Manuel
 *
 *  ATTENTION!   ATTENTION!   ATTENTION!   ATTENTION!   ATTENTION! */

#define _STDIO_UTILITY
#define _GNU_SOURCE
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

#ifdef WANT_WIDE
#include <wchar.h>
#include <wctype.h>
#include <locale.h>

#define Wvoid			wchar_t
#define Wchar			wchar_t
#define Wuchar			__uwchar_t
#define Wint			wchar_t

#else

#define Wvoid			void
#define Wchar			char
typedef unsigned char	__string_uchar_t;
#define Wuchar			__string_uchar_t
#define Wint			int

#endif

/**********************************************************************/
/* NOTE: If we ever do internationalized syserr messages, this will
 * have to be changed! */

#define _SYS_NERR			125
#if defined(__mips__) || defined(__sparc__)
/* sparce and mips have an extra error entry, as EDEADLK and EDEADLOCK have
 * different meanings on those platforms. */
#undef _SYS_NERR
#define _SYS_NERR			126
#endif

#define _SYS_ERRMSG_MAXLEN	 50

extern const char _string_syserrmsgs[];

#define _SYS_NSIG			32
#define _SYS_SIGMSG_MAXLEN	25

extern const char _string_syssigmsgs[];


#if _SYS_ERRMSG_MAXLEN < __UIM_BUFLEN_INT + 14
#define _STRERROR_BUFSIZE (__UIM_BUFLEN_INT + 14)
#else
#define _STRERROR_BUFSIZE _SYS_ERRMSG_MAXLEN
#endif

#if _SYS_SIGMSG_MAXLEN < __UIM_BUFLEN_INT + 14
#define _STRSIGNAL_BUFSIZE (__UIM_BUFLEN_INT + 14)
#else
#define _STRSIGNAL_BUFSIZE _SYS_SIGMSG_MAXLEN
#endif

/**********************************************************************/
#ifdef L__string_syserrmsgs

const char _string_syserrmsgs[] = {
	/*   0:    0,  8 */ "Success\0"
	/*   1:    8, 24 */ "Operation not permitted\0"
	/*   2:   32, 26 */ "No such file or directory\0"
	/*   3:   58, 16 */ "No such process\0"
	/*   4:   74, 24 */ "Interrupted system call\0"
	/*   5:   98, 19 */ "Input/output error\0"
	/*   6:  117, 26 */ "No such device or address\0"
	/*   7:  143, 23 */ "Argument list too long\0"
	/*   8:  166, 18 */ "Exec format error\0"
	/*   9:  184, 20 */ "Bad file descriptor\0"
	/*  10:  204, 19 */ "No child processes\0"
	/*  11:  223, 33 */ "Resource temporarily unavailable\0"
	/*  12:  256, 23 */ "Cannot allocate memory\0"
	/*  13:  279, 18 */ "Permission denied\0"
	/*  14:  297, 12 */ "Bad address\0"
	/*  15:  309, 22 */ "Block device required\0"
	/*  16:  331, 24 */ "Device or resource busy\0"
	/*  17:  355, 12 */ "File exists\0"
	/*  18:  367, 26 */ "Invalid cross-device link\0"
	/*  19:  393, 15 */ "No such device\0"
	/*  20:  408, 16 */ "Not a directory\0"
	/*  21:  424, 15 */ "Is a directory\0"
	/*  22:  439, 17 */ "Invalid argument\0"
	/*  23:  456, 30 */ "Too many open files in system\0"
	/*  24:  486, 20 */ "Too many open files\0"
	/*  25:  506, 31 */ "Inappropriate ioctl for device\0"
	/*  26:  537, 15 */ "Text file busy\0"
	/*  27:  552, 15 */ "File too large\0"
	/*  28:  567, 24 */ "No space left on device\0"
	/*  29:  591, 13 */ "Illegal seek\0"
	/*  30:  604, 22 */ "Read-only file system\0"
	/*  31:  626, 15 */ "Too many links\0"
	/*  32:  641, 12 */ "Broken pipe\0"
	/*  33:  653, 33 */ "Numerical argument out of domain\0"
	/*  34:  686, 30 */ "Numerical result out of range\0"
	/*  35:  716, 26 */ "Resource deadlock avoided\0"
	/*  36:  742, 19 */ "File name too long\0"
	/*  37:  761, 19 */ "No locks available\0"
	/*  38:  780, 25 */ "Function not implemented\0"
	/*  39:  805, 20 */ "Directory not empty\0"
	/*  40:  825, 34 */ "Too many levels of symbolic links\0"
	/*  41:  859,  1 */ "\0"
	/*  42:  860, 27 */ "No message of desired type\0"
	/*  43:  887, 19 */ "Identifier removed\0"
	/*  44:  906, 28 */ "Channel number out of range\0"
	/*  45:  934, 25 */ "Level 2 not synchronized\0"
	/*  46:  959, 15 */ "Level 3 halted\0"
	/*  47:  974, 14 */ "Level 3 reset\0"
	/*  48:  988, 25 */ "Link number out of range\0"
	/*  49: 1013, 29 */ "Protocol driver not attached\0"
	/*  50: 1042, 27 */ "No CSI structure available\0"
	/*  51: 1069, 15 */ "Level 2 halted\0"
	/*  52: 1084, 17 */ "Invalid exchange\0"
	/*  53: 1101, 27 */ "Invalid request descriptor\0"
	/*  54: 1128, 14 */ "Exchange full\0"
	/*  55: 1142,  9 */ "No anode\0"
	/*  56: 1151, 21 */ "Invalid request code\0"
	/*  57: 1172, 13 */ "Invalid slot\0"
	/*  58: 1185,  1 */ "\0"
	/*  59: 1186, 21 */ "Bad font file format\0"
	/*  60: 1207, 20 */ "Device not a stream\0"
	/*  61: 1227, 18 */ "No data available\0"
	/*  62: 1245, 14 */ "Timer expired\0"
	/*  63: 1259, 25 */ "Out of streams resources\0"
	/*  64: 1284, 30 */ "Machine is not on the network\0"
	/*  65: 1314, 22 */ "Package not installed\0"
	/*  66: 1336, 17 */ "Object is remote\0"
	/*  67: 1353, 22 */ "Link has been severed\0"
	/*  68: 1375, 16 */ "Advertise error\0"
	/*  69: 1391, 14 */ "Srmount error\0"
	/*  70: 1405, 28 */ "Communication error on send\0"
	/*  71: 1433, 15 */ "Protocol error\0"
	/*  72: 1448, 19 */ "Multihop attempted\0"
	/*  73: 1467, 19 */ "RFS specific error\0"
	/*  74: 1486, 12 */ "Bad message\0"
	/*  75: 1498, 38 */ "Value too large for defined data type\0"
	/*  76: 1536, 27 */ "Name not unique on network\0"
	/*  77: 1563, 29 */ "File descriptor in bad state\0"
	/*  78: 1592, 23 */ "Remote address changed\0"
	/*  79: 1615, 39 */ "Can not access a needed shared library\0"
	/*  80: 1654, 37 */ "Accessing a corrupted shared library\0"
	/*  81: 1691, 32 */ ".lib section in a.out corrupted\0"
	/*  82: 1723, 48 */ "Attempting to link in too many shared libraries\0"
	/*  83: 1771, 38 */ "Cannot exec a shared library directly\0"
	/*  84: 1809, 50 */ "Invalid or incomplete multibyte or wide character\0"
	/*  85: 1859, 44 */ "Interrupted system call should be restarted\0"
	/*  86: 1903, 19 */ "Streams pipe error\0"
	/*  87: 1922, 15 */ "Too many users\0"
	/*  88: 1937, 31 */ "Socket operation on non-socket\0"
	/*  89: 1968, 29 */ "Destination address required\0"
	/*  90: 1997, 17 */ "Message too long\0"
	/*  91: 2014, 31 */ "Protocol wrong type for socket\0"
	/*  92: 2045, 23 */ "Protocol not available\0"
	/*  93: 2068, 23 */ "Protocol not supported\0"
	/*  94: 2091, 26 */ "Socket type not supported\0"
	/*  95: 2117, 24 */ "Operation not supported\0"
	/*  96: 2141, 30 */ "Protocol family not supported\0"
	/*  97: 2171, 41 */ "Address family not supported by protocol\0"
	/*  98: 2212, 23 */ "Address already in use\0"
	/*  99: 2235, 32 */ "Cannot assign requested address\0"
	/* 100: 2267, 16 */ "Network is down\0"
	/* 101: 2283, 23 */ "Network is unreachable\0"
	/* 102: 2306, 36 */ "Network dropped connection on reset\0"
	/* 103: 2342, 33 */ "Software caused connection abort\0"
	/* 104: 2375, 25 */ "Connection reset by peer\0"
	/* 105: 2400, 26 */ "No buffer space available\0"
	/* 106: 2426, 40 */ "Transport endpoint is already connected\0"
	/* 107: 2466, 36 */ "Transport endpoint is not connected\0"
	/* 108: 2502, 46 */ "Cannot send after transport endpoint shutdown\0"
	/* 109: 2548, 35 */ "Too many references: cannot splice\0"
	/* 110: 2583, 21 */ "Connection timed out\0"
	/* 111: 2604, 19 */ "Connection refused\0"
	/* 112: 2623, 13 */ "Host is down\0"
	/* 113: 2636, 17 */ "No route to host\0"
	/* 114: 2653, 30 */ "Operation already in progress\0"
	/* 115: 2683, 26 */ "Operation now in progress\0"
	/* 116: 2709, 22 */ "Stale NFS file handle\0"
	/* 117: 2731, 25 */ "Structure needs cleaning\0"
	/* 118: 2756, 28 */ "Not a XENIX named type file\0"
	/* 119: 2784, 30 */ "No XENIX semaphores available\0"
	/* 120: 2814, 21 */ "Is a named type file\0"
	/* 121: 2835, 17 */ "Remote I/O error\0"
	/* 122: 2852, 20 */ "Disk quota exceeded\0"
	/* 123: 2872, 16 */ "No medium found\0"
	/* 124: 2888, 18 */ "Wrong medium type"
#if defined(__mips__) || defined(__sparc__)
	"\0"
	/* 125: 2906, 28 */ "File locking deadlock error"
#endif
	/* Note: for mips we are ignoring ECANCELED since glibc doesn't have a
	 * corresponsding message.*/
};

#endif
/**********************************************************************/
#ifdef L_sys_errlist

link_warning(_sys_errlist, "sys_nerr and sys_errlist are obsolete and uClibc support for them (in at least some configurations) will probably be unavailable in the near future.")

const char *const sys_errlist[] = {
	[0] =				_string_syserrmsgs + 0,
	[EPERM] =			_string_syserrmsgs + 8,
	[ENOENT] =			_string_syserrmsgs + 32,
	[ESRCH] =			_string_syserrmsgs + 58,
	[EINTR] =			_string_syserrmsgs + 74,
	[EIO] =				_string_syserrmsgs + 98,
	[ENXIO] =			_string_syserrmsgs + 117,
	[E2BIG] =			_string_syserrmsgs + 143,
	[ENOEXEC] =			_string_syserrmsgs + 166,
	[EBADF] =			_string_syserrmsgs + 184,
	[ECHILD] =			_string_syserrmsgs + 204,
	[EAGAIN] =			_string_syserrmsgs + 223,
	[ENOMEM] =			_string_syserrmsgs + 256,
	[EACCES] =			_string_syserrmsgs + 279,
	[EFAULT] =			_string_syserrmsgs + 297,
	[ENOTBLK] =			_string_syserrmsgs + 309,
	[EBUSY] =			_string_syserrmsgs + 331,
	[EEXIST] =			_string_syserrmsgs + 355,
	[EXDEV] =			_string_syserrmsgs + 367,
	[ENODEV] =			_string_syserrmsgs + 393,
	[ENOTDIR] =			_string_syserrmsgs + 408,
	[EISDIR] =			_string_syserrmsgs + 424,
	[EINVAL] =			_string_syserrmsgs + 439,
	[ENFILE] =			_string_syserrmsgs + 456,
	[EMFILE] =			_string_syserrmsgs + 486,
	[ENOTTY] =			_string_syserrmsgs + 506,
	[ETXTBSY] =			_string_syserrmsgs + 537,
	[EFBIG] =			_string_syserrmsgs + 552,
	[ENOSPC] =			_string_syserrmsgs + 567,
	[ESPIPE] =			_string_syserrmsgs + 591,
	[EROFS] =			_string_syserrmsgs + 604,
	[EMLINK] =			_string_syserrmsgs + 626,
	[EPIPE] =			_string_syserrmsgs + 641,
	[EDOM] =			_string_syserrmsgs + 653,
	[ERANGE] =			_string_syserrmsgs + 686,
	[EDEADLK] =			_string_syserrmsgs + 716,
	[ENAMETOOLONG] =	_string_syserrmsgs + 742,
	[ENOLCK] =			_string_syserrmsgs + 761,
	[ENOSYS] =			_string_syserrmsgs + 780,
	[ENOTEMPTY] =		_string_syserrmsgs + 805,
	[ELOOP] =			_string_syserrmsgs + 825,
	/*  	_string_syserrmsgs + 859, */
	[ENOMSG] =			_string_syserrmsgs + 860,
	[EIDRM] =			_string_syserrmsgs + 887,
	[ECHRNG] =			_string_syserrmsgs + 906,
	[EL2NSYNC] =		_string_syserrmsgs + 934,
	[EL3HLT] =			_string_syserrmsgs + 959,
	[EL3RST] =			_string_syserrmsgs + 974,
	[ELNRNG] =			_string_syserrmsgs + 988,
	[EUNATCH] =			_string_syserrmsgs + 1013,
	[ENOCSI] =			_string_syserrmsgs + 1042,
	[EL2HLT] =			_string_syserrmsgs + 1069,
	[EBADE] =			_string_syserrmsgs + 1084,
	[EBADR] =			_string_syserrmsgs + 1101,
	[EXFULL] =			_string_syserrmsgs + 1128,
	[ENOANO] =			_string_syserrmsgs + 1142,
	[EBADRQC] =			_string_syserrmsgs + 1151,
	[EBADSLT] =			_string_syserrmsgs + 1172,
	/*  	_string_syserrmsgs + 1185, */
	[EBFONT] =			_string_syserrmsgs + 1186,
	[ENOSTR] =			_string_syserrmsgs + 1207,
	[ENODATA] =			_string_syserrmsgs + 1227,
	[ETIME] =			_string_syserrmsgs + 1245,
	[ENOSR] =			_string_syserrmsgs + 1259,
	[ENONET] =			_string_syserrmsgs + 1284,
	[ENOPKG] =			_string_syserrmsgs + 1314,
	[EREMOTE] =			_string_syserrmsgs + 1336,
	[ENOLINK] =			_string_syserrmsgs + 1353,
	[EADV] =			_string_syserrmsgs + 1375,
	[ESRMNT] =			_string_syserrmsgs + 1391,
	[ECOMM] =			_string_syserrmsgs + 1405,
	[EPROTO] =			_string_syserrmsgs + 1433,
	[EMULTIHOP] =		_string_syserrmsgs + 1448,
	[EDOTDOT] =			_string_syserrmsgs + 1467,
	[EBADMSG] =			_string_syserrmsgs + 1486,
	[EOVERFLOW] =		_string_syserrmsgs + 1498,
	[ENOTUNIQ] =		_string_syserrmsgs + 1536,
	[EBADFD] =			_string_syserrmsgs + 1563,
	[EREMCHG] =			_string_syserrmsgs + 1592,
	[ELIBACC] =			_string_syserrmsgs + 1615,
	[ELIBBAD] =			_string_syserrmsgs + 1654,
	[ELIBSCN] =			_string_syserrmsgs + 1691,
	[ELIBMAX] =			_string_syserrmsgs + 1723,
	[ELIBEXEC] =		_string_syserrmsgs + 1771,
	[EILSEQ] =			_string_syserrmsgs + 1809,
	[ERESTART] =		_string_syserrmsgs + 1859,
	[ESTRPIPE] =		_string_syserrmsgs + 1903,
	[EUSERS] =			_string_syserrmsgs + 1922,
	[ENOTSOCK] =		_string_syserrmsgs + 1937,
	[EDESTADDRREQ] =	_string_syserrmsgs + 1968,
	[EMSGSIZE] =		_string_syserrmsgs + 1997,
	[EPROTOTYPE] =		_string_syserrmsgs + 2014,
	[ENOPROTOOPT] =		_string_syserrmsgs + 2045,
	[EPROTONOSUPPORT] =	_string_syserrmsgs + 2068,
	[ESOCKTNOSUPPORT] =	_string_syserrmsgs + 2091,
	[EOPNOTSUPP] =		_string_syserrmsgs + 2117,
	[EPFNOSUPPORT] =	_string_syserrmsgs + 2141,
	[EAFNOSUPPORT] =	_string_syserrmsgs + 2171,
	[EADDRINUSE] =		_string_syserrmsgs + 2212,
	[EADDRNOTAVAIL] =	_string_syserrmsgs + 2235,
	[ENETDOWN] =		_string_syserrmsgs + 2267,
	[ENETUNREACH] =		_string_syserrmsgs + 2283,
	[ENETRESET] =		_string_syserrmsgs + 2306,
	[ECONNABORTED] =	_string_syserrmsgs + 2342,
	[ECONNRESET] =		_string_syserrmsgs + 2375,
	[ENOBUFS] =			_string_syserrmsgs + 2400,
	[EISCONN] =			_string_syserrmsgs + 2426,
	[ENOTCONN] =		_string_syserrmsgs + 2466,
	[ESHUTDOWN] =		_string_syserrmsgs + 2502,
	[ETOOMANYREFS] =	_string_syserrmsgs + 2548,
	[ETIMEDOUT] =		_string_syserrmsgs + 2583,
	[ECONNREFUSED] =	_string_syserrmsgs + 2604,
	[EHOSTDOWN] =		_string_syserrmsgs + 2623,
	[EHOSTUNREACH] =	_string_syserrmsgs + 2636,
	[EALREADY] =		_string_syserrmsgs + 2653,
	[EINPROGRESS] =		_string_syserrmsgs + 2683,
	[ESTALE] =			_string_syserrmsgs + 2709,
	[EUCLEAN] =			_string_syserrmsgs + 2731,
	[ENOTNAM] =			_string_syserrmsgs + 2756,
	[ENAVAIL] =			_string_syserrmsgs + 2784,
	[EISNAM] =			_string_syserrmsgs + 2814,
	[EREMOTEIO] =		_string_syserrmsgs + 2835,
	[EDQUOT] =			_string_syserrmsgs + 2852,
	[ENOMEDIUM] =		_string_syserrmsgs + 2872,
	[EMEDIUMTYPE] =		_string_syserrmsgs + 2888,

#if EDEADLOCK != EDEADLK
	[EDEADLOCK] =		_string_syserrmsgs + 2906,
#endif

#if EWOULDBLOCK != EAGAIN
#warning EWOULDBLOCK does not equal EAGAIN
#endif

	/* For now, ignore the other arch-specific errors.  glibc only maps EPROCLIM. */

	/* some other mips errors */
#ifdef ECANCELED
#endif
#ifdef EINIT
#endif
#ifdef EREMDEV
#endif

	/* some other sparc errors */
#ifdef EPROCLIM
#endif
#ifdef ERREMOTE
#endif
};

int sys_nerr = sizeof(sys_errlist)/sizeof(sys_errlist[0]);

#endif
/**********************************************************************/
#ifdef L_wmemcpy
#define L_memcpy
#define Wmemcpy wmemcpy
#else
#define Wmemcpy memcpy
#endif

#ifdef L_memcpy

Wvoid *Wmemcpy(Wvoid * __restrict s1, const Wvoid * __restrict s2, size_t n)
{
	register Wchar *r1 = s1;
	register const Wchar *r2 = s2;

#ifdef __BCC__
	while (n--) {
		*r1++ = *r2++;
	}
#else
	while (n) {
		*r1++ = *r2++;
		--n;
	}
#endif

	return s1;
}

#endif
/**********************************************************************/
#ifdef L_wmemmove
#define L_memmove
#define Wmemmove wmemmove
#else
#define Wmemmove memmove
#endif

#ifdef L_memmove

Wvoid *Wmemmove(Wvoid *s1, const Wvoid *s2, size_t n)
{
#ifdef __BCC__
	register Wchar *s = (Wchar *) s1;
	register const Wchar *p = (const Wchar *) s2;

	if (p >= s) {
		while (n--) {
			*s++ = *p++;
		}
	} else {
		s += n;
		p += n;
		while (n--) {
			*--s = *--p;
		}
	}

	return s1;
#else
	register Wchar *s = (Wchar *) s1;
	register const Wchar *p = (const Wchar *) s2;

	if (p >= s) {
		while (n) {
			*s++ = *p++;
			--n;
		}
	} else {
		while (n) {
			--n;
			s[n] = p[n];
		}
	}

	return s1;
#endif
}

#endif
/**********************************************************************/
#ifdef L_wcscpy
#define L_strcpy
#define Wstrcpy wcscpy
#else
#define Wstrcpy strcpy
#endif

#ifdef L_strcpy

Wchar *Wstrcpy(Wchar * __restrict s1, const Wchar * __restrict s2)
{
	register Wchar *s = s1;

#ifdef __BCC__
	do {
		*s = *s2++;
	} while (*s++ != 0);
#else
	while ( (*s++ = *s2++) != 0 );
#endif

	return s1;
}

#endif
/**********************************************************************/
#ifdef L_wcsncpy
#define L_strncpy
#define Wstrncpy wcsncpy
#else
#define Wstrncpy strncpy
#endif

#ifdef L_strncpy

Wchar *Wstrncpy(Wchar * __restrict s1, register const Wchar * __restrict s2,
				size_t n)
{
	register Wchar *s = s1;

#ifdef __BCC__
	while (n--) {
		if ((*s = *s2) != 0) s2++; /* Need to fill tail with 0s. */
		++s;
	}
#else
	while (n) {
		if ((*s = *s2) != 0) s2++; /* Need to fill tail with 0s. */
		++s;
		--n;
	}
#endif
	
	return s1;
}

#endif
/**********************************************************************/
#ifdef L_wcscat
#define L_strcat
#define Wstrcat wcscat
#else
#define Wstrcat strcat
#endif

#ifdef L_strcat

Wchar *Wstrcat(Wchar * __restrict s1, register const Wchar * __restrict s2)
{
	register Wchar *s = s1;

	while (*s++);
	--s;
	while ((*s++ = *s2++) != 0);

	return s1;
}

#endif
/**********************************************************************/
#ifdef L_wcsncat
#define L_strncat
#define Wstrncat wcsncat
#else
#define Wstrncat strncat
#endif

#ifdef L_strncat

Wchar *Wstrncat(Wchar * __restrict s1, register const Wchar * __restrict s2,
				size_t n)
{
	register Wchar *s = s1;

	while (*s++);
	--s;
#if __BCC__
	while (n-- && ((*s = *s2++) != 0)) ++s;
#else
	while (n && ((*s = *s2++) != 0)) {
		--n;
		++s;
	}
#endif
	*s = 0;

	return s1;
}

#endif
/**********************************************************************/
#ifdef L_wmemcmp
#define L_memcmp
#define Wmemcmp wmemcmp
#else
#define Wmemcmp memcmp
#endif

#ifdef L_memcmp

#ifndef L_wmemcmp
weak_alias(memcmp,bcmp);
#endif

int Wmemcmp(const Wvoid *s1, const Wvoid *s2, size_t n)
{
	register const Wuchar *r1 = (const Wuchar *) s1;
	register const Wuchar *r2 = (const Wuchar *) s2;

#ifdef WANT_WIDE
	while (n && (*r1 == *r2)) {
		++r1;
		++r2;
		--n;
	}

	return (n == 0) ? 0 : ((*r1 < *r2) ? -1 : 1);
#else
	int r = 0;

	while (n-- && ((r = ((int)(*r1++)) - *r2++) == 0));

	return r;
#endif
}

#endif
/**********************************************************************/
#ifdef L_wcscmp
#define L_strcmp
#define Wstrcmp wcscmp
#else
#define Wstrcmp strcmp
#endif

#ifdef L_strcmp

#ifndef L_wcscmp
#warning implement strcoll and remove weak alias (or enable for C locale only)
weak_alias(strcmp,strcoll);
#endif

int Wstrcmp(register const Wchar *s1, register const Wchar *s2)
{
#ifdef WANT_WIDE
	while (*((Wuchar *)s1) == *((Wuchar *)s2)) {
		if (!*s1++) {
			return 0;
		}
		++s2;
	}

	return (*((Wuchar *)s1) < *((Wuchar *)s2)) ? -1 : 1;
#else
	int r;

	while (((r = ((int)(*((Wuchar *)s1))) - *((Wuchar *)s2++))
			== 0) && *s1++);
	
	return r;
#endif
}
#endif
/**********************************************************************/
#ifdef L_strcoll
#error implement strcoll and remove weak_alias!!

#if 0
extern unsigned char *_ctype_collate;
int strcoll(register const char *s1, const char *s2)
{
    int r;

    while (!(r = (_ctype_collate[(int)(*s1++)]-_ctype_collate[(int)(*s2++)])));

    return r;
}
#endif

#endif
/**********************************************************************/
#ifdef L_wcsncmp
#define L_strncmp
#define Wstrncmp wcsncmp
#else
#define Wstrncmp strncmp
#endif

#ifdef L_strncmp

int Wstrncmp(register const Wchar *s1, register const Wchar *s2, size_t n)
{
#ifdef WANT_WIDE
	while (n && (*((Wuchar *)s1) == *((Wuchar *)s2))) {
		if (!*s1++) {
			return 0;
		}
		++s2;
		--n;
	}

	return (n == 0) ? 0 : ((*((Wuchar *)s1) < *((Wuchar *)s2)) ? -1 : 1);
#else
	int r = 0;

	while (n--
		   && ((r = ((int)(*((unsigned char *)s1))) - *((unsigned char *)s2++))
			== 0)
		   && *s1++);

	return r;
#endif
}

#endif
/**********************************************************************/
#ifdef L_strxfrm
#error implement strxfrm
/* size_t strxfrm(char *dst, const char *src, size_t len); */
#endif	
/**********************************************************************/
#ifdef L_wmemchr
#define L_memchr
#define Wmemchr wmemchr
#else
#define Wmemchr memchr
#endif

#ifdef L_memchr

Wvoid *Wmemchr(const Wvoid *s, Wint c, size_t n)
{
	register const Wuchar *r = (const Wuchar *) s;
#ifdef __BCC__
	/* bcc can optimize the counter if it thinks it is a pointer... */
	register const char *np = (const char *) n;
#else
#define np n
#endif

	while (np) {
		if (*r == ((Wuchar)c)) {
			return (Wvoid *) r;	/* silence the warning */
		}
		++r;
		--np;
	}

	return NULL;
}
#undef np

#endif
/**********************************************************************/
#ifdef L_wcschr
#define L_strchr
#define Wstrchr wcschr
#else
#define Wstrchr strchr
#endif

#ifdef L_strchr

#ifndef L_wcschr
weak_alias(strchr,index);
#endif

Wchar *Wstrchr(register const Wchar *s, Wint c)
{
	do {
		if (*s == ((Wchar)c)) {
			return (Wchar *) s;	/* silence the warning */
		}
	} while (*s++);

	return NULL;
}

#endif
/**********************************************************************/
#ifdef L_wcscspn
#define L_strcspn
#define Wstrcspn wcscspn
#else
#define Wstrcspn strcspn
#endif

#ifdef L_strcspn

size_t Wstrcspn(const Wchar *s1, const Wchar *s2)
{
	register const Wchar *s;
	register const Wchar *p;

	for ( s=s1 ; *s ; s++ ) {
		for ( p=s2 ; *p ; p++ ) {
			if (*p == *s) goto done;
		}
	}
 done:
	return s - s1;
}

#endif
/**********************************************************************/
#ifdef L_wcspbrk
#define L_strpbrk
#define Wstrpbrk wcspbrk
#else
#define Wstrpbrk strpbrk
#endif

#ifdef L_strpbrk

Wchar *Wstrpbrk(const Wchar *s1, const Wchar *s2)
{
	register const Wchar *s;
	register const Wchar *p;

	for ( s=s1 ; *s ; s++ ) {
		for ( p=s2 ; *p ; p++ ) {
			if (*p == *s) return (Wchar *) s; /* silence the warning */
		}
	}
	return NULL;
}
#endif
/**********************************************************************/
#ifdef L_wcsrchr
#define L_strrchr
#define Wstrrchr wcsrchr
#else
#define Wstrrchr strrchr
#endif

#ifdef L_strrchr

#ifndef L_wcsrchr
weak_alias(strrchr,rindex);
#endif

Wchar *Wstrrchr(register const  Wchar *s, Wint c)
{
	register const Wchar *p;

	p = NULL;
	do {
		if (*s == (Wchar) c) {
			p = s;
		}
	} while (*s++);

	return (Wchar *) p;			/* silence the warning */
}

#endif
/**********************************************************************/
#ifdef L_wcsspn
#define L_strspn
#define Wstrspn wcsspn
#else
#define Wstrspn strspn
#endif

#ifdef L_strspn

size_t Wstrspn(const Wchar *s1, const Wchar *s2)
{
	register const Wchar *s = s1;
	register const Wchar *p = s2;

	while (*p) {
		if (*p++ == *s) {
			++s;
			p = s2;
		}
	}
	return s - s1;
}

#endif
/**********************************************************************/
#ifdef L_wcsstr
#define L_strstr
#define Wstrstr wcsstr
#else
#define Wstrstr strstr
#endif

#ifdef L_strstr

/* NOTE: This is the simple-minded O(len(s1) * len(s2)) worst-case approach. */

#ifdef L_wcsstr
weak_alias(wcsstr,wcswcs);
#endif

Wchar *Wstrstr(const Wchar *s1, const Wchar *s2)
{
	register const Wchar *s = s1;
	register const Wchar *p = s2;

	do {
		if (!*p) {
			return (Wchar *) s1;;
		}
		if (*p == *s) {
			++p;
			++s;
		} else {
			p = s2;
			if (!*s) {
				return NULL;
			}
			s = ++s1;
		}
	} while (1);
}

#endif
/**********************************************************************/
#undef Wstrspn
#undef Wstrpbrk

#ifdef L_wcstok
#define L_strtok_r
#define Wstrtok_r wcstok
#define Wstrspn wcsspn
#define Wstrpbrk wcspbrk
#else
#define Wstrtok_r strtok_r
#define Wstrspn strspn
#define Wstrpbrk strpbrk
#endif

#ifdef L_strtok_r

Wchar *Wstrtok_r(Wchar * __restrict s1, const Wchar * __restrict s2,
				 Wchar ** __restrict next_start)
{
	register Wchar *s;
	register Wchar *p;

#if 1
	if (((s = s1) != NULL) || ((s = *next_start) != NULL)) {
		if (*(s += Wstrspn(s, s2))) {
			if ((p = Wstrpbrk(s, s2)) != NULL) {
				*p++ = 0;
			}
		} else {
			p = s = NULL;
		}
		*next_start = p;
	}
	return s;
#else
	if (!(s = s1)) {
		s = *next_start;
	}
	if (s && *(s += Wstrspn(s, s2))) {
		if (*(p = s + Wstrcspn(s, s2))) {
			*p++ = 0;
		}
		*next_start = p;
		return s;
	}
	return NULL;				/* TODO: set *next_start = NULL for safety? */
#endif
}

#endif
/**********************************************************************/
/*  #ifdef L_wcstok */
/*  #define L_strtok */
/*  #define Wstrtok wcstok */
/*  #define Wstrtok_r wcstok_r */
/*  #else */
/*  #define Wstrtok strtok */
/*  #define Wstrtok_r strtok_r */
/*  #endif */

#ifdef L_strtok
#define Wstrtok strtok
#define Wstrtok_r strtok_r

Wchar *Wstrtok(Wchar * __restrict s1, const Wchar * __restrict s2)
{
	static Wchar *next_start;	/* Initialized to 0 since in bss. */
	return Wstrtok_r(s1, s2, &next_start);
}

#endif
/**********************************************************************/
#ifdef L_wmemset
#define L_memset
#define Wmemset wmemset
#else
#define Wmemset memset
#endif

#ifdef L_memset

Wvoid *Wmemset(Wvoid *s, Wint c, size_t n)
{
	register Wuchar *p = (Wuchar *) s;
#ifdef __BCC__
	/* bcc can optimize the counter if it thinks it is a pointer... */
	register const char *np = (const char *) n;
#else
#define np n
#endif

	while (np) {
		*p++ = (Wuchar) c;
		--np;
	}

	return s;
}
#undef np

#endif
/**********************************************************************/
#ifdef L_wcslen
#define L_strlen
#define Wstrlen wcslen
#else
#define Wstrlen strlen
#endif

#ifdef L_strlen

size_t Wstrlen(const Wchar *s)
{
	register const Wchar *p;

	for (p=s ; *p ; p++);

	return p - s;
}

#endif
/**********************************************************************/
/* ANSI/ISO end here */
/**********************************************************************/
#ifdef L_ffs

int ffs(int i)
{
#if 1
	/* inlined binary search method */
	char n = 1;
#if UINT_MAX == 0xffffU
	/* nothing to do here -- just trying to avoiding possible problems */
#elif UINT_MAX == 0xffffffffU
	if (!(i & 0xffff)) {
		n += 16;
		i >>= 16;
	}
#else
#error ffs needs rewriting!
#endif

	if (!(i & 0xff)) {
		n += 8;
		i >>= 8;
	}
	if (!(i & 0x0f)) {
		n += 4;
		i >>= 4;
	}
	if (!(i & 0x03)) {
		n += 2;
		i >>= 2;
	}
	return (i) ? (n + ((i+1) & 0x01)) : 0;

#else
	/* linear search -- slow, but small */
	int n;

	for (n = 0 ; i ; ++n) {
		i >>= 1;
	}
	
	return n;
#endif
}

#endif
/**********************************************************************/
#ifdef L_wcscasecmp
#define L_strcasecmp
#define Wstrcasecmp wcscasecmp
#else
#define Wstrcasecmp strcasecmp
#endif

#ifdef L_strcasecmp

int Wstrcasecmp(register const Wchar *s1, register const Wchar *s2)
{
#ifdef WANT_WIDE
	while ((*s1 == *s2) || (towlower(*s1) == towlower(*s2))) {
		if (!*s1++) {
			return 0;
		}
		++s2;
	}

	return (((Wuchar)towlower(*s1)) < ((Wuchar)towlower(*s2))) ? -1 : 1;
	/* TODO -- should wide cmp funcs do wchar or Wuchar compares? */
#else
	int r = 0;

	while ( ((s1 == s2) ||
			 !(r = ((int)( tolower(*((Wuchar *)s1))))
			   - tolower(*((Wuchar *)s2))))
			&& (++s2, *s1++));

	return r;
#endif
}

#endif
/**********************************************************************/
#ifdef L_wcsncasecmp
#define L_strncasecmp
#define Wstrncasecmp wcsncasecmp
#else
#define Wstrncasecmp strncasecmp
#endif

#ifdef L_strncasecmp

int Wstrncasecmp(register const Wchar *s1, register const Wchar *s2, size_t n)
{
#ifdef WANT_WIDE
	while (n && ((*s1 == *s2) || (towlower(*s1) == towlower(*s2)))) {
		if (!*s1++) {
			return 0;
		}
		++s2;
		--n;
	}

	return (n == 0)
		? 0
		: ((((Wuchar)towlower(*s1)) < ((Wuchar)towlower(*s2))) ? -1 : 1);
	/* TODO -- should wide cmp funcs do wchar or Wuchar compares? */
#else
	int r = 0;

	while ( n
			&& ((s1 == s2) ||
				!(r = ((int)( tolower(*((unsigned char *)s1))))
				  - tolower(*((unsigned char *)s2))))
			&& (--n, ++s2, *s1++));
	return r;
#endif
}
#endif
/**********************************************************************/
#ifdef L_wcsnlen
#define L_strnlen
#define Wstrnlen wcsnlen
#else
#define Wstrnlen strnlen
#endif

#ifdef L_strnlen

size_t Wstrnlen(const Wchar *s, size_t max)
{
	register const Wchar *p = s;
#ifdef __BCC__
	/* bcc can optimize the counter if it thinks it is a pointer... */
	register const char *maxp = (const char *) max;
#else
#define maxp max
#endif

	while (maxp && *p) {
		++p;
		--maxp;
	}

	return p - s;
}
#undef maxp
#endif
/**********************************************************************/
/* No wide analog. */

#ifdef L_memccpy

void *memccpy(void * __restrict s1, const void * __restrict s2, int c, size_t n)
{
	register char *r1 = s1;
	register const char *r2 = s2;

	while (n-- && (((unsigned char)(*r1++ = *r2++)) != ((unsigned char) c)));

	return (n == (size_t) -1) ? NULL : r1;
}

#endif
/**********************************************************************/
#undef Wstrlen
#undef Wstrcpy

#ifdef L_wcsdup
#define L_strdup
#define Wstrdup wcsdup
#define Wstrlen wcslen
#define Wstrcpy wcscpy
#else
#define Wstrdup strdup
#define Wstrlen strlen
#define Wstrcpy strcpy
#endif

#ifdef L_strdup

Wchar *Wstrdup(register const Wchar *s1)
{
	register Wchar *s;

    if ((s = malloc((Wstrlen(s1) + 1) * sizeof(Wchar))) != NULL) {
		Wstrcpy(s, s1);
	}

	return s;
}

#endif
/**********************************************************************/
#ifdef L_strerror

char *strerror(int errnum)
{
    static char buf[_SYS_ERRMSG_MAXLEN];

	_susv3_strerror_r(errnum, buf, sizeof(buf));

	return buf;
}

#endif
/**********************************************************************/
/* SUSv3 functions. */
/**********************************************************************/
#ifdef L__susv3_strerror_r

#if defined(__alpha__) || defined(__mips__) || defined(__sparc__)

static const unsigned char estridx[] = {
	0,							/* success is always 0 */
	EPERM,
	ENOENT,
	ESRCH,
	EINTR,
	EIO,
	ENXIO,
	E2BIG,
	ENOEXEC,
	EBADF,
	ECHILD,
	EAGAIN,
	ENOMEM,
	EACCES,
	EFAULT,
	ENOTBLK,
	EBUSY,
	EEXIST,
	EXDEV,
	ENODEV,
	ENOTDIR,
	EISDIR,
	EINVAL,
	ENFILE,
	EMFILE,
	ENOTTY,
	ETXTBSY,
	EFBIG,
	ENOSPC,
	ESPIPE,
	EROFS,
	EMLINK,
	EPIPE,
	EDOM,
	ERANGE,
	EDEADLK,
	ENAMETOOLONG,
	ENOLCK,
	ENOSYS,
	ENOTEMPTY,
	ELOOP,
	0,
	ENOMSG,
	EIDRM,
	ECHRNG,
	EL2NSYNC,
	EL3HLT,
	EL3RST,
	ELNRNG,
	EUNATCH,
	ENOCSI,
	EL2HLT,
	EBADE,
	EBADR,
	EXFULL,
	ENOANO,
	EBADRQC,
	EBADSLT,
	0,
	EBFONT,
	ENOSTR,
	ENODATA,
	ETIME,
	ENOSR,
	ENONET,
	ENOPKG,
	EREMOTE,
	ENOLINK,
	EADV,
	ESRMNT,
	ECOMM,
	EPROTO,
	EMULTIHOP,
	EDOTDOT,
	EBADMSG,
	EOVERFLOW,
	ENOTUNIQ,
	EBADFD,
	EREMCHG,
	ELIBACC,
	ELIBBAD,
	ELIBSCN,
	ELIBMAX,
	ELIBEXEC,
	EILSEQ,
	ERESTART,
	ESTRPIPE,
	EUSERS,
	ENOTSOCK,
	EDESTADDRREQ,
	EMSGSIZE,
	EPROTOTYPE,
	ENOPROTOOPT,
	EPROTONOSUPPORT,
	ESOCKTNOSUPPORT,
	EOPNOTSUPP,
	EPFNOSUPPORT,
	EAFNOSUPPORT,
	EADDRINUSE,
	EADDRNOTAVAIL,
	ENETDOWN,
	ENETUNREACH,
	ENETRESET,
	ECONNABORTED,
	ECONNRESET,
	ENOBUFS,
	EISCONN,
	ENOTCONN,
	ESHUTDOWN,
	ETOOMANYREFS,
	ETIMEDOUT,
	ECONNREFUSED,
	EHOSTDOWN,
	EHOSTUNREACH,
	EALREADY,
	EINPROGRESS,
	ESTALE,
	EUCLEAN,
	ENOTNAM,
	ENAVAIL,
	EISNAM,
	EREMOTEIO,
#ifdef __mips__
	0,							/* mips has an outrageous value for this... */
#else
	EDQUOT,
#endif
	ENOMEDIUM,
	EMEDIUMTYPE,
#if defined(__mips__) || defined(__sparc__)
	EDEADLOCK,
#endif
};

#endif


int _susv3_strerror_r(int errnum, char *strerrbuf, size_t buflen)
{
    register char *s;
    int i, retval;
    char buf[_SYS_ERRMSG_MAXLEN];
    static const char unknown[14] = {
		'U', 'n', 'k', 'n', 'o', 'w', 'n', ' ', 'e', 'r', 'r', 'o', 'r', ' '
    };

    retval = EINVAL;


#if defined(__alpha__) || defined(__mips__) || defined(__sparc__)
	/* Need to translate errno to string index. */
	for (i = 0 ; i < sizeof(estridx)/sizeof(estridx[0]) ; i++) {
		if (estridx[i] == errnum) {
			goto GOT_ESTRIDX;
		}
	}
	i = INT_MAX;	/* Failed, but may need to check mips special case. */
#ifdef __mips__
	if (errnum == EDQUOT) {	/* Deal with large EDQUOT value on mips */
		i = 122;
	}
#endif /* __mips__ */
 GOT_ESTRIDX:
#else
	/* No errno to string index translation needed. */
	i = errnum;
#endif

    if (((unsigned int) i) < _SYS_NERR) {
		/* Trade time for space.  This function should rarely be called
		 * so rather than keeping an array of pointers for the different
		 * messages, just run through the buffer until we find the
		 * correct string. */
		for (s = (char *) _string_syserrmsgs ; i ; ++s) {
			if (!*s) {
				--i;
			}
		}
		if (*s) {		/* Make sure we have an actual message. */
			retval = 0;
			goto GOT_MESG;
		}
    }

    s = _int10tostr(buf+sizeof(buf)-1, errnum) - sizeof(unknown);
    memcpy(s, unknown, sizeof(unknown));

 GOT_MESG:
    if (!strerrbuf) {		/* SUSv3  */
		buflen = 0;
    }
    i = strlen(s) + 1;
    if (i > buflen) {
		i = buflen;
		retval = ERANGE;
    }

    if (i) {
		memcpy(strerrbuf, s, i);
		strerrbuf[i-1] = 0;	/* In case buf was too small. */
    }

    if (retval) {
		__set_errno(retval);
    }

    return retval;
}

#endif
/**********************************************************************/
/* GNU extension functions. */
/**********************************************************************/
#ifdef L__glibc_strerror_r

weak_alias(_glibc_strerror_r,__strerror_r);

char *_glibc_strerror_r(int errnum, char *strerrbuf, size_t buflen)
{
    _susv3_strerror_r(errnum, strerrbuf, buflen);

    return strerrbuf;
}

#endif
/**********************************************************************/
#ifdef L_wmempcpy
#define L_mempcpy
#define Wmempcpy wmempcpy
#else
#define Wmempcpy mempcpy
#endif

#ifdef L_mempcpy

#ifndef L_wmempcpy
/* uClibc's old string implementation did this to cater to some app. */
weak_alias(mempcpy,__mempcpy);
#endif

Wvoid *Wmempcpy(Wvoid * __restrict s1, const Wvoid * __restrict s2, size_t n)
{
	register Wchar *r1 = s1;
	register const Wchar *r2 = s2;

#ifdef __BCC__
	while (n--) {
		*r1++ = *r2++;
	}
#else
	while (n) {
		*r1++ = *r2++;
		--n;
	}
#endif

	return r1;
}

#endif
/**********************************************************************/
#ifdef L_memrchr

void *memrchr(const void *s, int c, size_t n)
{
	register const unsigned char *r;
#ifdef __BCC__
	/* bcc can optimize the counter if it thinks it is a pointer... */
	register const char *np = (const char *) n;
#else
#define np n
#endif
	
	r = ((unsigned char *)s) + ((size_t) np);

	while (np) {
		if (*--r == ((unsigned char)c)) {
			return (void *) r;	/* silence the warning */
		}
		--np;
	}

	return NULL;
}
#undef np

#endif
/**********************************************************************/
#ifdef L_wcpcpy
#define L_stpcpy
#define Wstpcpy wcpcpy
#else
#define Wstpcpy stpcpy
#endif

#ifdef L_stpcpy

Wchar *Wstpcpy(register Wchar * __restrict s1, const Wchar * __restrict s2)
{
#ifdef __BCC__
	do {
		*s1 = *s2++;
	} while (*s1++ != 0);
#else
	while ( (*s1++ = *s2++) != 0 );
#endif

	return s1 - 1;
}

#endif
/**********************************************************************/
#ifdef L_wcpncpy
#define L_stpncpy
#define Wstpncpy wcpncpy
#else
#define Wstpncpy stpncpy
#endif

#ifdef L_stpncpy

Wchar *Wstpncpy(register Wchar * __restrict s1,
				register const Wchar * __restrict s2,
				size_t n)
{
	Wchar *s = s1;
	const Wchar *p = s2;

#ifdef __BCC__
	while (n--) {
		if ((*s = *s2) != 0) s2++; /* Need to fill tail with 0s. */
		++s;
	}
	return s1 + (s2 - p);
#else
	while (n) {
		if ((*s = *s2) != 0) s2++; /* Need to fill tail with 0s. */
		++s;
		--n;
	}
	return s1 + (s2 - p);
#endif
}

#endif
/**********************************************************************/
#ifdef L_bzero

void bzero(void *s, size_t n)
{
	register unsigned char *p = s;
#ifdef __BCC__
	/* bcc can optimize the counter if it thinks it is a pointer... */
	register const char *np = (const char *) n;
#else
#define np n
#endif

	while (np) {
		*p++ = 0;
		--np;
	}
}
#undef np

#endif
/**********************************************************************/
#ifdef L_bcopy

void bcopy(const void *s2, void *s1, size_t n)
{
#if 1
	memmove(s1, s2, n);
#else
#ifdef __BCC__
	register char *s;
	register const char *p;

	s = s1;
	p = s2;
	if (p >= s) {
		while (n--) {
			*s++ = *p++;
		}
	} else {
		s += n;
		p += n;
		while (n--) {
			*--s = *--p;
		}
	}
#else
	register char *s;
	register const char *p;

	s = s1;
	p = s2;
	if (p >= s) {
		while (n) {
			*s++ = *p++;
			--n;
		}
	} else {
		while (n) {
			--n;
			s[n] = p[n];
		}
	}
#endif
#endif
}

#endif
/**********************************************************************/
#ifdef L_strcasestr

char *strcasestr(const char *s1, const char *s2)
{
	register const char *s = s1;
	register const char *p = s2;

#if 1
	do {
		if (!*p) {
			return (char *) s1;;
		}
		if ((*p == *s)
			|| (tolower(*((unsigned char *)p)) == tolower(*((unsigned char *)s)))
			) {
			++p;
			++s;
		} else {
			p = s2;
			if (!*s) {
				return NULL;
			}
			s = ++s1;
		}
	} while (1);
#else
	while (*p && *s) {
		if ((*p == *s)
			|| (tolower(*((unsigned char *)p)) == tolower(*((unsigned char *)s)))
			) {
			++p;
			++s;
		} else {
			p = s2;
			s = ++s1;
		}
	}

	return (*p) ? NULL : (char *) s1;
#endif
}

#endif
/**********************************************************************/
#ifdef L_strndup

char *strndup(register const char *s1, size_t n)
{
	register char *s;

	n = strnlen(s1,n);			/* Avoid problems if s1 not nul-terminated. */

    if ((s = malloc(n + 1)) != NULL) {
		memcpy(s, s1, n);
		s[n] = 0;
	}

	return s;
}

#endif
/**********************************************************************/
#ifdef L_strsep

char *strsep(char ** __restrict s1, const char * __restrict s2)
{
	register char *s = *s1;
	register char *p;

#if 1
	p = NULL;
	if (s && *s && (p = strpbrk(s, s2))) {
		*p++ = 0;
	}
#else
	if (s && *s && *(p = s + strcspn(s, s2))) {
		*p++ = 0;
	} else {
		p = NULL;
	}
#endif
	*s1 = p;
	return s;
}

#endif
/**********************************************************************/
#ifdef L_wcschrnul
#define L_strchrnul
#define Wstrchrnul wcschrnul
#else
#define Wstrchrnul strchrnul
#endif

#ifdef L_strchrnul

Wchar *Wstrchrnul(register const Wchar *s, Wint c)
{
	--s;
	while (*++s && (*s != ((Wchar)c)));
	return (Wchar *) s;
}

#endif
/**********************************************************************/
#ifdef L_rawmemchr

void *rawmemchr(const void *s, int c)
{
	register const unsigned char *r = s;

	while (*r != ((unsigned char)c)) ++r;

	return (void *) r;	/* silence the warning */
}

#endif
/**********************************************************************/
#ifdef L_basename

char *basename(const char *path)
{
	register const char *s;
	register const char *p;

	p = s = path;

	while (*s) {
		if (*s++ == '/') {
			p = s;
		}
	}

	return (char *) p;
}

#endif
/**********************************************************************/
#ifdef L___xpg_basename

char *__xpg_basename(register char *path)
{
	static const char null_or_empty[] = ".";
	char *first;
	register char *last;

	first = (char *) null_or_empty;

	if (path && *path) {
		first = path;
		last = path - 1;

		do {
			if ((*path != '/') && (path > ++last)) {
				last = first = path;
			}
		} while (*++path);

		if (*first == '/') {
			last = first;
		}
		last[1] = 0;
	}

	return first;
}

#endif
/**********************************************************************/
#ifdef L_dirname

char *dirname(char *path)
{
	static const char null_or_empty_or_noslash[] = ".";
	register char *s;
	register char *last;
	char *first;

	last = s = path;

	if (s != NULL) {

	LOOP:
		while (*s && (*s != '/')) ++s;
		first = s;
		while (*s == '/') ++s;
		if (*s) {
			last = first;
			goto LOOP;
		}

		if (last == path) {
			if (*last != '/') {
				goto DOT;
			}
			if ((*++last == '/') && (last[1] == 0)) {
				++last;
			}
		}
		*last = 0;
		return path;
	}
 DOT:
	return (char *) null_or_empty_or_noslash;
}

#endif
/**********************************************************************/
#ifdef L_strlcat

/* OpenBSD function:
 * Append at most n-1-strlen(dst) chars from src to dst and nul-terminate dst.
 * Returns strlen(src) + strlen({original} dst), so truncation occurred if the
 * return val is >= n.
 * Note: If dst doesn't contain a nul in the first n chars, strlen(dst) is
 *       taken as n. */

size_t strlcat(register char *__restrict dst,
			   register const char *__restrict src,
			   size_t n)
{
	size_t len;
	char dummy[1];

	len = 0;

	while (1) {
		if (len >= n) {
			dst = dummy;
			break;
		}
		if (!*dst) {
			break;
		}
		++dst;
		++len;
	}

	while ((*dst = *src) != 0) {
		if (++len < n) {
			++dst;
		}
		++src;
	}

	return len;
}

#endif
/**********************************************************************/
#ifdef L_strlcpy

/* OpenBSD function:
 * Copy at most n-1 chars from src to dst and nul-terminate dst.
 * Returns strlen(src), so truncation occurred if the return value is >= n. */

size_t strlcpy(register char *__restrict dst,
			   register const char *__restrict src,
			   size_t n)
{
	const char *src0 = src;
	char dummy[1];

	if (!n) {
		dst = dummy;
	} else {
		--n;
	}

	while ((*dst = *src) != 0) {
		if (n) {
			--n;
			++dst;
		}
		++src;
	}

	return src - src0;
}

#endif
/**********************************************************************/
#ifdef L__string_syssigmsgs

const char _string_syssigmsgs[] = {
	/*   0:    0,  1 */ "\0"
	/*   1:    1,  7 */ "Hangup\0"
	/*   2:    8, 10 */ "Interrupt\0"
	/*   3:   18,  5 */ "Quit\0"
	/*   4:   23, 20 */ "Illegal instruction\0"
	/*   5:   43, 22 */ "Trace/breakpoint trap\0"
	/*   6:   65,  8 */ "Aborted\0"
	/*   7:   73, 10 */ "Bus error\0"
	/*   8:   83, 25 */ "Floating point exception\0"
	/*   9:  108,  7 */ "Killed\0"
	/*  10:  115, 22 */ "User defined signal 1\0"
	/*  11:  137, 19 */ "Segmentation fault\0"
	/*  12:  156, 22 */ "User defined signal 2\0"
	/*  13:  178, 12 */ "Broken pipe\0"
	/*  14:  190, 12 */ "Alarm clock\0"
	/*  15:  202, 11 */ "Terminated\0"
	/*  16:  213, 12 */ "Stack fault\0"
	/*  17:  225, 13 */ "Child exited\0"
	/*  18:  238, 10 */ "Continued\0"
	/*  19:  248, 17 */ "Stopped (signal)\0"
	/*  20:  265,  8 */ "Stopped\0"
	/*  21:  273, 20 */ "Stopped (tty input)\0"
	/*  22:  293, 21 */ "Stopped (tty output)\0"
	/*  23:  314, 21 */ "Urgent I/O condition\0"
	/*  24:  335, 24 */ "CPU time limit exceeded\0"
	/*  25:  359, 25 */ "File size limit exceeded\0"
	/*  26:  384, 22 */ "Virtual timer expired\0"
	/*  27:  406, 24 */ "Profiling timer expired\0"
	/*  28:  430, 15 */ "Window changed\0"
	/*  29:  445, 13 */ "I/O possible\0"
	/*  30:  458, 14 */ "Power failure\0"
	/*  31:  472, 16 */ "Bad system call"
};

#endif

/**********************************************************************/
#ifdef L_sys_siglist

const char *const sys_siglist[_NSIG] = {
	NULL,
	_string_syssigmsgs + 1,
	_string_syssigmsgs + 8,
	_string_syssigmsgs + 18,
	_string_syssigmsgs + 23,
	_string_syssigmsgs + 43,
	_string_syssigmsgs + 65,
	_string_syssigmsgs + 73,
	_string_syssigmsgs + 83,
	_string_syssigmsgs + 108,
	_string_syssigmsgs + 115,
	_string_syssigmsgs + 137,
	_string_syssigmsgs + 156,
	_string_syssigmsgs + 178,
	_string_syssigmsgs + 190,
	_string_syssigmsgs + 202,
	_string_syssigmsgs + 213,
	_string_syssigmsgs + 225,
	_string_syssigmsgs + 238,
	_string_syssigmsgs + 248,
	_string_syssigmsgs + 265,
	_string_syssigmsgs + 273,
	_string_syssigmsgs + 293,
	_string_syssigmsgs + 314,
	_string_syssigmsgs + 335,
	_string_syssigmsgs + 359,
	_string_syssigmsgs + 384,
	_string_syssigmsgs + 406,
	_string_syssigmsgs + 430,
	_string_syssigmsgs + 445,
	_string_syssigmsgs + 458,
	_string_syssigmsgs + 472,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

#endif
/**********************************************************************/
#ifdef L_strsignal

/* TODO: make a threadsafe version? */

char *strsignal(int signum)
{
    register char *s;
    int i;
    static char buf[_STRSIGNAL_BUFSIZE];
    static const char unknown[15] = {
		'U', 'n', 'k', 'n', 'o', 'w', 'n', ' ', 's', 'i', 'g', 'n', 'a', 'l', ' '
    };

    if (((unsigned int) signum) < _SYS_NSIG) {
		/* Trade time for space.  This function should rarely be called
		 * so rather than keeping an array of pointers for the different
		 * messages, just run through the buffer until we find the
		 * correct string. */
		for (s = (char *) _string_syssigmsgs, i = signum ; i ; ++s) {
			if (!*s) {
				--i;
			}
		}
		if (*s) {		/* Make sure we have an actual message. */
			goto DONE;
		}
    }

    s = _int10tostr(buf+sizeof(buf)-1, signum) - sizeof(unknown);
    memcpy(s, unknown, sizeof(unknown));

 DONE:
	return s;
}

#endif
/**********************************************************************/
#ifdef L_psignal

/* TODO: make this threadsafe with a reentrant version of strsignal? */

void psignal(int signum, register const char *message)
{
	/* If the program is calling psignal, it's a safe bet that printf and
	 * friends are used as well.  It is also possible that the calling
	 * program could buffer stderr, or reassign it. */

	register const char *sep;

	sep = ": ";
	if (!(message && *message)) { /* Caller did not supply a prefix message */
		message = (sep += 2);	/* or passed an empty string. */
	}

#if 1
	fprintf(stderr, "%s%s%s\n", message, sep, strsignal(signum));
#else
	/* Note: Assumes stderr not closed or buffered. */
	__STDIO_THREADLOCK(stderr);
	_stdio_fdout(STDERR_FILENO, message, sep, strsignal(signum));
	__STDIO_THREADUNLOCK(stderr);
#endif
}

#endif
/**********************************************************************/
