/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNJobQueue.cpp
Description:    Provides a mechanism for queuing and processing jobs

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNJobQueue.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNJobQueue::CRCNJobQueue() :
	m_tSemaphore(0, m_iMaximumSemaphoreCount),
	m_tThread(ProcessJob, this),
	m_bFinishing(false)
{
}

CRCNJobQueue::~CRCNJobQueue()
{
	// Wait until the job queue empties.
	while(!m_tJobList.IsEmpty())
		CRCNThread::RCNSleep(10);

	m_bFinishing = true;
	m_tSemaphore.Signal(1);


	int iExitCode;
	while(!m_tThread.GetExitCode(&iExitCode))
		CRCNThread::RCNSleep(10);

	RCNAssertStr(m_tJobList.IsEmpty(),"Job list should be empty by now!");
}

void CRCNJobQueue::InsertJob(CRCNJob* pJob)
{
	m_tJobList.Insert(pJob);
	m_tSemaphore.Signal(1);
}

int CRCNJobQueue::ProcessJob(void* pParameters)
{
	CRCNJobQueue* pJobQueue = (CRCNJobQueue*)pParameters;
	while(true)
	{
		pJobQueue->m_tSemaphore.Wait(INFINITE);
		if(pJobQueue->m_bFinishing)
			return 0;

		CRCNJob* pJob = pJobQueue->m_tJobList.RemoveByIndex(0);
		pJob->m_pfnJobFunction(pJob, pJob->m_pJobParameters);
	}
	return 0;
}

