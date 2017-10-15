/******************************************************************************
    Copyright (c) <2013> <bugshot>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

struct _POSITION {};

typedef struct _POSITION*	POSITION;

typedef struct _ListElem {

	void 	*data;
	struct _ListElem	*next;
	struct _ListElem	*prev;
	
}ListElem;

typedef struct CList {

	int		nCount;
	int		nMaxDataSize;
	
	ListElem	*pHeadNode;
	ListElem	*pTailNode;

	/* head,tail access */
	void* (*GetHead)(struct CList *pThis);
	void* (*GetTail)(struct CList *pThis);

	/* Operation */
	POSITION (*AddHead)(struct CList *pThis, const void* pData);
	POSITION (*AddTail)(struct CList *pThis, const void* pData);
	int (*RemoveHead)(struct CList *pThis);
	int (*RemoveTail)(struct CList *pThis);
	int (*RemoveAll)(struct CList *pThis);

	/* for iteration */
	POSITION (*GetHeadPosition)(struct CList *pThis);
	POSITION (*GetTailPosition)(struct CList *pThis);
	void* (*GetNext)(struct CList *pThis, POSITION* position);
	void* (*GetPrev)(struct CList *pThis, POSITION* position);

	/* Retrieval, modification */
	void* (*GetAt)(struct CList *pThis, POSITION position);
	int (*RemoveAt)(struct CList *pThis, POSITION position);
	int (*SetAt)(struct CList *pThis, POSITION position, const void* pData);

	/* Insertion */
	POSITION (*InsertNext)(struct CList *pThis, POSITION position, const void* pData);
	POSITION (*InsertPrev)(struct CList *pThis, POSITION position, const void* pData);

	/* Searching */
	POSITION (*FindIndex)(struct CList *pThis, int nIndex);

	/* Status */
	int (*GetCount)(struct CList *pThis);
	int (*IsEmpty)(struct CList *pThis);

} CList;

void InitList(struct CList *pThis, int nMaxDataSize);
void DestroyList(struct CList *pThis);

#endif
