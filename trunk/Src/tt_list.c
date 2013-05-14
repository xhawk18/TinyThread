#include "../Inc/tt_thread.h"

void listInit (LIST_T *list)
{
	listSetPrev (list, list);
	listSetNext (list, list);
#ifdef DEBUG_DUMP_LIST
	{
		static int index = 0;
		list->index = index++;
	}
#endif
}

/* Connect or disconnect two lists. */
void listToggleConnect (LIST_T *list1, LIST_T *list2)
{
	LIST_T *prev1 = listGetPrev (list1);
	LIST_T *prev2 = listGetPrev (list2);
	listSetNext (prev1, list2);
	listSetNext (prev2, list1);
	listSetPrev (list1, prev2);
	listSetPrev (list2, prev1);
}

/* Swap with previous node */
void listSwapBefore (LIST_T *node)
{
	LIST_T *prev = listGetPrev (node);
	LIST_T *next = listGetNext (node);
	if (prev != next)
	{
		LIST_T *prev_prev = listGetPrev (prev);
		listSetPrev (node, prev_prev);
		listSetNext (prev_prev, node);
		listSetNext (prev, next);
		listSetPrev (next, prev);
		listSetNext (node, prev);
		listSetPrev (prev, node);
	}
}


void listMove (LIST_T *list, LIST_T *node)
{
	listDetach (node);
	listAttach (list, node);
}


void listForEach (LIST_T *list, bool (*func) (LIST_T *node, void *arg), void *arg)
{
	LIST_T *node;
	for (node = listGetNext (list); node != list; node = listGetNext (node))
	{
		if ((*func) (node, arg) != false)
			break;
	}
}


/* Insert node into list,
   list should has already been sorted by (*bin_comp)(...),
 */
void listInsert (LIST_T *list, LIST_T *node, bool (*bin_comp) (LIST_T *p1st, LIST_T *p2nd, void *arg), void *arg)
{
	LIST_T *next;
	for (next = listGetNext (list); next != list; next = listGetNext (next))
	{
		if ((bin_comp) (node, next, arg) != false)
			break;
	}
	listDetach (node);
	listAttach (node, next);
}


/* Get length of list */
int listLength (LIST_T *list)
{
	LIST_T	*node;
	int		i = 0;
	for (node = listGetNext (list); node != list; node = listGetNext (node))
		i++;
	return i;
}


/* Get element at position index */
LIST_T *listGetAt (LIST_T *list, int index)
{
	LIST_T *node;

	if (index < 0)
		return NULL;
	
	for (node = listGetNext (list); node != list; node = listGetNext (node))
	{
		if (index-- == 0)
			return node;
	}

	return NULL;
}


#ifdef DEBUG_DUMP_LIST
void listDump (LIST_T *list)
{
	LIST_T *node;
	sysSafePrintf ("List: %d,%08x,%08x,%08x[ ", list->index, list, listGetPrev (list), listGetNext (list));
	for (node = listGetNext (list); node != list; node = listGetNext (node))
		sysSafePrintf ("%d,%08x,%08x,%08x, ", node->index, node, listGetPrev (node), listGetNext (node));
	sysSafePrintf ("]\n");
}
#endif

