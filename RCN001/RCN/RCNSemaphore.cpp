/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNSemaphore.cpp
Description:    Provides a platform/API-agnostic semaphore interface

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNSemaphore.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNSemaphore::CRCNSemaphore(int iInitialCount, int iMaxCount, const char* pName)
{
#if defined WIN32
	m_tSemaphore = CreateSemaphoreA(NULL, iInitialCount, iMaxCount, pName);
#endif
}

CRCNSemaphore::~CRCNSemaphore()
{
#if defined WIN32
	CloseHandle(m_tSemaphore);
#endif
}

void CRCNSemaphore::Wait(int iTimeout)
{
#if defined WIN32
	WaitForSingleObject(m_tSemaphore, iTimeout);
#endif
}

void CRCNSemaphore::Signal(int iCount)
{
#if defined WIN32
	ReleaseSemaphore(m_tSemaphore, iCount, NULL);
#endif
}