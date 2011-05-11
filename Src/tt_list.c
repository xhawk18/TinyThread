#if 0	// Move all codes to tt_list.h
#include <stdlib.h>
#include <stdbool.h>
#include "../Inc/InInc/tt_list.h"





void listInit (LIST_T *pList)
{
	pList->pPrev = pList;
	pList->pNext = pList;
#ifdef DEBUG_DUMP_LIST
	{
		static int index = 0;
		pList->index = index++;
	}
#endif
}


/* Connect two lists. */
void listConnect (LIST_T *pList1, LIST_T *pList2)
{
	LIST_T *pPrev1 = pList1->pPrev;
	LIST_T *pPrev2 = pList2->pPrev;
	pPrev1->pNext = pList2;
	pPrev2->pNext = pList1;
	pList1->pPrev = pPrev2;
	pList2->pPrev = pPrev1;
}


/* Disconnect tow lists. */
void listDisconnect (LIST_T *pList1, LIST_T *pList2)
{
	LIST_T *pPrev1 = pList1->pPrev;
	LIST_T *pPrev2 = pList2->pPrev;
	pPrev1->pNext = pList2;
	pPrev2->pNext = pList1;
	pList1->pPrev = pPrev2;
	pList2->pPrev = pPrev1;
}


void listSwapBefore (LIST_T *pNode)
{
	if (pNode->pPrev != pNode->pNext)
	{
		LIST_T *pPrev = pNode->pPrev;
		pNode->pPrev		= pPrev->pPrev;
		pPrev->pPrev->pNext	= pNode;
		pPrev->pNext		= pNode->pNext;
		pNode->pNext->pPrev	= pPrev;
		pNode->pNext = pPrev;
		pPrev->pPrev = pNode;
	}
}

void listAttach (LIST_T *pNode1, LIST_T *pNode2)
{
	listConnect (pNode1, pNode2);
}

void listDetach (LIST_T *pNode)
{
	listDisconnect (pNode, pNode->pNext);
}


/* Move pNode to pList, after moving,
   pNode->pNext == pList
   pList->pPrev == pNode
 */
void listMove (LIST_T *pList, LIST_T *pNode)
{
	listDetach (pNode);
	listAttach (pList, pNode);
}


void listForEach (LIST_T *pList, bool (*fnChecked)(LIST_T *pNode, void *pArg), void *pArg)
{
	LIST_T *pNode;
	for (pNode = pList->pNext; pNode != pList; pNode = pNode->pNext)
	{
		if ((*fnChecked)(pNode, pArg) != false)
			break;
	}
}


/* Insert pNode into pList,
   pList should has already been sorted by (*bin_comp)(...),
 */
void listInsert (LIST_T *pList, LIST_T *pNode, bool (*bin_comp)(LIST_T *p1st, LIST_T *p2nd, void *pArg), void *pArg)
{
	LIST_T *pPrev;
	for (pPrev = pList->pPrev; pPrev != pList; pPrev = pPrev->pPrev)
	{
		if ((bin_comp) (pNode, pPrev, pArg) != false)
			break;
	}
	listDetach (pNode);
	listAttach (pNode, pPrev);
}
int listLength (LIST_T *pList)
{
	LIST_T *pNode;
	int i = 0;
	for (pNode = pList->pNext; pNode != pList; pNode = pNode->pNext)
		i++;
	return i;
}


LIST_T *listGetAt (LIST_T *pList, int nIndex)
{
	LIST_T *pNode;

	if (nIndex < 0)
		return NULL;
	
	for (pNode = pList->pNext; pNode != pList; pNode = pNode->pNext)
	{
		if (nIndex-- == 0)
			return pNode;
	}

	return NULL;
}


bool listIsEmpty (LIST_T *pList)
{
	if (pList->pNext == pList)
		return true;
	else
		return false;
}

#ifdef DEBUG_DUMP_LIST
static void listDump (LIST_T *pList)
{
	LIST_T *p;
	sysSafePrintf ("List: %d [ ", pList->index);
	for (p = pList->pNext; p != pList; p = p->pNext)
		sysSafePrintf ("%d ", p->index);
	sysSafePrintf ("]\n");
}
#endif

#endif
