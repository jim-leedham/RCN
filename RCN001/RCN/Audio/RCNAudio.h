/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNAudio.h
Description:    Provides a platform/API-agnostic audio control interface

Notes:			The RCNAudio engine handles all low-level, platform-specific 
				logic and there exist derived versions for XAudio2 (used on
				Windows [Phone] systems - descendant of DirectSound), OpenAL 
				(used on Apple iOS and BlackBerry), and OpenSL (used on Android).

				The RCNAudioController acts as a bridge between RCNAudio and 
				the game. It is a static class, initialised and shutdown by the
				game. Game classes should use it to construct AudioSource objects
				(which can then be controlled directly). It also performs essential
				processing on all active AudioSources on a separate thread.

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNAUDIO_H_
#define _RCN_RCNAUDIO_H_

#if		defined (RCNWINDOWS)
#include <xaudio2.h>
#include <x3daudio.h>
#define AUDIOENGINE CRCNXAudio2
#define AUDIOSOURCE CRCNXAudio2Source
#elif	defined (RCNIOS)
#import	<OpenAL/al.h>
#import	<OpenAL/alc.h>
#define AUDIOENGINE CRCNOpenAL
#define AUDIOSOURCE CRCNOpenALSource
#elif	defined (RCNANDROID)
#include <SLES/OpenSLES.h>
#define AUDIOENGINE CRCNOpenSL
#define AUDIOSOURCE CRCNOpenSLSource
#endif

#include "RCNLinkedList.h"
#include "RCNAudioSource.h"

typedef CRCNLinkedList<CRCNAudioSource> CRCNAudioSourceList;

// -----------------------------------------
// FORWARD DECLARATIONS

class CRCNJobQueue;
class CRCNJob;
class CRCNCriticalSection;
class CRCNContainer;

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNAudio
{
public:

	virtual void				Initialise();
	virtual void				Process();
	virtual void				Shutdown();

private:

	virtual void*				GetEngine() = 0;

	friend class				CRCNAudioController;
};

class CRCNAudioController
{
public:

	static void					Initialise(const char* pXMLFilename);
	static void					Process();
	static void					Shutdown();

	static CRCNAudioSource*		Create(const char* pContainerName);

	static void					Remove(CRCNAudioSource* pAudioSource);

	static void					InsertJob(CRCNJob* pJob);
	static void					InsertStreamJob(CRCNJob* pJob);

	static CRCNCriticalSection&	GetCriticalSection();
	static CRCNCriticalSection& GetStreamCriticalSection();

protected:

	static void*				GetEngine();

	friend class				AUDIOSOURCE;

private:

	static void					ProcessJob(CRCNJob* pJob, void* param);
	static void					LoadXML(const char* pXMLFilename);

	static bool					ms_bInitialised;
	static CRCNAudio*			ms_pAudioEngine;
	static CRCNJobQueue*		ms_pJobQueue;
	static CRCNJob				ms_tProcessJob;

	static CRCNJobQueue*		ms_pStreamJobQueue;
	static CRCNAudioSourceList	ms_tSourceList;

	static double				ms_dLastProcessTime;

	static int					ms_iNumberOfContainers;
	static CRCNContainer*		ms_pContainers;	
};


#endif	// _RCN_RCNAUDIO_H_