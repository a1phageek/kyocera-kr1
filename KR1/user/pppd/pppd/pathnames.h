/*
 * define path names
 *
 * $Id: pathnames.h,v 1.2 2003/09/04 07:40:01 rupert Exp $
 */

#ifdef HAVE_PATHS_H
#include <paths.h>

#else
#define _PATH_DEVNULL	"/dev/null"
#endif

#ifdef PATH_ETC_CONFIG
wrong
#define _ROOT_PATH
#define _PATH_VARRUN 	"/var/run/"
#define _PATH_UPAPFILE 	"/etc/config/pap-secrets"
#define _PATH_CHAPFILE 	"/etc/config/chap-secrets"
#define _PATH_SYSOPTIONS "/var/ppp/options"
#define _PATH_IPUP	"/etc/config/ip-up"
#define _PATH_DEFAULT_IPUP	"/etc/default/ip-up"
#define _PATH_IPDOWN	"/bin/ip-down"
#define _PATH_DEFAULT_IPDOWN	"/bin/ip-down"
#define _PATH_AUTHUP	"/etc/config/auth-up"
#define _PATH_AUTHDOWN	"/etc/config/auth-down"
#define _PATH_TTYOPT	"/etc/config/options."
#define _PATH_CONNERRS	"/var/log/connect-errors"
#define _PATH_USEROPT	".ppprc"
#define _PATH_PEERFILES	"/etc/config/peers/"
#define _PATH_RESOLV	"/etc/config/resolv.conf"
#define _PATH_TEMP      "/etc/config/resolv.XXXXXX"
#else
#define _PATH_VARRUN 	"/etc/ppp/"
#define _PATH_UPAPFILE 	"/var/ppp/pap"
#define _PATH_CHAPFILE 	"/etc/ppp/chap-secrets"
#define _PATH_SYSOPTIONS "/var/ppp/options"
#define _PATH_IPUP	"/bin/ip-up"
#define _PATH_IPDOWN	"/bin/ip-down"
#define _PATH_AUTHUP	"/var/ppp/auth-up"
#define _PATH_AUTHDOWN	"/var/ppp/auth-down"
#define _PATH_TTYOPT	"/var/ppp/options."
#define _PATH_CONNERRS	"/var/ppp/connect-errors"
#define _PATH_USEROPT	".ppprc"
#define _PATH_PEERFILES	"/var/ppp/peers/"
#define _PATH_RESOLV	"/var/resolv.conf"
#define _PATH_TEMP      "/tmp/resolv.XXXXXX"
#endif

#ifdef IPX_CHANGE
#define _PATH_IPXUP	"/etc/ppp/ipx-up"
#define _PATH_IPXDOWN	"/etc/ppp/ipx-down"
#endif /* IPX_CHANGE */
