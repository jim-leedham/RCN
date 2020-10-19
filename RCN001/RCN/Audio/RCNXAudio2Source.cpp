/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNXAudio2Source.h
Description:    Provides an XAudio2-specific audio source implementation

*---------------------------------------------------------------------------*/

#include "stdheader.h"

#include "RCNXAudio2Source.h"
#include "RCNContainer.h"
#include "RCNAudio.h"
#include "RCNFile.h"
#include "RCNMP3.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNXAudio2Source::CRCNXAudio2Source(CRCNContainer* pContainer) : CRCNAudioSource(pContainer)
{
	m_pMP3Decoder = NULL;
	m_pVoice = NULL;
}

CRCNXAudio2Source::~CRCNXAudio2Source()
{
	RCNAssertStr(m_ePlaybackState == EStopped, "Attempting to destroy an audio source that is still playing!");

	CRCNAudioController::GetStreamCriticalSection().Lock();
	// Make sure no new buffer fill requests are accepted
	m_uStreamCount = 0;
	CRCNAudioController::GetStreamCriticalSection().Unlock();

	if(m_pVoice)
	{
		m_pVoice->DestroyVoice();
		m_pVoice = NULL;
	}

	if(m_pMP3Decoder)
	{
		RCNDelete m_pMP3Decoder;
		m_pMP3Decoder = NULL;
	}

	if(m_pXHeader)
	{
		RCNDelete [] m_pXHeader;
		m_pXHeader = NULL;
	}
}

void CRCNXAudio2Source::Start()
{
	RCNAssert(m_pVoice != NULL);
	m_pVoice->Start(0);
	m_ePlaybackState = CRCNAudioSource::EPlaying;
}

void CRCNXAudio2Source::Stop()
{
	RCNAssert(m_pVoice != NULL);
	m_pVoice->Stop(0);
	m_ePlaybackState = CRCNAudioSource::EStopped;
}

void CRCNXAudio2Source::ResetBuffer()
{
	RCNAssert(m_eFileState == ELoaded);

	CRCNAudioController::GetStreamCriticalSection().Lock();

	WaitForOutstandingStreamOperations();

	switch(m_pContainer->m_eFormat)
	{
	case EFormat_MP3:
		{
			if(m_pMP3Decoder == NULL)
				m_pMP3Decoder = RCNNew CRCNMP3(m_pXHeader->nAvgBytesPerSec);
		}
		break;
	}

	if(!m_pVoice)
	{
		RCNAssert(m_pXHeader);
		unsigned int uFlags = 0;	// TODO: Support 3D (uFlags = XAUDIO2_VOICE_USEFILTER)
		bool bUseCallback = (m_tMetadata.uTotalDataSize > m_tMetadata.uChunkSize * 2); // Only need to use the stream callback if there's more than two chunks worth of data
		HRESULT hResult = static_cast<IXAudio2*>(CRCNAudioController::GetEngine())->CreateSourceVoice(&m_pVoice, m_pXHeader, uFlags, XAUDIO2_DEFAULT_FREQ_RATIO, (bUseCallback ? this : NULL));
		RCNAssert(!FAILED(hResult));
		m_iUpdateFlags = ~0;
	}

	// Buffer the data
	XAUDIO2_BUFFER tBuffer = { 0 };

	m_uFilePos = 0;

	tBuffer.pAudioData = m_tMetadata.pData;

	m_uStreamCount++;
	TBufferFillResult tResult = QueryBuffer(m_uFilePos, 0);

	// Set up the first buffer
	tResult = FillBuffer((void*)tBuffer.pAudioData, SuggestBufferAmount(), 0);
	tBuffer.AudioBytes = tResult.uBytesToRead;

	if(tResult.bEOF)
	{
		tBuffer.Flags = XAUDIO2_END_OF_STREAM;

		if(m_bLoop)
		{
			m_uFilePos = 0;
			tResult.bEOF = false;
		}
	}

	WaitForOutstandingStreamOperations();

	tBuffer.pContext = (void*)tBuffer.pAudioData;

	if(m_pMP3Decoder)
		m_pMP3Decoder->Decode(tBuffer.pAudioData, tBuffer.AudioBytes);

	HRESULT hr = m_pVoice->SubmitSourceBuffer(&tBuffer);
	RCNAssert(!FAILED(hr));

	if(!tResult.bEOF)
	{
		// Set up the second buffer
		tBuffer.pAudioData = m_tMetadata.pData + m_tMetadata.uChunkSize;
		tBuffer.PlayBegin = tBuffer.PlayLength = 0;
		m_uStreamCount++;
		tResult = FillBuffer((void*)tBuffer.pAudioData, SuggestBufferAmount(), 0);
		tBuffer.AudioBytes = tResult.uBytesToRead;
		RCNAssert(!tResult.bEOF || m_uFilePos != 0); // Should always have some data left; if the file was two chunks or less then it would have been supplied to XAudio as one buffer

		WaitForOutstandingStreamOperations();

		tBuffer.pContext = (void*)tBuffer.pAudioData;

		if(m_pMP3Decoder)
			m_pMP3Decoder->Decode(tBuffer.pAudioData, tBuffer.AudioBytes);

		hr = m_pVoice->SubmitSourceBuffer(&tBuffer);
		RCNAssert(!FAILED(hr));
	}

	// Assume that the mutex was unlocked long enough for us to receive all the pending OnBufferEnd callbacks
	// Reset the stream request count to 0 to make sure that the buffer fills don't actually happen
	m_uStreamCount = 0;

	CRCNAudioController::GetStreamCriticalSection().Unlock();
}

void CRCNXAudio2Source::OnBufferEnd(void* pBufferContext)
{
	CRCNAudioController::GetStreamCriticalSection().Lock();

	if(m_uStreamCount > 1)
		return;

	TBufferFillJobParamaters* pJobParameters = RCNNew TBufferFillJobParamaters();
	pJobParameters->pSource = this;
	pJobParameters->pBuffer = pBufferContext;

	m_uStreamCount++;
	CRCNJob* pJob = RCNNew CRCNJob(FillBufferJob, pJobParameters);
	CRCNAudioController::InsertStreamJob(pJob);

	CRCNAudioController::GetStreamCriticalSection().Unlock();
}

void CRCNXAudio2Source::FillBufferJob(CRCNJob* pJob, void* pParameters)
{
	RCNDelete pJob;

	TBufferFillJobParamaters* pJobParameters = (TBufferFillJobParamaters*)pParameters;
	CRCNXAudio2Source* pSource = pJobParameters->pSource;
	TBufferFillResult tResult = pSource->FillBuffer(pJobParameters->pBuffer, pSource->SuggestBufferAmount(), 1);
	if(tResult.bEOF && pSource->m_bLoop)
		pSource->m_uFilePos = 0;

	RCNDelete pJobParameters;
}

void CRCNXAudio2Source::StreamCallback(void* pBuffer, const TBufferFillResult& tResult, unsigned int uParam)
{
	// Tell XAudio about the new data if required
	if(uParam && tResult.uBytesToRead)
	{
		XAUDIO2_BUFFER tBuffer	= {0};
		tBuffer.pAudioData		= (unsigned char *) pBuffer;
		tBuffer.pContext		= pBuffer;
		tBuffer.AudioBytes		= tResult.uBytesToRead;

		if(tResult.bEOF && !m_bLoop)
			tBuffer.Flags = XAUDIO2_END_OF_STREAM;

		if(m_pMP3Decoder)
			m_pMP3Decoder->Decode(tBuffer.pAudioData, tBuffer.AudioBytes);

		HRESULT hr = m_pVoice->SubmitSourceBuffer(&tBuffer);
		RCNAssert(!FAILED(hr));
	}

	// Decrease stream job count
	CRCNAudioController::GetCriticalSection().Lock();
	CRCNAudioController::GetStreamCriticalSection().Lock();
	m_uStreamCount--;
	CRCNAudioController::GetCriticalSection().Unlock();
	CRCNAudioController::GetStreamCriticalSection().Unlock();
}

void CRCNXAudio2Source::Update()
{
	if(m_pVoice == NULL)
		return;

	XAUDIO2_VOICE_STATE tState;
	m_pVoice->GetState(&tState);
	if(tState.BuffersQueued == 0)
	{
		m_ePlaybackState = CRCNAudioSource::EStopped;
	}

	if(m_iUpdateFlags != 0)
	{
		if(m_iUpdateFlags & m_iUpdateVolume)
		{
			m_pVoice->SetVolume(GetVolume() * m_pContainer->m_fVolume);
		}

		m_iUpdateFlags = 0;
	}
}

void CRCNXAudio2Source::Initialise(CRCNFile* pFile)
{
	m_pFile = pFile;

	switch(m_pContainer->m_eFormat)
	{
	case EFormat_MP3:
		{
			// Check header looks like a valid mp3 file
			unsigned int iNumChannel		= 0;
			unsigned int iFrequency			= 0;
			unsigned int iAverageBitrate	= 0;
			const bool bIsMp3 = CRCNMP3::IsMP3(m_pFile, &iNumChannel, &iFrequency, &iAverageBitrate);

			RCNAssertStr(bIsMp3, "File does not appear to be an MP3");

			m_pXHeader = (WAVEFORMATEX *) RCNNew unsigned char[sizeof(WAVEFORMATEX)];
			m_pXHeader->wFormatTag		= WAVE_FORMAT_PCM;
			m_pXHeader->nChannels		= iNumChannel;
			m_pXHeader->nSamplesPerSec	= iFrequency;
			m_pXHeader->nAvgBytesPerSec	= iNumChannel * 2 * iFrequency;
			m_pXHeader->nBlockAlign		= iNumChannel * 2;
			m_pXHeader->wBitsPerSample	= 16;
			m_pXHeader->cbSize			= 0;

			m_tMetadata.uNumChannels	= iNumChannel;
			m_tMetadata.uSampleRate		= iFrequency;

			// Enough for approx 1 second of compressed data - and a sensible byte multiple
			m_tMetadata.uChunkSize = ((iAverageBitrate / 8) + 1023) & ~1023;

			// Allocate one data buffer, big enough to hold 2 chunks
			m_tMetadata.pData = RCNNew unsigned char[m_tMetadata.uChunkSize * 2];
		}
		break;
	}

	m_eFileState = ELoaded;
}