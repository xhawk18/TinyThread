#ifndef INC__LIST_H__
#define INC__LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TT_RAM_PTRDIFF_T
	#undef	TT_RAM_PTR_IGNORE_BIT
	#undef	TT_RAM_START_ADDR
	#define	TT_RAM_PTRDIFF_T		ptrdiff_t
	#define	TT_RAM_PTR_IGNORE_BIT	0
	#define	TT_RAM_START_ADDR		0
#endif

//#define DEBUG_DUMP_LIST
struct tagList;
typedef struct tagList
{
	TT_RAM_PTRDIFF_T next;
	TT_RAM_PTRDIFF_T prev;
#ifdef DEBUG_DUMP_LIST
	int index;
#endif
} LIST_T;


#define GetOffsetSize(tParent,eMyName) \
	((int)&((tParent *)0)->eMyName)

#define GetParentAddr(pMe,tParent,eMyName) \
	((tParent *)((char *)(pMe) - (int)&((tParent *)0)->eMyName))

TT_INLINE LIST_T *listGetPrev (LIST_T *list)
{
	//return list->prev;
	return (LIST_T *)(TT_RAM_START_ADDR + (list->prev << TT_RAM_PTR_IGNORE_BIT));
}

TT_INLINE LIST_T *listGetNext (LIST_T *list)
{
	//return list->next;
	return (LIST_T *)(TT_RAM_START_ADDR + (list->next << TT_RAM_PTR_IGNORE_BIT));
}

TT_INLINE void listSetPrev (LIST_T *list, LIST_T *other)
{
	//list->prev = other;
	list->prev = ((uint32_t)other - TT_RAM_START_ADDR) >> TT_RAM_PTR_IGNORE_BIT;
}

TT_INLINE void listSetNext (LIST_T *list, LIST_T *other)
{
	//list->next = other;
	list->next = ((uint32_t)other - TT_RAM_START_ADDR) >> TT_RAM_PTR_IGNORE_BIT;
}

/* Initilization a list */
void listInit (LIST_T *list);

/* Connect or disconnect two lists. */
void listToggleConnect (LIST_T *list1, LIST_T *list2);

/* Connect two lists. */
TT_INLINE void listConnect (LIST_T *list1, LIST_T *list2)
{
	listToggleConnect (list1, list2);
}

/* Disconnect tow lists. */
TT_INLINE void listDisconnect (LIST_T *list1, LIST_T *list2)
{
	listToggleConnect (list1, list2);
}

/* Swap with previous node */
void listSwapBefore (LIST_T *node);

/* Same as listConnect */
TT_INLINE void listAttach (LIST_T *node1, LIST_T *node2)
{
	listConnect (node1, node2);
}

/* Make node in detach mode */
TT_INLINE void listDetach (LIST_T *node)
{
	listDisconnect (node, listGetNext(node));
}

/* Move node to list, after moving,
   node->next == list
   list->prev == node
 */
void listMove (LIST_T *list, LIST_T *node);

void listForEach (LIST_T *list, bool (*func)(LIST_T *node, void *arg), void *arg);

/* Insert node into list,
   list should has already been sorted by (*bin_comp)(...),
 */
void listInsert (LIST_T *list, LIST_T *node, bool (*bin_comp)(LIST_T *p1st, LIST_T *p2nd, void *arg), void *arg);

/* Get length of list */
int listLength (LIST_T *list);

/* Get element at position index */
LIST_T *listGetAt (LIST_T *list, int index);

/* Check if list is empty */
TT_INLINE bool listIsEmpty (LIST_T *list)
{
	return (listGetNext(list) == list);
}


#ifdef DEBUG_DUMP_LIST
void listDump (LIST_T *list);
#endif


#ifdef __cplusplus
} // extern "C"
#endif

#endif
