/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNLinkedList.h
Description:    A common linked list interface. Primarily used by RCNJobQueue.

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNLINKEDLIST_H_
#define _RCN_RCNLINKEDLIST_H_

// -----------------------------------------
// INCLUDES

#include "RCNMemUtil.h"
#include "RCNUtil.h"
#include "RCNAssert.h"

// -----------------------------------------
// CLASS DECLARATIONS

// NOTE: Although I hate to put function definitions in header files, in this case it is necessary

template <typename T>
class CRCNLinkedListItem
{
public:
	CRCNLinkedListItem<T>::CRCNLinkedListItem(T* pData, CRCNLinkedListItem<T>* pNextItem)
	{
		m_pData = pData;
		m_pNextItem = pNextItem;
	}

	T* m_pData;
	CRCNLinkedListItem<T>* m_pNextItem;
};


template <typename T>
class CRCNLinkedList
{
public:
	CRCNLinkedList<T>::CRCNLinkedList()
	{
		m_pHead = NULL;
		m_pTail = NULL;
		m_iCount = 0;
	}

	void CRCNLinkedList<T>::Insert(T* pData)
	{
		CRCNLinkedListItem<T>* pNewItem = RCNNew CRCNLinkedListItem<T>(pData, NULL);

		if(m_pHead == NULL)
		{
			m_pHead = pNewItem;
		}
		else
		{
			CRCNLinkedListItem<T>* pItem = m_pHead;
			while(pItem->m_pNextItem != NULL)
			{
				pItem = pItem->m_pNextItem;
			}

			pItem->m_pNextItem = pNewItem;
			m_pTail = pNewItem;
		}

		++m_iCount;
	}

	T* CRCNLinkedList<T>::RemoveByIndex(int iIndex)
	{
		RCNAssertStr(iIndex < m_iCount, "Index must be less than total number of items in the list");

		T* pData;
		CRCNLinkedListItem<T>* pItem;

		if(iIndex == 0)
		{
			pItem = m_pHead;
			m_pHead = m_pHead->m_pNextItem;
		}
		else
		{
			int iCount = 0;
			CRCNLinkedListItem<T>* pPreviousItem = m_pHead;

			while(iCount < iIndex)
			{
				RCNAssert(pPreviousItem->m_pNextItem != NULL);
				pPreviousItem = pPreviousItem->m_pNextItem;
				iCount++;
			}

			pItem = pPreviousItem->m_pNextItem;

			if(pItem == m_pTail)
			{
				m_pTail = pPreviousItem;
			}

			pPreviousItem->m_pNextItem = pItem->m_pNextItem;
		}

		m_iCount--;
		pData = pItem->m_pData;
		RCNDelete pItem;
		return pData;
	}

	T* CRCNLinkedList<T>::Remove(T* pData)
	{
		CRCNLinkedListItem<T>* pItemToRemove;
		CRCNLinkedListItem<T>* pItem;
		pItem = m_pHead;

		RCNAssertStr(pItem != NULL, "List cannot be empty");

		if(pItem->m_pData == pData)
		{
			pItemToRemove = pItem;
			m_pHead = pItem->m_pNextItem;
		}
		else
		{
			while(pItem->m_pNextItem != NULL && pItem->m_pNextItem->m_pData != pData)
			{
				pItem = pItem->m_pNextItem;
			}

			if(pItem->m_pNextItem == NULL)
			{
				RCNAssertStr(false, "Could not find list item with matching data");
				return NULL;
			}
			else
			{
				pItemToRemove = pItem->m_pNextItem;
				pItem->m_pNextItem = pItem->m_pNextItem->m_pNextItem;
				if(pItem->m_pNextItem == NULL)
					m_pTail = pItem;
			}

		}

		m_iCount--;
		pData = pItemToRemove->m_pData;
		RCNDelete pItemToRemove;
		return pData;
	}

	bool CRCNLinkedList<T>::IsEmpty()
	{
		return m_iCount == 0;
	}
	
	void CRCNLinkedList<T>::Iterate(void (*CallbackFunc)(T* , void* ), void *pUserData)
	{
		CRCNLinkedListItem<T>* pItem;
		pItem = m_pHead;
		while(pItem)
		{
			CRCNLinkedListItem<T>* pNext = pItem->m_pNextItem;
			(CallbackFunc)(pItem->m_pData, pUserData);
			pItem = pNext;
		}
	}

	CRCNLinkedListItem<T>*	m_pHead;
	CRCNLinkedListItem<T>*	m_pTail;

private:

	int	m_iCount;
};

#endif	// _RCN_RCNLINKEDLIST_H_