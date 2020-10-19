/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNJobQueue.h
Description:    Provides a mechanism for queuing and processing jobs

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNJOBQUEUE_H_
#define _RCN_RCNJOBQUEUE_H_

// -----------------------------------------
// INCLUDES

#include "RCNJob.h"
#include "RCNLinkedList.h"
#include "RCNThread.h"
#include "RCNSemaphore.h"

typedef CRCNLinkedList<CRCNJob> CRCNJobList;

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNJobQueue
{
public:

	CRCNJobQueue();
	~CRCNJobQueue();

	void InsertJob(CRCNJob* pJob);

private:

	static const int			m_iMaximumSemaphoreCount = 128;

	CRCNLinkedList<CRCNJob>		m_tJobList;
	CRCNThread					m_tThread;
	CRCNSemaphore				m_tSemaphore;

	bool						m_bFinishing;

	static int		ProcessJob(void* pParameters);
};

#endif	// _RCN_RCNJOBQUEUE_H_