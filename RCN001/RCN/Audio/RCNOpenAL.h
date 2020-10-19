/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNOpenAL.h
Description:    Provides an OpenAL-specific audio control interface. Intended
				primarily for use with iOS

Notes:			This OpenAL implementation comes from the RCNAudio library
				by Jim Leedham, but since this engine does not yet fully support
				iOS it is unknown whether this class will need any changes

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNOPENAL_H_
#define _RCN_RCNOPENAL_H_

#if defined (RCNIOS)
#include "RCNAudio.h"

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNOpenAL : public CRCNAudio
{
public:

	virtual void		Initialise();
	virtual void		Process();
	virtual void		Shutdown(); 

private:

	unsigned int		m_uAudioSessionCategory;
	ALCdevice*			m_pOpenALDevice;
	ALCcontext*			m_pOpenALContext;
};

#endif	// (RCNIOS)
#endif	// _RCN_RCNOPENAL_H_