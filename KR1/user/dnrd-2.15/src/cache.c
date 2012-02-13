/*

    File: cache.c -- add response caching
    
    Copyright (C) 1999 by Wolfgang Zekoll  <wzk@quietsche-entchen.de>

    This source is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2, or (at your option)
    any later version.

    This source is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "common.h"
#include "dns.h"
#include "cache.h"
#include "lib.h"
#include "dns.h"
#include "srvnode.h"
#include <board.h> /* exported function prototypes */
	/*
	 * Cache time calculations are done in seconds.  CACHE_TIMEUNIT
	 * defines a minute.  You might want to set it to 1 for
	 * debugging.
	 */

#define	CACHE_TIMEUNIT		60

	/*
	 * DNS queries that have been answered positively are stored for
	 * CACHE_TIME, errors for only CACHE_NEGTIME minutes.  Reusing
	 * a positive answer resets the entry's expire time.  After
	 * CACHE_MAXTIME the item is removed anyway.
	 */

#define	CACHE_NEGTIME		(     5 * CACHE_TIMEUNIT)
// Dino Chang 2005/02/04
#define	CACHE_TIME		(     5 * CACHE_TIMEUNIT)
//#define	CACHE_TIME		(    60 * CACHE_TIMEUNIT)
// Dino
#define CACHE_MAXTIME		(6 * 60 * CACHE_TIMEUNIT)

	/*
	 * The expire function can be called as often as wanted.
	 * It will however wait CACHE_MINCYCLE between two
	 * expire runs.
	 */

#define	CACHE_MINCYCLE		(5 * CACHE_TIMEUNIT)

	/*
	 * If after an expire the cache holds more than CACHE_HIGHWATER
	 * items the oldest items are removed until there are only
	 * CACHE_LOWWATER left.
	 */

#define	CACHE_HIGHWATER		1000
#define	CACHE_LOWWATER		 800


typedef struct _cache {
    unsigned int  code;
    char	 *name;		/* Objectname */
    int		  type, class;	/* Query type and class. */

    int		  positive;	/* Positive or error response? */
    unsigned long created;
    unsigned long lastused;
    unsigned long expires;

    dnsheader_t	*p;		/* The DNS packet with decoded header */

    srvnode_t *server; /* the server that gave this answer */
    struct _cache *next, *prev;
} cache_t;


#ifdef SUPPORT_MODEL_DOMAIN_NAME
#define DEFAULT_MODEL_DN "dlink.di624s"

#define IP_ADDR(w,x,y,z)    ((unsigned long)(((unsigned long)w << 24) | \
                                             ((unsigned long)x << 16) | \
                                             ((unsigned long)y << 8) | \
                                             (unsigned long)z))
                                             
romeCfgParam_t *DNS_pRomeCfgParam;
extern int getRomeCfgParamPtr(romeCfgParam_t **);
#endif

char	cache_param[256]	= "";
int	cache_onoff		= 1;
long	cache_highwater		= CACHE_HIGHWATER;
long	cache_lowwater		= CACHE_LOWWATER;

static cache_t *cachelist	= NULL;
static cache_t *lastcache	= NULL;

static int cache_hits		= 0;
static int cache_misses		= 0;


static int free_cx(cache_t *cx)
{
    free_packet(cx->p);
    free(cx->name);
    free(cx);
    
    return (0);
}

static cache_t *create_cx(dnsheader_t *x, rr_t *query, srvnode_t *server)
{
    cache_t	*cx;

    cx = allocate(sizeof(cache_t));

    cx->name = strdup(query->name);
    cx->code = get_stringcode(cx->name);

    cx->positive = x->ancount;
    cx->type     = query->type;
    cx->class    = query->class;
    cx->p        = x;
    cx->lastused = time(NULL);
    cx->server = server;

    return (cx);
}

static cache_t *append_cx(cache_t *cx)
{
    if (lastcache == NULL) {
	cachelist = cx;
	lastcache = cx;
    }
    else {
	lastcache->next = cx;
	cx->prev = lastcache;
	lastcache = cx;
    }

    cx->created = time(NULL);
    log_debug("cache: added %s, type= %d, class: %d, ans= %d\n",
	      cx->name, cx->type, cx->class, cx->p->ancount);

    return (cx);
}

static cache_t *remove_cx(cache_t *cx)
{
    if (cx->next != NULL) {
	cx->next->prev = cx->prev;
    }
    else {
	lastcache = cx->prev;
    }

    if (cx->prev != NULL) {
	cx->prev->next = cx->next;
    }
    else {
	cachelist = cx->next;
    }

    return (cx);
}



/*
 * cache_dnspacket()
 *
 * In:      packet - the response packet to cache.
 *          len    - length of the response packet.
 *
 * Returns: 0, all the time.
 *
 * Take the response packet and look if it meets some basic
 * conditions for caching.  If so put the entire response into
 * our cache.
 */
int cache_dnspacket(void *packet, int len, srvnode_t *server)
{
    dnsheader_t *x;
    rr_t	query;
    cache_t	*cx = NULL;

    if ((cache_onoff == 0) ||
	(parse_query(&query, packet, len) == NULL) ||
	(GET_QR(query.flags) == 0) ||
	(*query.name == 0)) {
	return (0);
    }
	
    x = parse_packet(packet, len);
    /* get_dnsquery(x, &query); */

    /*
     * Ok, the packet is interesting for us.  Let's put it into our
     * cache list.
     */
#ifndef EMBED
    sem_wait(&dnrd_sem);
#endif
    cx = create_cx(x, &query, server);
    append_cx(cx);

    /*
     * Set the expire time of the cached object.
     */
    cx->lastused = time(NULL);
    cx->expires  = cx->lastused +
	           ((cx->p->ancount > 0) ? CACHE_TIME : CACHE_NEGTIME);
#ifndef EMBED
    sem_post(&dnrd_sem);
#endif
    return (0);
}


/*
 * cache_lookup()
 *
 * In/Out:  packet - the query packet on input, response on output.
 *          len    - length of the query packet.
 *
 * Returns: length of the response packet "cached",
 *          0 if no cached response is available.
 *
 * Look into the query packet too see if it is a lookup request that
 * might already have been cached.  If so, copy the data to the
 * cached pointer returning the length of the cached packet
 * as return value.
 *
 * The function assumes that the area cached points to is
 * large enough.
 */
int cache_lookup(void *packet, int len)
{
    unsigned int code;
    /*    dnsheader_t *x;*/
    rr_t	query;
    cache_t	*cx = NULL;
#ifdef SUPPORT_MODEL_DOMAIN_NAME
    unsigned long answer[4] = {htonl(0xc00c0001), htonl(0x00010000), htonl(0x0e100004), htonl(0x00000000)};//add by raymondlin
    unsigned long ipaddr = 0;
    dnsheader_t *ptr;
    int dns_len, offset, str_len;
    ifCfgParam_t ramLanCfgParam;
#endif    

    if ((cache_onoff == 0) ||
	(parse_query(&query, packet, len) == NULL) ||
	(GET_QR(query.flags) == 1) ||
	(*query.name == 0) ||
	(query.class != DNS_CLASS_INET)) {
	return (0);
    }


    //printf("query: name %s, type= %d, class: %d, code= %d\n", query.name, query.type, query.class, code);
#ifdef SUPPORT_MODEL_DOMAIN_NAME
    if (!getRomeCfgParamPtr(&DNS_pRomeCfgParam)) 
    {
        // retrieve Model Default Domain Name and Model IP Address
    	memcpy((void*)&ramLanCfgParam, &(DNS_pRomeCfgParam->ifCfgParam[1]),sizeof(ifCfgParam_t));
    	ipaddr = IP_ADDR(ramLanCfgParam.ipAddr[0], ramLanCfgParam.ipAddr[1], ramLanCfgParam.ipAddr[2], ramLanCfgParam.ipAddr[3]);
    	//printf("DNRD Main(): Model_DN = %s, IP = %lx\n",DEFAULT_MODEL_DN, ipaddr);
    }       
    if( strcmp(query.name, DEFAULT_MODEL_DN) == 0)//add by raymondlin DEFAULT_MODEL_DOMAIN_NAME
    {
    	//printf("raymondlin add domain dns\n");
    	//put answer here and update udp length
    	ptr = (dnsheader_t *)packet;
    	ptr->ancount = htons(ntohs(ptr->ancount) + 1); 
	answer[3] = ipaddr;
	str_len = strlen(query.name);
	offset = 12+2+str_len+4;//dns header+<2+query name len>+2(type)+2(class)
    	//printf("str len = %d\n",str_len);
    	memcpy(packet + offset, answer, 16);
    	//dns_len = ptr->len + 16;
    	dns_len = offset + 16;
    	//printf("dns answer offset = %d, dns_len = %d.\n", offset,dns_len);//add by raymondlin
    	return (dns_len);
    }
#endif
    /*
     * The query could be in the cache.  Let's take the packet ...
     * ... and search our cache for this request.
     */
    code = get_stringcode(query.name);
    for (cx = cachelist; cx != NULL; cx = cx->next) {
      if (cx->code == code  &&
	  cx->type == query.type  &&
	  cx->class == query.class  &&
	  strcasecmp(cx->name, query.name) == 0) {
	
	log_debug("cache: found %s, type= %d, class: %d, ans= %d, hdr_len=%d\n",
		  cx->name, cx->type, cx->class, cx->p->ancount, cx->p->len);

	if (cx->positive > 0) {
	  cx->lastused = time(NULL);
	  cx->expires  = cx->lastused + CACHE_TIME;
	}

	memcpy(packet + 2, cx->p->packet + 2, cx->p->len - 2);
	cache_hits++;

	/* lets check if the server is active */
	if (ignore_inactive_cache_hits && cx->server->inactive ) {
	  log_debug("server is inactive. Skipping cache entry");
	  return (0);
	}

	return (cx->p->len);
      }
    }

    cache_misses++;
    return (0);
}


/*
 * Item expiration
 */

/*
 * cmp_cachemru() - Compare the "ages" of two cache entries.
 *
 * In:      cx - pointer to first cache entry. 
 *          cy - pointer to second cache entry.
 *
 * Returns: >0 if cx is older than cy
 *          <0 if cx is younger than cy
 *           0 if cx is equal to cy
 */
static int cmp_cachemru(const void *x, const void * y)
{
    const cache_t **cx = (const cache_t **)x;
    const cache_t **cy = (const cache_t **)y;
    return ((*cy)->lastused - (*cx)->lastused);
}

/*
 * expire_oldest() - expire cache entries until lowwater is reached
 *
 * In:      itemcount - number of entries in cache.
 *
 * Returns: 0, always.
 *
 * Sorts all the entries in order of age, oldest first.
 * Then removes the first (itemcount - cache_lowwater) entries.
 */
static int expire_oldest(int itemcount)
{
    int	     i, n;
    cache_t *cx, **cv;
    
    if ((n = itemcount - cache_lowwater) < 0) return (0);

    cv = allocate(itemcount * sizeof(cache_t *));
    for (i = 0, cx = cachelist; i < itemcount; i++, cx = cx->next) {
	cv[i] = cx;
    }

    qsort(cv, itemcount, sizeof(cache_t *), cmp_cachemru);
    for (i=0; i<n; i++) {
	cx = cv[i];
	remove_cx(cx);
	free_cx(cx);
    }

    log_msg(LOG_NOTICE, "cache: %d of %d mru expired, %d remaining\n",
	   n, itemcount, itemcount - n);

    return (0);
}

/*
 * cache_expire() - Expire old entries from the cache.
 *
 * Returns: 0, always.
 *
 * When called, this function walks the entire cachelist, looking for entries
 * that should be expired.
 */
int cache_expire(void)
{
    int	           total, expired;
    time_t         now;
    cache_t	  *cx, *next;
    static time_t  lastexpire = 0;

    if (cache_onoff == 0) return (0);

    /* Initialize lastexpire to the current time */
    if (lastexpire == 0) lastexpire = time(NULL);

    now = time(NULL);
    if (now - lastexpire < CACHE_MINCYCLE) return (0);

    total = 0;
    expired = 0;

    cx = cachelist;
    while (cx != NULL) {
	total += 1;
	next = cx->next;

	if (cx->expires < now  ||  (now - cx->created) > CACHE_MAXTIME) {
	    remove_cx(cx);
	    free_cx(cx);
	    expired++;
	}
		
	cx = next;
    }


    log_debug("cache stats: %d entries, %d missed, %d hits",
    	      total, cache_misses, cache_hits);
    cache_misses = 0;
    cache_hits   = 0;

    if (expired > 0) {
	log_debug("cache: %d of %d expired, %d remaining\n",
		  expired, total, total - expired);
    }

    total = total - expired;
    if (total > cache_highwater) {
	expire_oldest(total);
    }

    lastexpire = now;
    return (0);
}

/*
 * cache_expireAll() - Expire all entries from the cache.
 *
 *
 * When called, this function walks the entire cachelist and expire all entries
 */
void cache_expireAll(void)
{
	int	           total, expired;
	cache_t	  *cx, *next;

	if (cache_onoff == 0)
		return;

	total = 0;
	expired = 0;
	cx = cachelist;
	while (cx != NULL) {
		total += 1;
		next = cx->next;
		remove_cx(cx);
		free_cx(cx);
		expired++;		
		cx = next;
	}

	log_debug("cache stats: %d entries, %d missed, %d hits",
		total, cache_misses, cache_hits);

	cache_misses	= 0;
	cache_hits		= 0;

	if (expired > 0) {
		log_debug("cache: %d of %d expired, %d remaining\n",
			expired, total, total - expired);
	}

	return;
}


/*
 * cache_init()
 *
 * Returns: 0, always.
 *
 * Called once from main() to initialize our cache.
 * Caching is turned on by default.
 */
int cache_init(void)
{
    if (*cache_param != 0) {
	if (strcmp(cache_param, "off") == 0) {
	    cache_onoff = 0;
	}
	else if (isdigit((int)(*cache_param))) {
	    char *p = strchr(cache_param, ':');

	    if (p == NULL) {
		cache_highwater = atoi(cache_param);
		cache_lowwater  = (cache_highwater * 75) / 100;
	    }
	    else {
		*p++ = 0;
		cache_lowwater  = atoi(cache_param);
		cache_highwater = atoi(p);
	    }
	}
	else {
	    log_msg(LOG_ERR, "invalid cache parameter: %s", cache_param);
	}
    }

    if (cache_onoff == 0) {
	log_msg(LOG_NOTICE, "caching turned off");
    }
    else {
	if (cache_highwater < 0) {
	    cache_highwater = CACHE_HIGHWATER;
	    cache_lowwater  = CACHE_LOWWATER;
	}
	else if (cache_lowwater > cache_highwater) {
	    cache_lowwater  = (cache_highwater * 75) / 100;
	}

	log_debug("cache low/high: %d/%d", cache_lowwater, cache_highwater);
    }

    return (0);
}



