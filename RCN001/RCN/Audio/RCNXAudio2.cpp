/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNAudio.cpp
Description:    Provides an XAudio2-specific audio control interface. Intended
				primarily for use with Windows

Note:			Uses X3DAudio for 3D sound

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNXAudio2.h"

// -----------------------------------------
// CLASS DEFINITIONS

void CRCNXAudio2::Initialise()
{
	ms_pXAudio2			= NULL;
	ms_pMasteringVoice	= NULL;

	HRESULT hResult;
	hResult = CoInitialize(NULL);

	// Create the XAudio2 instance
	hResult = XAudio2Create(&ms_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if(FAILED(hResult))
	{
		RCNAssertStr(false, "XAudio2Create failed!");
		return;
	}

	// Create the mastering voice
	hResult = ms_pXAudio2->CreateMasteringVoice(&ms_pMasteringVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL);
	if (FAILED(hResult))
	{
		RCNAssertStr(false, "ms_pXAudio2->CreateMasteringVoice failed!");
		return;
	}

	// Initialise X3DAudio
	static const float AL_SPEED_OF_SOUND = 343.3f;
	XAUDIO2_DEVICE_DETAILS tDeviceDetails;
	ms_pXAudio2->GetDeviceDetails(0, &tDeviceDetails);
	DWORD iSpeakers = tDeviceDetails.OutputFormat.dwChannelMask;
	X3DAudioInitialize(iSpeakers, AL_SPEED_OF_SOUND, ms_p3DHandle);
	
	// Initialise the listener
	memset(&ms_tListener, 0, sizeof(ms_tListener));
	ms_tListener.OrientFront.z = 1.0f;
	ms_tListener.OrientTop.y = 1.0f;
	ms_tListener.pCone = NULL;
}

void CRCNXAudio2::Process()
{

}

void CRCNXAudio2::Shutdown()
{
	// Destroy mastering voice
	ms_pMasteringVoice->DestroyVoice();
	ms_pMasteringVoice = NULL;

	ms_pXAudio2->Release();
	ms_pXAudio2 = NULL;
}

void* CRCNXAudio2::GetEngine()
{
	return ms_pXAudio2;
}