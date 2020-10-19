/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNXAudio2.h
Description:    Provides an XAudio2-specific audio control interface. Intended
				primarily for use with Windows

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNXAUDIO2_H_
#define _RCN_RCNXAUDIO2_H_

#include "RCNAudio.h"

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNXAudio2 : public CRCNAudio
{
public:

	virtual void			Initialise();
	virtual void			Process();
	virtual void			Shutdown(); 

	virtual void*			GetEngine();

protected:

	IXAudio2*				ms_pXAudio2;
	IXAudio2MasteringVoice*	ms_pMasteringVoice;
	X3DAUDIO_HANDLE			ms_p3DHandle;
	X3DAUDIO_LISTENER		ms_tListener;
};

#endif	// _RCN_RCNXAUDIO2_H_