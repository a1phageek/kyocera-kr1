#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 #include <string.h>
#include "../../inc/upnp_mm.h"

#define MAX_ALLOC			1000
#define MAX_TEST_COUNT		50
#define REINIT_COUNT		10
#define DESTROY_COUNT		1000

#define MAX_ALLOCATE_SIZE	6000
#define MAX_NUM				1024	/* for calloc */
#define REALLOC_COUNT		100		/* for realloc */

#define TITLE(expr)	\
	do { \
	printf("\n\n");	\
	printf("\t\t\t\t\t-----<< " #expr " >>-----\n");					\
	printf("\t===============================================================================================\n");	\
	}while (0)
#ifdef _UPNP_MM_DEBUG_
#define SUBTITLE(expr)	\
	do { \
	printf("\n===================================================\n");	\
	printf("\t " #expr " .......\n");					\
	printf("===================================================\n");	\
	}while (0)
#else
#define SUBTITLE(expr)	\
	do { \
		printf(".");	\
		fflush(0);		\
	}while (0)
#endif

#define SUBSUBTITLE(expr)	\
	do { \
	printf("\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");	\
	printf("\t " #expr "\n");					\
	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");	\
	}while (0)

void *my_alloc[MAX_ALLOC];
unsigned long long TotalAlloc, TotalFree, NullAlloc, NullFree, TotalAllocSize;

// free
static inline void fr()
{
	int i;
	for ( i = 0 ; i < MAX_ALLOC ; i ++)
	{
		TotalFree ++;
		if (!my_alloc[i])
		{
			NullFree ++;
		}
		upnp_free(my_alloc[i]);
		my_alloc[i] = NULL;
	}
}

// allocate
static void * al()
{
	int size, i;
	void * retptr = NULL;

	TotalAlloc ++;

	size = (random() % MAX_ALLOCATE_SIZE);

	TotalAllocSize += size;

	retptr = (void *)upnp_malloc(size);

	if (!retptr)
	{
		printf("allocate Warning: out of memory (size : %d)\n", size);
		upnp_mm_dump();
		NullAlloc ++;
	}

	return retptr;
}

// calloc
static void * cal()
{
	int i, size, num, one_size;
	void * retptr = NULL;
	void * ptr;

	TotalAlloc ++;

calloc_start:
	size = (random() % MAX_ALLOCATE_SIZE);
	num = (random() % MAX_NUM);


	if (!num)
		one_size = size;
	else if (size < num)
		one_size = 0;
	else
		one_size = size / num;

	retptr = (void *)upnp_calloc(num, one_size);

	size = (num * one_size);

	TotalAllocSize += size;

	/* check phase */
	if (!retptr)
	{
		printf("callocate Warning: out of memory (size : %d)\n", size);
		upnp_mm_dump();
		NullAlloc ++;
	} else
	{
		if (size)
		{
			ptr = retptr;
			for (i = 0; i < size; i ++, ptr ++)
			{
				if (*((char*)(ptr)) != 0)
				{
					printf("Error: calloc doesn't clean allocated memory (%d) ! (size : [%d * %d])\n", *(char*)ptr, num, one_size);
					upnp_mm_dump();
					exit(1);
				}
			}
		}
	}
	return retptr;
}

// realloc
static void * real()
{
	int j,size, oldsize;
	void * current = NULL;

	oldsize = 0;

	for (j = 0 ; j < REALLOC_COUNT ; j ++)
	{
		TotalAlloc ++;

		size = (random() % MAX_ALLOCATE_SIZE);

		TotalAllocSize += size;

		if (oldsize && current)
		{
			memset(current, 'A', oldsize);
		}

		current = (void *)upnp_realloc(current, size);

		if (current && size && oldsize)
		{
			void *ptr = current;
			int c;
			int m = ((size > oldsize)?oldsize:size);
			for (c = 0 ; c <m ; c ++, ptr ++)
			{
				if (*((char*)ptr) != 'A')
				{
					printf("strdup error ! duplicated character error : %c [expected: %c]\n", *((char*)ptr), 'A');
					upnp_mm_dump();
					exit(1);
				}
			}
		}

		if ((!current) && size)
		{
			printf("reallocate Warning: out of memory (size : %d)\n", size);
			upnp_mm_dump();
			NullAlloc ++;
		}

		oldsize = size;
	}
	return current;
}

// strdup
static void * sdu()
{
	int size = 0;
	char *temp = NULL;
	char *retptr = NULL;

	while (!size)
	{
		size = (random() % MAX_ALLOCATE_SIZE);
	}

	TotalAlloc ++;

	TotalAllocSize += size;

	temp = (char *)upnp_malloc(size);

	if (!temp)
	{
		if (size)
		{
			printf("allocate Warning: out of memory (size : %d)\n", size);
			upnp_mm_dump();
			NullAlloc ++;
		}
		return NULL;
	}

	bzero(temp, size);
	memset((void *)temp, 'A', (size - 1));

	TotalAlloc ++;

	retptr = upnp_strdup(temp);

	upnp_free(temp);

	/* check phase */
	if (retptr)
	{
		char *ptr = retptr;
		int i;
		int slen = strlen(retptr);

		TotalAllocSize += size;

		if (slen != (size - 1))
		{
			printf("strdup error ! duplicated string length : %d [expected: %d]\n", slen, (size - 1));
			upnp_mm_dump();
			exit(1);
		}

		if (slen)
		{
			for (i = 0 ; i < slen ; i ++, ptr ++)
			{
				if ((*ptr) != 'A')
				{
					printf("strdup error ! duplicated character error : %c [expected: %c]\n", *ptr, 'A');
					upnp_mm_dump();
					exit(1);
				}
			}
		}
		
	}else
	{
		printf("strdup Warning: out of memory (size : %d)\n", size);
		upnp_mm_dump();
		NullAlloc ++;
	}

	return retptr;
}

static inline void allo()
{
	/*		 malloc/free		*/

	int i;

	SUBTITLE(malloc);

	/* allocation phase */
	for ( i = 0 ; i < MAX_ALLOC ; i ++)
		my_alloc[i] = al();
	/* free phase */
	fr();
}

static inline void callo()
{
	/*		calloc/free		*/

	int i;

	SUBTITLE(calloc);

	/* allocation phase */
	for (i = 0 ; i < MAX_ALLOC ; i ++)
		my_alloc[i] = cal();
	/* free phase */
	fr();
}

static inline void reallo()
{
	/*		realloc/free		*/

	int i;

	SUBTITLE(realloc);

	/* allocation phase */
	for (i = 0 ; i < MAX_ALLOC ; i ++)
		my_alloc[i] = real();
	/* free phase */
	fr();
}

static inline void sdup()
{
	/*		duplicate string	*/
	int i;

	SUBTITLE(strdup);

	/* allocation phase */
	for (i = 0 ; i < MAX_ALLOC ; i ++)
		my_alloc[i] = sdu();
	/* free phase */
	fr();
}


static inline void hybrid()
{
	/*		hybrid test		*/

	int i, j;

	SUBTITLE(hybrid);

	/* allocation phase */
	for (i = 0 ; i < MAX_ALLOC ; i ++)
	{
		j = (random() % 4);

		switch(j)
		{
			case 0:	/* alloc */
				my_alloc[i] = al();
			break;
			case 1: /* calloc */
				my_alloc[i] = cal();
			break;
			case 2: /* strdup */
				my_alloc[i] = sdu();
			break;
			default: /* realloc */
				my_alloc[i] = real();
			break;
		}
	}
	/* free phase */
	fr();
}

static inline void ran()
{
	/*		random test		*/

	int i, j, r;
	int total_free;

	SUBTITLE(random);

	for (i = 0 ; i < MAX_ALLOC ; i ++)
	{
		if (my_alloc[i])
		{
			printf("Warning! allocate buffer[%d] is not clear !\n", i);
			upnp_free(my_alloc[i]);
			my_alloc[i] = NULL;
		}
	}

	/* start random free/alloc phase */
	total_free = 0;
	while (total_free < MAX_ALLOC)
	{
		r = (random() % MAX_ALLOC);

		if (my_alloc[r])
		{
			total_free ++;
			upnp_free(my_alloc[r]);
			my_alloc[r] = NULL;
		}else
		{
			j = (random() % 4);

			switch(j)
			{
				case 0:	/* alloc */
					my_alloc[r] = al();
				break;
				case 1: /* calloc */
					my_alloc[r] = cal();
				break;
				case 2: /* strdup */
					my_alloc[r] = sdu();
				break;
				default: /* realloc */
					my_alloc[r] = real();
				break;
			}
		}
	}
	/* free phase */
	for (i = 0 ; i < MAX_ALLOC ; i ++)
	{
		if (my_alloc[i])
		{
			upnp_free(my_alloc[i]);
			my_alloc[i] = NULL;
		}
	}
}


int main()
{
	int idx, idx2;
	int cnt[10] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

	srandom(time(NULL));

	/* reset counter */

	TotalAlloc		= 0;
	TotalFree		= 0;
	NullAlloc			= 0;
	NullFree			= 0;
	TotalAllocSize	= 0;

	TITLE(Test Start);

	for (idx = 0 ; idx < DESTROY_COUNT ; idx ++)
	{
		int i;

		/* Init mm spool */
		for (i = 0 ; i < 10 ; i ++)
			cnt [i] = (random() % 100);

		/* Init */
		upnp_mm_init(cnt);

		SUBSUBTITLE(Start Round......);

		/* Dump */
		upnp_mm_dump();


		for (idx2 = 0 ; idx2 < REINIT_COUNT ; idx2 ++)
		{
			int i;

			SUBSUBTITLE(Reinit mm system......);
			
			bzero(my_alloc, (sizeof(void *) * MAX_ALLOC));

			for (i = 0 ; i < MAX_TEST_COUNT ; i ++)
			{
				allo();
				callo();
				reallo();
				sdup();
				hybrid();
				ran();
			}
			/* Reinit */
			upnp_mm_reinit();
		}

		SUBSUBTITLE(One Round Complete......);

		/* Dump */
		upnp_mm_dump();

		/* Destroy */
		upnp_mm_destroy();
	}

	/* Dump counter */
	TITLE(Test Complete);
	printf("\tTotalAlloc\t(Total Allocation Count)\t:\t%u\n", TotalAlloc);
	printf("\tTotalFree\t(Total Free Count)\t\t:\t%u\n", TotalFree);

	printf("\tNullAlloc\t(Total Count of out-of-memory)\t:\t%u", NullAlloc);
	if (NullAlloc)
		printf("\t(This COULD cause daemon stop!)\n");
	else
		printf("\n");
	
	printf("\tNullFree\t(Free with the NULL pointer)\t:\t%u\n", NullFree);
	printf("\tTotalAllocSize\t(Total Allocated memory size)\t:\t%u\n", TotalAllocSize);
	printf("\n");
	return 0;
}
