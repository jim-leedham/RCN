/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNCriticalSection.cpp
Description:    Provides an RCN-specific critical section implementation

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNCriticalSection.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNCriticalSection::CRCNCriticalSection()
{
#if defined WIN32
	InitializeCriticalSection(&m_tCriticalSection);
#endif
}

CRCNCriticalSection::~CRCNCriticalSection()
{
#if defined WIN32
	DeleteCriticalSection(&m_tCriticalSection);
#endif
}

bool CRCNCriticalSection::Lock()
{
#if defined WIN32
	EnterCriticalSection(&m_tCriticalSection);
#endif
	return true;
}

void CRCNCriticalSection::Unlock()
{
#if defined WIN32
	LeaveCriticalSection(&m_tCriticalSection);
#endif
}

