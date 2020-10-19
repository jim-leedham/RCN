/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNCriticalSection.h
Description:    Provides an RCN-specific critical section implementation

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNCRITICALSECTION_H_
#define _RCN_RCNCRITICALSECTION_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNCriticalSection
{
public:

	CRCNCriticalSection();
	~CRCNCriticalSection();

	bool Lock();
	void Unlock();

private:
#if defined WIN32
	CRITICAL_SECTION m_tCriticalSection;
#endif
};

#endif	// _RCN_RCNCRITICALSECTION_H_