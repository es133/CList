#include "list.h"

/*-----------------------------------------------------------------------------
 * static function declaration
 * --------------------------------------------------------------------------*/
/* head, tail access */
static void* CListGetHead(struct CList *pThis);
static void* CListGetTail(struct CList *pThis);

/* operation */
static POSITION CListAddHead(struct CList *pThis, const void* pData);
static POSITION CListAddTail(struct CList *pThis, const void* pData);
static int CListRemoveHead(struct CList *pThis);
static int CListRemoveTail(struct CList *pThis);
static int CListRemoveAll(struct CList *pThis);

/* for iteration */
static POSITION CListGetHeadPosition(struct CList *pThis);
static POSITION CListGetTailPosition(struct CList *pThis);
static void* CListGetNext(struct CList *pThis, POSITION* position);
static void* CListGetPrev(struct CList *pThis, POSITION* position);

/* retrieval, modification */
static void* CListGetAt(struct CList *pThis, POSITION position);
static int CListRemoveAt(struct CList *pThis, POSITION position);
static int CListSetAt(struct CList *pThis, POSITION position, const void* pData);

/* Insertion */
static POSITION CListInsertNext(struct CList *pThis, POSITION position, const void* pData);
static POSITION CListInsertPrev(struct CList *pThis, POSITION position, const void* pData);

/* Searching */
static POSITION CListFindIndex(struct CList *pThis, int nIndex);

/* Status */
static int CListGetCount(struct CList *pThis);
static int CListIsEmpty(struct CList *pThis);

/*--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Function: InitList
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 * 	- nMaxDataSize : Max size of list element 
 *
 * Return Value:
 *
 * Desc: Initialize list instance, binding list functions, initialize local 
 *       vars.
 *
 *
 *
 * --------------------------------------------------------------------------*/
void InitList(struct CList *pThis, int nMaxDataSize)
{

	pThis->nMaxDataSize = nMaxDataSize;
	pThis->nCount = 0;

	pThis->pHeadNode = NULL;
	pThis->pTailNode = pThis->pHeadNode;

	/* head/tail access */
	pThis->GetHead = CListGetHead;
	pThis->GetTail = CListGetTail;

	/* Operation */
	pThis->AddHead = CListAddHead;
	pThis->AddTail = CListAddTail;
	pThis->RemoveHead = CListRemoveHead;
	pThis->RemoveTail = CListRemoveTail;
	pThis->RemoveAll = CListRemoveAll;

	/* for iteration */
	pThis->GetHeadPosition = CListGetHeadPosition;
	pThis->GetTailPosition = CListGetTailPosition;
	pThis->GetNext = CListGetNext;
	pThis->GetPrev = CListGetPrev;

	/* Retrieval, modification */
	pThis->GetAt = CListGetAt;
	pThis->RemoveAt = CListRemoveAt;
	pThis->SetAt = CListSetAt;

	/* Insertion */
	pThis->InsertNext = CListInsertNext;
	pThis->InsertPrev = CListInsertPrev;

	/* Search */
	pThis->FindIndex = CListFindIndex;

	/* Status */
	pThis->GetCount = CListGetCount;
	pThis->IsEmpty = CListIsEmpty;
}

/*-----------------------------------------------------------------------------
 * Function: DestroyList
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 *
 * Return Value:
 *
 * Desc: remove list instance
 *
 * --------------------------------------------------------------------------*/
void DestroyList(struct CList *pThis) {

	if (pThis == NULL)
		return;
	
	/* remove all list elements*/
	pThis->RemoveAll(pThis);

	/* initialize local var */
	pThis->nCount = 0;

	pThis->pHeadNode = NULL;
	pThis->pTailNode = pThis->pHeadNode;

	/* unbind all member functions */
	/* head/tail access */
	pThis->GetHead = NULL;
	pThis->GetTail = NULL;

	/* Operation */
	pThis->AddHead = NULL;
	pThis->AddTail = NULL;
	pThis->RemoveHead = NULL;
	pThis->RemoveTail = NULL;
	pThis->RemoveAll = NULL;

	/* for iteration */
	pThis->GetHeadPosition = NULL;
	pThis->GetTailPosition = NULL;
	pThis->GetNext = NULL;
	pThis->GetPrev = NULL;

	/* Retrieval, modification */
	pThis->GetAt = NULL;
	pThis->RemoveAt = NULL;
	pThis->SetAt = NULL;

	/* Insertion */
	pThis->InsertNext = NULL;
	pThis->InsertPrev = NULL;

	/* Status */
	pThis->GetCount = NULL;
	pThis->IsEmpty = NULL;
}

/*-----------------------------------------------------------------------------
 * Function: CListGetHead
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 *
 * Return Value:
 *	- head node data
 *
 * Desc: 
 *	- get headnode data pointer
 *
 * --------------------------------------------------------------------------*/
static void* CListGetHead(struct CList *pThis) {

	if (pThis == NULL || pThis->pHeadNode == NULL || pThis->pHeadNode->data == NULL)
		return NULL;

	return pThis->pHeadNode->data;

}
/*-----------------------------------------------------------------------------
 * Function: CListGetTail
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 *
 * Return Value:
 *  - tailnode data 
 *
 * Desc: 
 *	- get tailnode data pointer
 *
 * --------------------------------------------------------------------------*/
static void* CListGetTail(struct CList *pThis) {

	if (pThis == NULL || pThis->pHeadNode == NULL || pThis->pHeadNode->data == NULL)
		return NULL;

	return pThis->pTailNode->data;
}
/*-----------------------------------------------------------------------------
 * Function: CListAddHead 
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 * 	- pData : data insert to list
 *
 * Return Value:
 *  - headnode position
 *
 * Desc: 
 *	- add list element to list head
 *
 * --------------------------------------------------------------------------*/
static POSITION CListAddHead(struct CList *pThis, const void* pData) {

	POSITION	pos;
	ListElem	*pListElem;

	if ((pThis == NULL) || (pData == NULL))
		return NULL;

	pListElem = (ListElem *)calloc(1, sizeof(ListElem));

	if (pListElem == NULL)
		return NULL;

	pListElem->data = calloc(1, pThis->nMaxDataSize);

	if (pListElem->data == NULL)
		return NULL;

	memcpy(pListElem->data, pData, pThis->nMaxDataSize);

	if(pThis->pHeadNode == NULL) 
    {
		pThis->pHeadNode = pListElem;
		pThis->pTailNode = pListElem;
	}
	else 
    {
		pListElem->prev = NULL;
		pThis->pHeadNode->prev = pListElem;
		pListElem->next = pThis->pHeadNode;

		pThis->pHeadNode = pListElem;
	}

	pos = (POSITION)pThis->pHeadNode;
	pThis->nCount++;

	return pos;
}
/*-----------------------------------------------------------------------------
 * Function: CListAddTail
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 * 	- pData : data insert to list
 *
 * Return Value:
 * 	- tail node position
 *
 * Desc: 
 * 	- add list element to list tail 
 *
 * --------------------------------------------------------------------------*/
static POSITION CListAddTail(struct CList *pThis, const void* pData) {

	POSITION	pos;
	ListElem	*pListElem;

	if (pThis == NULL || pData == NULL)
		return NULL;

	pListElem = (ListElem *)calloc(1, sizeof(ListElem));

	if (pListElem == NULL)
		return NULL;

	pListElem->data = calloc(1, pThis->nMaxDataSize);

	if (pListElem->data == NULL)
		return NULL;

	memcpy(pListElem->data, pData, pThis->nMaxDataSize);

	if(pThis->pHeadNode == NULL) 
    {
		pThis->pHeadNode = pListElem;
		pThis->pTailNode = pListElem;
	}
	else 
    {
		pListElem->prev = pThis->pTailNode;
		pThis->pTailNode->next = pListElem;
		pListElem->next = NULL;

		pThis->pTailNode = pListElem;
	}

	pos = (POSITION)pThis->pTailNode;
	pThis->nCount++;
	return pos;
}
/*-----------------------------------------------------------------------------
 * Function: CListRemoveHead
 *
 * Parameter:
 *  - pThis : CList instance pointer (emulates c++ this pointer)
 *
 * Return Value:
 *  - return -1, if list object is null or headnode is not initialized 
 *
 * Desc: remove head node
 *
 * --------------------------------------------------------------------------*/
static int CListRemoveHead(CList *pThis) {

    ListElem    *pListElem;

    if (pThis == NULL)
        return -1;

    if (pThis->pHeadNode == NULL)
        return -1;

    pListElem = pThis->pHeadNode;
    pThis->pHeadNode = pThis->pHeadNode->next;
    
    if(pThis->pHeadNode != NULL)
        pThis->pHeadNode->prev = NULL;

    free(pListElem->data);
    free(pListElem);

    pThis->nCount--;

    return 0;
}

/*-----------------------------------------------------------------------------
 * Function: CListRemoveTail
 *
 * Parameter:
 *  - pThis : CList instance pointer (emulates c++ this pointer)
 *
 * Return Value:
 *  - return -1, if list object is null or headnode is not initialized 
 *
 * Desc: remove tail node
 *
 * --------------------------------------------------------------------------*/
static int CListRemoveTail(CList *pThis) {

    ListElem    *pListElem;

    if (pThis == NULL)
        return -1;

    if (pThis->pHeadNode == NULL)
        return -1;
     
    pListElem = pThis->pTailNode;
    pThis->pTailNode = pThis->pTailNode->prev;

    if(pThis->pTailNode != NULL)
        pThis->pTailNode->next = NULL;

    free(pListElem->data);
    free(pListElem);
    pThis->nCount--;

    return 0;
}


/*-----------------------------------------------------------------------------
 * Function: CListRemoveAll
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 *
 * Return Value:
 * 	- NONE
 *
 * Desc: remove all list elements
 *
 * --------------------------------------------------------------------------*/
static int CListRemoveAll(CList *pThis) {

    if (pThis->pHeadNode == NULL || pThis->nCount == 0)
		return 0;

	while (pThis->pHeadNode != pThis->pTailNode) {
		pThis->RemoveTail(pThis);
	}

    free (pThis->pHeadNode->data);
    free (pThis->pHeadNode);
    pThis->nCount = 0;

	return 0;
}

/*-----------------------------------------------------------------------------
 * Function: CListGetHeadPosition
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 *
 * Return Value:
 * 	- return head node position
 *
 * Desc: 
 * 	- To get headnode position. Using this, you can iterate whole list elements
 *
 * --------------------------------------------------------------------------*/
static POSITION CListGetHeadPosition(CList *pThis) {

	POSITION pos;
	
	if (pThis == NULL)
		return NULL;

	pos = (POSITION)pThis->pHeadNode;

	return pos;
}
/*-----------------------------------------------------------------------------
 * Function: CListGetTailPosition
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 *
 * Return Value:
 * 	- return tail node position
 *
 * Desc: 
 * 	- To get headnode position. Using this, you can iterate whole list elements
 *
 * --------------------------------------------------------------------------*/
static POSITION CListGetTailPosition(CList *pThis) {

	POSITION pos;
	
	if (pThis == NULL)
		return NULL;

	pos = (POSITION)pThis->pTailNode;

	return pos;
}
/*-----------------------------------------------------------------------------
 * Function: CListGetNext
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 * 	- position : position next to current list element
 *
 * Return Value:
 * 	- data pointer next to current element
 *
 * Desc: 
 * 	- get current element's data pointer, returns next element position
 *
 * --------------------------------------------------------------------------*/
static void* CListGetNext(CList *pThis, POSITION* position) {

	ListElem *pListElem;

	if ((pThis == NULL) || position == NULL)
		return NULL;

	pListElem = (ListElem *)*position;

    if (pListElem == NULL)
        return NULL;

	//move position
	*position = (POSITION)pListElem->next;

	//return value
	return pListElem->data;
	
}
/*-----------------------------------------------------------------------------
 * Function: CListGetPrev
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 * 	- position : position previous to current list element
 *
 * Return Value:
 * 	- data pointer previous to current element
 *
 * Desc: 
 * 	- get previous element's data pointer, returns previous element position
 *
 * --------------------------------------------------------------------------*/
static void* CListGetPrev(CList *pThis, POSITION* position) {

	ListElem *pListElem;

	if ((pThis == NULL) || position == NULL)
		return NULL;

	pListElem = (ListElem *)*position;

	//move position
	*position = (POSITION)pListElem->prev;

	//return value
	return pListElem->data;
}
/*-----------------------------------------------------------------------------
 * Function: CListGetAt
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 * 	- position : position want to access.
 *
 * Return Value:
 * 	- data from position designates elements
 *
 * Desc: 
 * 	- get specific element's data pointer which pointed by position var 
 *
 * --------------------------------------------------------------------------*/
static void* CListGetAt(CList *pThis, POSITION position) {

	ListElem *pListElem;

	if ((pThis == NULL) || position == NULL)
		return NULL;

	pListElem = (ListElem *)position;

	//return value
	return pListElem->data;
}
/*-----------------------------------------------------------------------------
 * Function: CListRemoveAt
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 * 	- position : position want to remove.
 *
 * Return Value:
 *  - Return -1 if pThis is NULL. 
 *
 * Desc: remove list element at position
 *
 * --------------------------------------------------------------------------*/
static int CListRemoveAt(CList *pThis, POSITION position) {

	ListElem *pListElem;

	if ((pThis == NULL) || (pThis->nCount == 0) || 
        (pThis->pHeadNode == NULL) || position == NULL )
		return -1;

	pListElem = (ListElem *)position;

    if (pThis->pHeadNode == pListElem)
    {
        pThis->pHeadNode = pListElem->next;
        pListElem->next = NULL;
        pThis->pHeadNode->prev = NULL;

    } 
    else if (pThis->pTailNode == pListElem)
    {
        pThis->pTailNode = pListElem->prev;
        pListElem->prev = NULL;
        pThis->pTailNode->next = NULL;
    }
    else
    {
        pListElem->prev->next = pListElem->next;
        pListElem->next->prev = pListElem->prev;
        pListElem->prev = NULL;
        pListElem->next = NULL;
    }

    free(pListElem->data);
    free(pListElem);
	pThis->nCount--;
	return 0;
}
/*-----------------------------------------------------------------------------
 * Function: CListSetAt
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 * 	- position : list element location which to replce
 * 	- pData : replacing data
 *
 * Return Value:
 * 	- Return -1 if list object , data is null or position points object is null
 * 	and elements data object is null, else returns 0
 *
 * Desc: Replace specific element's data
 *
 * --------------------------------------------------------------------------*/
static int CListSetAt(CList *pThis, POSITION position, const void* pData) {

	ListElem *pListElem;

	if ((pThis == NULL) || (pData == NULL) || position == NULL)
		return -1;

	pListElem = (ListElem *)position;
	if ((pListElem == NULL) || (pListElem->data == NULL))
		return -1;

	memcpy(pListElem->data, pData, pThis->nMaxDataSize);

	return 0;
}
/*-----------------------------------------------------------------------------
 * Function: CListInsertNext
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 * 	- position : list element location which to insert
 * 	- pData : data to be inserted
 *
 * Return Value:
 * 	- Return NULL if list object is null 
 * 	- Return NULL if fails to allcate memory for new list element, element data
 * 	- if succeded, it returns position
 *
 * Desc: 
 * 	- Insert new data to list element next to current position
 *
 * --------------------------------------------------------------------------*/
static POSITION CListInsertNext(CList *pThis, POSITION position, const void* pData) {

	POSITION	pos;
	ListElem 	*pListElem;
	ListElem 	*pListElemPrev;

	if ((pThis == NULL) || (pData == NULL) || position == NULL)
		return NULL;

	pListElem = (ListElem *)calloc(1, sizeof(ListElem));

	if (pListElem == NULL)
		return NULL;

	pListElem->data = calloc(1, pThis->nMaxDataSize);

	if (pListElem->data == NULL)
		return NULL;

	memcpy(pListElem->data, pData, pThis->nMaxDataSize);

	pListElemPrev = (ListElem *)position;

	//if tailnode
	if (pListElemPrev->next == NULL) {
		pos = pThis->AddTail(pThis, pData);	
	}
	else {
		//move position
		pListElem->prev = pListElemPrev;
		pListElem->next = pListElemPrev->next;
		pListElem->next->prev = pListElem;
		pListElemPrev->next = pListElem;
		pos = (POSITION)pListElem;	
	    pThis->nCount++;
	}

	//return value
	return pos;
}
/*-----------------------------------------------------------------------------
 * Function: CListInsertPrev
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 * 	- position : list element location which to insert
 * 	- pData : data to be inserted
 *
 * Return Value:
 * 	- Return NULL if list object is null 
 * 	- Return NULL if fails to allcate memory for new list element, element data
 * 	- if succeded, it returns position
 *
 * Desc: 
 * 	- Insert new data to list element previous to current position
 *
 * --------------------------------------------------------------------------*/
static POSITION CListInsertPrev(CList *pThis,  POSITION position, const void* pData) {

	POSITION	pos;
	ListElem 	*pListElem;
	ListElem 	*pListElemNext;

	if ((pThis == NULL) || (pData == NULL) || position == NULL)
		return NULL;

	pListElem = (ListElem *)calloc(1, sizeof(ListElem));

	if (pListElem == NULL)
		return NULL;

	pListElem->data = calloc(1, pThis->nMaxDataSize);

	if (pListElem->data == NULL)
		return NULL;

	memcpy(pListElem->data, pData, pThis->nMaxDataSize);

	pListElemNext = (ListElem *)position;


	// if headnode
	if (pListElemNext->prev == NULL) 
    {
		pos = pThis->AddHead(pThis, pData);	
	}
	else 
    {
		//move position
		pListElem->next = pListElemNext;
		pListElem->prev = pListElemNext->prev;
		pListElem->prev->next= pListElem;
		pListElemNext->prev = pListElem;
		pos = (POSITION)pListElem;	
        pThis->nCount++;
	}

	//return value
	return pos;
}
/*-----------------------------------------------------------------------------
 * Function: CListFindIndex
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 *
 * Return Value:
 * 	- return positoin nIndex points
 *
 * Desc: 
 *
 * --------------------------------------------------------------------------*/
static POSITION CListFindIndex(struct CList *pThis, int nIndex) {

	POSITION pos = NULL;
	int i = 0;

	if (nIndex < 0 || nIndex >= pThis->nCount) 
    {
		return NULL;
	}

    pos = pThis->GetHeadPosition(pThis);

    for( i = 0 ; i < nIndex ; i++) 
    {
        pThis->GetNext(pThis, &pos);
    }

	return pos;
}
/*-----------------------------------------------------------------------------
 * Function: CListGetCount
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 *
 * Return Value:
 * 	- Return current list elements number
 *
 * Desc: 
 *
 * --------------------------------------------------------------------------*/
static int CListGetCount(CList *pThis) {

	if (pThis == NULL)
		return 0;

	return pThis->nCount;
}
/*-----------------------------------------------------------------------------
 * Function: CListIsEmpty
 *
 * Parameter:
 * 	- pThis : CList instance pointer (emulates c++ this pointer)
 *
 * Return Value:
 * 	- if no list element return 0 else return 1
 *
 * Desc: 
 *
 * --------------------------------------------------------------------------*/
static int CListIsEmpty(CList *pThis) {

	if (pThis == NULL)
		return 0;

	if (pThis->pHeadNode->data != NULL)
		return 1;
	else
		return 0;
}
