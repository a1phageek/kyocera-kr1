#ifndef UPNP_MM_H
#define UPNP_MM_H

//#include "upnp.h"

#ifndef SUCCESS
#define SUCCESS (0)
#endif
#ifndef FAILED
#define FAILED	(-1)
#endif

/*
	Data Structure
*/
typedef struct heap_s
{
	struct heap_s**	head;
	struct heap_s*	next;
} heap_t;

/*
	External Functions
*/
int upnp_mm_init(int cnt[]);
int upnp_mm_destroy(void);
int upnp_mm_reinit(void);
void *upnp_realloc(void *ptr, int size);
void *upnp_calloc(int nmemb, int size);
void *upnp_malloc(int size);
void upnp_free(void *ptr);
void upnp_mm_dump();

char *upnp_strdup(const char *s);
#endif /* UPNP_MM_H */
