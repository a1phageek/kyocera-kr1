/*
 * Automatically generated C config: don't edit
 */
#if !defined __FEATURES_H && !defined __need_uClibc_config_h
#error Never include <bits/uClibc_config.h> directly; use <features.h> instead.
#endif
#define AUTOCONF_INCLUDED

/*
 * Version Number
 */
#define __UCLIBC_MAJOR__ 0
#define __UCLIBC_MINOR__ 9
#define __UCLIBC_SUBLEVEL__ 20

/*
 * Target Architecture Features and Options
 */
#define __HAVE_ELF__ 1
#define __ARCH_CFLAGS__ "-mno-split-addresses"
#define __ARCH_LDFLAGS__ "-mips1 -fomit-frame-pointer -O2 -msoft-float"
#undef __UCLIBC_HAS_MMU__
#define __UCLIBC_HAS_FLOATS__ 1
#undef __HAS_FPU__
#define __UCLIBC_HAS_SOFT_FLOAT__ 1
#undef __DO_C99_MATH__
#define __WARNINGS__ "-Wall"
#define __KERNEL_SOURCE__ "/home/hiwu/workingbox/src/toolchain20030818/build_mips/linux"
#define __UCLIBC_UCLINUX_BROKEN_MUNMAP__ 1
#define __EXCLUDE_BRK__ 1
#define __C_SYMBOL_PREFIX__ ""
#define __HAVE_DOT_CONFIG__ 1

/*
 * General Library Settings
 */
#define __DOPIC__ 1
#undef __HAVE_SHARED__
#undef __UCLIBC_CTOR_DTOR__
#undef __UCLIBC_PROFILING__
#define __UCLIBC_HAS_THREADS__ 1
#undef __PTHREADS_DEBUG_SUPPORT__
#undef __UCLIBC_HAS_LFS__
#define __MALLOC__ 1
#undef __MALLOC_930716__
#undef __UCLIBC_DYNAMIC_ATEXIT__
#undef __HAS_SHADOW__
#undef __UCLIBC_HAS_REGEX__
#define __UNIX98PTY_ONLY__ 1
#define __ASSUME_DEVPTS__ 1
#undef __UCLIBC_HAS_TM_EXTENSIONS__

/*
 * Networking Support
 */
#undef __UCLIBC_HAS_IPV6__
#define __UCLIBC_HAS_RPC__ 1
#undef __UCLIBC_HAS_FULL_RPC__

/*
 * String and Stdio Support
 */
#define __UCLIBC_HAS_WCHAR__ 1
#undef __UCLIBC_HAS_LOCALE__
#undef __USE_OLD_VFPRINTF__

/*
 * Library Installation Options
 */
#define __DEVEL_PREFIX__ "/home/hiwu/opt_mips"
#define __SYSTEM_DEVEL_PREFIX__ "/home/hiwu/opt_mips"
#define __DEVEL_TOOL_PREFIX__ "/home/hiwu/opt_mips/usr"

/*
 * uClibc hacking options
 */
#undef __DODEBUG__
#undef __DOASSERTS__
#undef __UCLIBC_MALLOC_DEBUGGING__
