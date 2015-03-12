/*
 * =====================================================================================
 *
 *       Filename:  KavonQueue.h
 *
 *    Description:  operations of queue includes
 *
 *        Version:  1.0
 *        Created:  2013年06月05日 13时51分51秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kavon Ma (mn), kavon.ma@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 *
 * =====================================================================================
 */

#ifndef KAVONQUEUE_H
#define KAVONQUEUE_H

#include "ElemType.h"

typedef struct QNode
{
	ElemType data;
    struct QNode *next;
}QNode_t, *QNodePtr;

typedef struct KavonQueue
{
	QNodePtr head;
	QNodePtr tail;
}*KavonQueuePtr;

bool InitKavonQueue(KavonQueuePtr Q);
void DestroyKavonQueue(KavonQueuePtr Q);
bool EmptyKavonQueue(KavonQueuePtr Q);
int KavonQueueLength(KavonQueuePtr Q);
bool InsertKavonQueue(KavonQueuePtr Q, ElemType e);
bool GetHead(KavonQueuePtr Q, ElemType *e);
void KavonQueueTraverse(KavonQueuePtr Q, void (*fp)(ElemType));
void ClearKavonQueue(KavonQueuePtr Q);
bool RemoveHead(KavonQueuePtr Q, ElemType *e);

#endif /* KAVONQUEUE_H */
