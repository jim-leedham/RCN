/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNAudioSource.h
Description:    Provides a platform/API-agnostic audio source interface

*---------------------------------------------------------------------------*/

#include "stdheader.h"

#include "RCNAudioSource.h"
#include "RCNContainer.h"
#include "RCNFileSystem.h"
#include "RCNFile.h"
#include "RCNMP3.h"

// -----------------------------------------
// STATIC DEFINITIONS

float				CRCNAudioSource::ms_fMinimumVolume	= 0.0f;
float				CRCNAudioSource::ms_fMaximumVolume	= 1.0f;

// -----------------------------------------
// CLASS DEFINITIONS

CRCNAudioSource::CRCNAudioSource()
{
	m_pFile			= NULL;
}

CRCNAudioSource::CRCNAudioSource(CRCNContainer* pContainer)
{
	m_pContainer = pContainer;

	RCNAssert(m_pContainer->m_eFormat != EFormat_INVALID);

	m_eFileState			= EIdle;
	m_ePlaybackState		= EInitial;
	m_uStreamCount			= 0;
	m_uFilePos				= 0;
	m_iBufferFillDataIndex	= 0;
	m_iUpdateFlags			= ~0;
	m_bReleased				= false;
	m_iFadeRemain			= 0;
	m_fFadeDelta			= 0.0f;
	m_bFade					= false;
	m_fVolume				= 1.0f;
	m_bLoop					= m_pContainer->m_bLoop;

	CRCNJob* pInitialiseJob = RCNNew CRCNJob(InitialiseJob, this);
	CRCNAudioController::InsertJob(pInitialiseJob);

}

CRCNAudioSource::~CRCNAudioSource()
{
	m_ePlaybackState = EInitial;
	m_iUpdateFlags	 = ~0;
	m_uStreamCount	 = ~0;

	if(m_tMetadata.pData)
	{
		RCNDelete [] m_tMetadata.pData;
		m_tMetadata.pData = NULL;
	}

	if(m_pFile)
	{
		RCNDelete m_pFile;
		m_pFile = NULL;
	}
}

void CRCNAudioSource::Play()
{
	CRCNAudioController::GetCriticalSection().Lock();

	switch(m_ePlaybackState)
	{
	case EInitial:
	case EStopped:
		{
			CRCNJob* pPlayJob = RCNNew CRCNJob(PlayJob, this);
			CRCNAudioController::InsertJob(pPlayJob);
		}
		break;
	case EPaused:
		// TODO
		break;
	}

	CRCNAudioController::GetCriticalSection().Unlock();
}

void CRCNAudioSource::Process(CRCNAudioSource* pSource, void* pParameters)
{
	if(pSource->m_bReleased && pSource->m_eFileState == EReleased && pSource->m_ePlaybackState == EStopped)
	{
		CRCNAudioController::Remove(pSource);
		return;
	}

	int iDeltaTime = *(int*)pParameters;
	pSource->Process(iDeltaTime);
}

void CRCNAudioSource::Process(int iDeltaTime)
{
	if(m_iFadeRemain > 0)
	{
		int iFadeDelta = Min(m_iFadeRemain, iDeltaTime);
		m_iFadeRemain -= iFadeDelta;
		SetVolume(GetVolume() + (m_fFadeDelta * iFadeDelta));
	}

	Update();

	if(m_bReleased && m_eFileState < EReleased)
	{
		switch(m_eRelease)
		{
		case EImmediate:
			{
				Stop();
				m_eFileState = EReleased;
			}
			break;
		case EOnceFinished:
			{
				// NOTE: If this source loops then EOnceFinished will have the same effect as EImmediate
				if(m_bLoop)
					Stop();

				m_eFileState = EReleased;
			}
			break;
		case EOnceFaded:
			{
				if(m_iFadeRemain <= 0)
				{
					Stop();
					m_eFileState = EReleased;
				}
			}
			break;
		}
	}
}

void CRCNAudioSource::InitialiseJob(CRCNJob* pJob, void* pParameters)
{
	RCNDelete pJob;
	CRCNAudioSource* pAudioSource = (CRCNAudioSource*) pParameters;

	CRCNFile* pFile = CRCNFileSystem::Open(pAudioSource->m_pContainer->m_pSource, CRCNFileSystem::EAccessMode::EReadOnly);
	RCNAssertStr(pFile != NULL && pFile->IsOpen(), "Failed to open file");
	pAudioSource->m_tMetadata.uTotalDataSize = (unsigned int)pFile->GetLength();

	pAudioSource->Initialise(pFile);
}


void CRCNAudioSource::PlayJob(CRCNJob* pJob, void* pParameters)
{
	RCNDelete pJob;
	CRCNAudioSource* pAudioSource = (CRCNAudioSource*) pParameters;

	// Stall until the source has sufficiently buffered
	while(pAudioSource->m_eFileState != EFileState::ELoaded)
	{
		CRCNThread::RCNSleep(10);
	}

	CRCNAudioController::GetCriticalSection().Lock();

	pAudioSource->ResetBuffer();
	pAudioSource->Update();
	pAudioSource->Start();

	CRCNAudioController::GetCriticalSection().Unlock();
}

void CRCNAudioSource::Initialise(CRCNFile* pFile)
{
	// Typically overridden by the platform-specific AudioSource
}

void CRCNAudioSource::SetLoop(bool bLoop)
{
	CRCNAudioController::GetCriticalSection().Lock();

	m_bLoop = bLoop;
	m_iUpdateFlags |= m_iUpdateLoop;
	Update();

	CRCNAudioController::GetCriticalSection().Unlock();
}

void CRCNAudioSource::Update()
{
	// Typically overridden by the platform-specific AudioSource
}

void CRCNAudioSource::ResetBuffer()
{
	// Typically overridden by the platform-specific AudioSource
}

void CRCNAudioSource::Start()
{
	// Typically overridden by the platform-specific AudioSource
}

void CRCNAudioSource::Stop()
{
	// Typically overridden by the platform-specific AudioSource
}

CRCNAudioSource::TBufferFillResult CRCNAudioSource::QueryBuffer(unsigned int uFilePos, unsigned int uMaxBytes)
{
	TBufferFillResult tBufferFillResult;

	tBufferFillResult.uBytePos = uFilePos;
	tBufferFillResult.uBytesToRead = m_tMetadata.uTotalDataSize - uFilePos;
	tBufferFillResult.bEOF = true;
	if(uMaxBytes != 0 && (uMaxBytes < tBufferFillResult.uBytesToRead))
	{
		tBufferFillResult.uBytesToRead = uMaxBytes;
		tBufferFillResult.bEOF = false;
	}
	tBufferFillResult.uStartSample = 0;
	return tBufferFillResult;
}

unsigned int CRCNAudioSource::SuggestBufferAmount()
{
	// Find out how much data is available
	TBufferFillResult tAvailable = QueryBuffer(m_uFilePos, 0);

	if(tAvailable.uBytesToRead >= m_tMetadata.uChunkSize)
		return m_tMetadata.uChunkSize;
	else
		return tAvailable.uBytesToRead;
}

CRCNAudioSource::TBufferFillResult CRCNAudioSource::FillBuffer(void* pBuffer, unsigned int uBufferSize, unsigned int uParam)
{
	RCNAssert(m_eFileState >= ELoaded);

	TBufferFillResult tResult = QueryBuffer(m_uFilePos, uBufferSize);

	unsigned int uCurPos = tResult.uBytePos;
	RCNAssert(tResult.uBytePos + tResult.uBytesToRead <= m_tMetadata.uTotalDataSize);
	RCNAssert(uCurPos + tResult.uBytesToRead <= m_pFile->GetLength());

	m_uFilePos += tResult.uBytesToRead;

	// Read into the buffer
	TBufferFillData* pFillData = &m_tBufferFillData[m_iBufferFillDataIndex];
	m_iBufferFillDataIndex = (m_iBufferFillDataIndex + 1) & (ms_iMaxBuffers - 1);
	pFillData->tResult = tResult;
	pFillData->pSource = this;
	CRCNFileAsyncEvent* pEvent = &pFillData->tEvent;

	pEvent->AddCallback(FillBufferCallback, pFillData);

	m_pFile->ReadAsync(pBuffer, uCurPos, tResult.uBytesToRead, (void*)uParam, pEvent);

	return tResult;
}

void CRCNAudioSource::FillBufferCallback(CRCNFileAsyncEvent* pEvent, const TRCNFileAsyncEventData* pData, void* pParameters)
{
	RCNAssert(pData->iCount == pData->iResult);

	TBufferFillData* pFillData = (TBufferFillData*) pParameters;

	CRCNAudioController::GetCriticalSection().Lock();

	pFillData->pSource->StreamCallback(pData->pBuffer, pFillData->tResult, (unsigned int)pData->pUserData);

	CRCNAudioController::GetCriticalSection().Unlock();
}

void CRCNAudioSource::WaitForOutstandingStreamOperations()
{
	if(m_uStreamCount > 0)
	{
		CRCNAudioController::GetStreamCriticalSection().Unlock();
		CRCNAudioController::GetCriticalSection().Unlock();
		while(m_uStreamCount > 0)
			Sleep(15);
		CRCNAudioController::GetStreamCriticalSection().Lock();
		CRCNAudioController::GetCriticalSection().Lock();
	}
}

void CRCNAudioSource::SetFade(float fTargetVolume, int iDuration)
{
	m_bFade = true;
	m_iFadeRemain = iDuration;
	m_fFadeDelta = -(GetVolume() - fTargetVolume) / iDuration;
}

void CRCNAudioSource::Release(CRCNAudioSource* pSource, void* pParameters)
{
	pSource->Release(EImmediate);
}

void CRCNAudioSource::Release(ERelease eRelease)
{
	m_eRelease = eRelease;
	m_bReleased = true;
}

void CRCNAudioSource::SetVolume(float fVolume)
{
	if(fVolume < ms_fMinimumVolume)
		return;

	if(fVolume > ms_fMaximumVolume)
		return;

	m_fVolume = fVolume;
	m_iUpdateFlags |= m_iUpdateVolume;
	Update();
}

float CRCNAudioSource::GetVolume()
{
	return m_fVolume;
}
