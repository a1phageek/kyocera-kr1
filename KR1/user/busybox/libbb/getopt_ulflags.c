/* vi: set sw=4 ts=4: */
/*
 * universal getopt_ulflags implementation for busybox
 *
 * Copyright (C) 2003  Vladimir Oleynik  <dzo@simtreas.ru>
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

#include <getopt.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "libbb.h"

/*
You can set bb_opt_complementaly as string with one or more
complementaly or incongruously options.
If sequential founded option haved from this string
then your incongruously pairs unsets and complementaly make add sets.
Format:
one char - option for check,
chars - complementaly option for add sets.
- chars - option triggered for unsets.
~ chars - option incongruously.
*       - option list, called add_to_list(*ptr_from_usaged, optarg)
:       - separator.
Example: du applet can have options "-s" and "-d size"
If getopt found -s then -d option flag unset or if found -d then -s unset.
For this result you must set bb_opt_complementaly = "s-d:d-s".
Result have last option flag only from called arguments.
Warning! You can check returned flag, pointer to "d:" argument seted
to own optarg always.
Example two: cut applet must only one type of list may be specified,
and -b, -c and -f incongruously option, overwited option is error also.
You must set bb_opt_complementaly = "b~bcf:c~bcf:f~bcf".
If called have more one specified, return value have error flag -
high bite set (0x80000000UL).
Example three: grep applet can have one or more "-e pattern" arguments.
You should use bb_getopt_ulflags() as
llist_t *paterns;
bb_opt_complementaly = "e*";
bb_getopt_ulflags (argc, argv, "e:", &paterns);
*/

const char *bb_opt_complementaly;

typedef struct
{
	char opt;
	char list_flg;
	unsigned long switch_on;
	unsigned long switch_off;
	unsigned long incongruously;
	void **optarg;              /* char **optarg or llist_t **optarg */
} t_complementaly;

/* You can set bb_applet_long_options for parse called long options */

static const struct option bb_default_long_options[] = {
     /*   { "help", 0, NULL, '?' }, */
	{ 0, 0, 0, 0 }
};

const struct option *bb_applet_long_options = bb_default_long_options;


unsigned long
bb_getopt_ulflags (int argc, char **argv, const char *applet_opts, ...)
{
	unsigned long flags = 0;
  int c = 0;
	const char *s;
  t_complementaly *complementaly;
  t_complementaly *on_off;
  va_list p;

  va_start (p, applet_opts);

  for (s = applet_opts; *s; s++) {
	c++;
	while (s[1] == ':') {
	    /* check GNU extension "o::" - optional arg */
	    s++;
	}
  }
  complementaly = xcalloc (c + 1, sizeof (t_complementaly));
  c = 0;
  for (s = applet_opts; *s; s++) {
	complementaly->opt = *s;
	complementaly->switch_on |= (1 << c);
	c++;
	if (s[1] == ':') {
	  complementaly->optarg = va_arg (p, void **);
	  do
		s++;
	  while (s[1] == ':');
		}
	complementaly++;
	}
  complementaly->opt = 0;
  complementaly -= c;
  c = 0;
  for (s = bb_opt_complementaly; s && *s; s++) {
    t_complementaly *pair;

    if (*s == ':') {
	  c = 0;
	  continue;
    }
    if (c)
	  continue;
    for (on_off = complementaly; on_off->opt; on_off++)
	  if (on_off->opt == *s)
	    break;
    pair = on_off;
    for(s++; *s && *s != ':'; s++) {
      if (*s == '-' || *s == '~') {
	  c = *s;
      } else if(*s == '*') {
	pair->list_flg++;
      } else {
	  unsigned long *pair_switch = &(pair->switch_on);

	  if(c)
	    pair_switch = c == '-' ? &(pair->switch_off) : &(pair->incongruously);
	  for (on_off = complementaly; on_off->opt; on_off++)
	    if (on_off->opt == *s) {
		  *pair_switch |= on_off->switch_on;
		  break;
	    }
      }
    }
    s--;
  }

  while ((c = getopt_long (argc, argv, applet_opts,
			    bb_applet_long_options, NULL)) > 0) {

	for (on_off = complementaly; on_off->opt != c; on_off++) {
	    if(!on_off->opt)
		bb_show_usage ();
	}
	if(flags & on_off->incongruously)
	    flags |= 0x80000000UL;
	flags &= ~on_off->switch_off;
	flags |= on_off->switch_on;
	if(on_off->list_flg) {
	   *(llist_t **)(on_off->optarg) =
		llist_add_to(*(llist_t **)(on_off->optarg), optarg);
	} else if (on_off->optarg) {
	    *(char **)(on_off->optarg) = optarg;
	}
  }
  free(complementaly);
	return flags;
}
