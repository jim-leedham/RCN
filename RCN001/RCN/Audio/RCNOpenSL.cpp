/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNOpenSL.cpp
Description:    Provides an OpenSL-specific audio control interface. Intended
				primarily for use with Android

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"

#if defined (RCNANDROID)
#include "RCNOpenSL.h"

// -----------------------------------------
// STATIC DEFINITIONS


// -----------------------------------------
// CLASS DEFINITIONS

void CRCNOpenSL::Initialise()
{
	SLresult result = slCreateEngine(&ms_tEngineObject, 0, NULL, 0, NULL, NULL);
	if(result != SL_RESULT_SUCCESS)
	{
		RCNAssertStr(false, "slCreateEngine failed!");
		return;
	}

	result = (*ms_tEngineObject)->Realize(ms_tEngineObject, SL_BOOLEAN_FALSE);
	if(result != SL_RESULT_SUCCESS)
	{
		RCNAssertStr(false, "ms_tEngineObject->Realize failed!");
		return;
	}

	result = (*ms_tEngineObject)->GetInterface(ms_tEngineObject, SL_IID_ENGINE, (void*)&ms_tEngineItf);
	if(result ! =SL_RESULT_SUCCESS)
	{
		RCNAssertStr(false, "ms_tEngineObject->GetInterface failed!");
		return;
	}

	result = (*ms_tEngineItf)->CreateOutputMix(ms_tEngineItf, &ms_tOutputMixObject, 0, NULL, NULL);
	if(result != SL_RESULT_SUCCESS)
	{
		RCNAssertStr(false, "ms_tEngineItf->CreateOutputMix failed!");
		return;
	}

	result = (*ms_tOutputMixObject)->Realize(ms_tOutputMixObject, SL_BOOLEAN_FALSE);
	if(result != SL_RESULT_SUCCESS)
	{
		RCNAssertStr(false, "ms_tOutputMixObject->Realize failed!");
		return;
	}
}

void CRCNOpenSL::Process()
{

}

void CRCNOpenSL::Shutdown()
{
	if(ms_tOutputMixObject != NULL)
	{
		(*ms_tOutputMixObject)->Destroy(ms_tOutputMixObject);
		ms_tOutputMixObject = NULL;
	}

	if(ms_tEngineObject != NULL)
	{
		(*ms_tEngineObject)->Destroy(ms_tEngineObject);
		ms_tEngineItf		= NULL;
		ms_tEngineObject	= NULL;
	}
}

#endif	// (RCNANDROID)