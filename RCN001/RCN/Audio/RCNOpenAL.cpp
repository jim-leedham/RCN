/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNOpenAL.cpp
Description:    Provides an OpenAL-specific audio control interface. Intended
				primarily for use with iOS

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"

#if defined (RCNIOS)
#include "RCNOpenAL.h"

// -----------------------------------------
// STATIC DEFINITIONS


// -----------------------------------------
// CLASS DEFINITIONS

void CRCNOpenAL::Initialise()
{
	// AudioSession related functions are unique to iOS and not related to OpenAL!
	// However, unless we support other OpenAL based platforms (e.g. BlackBerry)
	// this is a good enough place to initialise the AudioSession

    OSStatus result;

	// Create the AudioSession instance
	result = AudioSessionInitialize(NULL, NULL, InterruptionListener, NULL);
	if(result != kAudioSessionNoError)
	{
		RCNAssertStr(false, "AudioSessionInitialize failed!");
		return;
	}

	m_uAudioSessionCategory = kAudioSessionCategory_AmbientSound;
	result = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(m_uAudioSessionCategory), &m_uAudioSessionCategory);
	if(result != kAudioSessionNoError)
	{
		RCNAssertStr(false, "AudioSessionSetProperty failed!");
		return;
	}

	result = AudioSessionSetActive(true);
	if(result != kAudioSessionNoError)
	{
		RCNAssertStr(false, "AudioSessionSetActive failed!");
		return;
	}

	alGetError(); // Clear the error log

	// Now the true OpenAL initialisation
	m_pOpenALDevice = alcOpenDevice(NULL);
	ALenum err = alGetError();
	if(err != AL_NO_ERROR)
	{
		RCNAssertStr(false, "alcOpenDevice failed!");
		m_pOpenALDevice = NULL;
		return;
	}

	// One final iOS-specific piece of code!
	alcMacOSXMixerOutputRateProcPtr pSetFrequencyFunction = (alcMacOSXMixerOutputRateProcPtr)alcGetProcAddress(m_pOpenALDevice, "alcMacOSXMixerOutputRate");
	if (pSetFrequencyFunction)
	{
		pSetFrequencyFunction(22050);
	}

	// And back to pure OpenAL
	m_pOpenALContext = alcCreateContext(m_pOpenALDevice, NULL);
	err = alGetError();
	if(err != AL_NO_ERROR)
	{
		RCNAssertStr(false, "alcCreateContext failed!");
		alcCloseDevice(m_pOpenALDevice);
		m_pOpenALContext	= NULL;
		m_pOpenALDevice	= NULL;
		return;
	}

	alcMakeContextCurrent(m_pOpenALContext);
	err = alGetError();
	if(err != AL_NO_ERROR)
	{
		RCNAssertStr(false, "alcMakeContextCurrent failed!");
		alcDestroyContext(m_pOpenALContext);
		alcCloseDevice(m_pOpenALDevice);
		m_pOpenALContext	= NULL;
		m_pOpenALDevice	= NULL;
		return;
	}

	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
}

void CRCNOpenAL::Process()
{

}

void CRCNOpenAL::Shutdown()
{
	alcMakeContextCurrent(NULL);

	alcDestroyContext(m_pOpenALContext);
	m_pOpenALContext = NULL;

	alcCloseDevice(m_pOpenALDevice);
	m_pOpenALDevice = NULL;

	// iOS-specific:
	AudioSessionSetActive(false);
}

#endif	// (RCNIOS)