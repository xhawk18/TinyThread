#ifndef INC__MEMORY_POOL_H__
#define INC__MEMORY_POOL_H__

#ifdef __cplusplus
extern "C" {
#endif


#define MEMPOOL_ALIGNMENT	32

#define MEMPOOL_BUFFER_ADDR(addr)	(addr)
#define MEMPOOL_ALIGN_SIZE(size)	(((size) + MEMPOOL_ALIGNMENT - 1) / MEMPOOL_ALIGNMENT * MEMPOOL_ALIGNMENT)


/*
Here's a sample:

typedef struct
{
	...
	LIST_T	list;
	UINT32	ref_count;
	...
} SOME_STRUCT_T;
#define MEMORY_BLOCK_NUM	64

char g_acMemoryBuffer[MEMORYPOOL_SIZE (sizeof (SOME_STRUCT_T), MEMORY_BLOCK_NUM)];

LIST_T *pMemListHeader = memInit (
	g_acMemoryBuffer,
	sizeof (g_acMemoryBuffer),
	sizeof (SOME_STRUCT_T),
	(size_t) &((SOME_STRUCT_T *) 0)->list);

*/


/* Initialize a memory pool by connecting the memory blocks with list structure. */
__INLINE LIST_T *__memInit (void *pBuffer, size_t szBuffer, size_t szEach,
	size_t szListOffset)
{
	if (szBuffer < sizeof (LIST_T))
		return NULL;
	else
	{
		size_t i;
		LIST_T *pListHead = (LIST_T *) pBuffer;
		listInit (pListHead);
	
		szEach = MEMPOOL_ALIGN_SIZE (szEach);
		for (i = MEMPOOL_ALIGN_SIZE (sizeof (LIST_T));
			i + szEach <= szBuffer; i += szEach)
		{
			LIST_T *pListThis = (LIST_T *)((char *) pBuffer + i + szListOffset);
			listInit (pListThis);
			listAttach (pListHead, pListThis);
		}
		return pListHead;
	}
}

#define memInit(pBuffer,szBuffer,TYPE_T) \
	__memInit (pBuffer, szBuffer, sizeof (TYPE_T), (size_t) &((TYPE_T *) 0)->list)


/* Allocate a memory blocks from a list structure. */
__INLINE LIST_T *memNew (void *pBuffer)
{
	LIST_T *pReturn;
	LIST_T *pListHead = (LIST_T *) pBuffer;

	if (pListHead->pNext == pListHead)
	{
		pReturn = NULL;	//No enough buffer.
	}
	else
	{
		pReturn = pListHead->pNext;
		listDetach (pReturn);
	}
	return pReturn;
}


/* Free a memory blocks (to the list structure). */
__INLINE void memDel (void *pBuffer, LIST_T *pListThis)
{
	LIST_T *pListHead = (LIST_T *) pBuffer;
	listAttach (pListHead, pListThis);
}


/* Get the memory block. */
__INLINE LIST_T *memGet (void *pBuffer, int nIndex)
{
	LIST_T *pListHead = (LIST_T *) pBuffer;
	return listGetAt (pListHead, nIndex);
}

/* Get number of memory blocks. */
__INLINE int memNum (void *pBuffer)
{
	LIST_T *pListHead = (LIST_T *) pBuffer;
	return listLength (pListHead);
}



#define MEMORYPOOL_SIZE(struct_size,count) \
( \
	MEMPOOL_ALIGN_SIZE (sizeof (LIST_T)) \
	+ MEMPOOL_ALIGN_SIZE (struct_size) * (count) \
)


#ifdef __cplusplus
} // extern "C"
#endif

#endif

