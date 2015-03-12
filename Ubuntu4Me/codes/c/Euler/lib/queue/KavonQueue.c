/*
 * =====================================================================================
 *
 *       Filename:  KavonQueue.c
 *
 *    Description:  define operations of queue
 *
 *        Version:  1.0
 *        Created:  2013年06月05日 14时02分37秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kavon Ma (mn), kavon.ma@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <assert.h>
#include "KavonQueue.h"

bool InitKavonQueue(KavonQueuePtr Q)
{
	Q->head = Q->tail = (QNodePtr)malloc(sizeof(QNode_t));
	if (Q->head == NULL)
	{
		return FALSE;
	}
	else
	{
		Q->head->next = NULL;
		return TRUE;
	}
}

void DestroyKavonQueue(KavonQueuePtr Q)
{
	assert(Q != NULL);

	while (Q->head)
	{
		Q->tail = Q->head->next;
		free(Q->head);
		Q->head = Q->tail;
	}
}

bool EmptyKavonQueue(KavonQueuePtr Q)
{
	assert(Q->head != NULL && Q->tail !=NULL);

	if (Q->head == Q->tail)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int KavonQueueLength(KavonQueuePtr Q)
{
	assert(Q->head != NULL);

	QNodePtr p = Q->head;
	int length = 0;

	while (p != Q->tail)
	{
		length++;
		p = p->next;
	}

	return length;
}

bool InsertKavonQueue(KavonQueuePtr Q, ElemType e)
{
	QNodePtr temp = (QNodePtr)malloc(sizeof(QNode_t));
	if (temp == NULL)
	{
		return FALSE;
	}
	else
	{
		temp->data = e;
		temp->next = NULL;
		Q->tail->next = temp;
		Q->tail = temp;
		return TRUE;
	}
}

bool GetHead(KavonQueuePtr Q, ElemType *e)
{
	assert(Q->head != NULL);

	if (EmptyKavonQueue(Q))
	{
		return FALSE;
	}
	else
	{
		*e = Q->head->next->data; /* Attention! Q->head->next->data */
		return TRUE;
	}
}

void KavonQueueTraverse(KavonQueuePtr Q, void (*fp)(ElemType))
{
	assert(Q->head != NULL);

	QNodePtr p = Q->head->next;
	while (p)
	{
		(*fp)(p->data);
		p = p->next;
	}
}

void ClearKavonQueue(KavonQueuePtr Q)
{
	assert(Q->head != NULL);
	QNodePtr p = Q->head->next;

	while (p)
	{
		Q->head->next = p->next;
		free(p);
		p = Q->head->next;
	}
}

bool RemoveHead(KavonQueuePtr Q, ElemType *e)
{
	if (Q->head == Q->tail)
	{
		return FALSE;
	}
	else
	{
		QNodePtr temp = Q->head->next;
		*e = temp->data;
		Q->head->next = temp->next;

		if (temp == Q->tail)
		{
			Q->tail = Q->head;
		}

		free(temp);
		return TRUE;
	}
}
