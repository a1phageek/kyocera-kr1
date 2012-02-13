/*********************************************************************
	by chenyl:
		generate a pseudo-layer to maintain malloc of Upnp daemon
**********************************************************************/
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <semaphore.h>
#include "upnp_mm.h"

#define UPNP_MM_LEVEL		10

static heap_t *systemHeap					= NULL;
static void *_heapHdr[UPNP_MM_LEVEL]		= {NULL};
static int heapTotalSize[UPNP_MM_LEVEL]		= {0};
static int heapSize[UPNP_MM_LEVEL]			= {4, 8, 16, 64, 128, 256, 512, 1024, 2048, 4096};

static heap_t *heapPtr[UPNP_MM_LEVEL]		= {NULL};
static int heapUseCount[UPNP_MM_LEVEL]		= {0};

static sem_t sem;

/**********************************************************************
	Function:
		upnp_mm_init()
	Comment:
		Initiate upnp mm system, allocate memory and construct linked list.
**********************************************************************/
int upnp_mm_init(int cnt[])
{
	int idx;

	if (sem_init(&sem, 0, 1) == -1)
	{
		printf("upnp FATAL: Cannt create initiate semaphore!\n");
	}

	sem_wait(&sem);

	for (idx = 0 ; idx < UPNP_MM_LEVEL ; idx ++)
	{
		int totalSize;
		int i;
		heap_t *hp;

		totalSize = (sizeof(heap_t) + heapSize[idx]) * cnt[idx];

		if (!totalSize)
			continue;

		if (!(_heapHdr[idx] = malloc(totalSize)))
		{
			printf("upnp FATAL: Out of memory\n");
			upnp_mm_destroy();
			sem_post(&sem);
			return FAILED;
		}
		heapTotalSize[idx] = cnt[idx];
		heapPtr[idx] = (heap_t *)_heapHdr[idx];

		hp = heapPtr[idx];
		for (i = 0 ; i < cnt[idx] ; i ++)
		{
			hp->head = &(heapPtr[idx]);

			if (i < (cnt[idx] - 1))
			{
				hp->next = (heap_t *)((uint)hp + (uint)(sizeof(heap_t) + heapSize[idx]));
				hp = hp->next;
			}else
			{	// final entry
				hp->next = NULL;
			}
		}
		heapUseCount[idx] = 0;
	}

	sem_post(&sem);
	return SUCCESS;
}
/**********************************************************************
	Function:
		upnp_mm_destroy()
	Comment:
		Free all allocated memory and destruct mm sytem
		mm system need (upnp_mm_init()) to re-construct it.
**********************************************************************/
int upnp_mm_destroy(void)
{
	int idx;

	sem_destroy(&sem);

	for (idx = 0 ; idx < UPNP_MM_LEVEL ; idx ++)
	{
		if (heapTotalSize[idx])
		{
			assert(_heapHdr[idx]);
			free(_heapHdr[idx]);
			heapTotalSize[idx] = 0;
			heapUseCount[idx] = 0;
		}
	}
	return SUCCESS;
}
/**********************************************************************
	Function:
		upnp_mm_reinit()
	Comment:
		clear all entries in mm system instead of free it.
**********************************************************************/
int upnp_mm_reinit(void)
{
	int idx;

	sem_wait(&sem);

	for (idx = 0 ; idx < UPNP_MM_LEVEL ; idx ++)
	{
		if (heapUseCount[idx])
		{
			heap_t *	hp;
			int		i;
			assert(_heapHdr[idx]);
			assert(heapTotalSize[idx]);

			heapPtr[idx] = (heap_t *)_heapHdr[idx];

			hp = heapPtr[idx];
			for (i = 0 ; i < heapTotalSize[idx] ; i ++)
			{
				hp->head = &(heapPtr[idx]);

				if (i < (heapTotalSize[idx] - 1))
				{
					hp->next = (heap_t *)((uint)hp + (uint)(sizeof(heap_t) + heapSize[idx]));
					hp = hp->next;
				}else
				{	// final entry
					hp->next = NULL;
				}
			}
			heapUseCount[idx] = 0;
		}
	}

	sem_post(&sem);
	
	return SUCCESS;
}
/**********************************************************************
	Function:
		upnp_calloc()
	Comment:
		reallocate function
**********************************************************************/
void *upnp_realloc(void *ptr, int size)
{ 
	if (!ptr)
	{
		return upnp_malloc(size);
	}else
	{
		// ptr != NULL
		if (!size)
		{
			upnp_free(ptr);
			return NULL;
		}else
		{	// ptr != NULL, size != 0
			heap_t *hp = NULL;

			if ((uint)ptr < sizeof(heap_t))
			{
				goto sys_realloc;
			}

			hp = (heap_t *)((uint)ptr - sizeof(heap_t));

			if (hp->head == &systemHeap)
			{
				void *retptr = NULL;
				if (hp->next != systemHeap)
				{
					goto fatal_realloc;
				}
				// call system realloc if the allocated memory is from system
				retptr =  (void *)((uint)realloc((void *)hp, (size + sizeof(heap_t))));
				if (retptr)
				{
					((heap_t*)retptr)->head = &systemHeap;
					((heap_t*)retptr)->next = systemHeap;
					retptr += sizeof(heap_t);
				}
				return retptr;
			}else
			{
				int idx;

				for (idx = 0 ; idx < UPNP_MM_LEVEL ; idx ++)
				{
					if (hp->head == &heapPtr[idx])	// bingo !
					{
						if (size > heapSize[idx])
						{	// we must change to new heap for this allocation
							void *retptr = NULL;
							retptr = upnp_malloc(size);
							if (retptr)
							{
								memcpy(retptr, ptr, heapSize[idx]);
								upnp_free(ptr);
							}
							return retptr;
						}else if ((idx > 0) && (size <= heapSize[idx - 1]))
						{
							void *retptr = NULL;
							retptr = upnp_malloc(size);
							if (retptr)
							{
								memcpy(retptr, ptr, size);
								upnp_free(ptr);
								return retptr;
							}
							// cannot allocate memory: return ptr
						}
						return ptr;
					}
				}
			}
	fatal_realloc:
			printf("upnp FATAL: reallocate memory (0x%X) for unknown memory!\n", (uint)ptr);
	sys_realloc:
			return realloc(ptr, size);
		}
	}
	// error: return NULL
	assert(0);
	return NULL;
}
/**********************************************************************
	Function:
		upnp_calloc()
	Comment:
		array allocate function, clear memory before return
**********************************************************************/
void *upnp_calloc(int nmemb, int size)
{
	void *retPtr = NULL;
	int totalSize;

	totalSize = nmemb * size;

	retPtr = upnp_malloc(totalSize);
	if (retPtr && totalSize)
		bzero(retPtr, totalSize);
	
out:
	return retPtr;
}

/**********************************************************************
	Function:
		upnp_malloc()
	Comment:
		allocate function
**********************************************************************/
void *upnp_malloc(int size)
{
	int idx;
	void *retPtr = NULL;

	sem_wait(&sem);

	for (idx = 0 ; idx < UPNP_MM_LEVEL ; idx ++)
	{
		if (size < heapSize[idx])
		{
			if (heapUseCount[idx] < heapTotalSize[idx])
			{
				retPtr = (void *)((uint)heapPtr[idx] + sizeof(heap_t));
				heapPtr[idx] = heapPtr[idx]->next;
				heapUseCount[idx] ++;
				goto out;
			}else
			{
#ifdef _UPNP_MM_DEBUG_
				printf("upnp Warning: heap size[%d] allocated not enough\n", heapSize[idx]);
#endif
				continue;
			}
		}
	}
	// size is too large, so we cannot allocate memory in mm system, allocate it from
#ifdef _UPNP_MM_DEBUG_
	printf("upnp Warning: Allocate new memory (size: %d) instead of Upnp mm system managed...\n", size);
#endif
	retPtr = malloc(sizeof(heap_t) + size);
	if (retPtr)
	{
		((heap_t*)retPtr)->head = &systemHeap;
		((heap_t*)retPtr)->next = systemHeap;
		retPtr += sizeof(heap_t);
	}
out:
	sem_post(&sem);
	
	return retPtr;
}
/**********************************************************************
	Function:
		upnp_free()
	Comment:
		free function
**********************************************************************/
void upnp_free(void *ptr)
{
	heap_t *hp;

	sem_wait(&sem);

	if (!ptr)
		goto out;

	if ((uint)ptr <= sizeof(heap_t))
		goto miss_free;
	
	hp = (heap_t *)((uint)ptr - sizeof(heap_t));

	if (hp->head != &systemHeap)
	{	// allocated from upnp mm system
		int idx;
		for (idx = 0 ; idx < UPNP_MM_LEVEL ; idx ++)
		{
			if (hp->head == &(heapPtr[idx]))	// bingo!
			{
				hp->next = heapPtr[idx];
				heapPtr[idx] = hp;
				heapUseCount[idx] --;
				goto out;
			}
		}
miss_free:
		// can NOT find head of this freed pointer -> FATAL error!
		printf("upnp FATAL: freed memory[%X] misses allocate source!\n", (uint)ptr);
		// try to free this ptr anyway
		free(ptr);
	}else
	{	//allocated from malloc, free it
		if (hp->next != systemHeap)
			goto miss_free;	// "next" field != systemHeap -> error
#ifdef _UPNP_MM_DEBUG_
		printf("upnp : free memory allocated from system...\n");
#endif
		free((void *)hp);
	}
out:
	sem_post(&sem);
	return;
}
/**********************************************************************
	Function:
		upnp_mm_dump()
	Comment:
		function to dump mm information
**********************************************************************/
void upnp_mm_dump()
{
	int idx;

	sem_wait(&sem);

	printf("\tSize\tTotal\tUsed\n");
	printf("=============================\n");
	for (idx = 0 ; idx < UPNP_MM_LEVEL ; idx ++)
	{
		if (heapTotalSize[idx])
		{
			printf("\t%d\t%d\t%d\n", heapSize[idx], heapTotalSize[idx], heapUseCount[idx]);
		}
	}
	printf("=============================\n");

	sem_post(&sem);
}
/**********************************************************************
				String Related Functions
**********************************************************************/

/**********************************************************************
	Function:
		upnp_strdup()
	Comment:
		duplicate string
**********************************************************************/

char *upnp_strdup(const char *s)
{
	int len;
	char * ret = NULL;

	if (!s)
		return NULL;

	len = strlen(s);
	len += 1;

	ret = upnp_malloc(len);
	if (ret)
	{
		strcpy(ret, s);
	}
	return ret;
}

