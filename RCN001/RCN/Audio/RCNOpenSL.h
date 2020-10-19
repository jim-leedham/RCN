/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNOpenSL.h
Description:    Provides an OpenSL-specific audio control interface. Intended
				primarily for use with Android

Notes:			This OpenSL implementation comes from the RCNAudio library
				by Jim Leedham, but since this engine does not yet fully support
				Android it is unknown whether this class will need any changes

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNOPENSL_H_
#define _RCN_RCNOPENSL_H_

#if defined (RCNANDROID)
#include "RCNAudio.h"

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNOpenSL : public CRCNAudio
{
public:

	virtual void		Initialise();
	virtual void		Process();
	virtual void		Shutdown(); 

private:

	SLObjectItf			ms_tEngineObject;
	SLEngineItf			ms_tEngineItf;
	SLObjectItf			ms_tOutputMixObject;
};

#endif	// (RCNANDROID)
#endif	// _RCN_RCNOPENSL_H_