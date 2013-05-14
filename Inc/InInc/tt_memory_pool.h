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
TT_INLINE LIST_T *__memInit (
	void *buffer,
	size_t buffer_size,
	size_t each_size,
	size_t list_offset)
{
	if (buffer_size < sizeof (LIST_T))
		return NULL;
	else
	{
		size_t i;
		LIST_T *head = (LIST_T *)buffer;
		listInit (head);
	
		each_size = MEMPOOL_ALIGN_SIZE (each_size);
		for (i = MEMPOOL_ALIGN_SIZE (sizeof (LIST_T));
			i + each_size <= buffer_size; i += each_size)
		{
			LIST_T *node = (LIST_T *)((char *)buffer + i + list_offset);
			listInit (node);
			listAttach (head, node);
		}
		return head;
	}
}

#define memInit(buffer,buffer_size,TYPE_T) \
	__memInit (buffer, buffer_size, sizeof (TYPE_T), (size_t) &((TYPE_T *) 0)->list)


/* Allocate a memory blocks from a list structure. */
TT_INLINE LIST_T *memNew (void *buffer)
{
	LIST_T *node;
	LIST_T *head = (LIST_T *)buffer;

	if (listIsEmpty (head))
	{
		node = NULL;	//No enough buffer.
	}
	else
	{
		node = listGetNext (head);
		listDetach (node);
	}
	return node;
}


/* Free a memory blocks (to the list structure). */
TT_INLINE void memDel (void *buffer, LIST_T *node)
{
	LIST_T *head = (LIST_T *)buffer;
	listAttach (head, node);
}


/* Get the memory block. */
TT_INLINE LIST_T *memGet (void *buffer, int index)
{
	LIST_T *head = (LIST_T *)buffer;
	return listGetAt (head, index);
}

/* Get number of memory blocks. */
TT_INLINE int memNum (void *buffer)
{
	LIST_T *head = (LIST_T *)buffer;
	return listLength (head);
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

