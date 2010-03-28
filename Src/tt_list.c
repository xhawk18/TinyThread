#include <stdlib.h>
#include "../Inc/InInc/tt_sys.h"
#include "../Inc/InInc/list.h"


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


void listAttach (LIST_T *pNode1, LIST_T *pNode2)
{
	listConnect (pNode1, pNode2);
}

void listDetach (LIST_T *pNode)
{
	listDisconnect (pNode, pNode->pNext);
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


BOOL listIsEmpty (LIST_T *pList)
{
	if (pList->pNext == pList)
		return TRUE;
	else
		return FALSE;
}


