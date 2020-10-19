/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNThread.h
Description:    Provides a common thread interface.

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNTHREAD_H_
#define _RCN_RCNTHREAD_H_

typedef int (*FNThreadFunction)(void* pJobParameters);

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNThread
{
public:

				CRCNThread(FNThreadFunction pfnThreadFunction, void* pThreadParameters);
	
	bool			GetExitCode(int* pCode);
	static void		RCNSleep(int iMilliseconds);

private:

#if defined WIN32
	HANDLE	m_pThread;
	DWORD	m_iThreadID;
#endif

};

#endif	// _RCN_RCNTHREAD_H_