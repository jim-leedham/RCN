/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNThread.cpp
Description:    Provides a common thread interface.

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNThread.h"

typedef struct TRCNThreadFunctionWrapper
{
	int (*pFunction)(void *);
	void *pParameters;
} TRCNThreadFunctionWrapper;


DWORD __stdcall RCNThreadFunctionWrapper(LPVOID param)
{
	TRCNThreadFunctionWrapper pfnThreadFunctionWrapper = *((TRCNThreadFunctionWrapper *) param);
	RCNDelete param;
	return (pfnThreadFunctionWrapper.pFunction)(pfnThreadFunctionWrapper.pParameters);
}

// -----------------------------------------
// CLASS DEFINITIONS

CRCNThread::CRCNThread(FNThreadFunction pfnThreadFunction, void* pThreadParameters)
{
#if defined WIN32

	TRCNThreadFunctionWrapper* pThreadFunctionWrapper = RCNNew TRCNThreadFunctionWrapper;
	pThreadFunctionWrapper->pFunction = pfnThreadFunction;
	pThreadFunctionWrapper->pParameters = pThreadParameters;

	m_pThread = CreateThread(NULL, NULL, RCNThreadFunctionWrapper, pThreadFunctionWrapper, 0, &m_iThreadID);
#endif
}

bool CRCNThread::GetExitCode(int* pCode)
{
#if defined WIN32
	DWORD iReturn;
	BOOL result = GetExitCodeThread(m_pThread, &iReturn);

	RCNAssertStr(result != 0, "Error returned by GetExitCodeThread()\n");

	if(iReturn != STILL_ACTIVE)
	{
		*pCode = iReturn;
		return true;
	}

	return false;
#endif
}

void CRCNThread::RCNSleep(int iMilliseconds)
{
	Sleep(iMilliseconds);
}
