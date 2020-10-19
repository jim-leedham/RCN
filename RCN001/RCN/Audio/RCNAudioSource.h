/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNAudioSource.h
Description:    Provides a platform/API-agnostic audio source interface

Notes:			Currently we only support streamed audio sources. The next step
				will be to add support for patchbanks.

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNAUDIOSOURCE_H_
#define _RCN_RCNAUDIOSOURCE_H_

#include "RCNFile.h"

// -----------------------------------------
// FORWARD DECLARATIONS

class CRCNContainer;
class CRCNFile;
class CRCNMP3;
class CRCNJob;

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNAudioSource
{
public:

	enum EFormat
	{
		EFormat_INVALID,
		EFormat_MP3,
	};

	enum EFileState
	{
		EIdle,
		ELoading,
		ELoaded,
		EReleased,
	};

	enum EPlaybackState
	{
		EInitial,
		EPlaying,
		EStopped,
		EPaused,
	};

	enum ERelease
	{
		EImmediate,
		EOnceFinished,
		EOnceFaded,
	};

	struct TMetadata 
	{
		unsigned char			uNumChannels;	// Number of channels
		unsigned int			uSampleRate;	// Rate per channel, Hz
		unsigned char*			pData;			// Data
		unsigned int			uTotalDataSize; // Total data length in bytes (including any potential header that's been included)
		unsigned int			uChunkSize;		// Chunk size in bytes, 0 if not chunked.
	};

	struct TBufferFillResult 
	{
		unsigned int			uFileIdx;		// Which stream file to read from
		unsigned int			uBytesToRead;	// Number of bytes that were read (or number of bytes available for QueryBuffer())
		bool					bEOF;			// True if the stream has reached its end
		unsigned int			uStartSample;	// Sample to start playback at within the buffer (inclusive)
		unsigned int			uEndSample;		// Sample to end playback at within the buffer (exclusive)
		unsigned int			uBytePos;		// Start position of the data in the file (in bytes), measured relative to the start of the stream (i.e. doesn't count header bytes).
	};

	struct TBufferFillData 
	{
		TBufferFillResult		tResult;		// The result of the fill operation
		CRCNFileAsyncEvent		tEvent;			// File system event (called when the operation completes)
		CRCNAudioSource*		pSource;		// The AudioSource being filled
	};

								CRCNAudioSource(CRCNContainer* pContainer);
	virtual						~CRCNAudioSource();

	void						Play();
	void						SetLoop(bool bLoop);
	void						SetFade(float fTargetVolume, int iDuration);
	void						Release(ERelease eRelease);
	void						SetVolume(float fVolume);
	float						GetVolume();

	static float				ms_fMaximumVolume;
	static float				ms_fMinimumVolume;

protected:

	static void					Process(CRCNAudioSource* pSource, void* pParameters);
	static void					Release(CRCNAudioSource* pSource, void* pParameters);

	virtual void				Initialise(CRCNFile* pFile);
	virtual void				ResetBuffer();

	TBufferFillResult			QueryBuffer(unsigned int uFilePos, unsigned int uMaxBytes);
	virtual unsigned int		SuggestBufferAmount();
	virtual TBufferFillResult	FillBuffer(void* pBuffer, unsigned int uBufferSize, unsigned int uParam);
	static void					FillBufferCallback(CRCNFileAsyncEvent* pEvent, const TRCNFileAsyncEventData* pData, void* pParameters);
	virtual void				StreamCallback(void* pBuffer, const TBufferFillResult& tResult, unsigned int uParam) = 0;
	virtual void				WaitForOutstandingStreamOperations();

	CRCNContainer*				m_pContainer;

	static const int			ms_iMaxBuffers = 2;
	TBufferFillData				m_tBufferFillData[ms_iMaxBuffers];
	int							m_iBufferFillDataIndex;

	CRCNFile*					m_pFile;
	TMetadata					m_tMetadata;

	EPlaybackState				m_ePlaybackState;
	EFileState					m_eFileState;

	unsigned int				m_uStreamCount;
	unsigned int				m_uFilePos;

	CRCNMP3*					m_pMP3Decoder;

	static const int			m_iUpdateVolume	= 0x01;
	static const int			m_iUpdateLoop	= 0x02;
	static const int			m_iUpdatePitch	= 0x04;

	int							m_iUpdateFlags;
	bool						m_bLoop;

	friend class				CRCNAudio;
	friend class				CRCNAudioController;

private:

	CRCNAudioSource();									// Prevent void construction
	CRCNAudioSource(const CRCNAudioSource&);            // Prevent copy construction
	CRCNAudioSource& operator=(const CRCNAudioSource&); // Prevent assignment

	static void					InitialiseJob(CRCNJob* pJob, void* pParameters);
	static void					PlayJob(CRCNJob* pJob, void* pParameters);

	void						Process(int iDeltaTime);
	virtual void				Update();
	virtual void				Start();
	virtual void				Stop();

	bool						m_bReleased;
	ERelease					m_eRelease;

	float						m_fVolume;
	bool						m_bFade;
	int							m_iFadeRemain;
	float						m_fFadeDelta;
};

#endif	// _RCN_RCNAUDIOSOURCE_H_