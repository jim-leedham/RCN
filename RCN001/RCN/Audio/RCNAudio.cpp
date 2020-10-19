/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNAudio.cpp
Description:    Provides a platform/API-agnostic audio control interface

*---------------------------------------------------------------------------*/

#include "stdheader.h"
#include "RCNAudio.h"
#include "RCNContainer.h"

#if defined RCNWINDOWS
#include "RCNXAudio2.h"
#include "RCNXAudio2Source.h"
#elif defined RCNIOS
#include "RCNOpenAL.h"
#include "RCNOpenALSource.h"
#elif defined RCNANDROID
#include "RCNOpenSL.h"
#include "RCNOpenSLSource.h"
#endif

// -----------------------------------------
// STATIC DEFINITIONS

bool					CRCNAudioController::ms_bInitialised		= false;
CRCNAudio*				CRCNAudioController::ms_pAudioEngine		= NULL;
CRCNJobQueue*			CRCNAudioController::ms_pJobQueue			= NULL;
CRCNJobQueue*			CRCNAudioController::ms_pStreamJobQueue		= NULL;
CRCNJob					CRCNAudioController::ms_tProcessJob;
CRCNAudioSourceList		CRCNAudioController::ms_tSourceList;
double					CRCNAudioController::ms_dLastProcessTime;
int						CRCNAudioController::ms_iNumberOfContainers;
CRCNContainer*			CRCNAudioController::ms_pContainers;

// -----------------------------------------
// CLASS DEFINITIONS

void CRCNAudio::Initialise()
{

}

void CRCNAudio::Process()
{

}

void CRCNAudio::Shutdown()
{

}

void CRCNAudioController::Initialise(const char* pXMLFilename)
{
	if(ms_bInitialised)
		return;

	// First initialise the audio engine

	GetCriticalSection().Lock();

	ms_pJobQueue = RCNNew CRCNJobQueue();

	ms_pAudioEngine = RCNNew AUDIOENGINE();
	ms_pAudioEngine->Initialise();

	ms_bInitialised = true;

	ms_dLastProcessTime = CRCNTimer::GetTicks();;
	ms_tProcessJob.SetJobFunction(ProcessJob, NULL);
	ms_pJobQueue->InsertJob(&ms_tProcessJob);

	ms_pStreamJobQueue = RCNNew CRCNJobQueue();

	GetCriticalSection().Unlock();

	// Now load the XML
	LoadXML(pXMLFilename);
}

void CRCNAudioController::Process()
{
	if(!ms_bInitialised)
		return;

	GetCriticalSection().Lock();
	ms_pAudioEngine->Process();
	GetCriticalSection().Unlock();
}

void CRCNAudioController::Shutdown()
{
	if(!ms_bInitialised)
		return;

	GetCriticalSection().Lock();

	ms_tSourceList.Iterate(CRCNAudioSource::Release, NULL);

	GetCriticalSection().Unlock();

	while(!ms_tSourceList.IsEmpty())
		CRCNThread::RCNSleep(10);

	ms_bInitialised = false;

	RCNDelete ms_pJobQueue;
	ms_pJobQueue = NULL;

	RCNDelete ms_pStreamJobQueue;
	ms_pStreamJobQueue = NULL;

	GetCriticalSection().Lock();

	ms_pAudioEngine->Shutdown();
	RCNDelete ms_pAudioEngine;
	ms_pAudioEngine = NULL;

	if(ms_pContainers)
		RCNDelete [] ms_pContainers;

	GetCriticalSection().Unlock();
}

void CRCNAudioController::InsertJob(CRCNJob* pJob)
{
	if(!ms_bInitialised)
		return;

	ms_pJobQueue->InsertJob(pJob);
}

void CRCNAudioController::InsertStreamJob(CRCNJob* pJob)
{
	if(!ms_bInitialised)
		return;

	ms_pStreamJobQueue->InsertJob(pJob);
}

void CRCNAudioController::ProcessJob(CRCNJob* pJob, void* param)
{
	if(!ms_bInitialised)
		return;

	CRCNThread::RCNSleep(15);

	GetCriticalSection().Lock();

	double dCurrentProcessTime = CRCNTimer::GetTicks();
	int iDeltaTime = (int)(dCurrentProcessTime - ms_dLastProcessTime);
	ms_dLastProcessTime = dCurrentProcessTime;

	ms_tSourceList.Iterate(CRCNAudioSource::Process, &iDeltaTime);

	if(ms_bInitialised)
		ms_pJobQueue->InsertJob(&ms_tProcessJob);

	GetCriticalSection().Unlock();
}

CRCNAudioSource* CRCNAudioController::Create(const char* pContainerName)
{
	// Find the appropriate Container and create an AudioSource based on the
	// Container template

	int iContainer;
	for(iContainer = 0; iContainer < ms_iNumberOfContainers; iContainer++)
	{
		// TODO: Hash container names at load time, then compare hashes here for performance
		if(stricmp(ms_pContainers[iContainer].GetName(), pContainerName) == 0)
			break;
	}

	if(iContainer == ms_iNumberOfContainers)
	{
		RCNAssertStr(false, "Could not find container with matching name");
		return NULL;
	}

	CRCNAudioSource* pAudioSource = RCNNew AUDIOSOURCE(&ms_pContainers[iContainer]);
	ms_tSourceList.Insert(pAudioSource);	
	return pAudioSource;
}

CRCNCriticalSection& CRCNAudioController::GetCriticalSection()
{
	static CRCNCriticalSection criticalSection;
	return criticalSection;
}

CRCNCriticalSection& CRCNAudioController::GetStreamCriticalSection()
{
	static CRCNCriticalSection streamCriticalSection;
	return streamCriticalSection;
}

void* CRCNAudioController::GetEngine()
{
	if(!ms_bInitialised)
		return NULL;

	return ms_pAudioEngine->GetEngine();
}

void CRCNAudioController::Remove(CRCNAudioSource* pAudioSource)
{
	ms_tSourceList.Remove(pAudioSource);
	RCNDelete pAudioSource;
}

void CRCNAudioController::LoadXML(const char* pXMLFilename)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(pXMLFilename);

	RCNAssert(doc.FirstChildElement("Audio") != NULL);
	tinyxml2::XMLElement* pAudioElement = doc.FirstChildElement("Audio");

	tinyxml2::XMLElement* pContainersElement = pAudioElement->FirstChildElement("Containers");
	tinyxml2::XMLElement* pContainerElement;
	ms_iNumberOfContainers = 0;

	for(pContainerElement = pContainersElement->FirstChildElement(); pContainerElement != NULL; pContainerElement = pContainerElement->NextSiblingElement())
	{
		ms_iNumberOfContainers++;
	}

	if(ms_pContainers)
		RCNDelete [] ms_pContainers;

	ms_pContainers = RCNNew CRCNContainer[ms_iNumberOfContainers];
	int iContainerLoadCount = 0;

	for(pContainerElement = pContainersElement->FirstChildElement(); pContainerElement != NULL; pContainerElement = pContainerElement->NextSiblingElement())
	{
		ms_pContainers[iContainerLoadCount].LoadXML(pContainerElement);
		iContainerLoadCount++;
	}
}

