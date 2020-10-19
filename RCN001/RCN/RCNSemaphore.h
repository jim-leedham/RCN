/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNSemaphore.h
Description:    Provides a platform/API-agnostic semaphore interface

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNSEMAPHORE_H_
#define _RCN_RCNSEMAPHORE_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNSemaphore
{
public:

	CRCNSemaphore(int iInitialCount = 1, int iMaxCount = 1, const char * pName = NULL);
	~CRCNSemaphore();

	void Wait(int iTimeout);
	void Signal(int iCount);

private:
#if defined WIN32
	HANDLE m_tSemaphore;
#endif
};

#endif	// _RCN_RCNSEMAPHORE_H_