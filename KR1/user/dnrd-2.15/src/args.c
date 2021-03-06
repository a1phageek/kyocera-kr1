/*
 * args.c - data and functions dealing with command-line argument processing.
 *
 * Copyright (C) 1998 Brad M. Garcia <garsh@home.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#if defined(__GNU_LIBRARY__)
#   include <getopt.h>
#endif
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pwd.h>

#include "args.h"
#include "common.h"
#include "lib.h"
#include "cache.h"


/*
 * Definitions for both long and short forms of our options.
 * See man page for getopt for more details.
 */
#if defined(__GNU_LIBRARY__)
static struct option long_options[] =
{
    {"address",      1, 0, 'a'},
    {"load-balance", 0, 0, 'b'},
    {"cache",        1, 0, 'c'},
    {"debug",        0, 0, 'd'},
    {"help",         0, 0, 'h'},
    {"ignore",       0, 0, 'i'},
    {"kill",         0, 0, 'k'},
    {"log",          0, 0, 'l'},
#ifndef EXCLUDE_MASTER
    {"master",       1, 0, 'm'},
#endif
    {"retry",        1, 0, 'r'},
    {"server",       1, 0, 's'},
    {"timeout",      1, 0, 't'},
    {"uid",          1, 0, 'u'},
    {"version",      0, 0, 'v'},
    {"chroot-path",  1, 0, 'p'},
    {0, 0, 0, 0}
};
#endif /* __GNU_LIBRARY__ */

#ifndef EXCLUDE_MASTER
const char short_options[] = "a:bc:dhiklm:s:t:u:v";
#else
const char short_options[] = "a:bc:dhikl:s:t:u:v";
#endif

/*
 * give_help()
 *
 * Abstract: Prints out the version number and a usage statement.
 */
 #ifndef EMBED
static void give_help()
{
    printf("dnrd version %s\n", version);
    printf("\nusage: %s [options]\n", progname);
    printf("  Valid options are\n");
    printf("    -a, --address=LOCALADDRESS\n"
	   "                              "
	   "Only bind to the port on the given address,\n"
	   "                              rather than all local addresses\n");
    printf("    -b, --load-balance        Round-Robin load balance forwarding servers\n");
    printf("    -c, --cache=off|[LOW:]HIGH\n");
    printf("    -d, --debug               "
	   "Turn on debugging - run in foreground.\n");
    printf("    -h, --help                Print this message, then exit.\n");
    printf("    -i, --ignore              Ignore cache for disabled servers\n");
    printf("    -k, --kill                Kill a running daemon.\n");
    printf("    -l, --log                 Send all messages to syslog.\n");
#ifndef EXCLUDE_MASTER
    printf("    -m, --master=MASTERMODE\n");
#endif
    printf("    -r, --retry=N             Set retry interval to N seconds\n");
    printf("    -s, --server=IPADDR(:domain)\n"
	   "                              "
	   "Set the DNS server.  You can specify an\n"
	   "                              "
	   "optional domain name, in which case a DNS\n"
	   "                              "
	   "request will only be sent to that server for\n"
	   "                              names in that domain.\n"
	   "                              "
	   "(Can be used more than once for multiple or\n"
	   "                              backup servers)\n");
    printf("    -t, --timeout=N           Set forward DNS server timeout to N\n");
    printf("    -u, --uid=ID              "
	   "Username or numeric id to switch to\n");
    /*
    printf("    -p, --chroot-path=CHROOTPATH\n"
	   "                              "
	   "The chroot path. dnrd will chroot to this dir\n");
    */
    printf("    -v, --version             "
	   "Print out the version number and exit.\n");
    printf("\n");
}
#endif


/*
 * parse_args()
 *
 * In:      argc - number of command-line arguments.
 *          argv - string array containing command-line arguments.
 *
 * Returns: an index into argv where we stopped parsing arguments.
 *
 * Abstract: Parses command-line arguments.  In some cases, it will
 *           set the appropriate global variables and return.  Otherwise,
 *           it performs the appropriate action and exits.
 *
 * Assumptions: Currently, we check to make sure that there are no arguments
 *              other than the defined options, so the return value is
 *              pretty useless and should be ignored.
 */
int parse_args(int argc, char **argv)
{
  static int load_balance = 0;
    int c;
    /*    int gotdomain = 0;*/

    progname = strrchr(argv[0], '/');
    if (!progname) progname = argv[0];

    while(1) {
#if defined(__GNU_LIBRARY__)
	c = getopt_long(argc, argv, short_options, long_options, 0);
#else
	c = getopt(argc, argv, short_options);
#endif
	if (c == -1) break;
	switch(c) {
	  case 'a': {
	      if (!inet_aton(optarg, &recv_addr.sin_addr)) {
		  log_msg(LOG_ERR, "%s: Bad ip address \"%s\"\n",
			  progname, optarg);
		  exit(-1);
	      }
	      break;
	  }
	case 'b': {
	  load_balance = 1;
	  break;
	}
	  case 'c': {
	      copy_string(cache_param, optarg, sizeof(cache_param));
	      break;
	  }
	  case 'd': {
	      opt_debug++;
	      break;
	  }
	  case 'h': {
#ifndef EMBED
	      give_help();
	      exit(0);
#endif
	      break;
	  }
	  case 'i' : {
	    ignore_inactive_cache_hits = 1; 
	    break;
	  }
#ifdef NOEMBED
	  case 'k': {
	      if (!kill_current()) {
		  printf("No %s daemon found.  Exiting.\n", progname);
	      }
	      exit(0);
	      break;
	  }
#endif
	  case 'l': {
	      gotterminal = 0;
	      break;
	  }
#ifndef EXCLUDE_MASTER
	  case 'm': {
#ifndef EMBED 
	      copy_string(master_param, optarg, sizeof(master_param));
#endif
	      break;
	  }
#endif
	  case 'r': {
	    reactivate_interval = atoi(optarg);
	    log_debug("Setting retry interval to %i seconds.", 
		      reactivate_interval);
	    break;
	  }
	  case 's': {
	    domnode_t *p;
	    char *s,*sep = strchr(optarg, (int)':');
	    
	    if (sep) { /* is a domain specified? */
	      s = make_cname(strnlwr(sep+1,200),200);
	      *sep = 0;
	      if ( (p=search_domnode(domain_list, s)) == NULL) {
		p=add_domain(domain_list, load_balance, s, 200);
		log_debug("Added domain %s %s load balancing", sep+1, 
			  load_balance ? "with" : "without");
	      }
	    } else p=domain_list;
	    if (!add_srv(last_srvnode(p->srvlist), optarg)) {
	      log_msg(LOG_ERR, "%s: Bad ip address \"%s\"\n",
		      progname, optarg);
	      exit(-1);
	    } else {
	      log_debug("Server %s added to domain %s", optarg, 
			sep ? sep+1:"(default)");
	    }
	    if (p->roundrobin != load_balance) {
	      p->roundrobin =load_balance;
	      log_debug("Turned on load balancing for domain %s",
			cname2asc(p->domain));
	    }
	    if (sep) *sep = ':';
	    break;
	  }
	  case 't': {
	    forward_timeout = atoi(optarg);
	    log_debug("Setting retry interval to %i seconds.", 
		      reactivate_interval);
	    break;
	  }
	  case 'u': {
#ifndef EMBED
	      char *ep;
	      struct passwd *pwent;
	      daemonuid = (uid_t)strtoul(optarg, &ep, 10);

	      pwent = *ep ? getpwnam(optarg) : getpwuid(daemonuid);
	      if (!pwent) {
		  log_msg(LOG_ERR, "%s: Bad uid \"%s\"\n", progname, optarg);
		  exit(-1);
	      }

	      daemonuid = pwent->pw_uid;
	      daemongid = pwent->pw_gid;
#endif
	      break;
	  }
	  case 'v': {
	      printf("dnrd version %s\n\n", version);
	      exit(0);
	      break;
	  }

	  case 'p': {
	    strncpy(chroot_path, optarg, sizeof(chroot_path));
	    log_debug("Using %s as chroot");
	    break;
	  }
	  case ':': {
	      log_msg(LOG_ERR, "%s: Missing parameter for \"%s\"\n",
		      progname, argv[optind]);
	      exit(-1);
	      break;
	  }
	  case '?':
	  default: {
	      /* getopt_long will print "unrecognized option" for us */
#ifndef EMBED
	      give_help();
#endif
	      exit(-1);
	      break;
	  }
	}
    }

    if (optind != argc) {
	log_msg(LOG_ERR, "%s: Unknown parameter \"%s\"\n",
		progname, argv[optind]);
	exit(-1);
    }
    return optind;
}
