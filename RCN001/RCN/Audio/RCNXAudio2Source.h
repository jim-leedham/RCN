/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNXAudio2Source.h
Description:    Provides an XAudio2-specific audio source implementation

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNXAUDIO2SOURCE_H_
#define _RCN_RCNXAUDIO2SOURCE_H_

#include "RCNAudioSource.h"

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNXAudio2Source : public CRCNAudioSource, public IXAudio2VoiceCallback
{
public:

								CRCNXAudio2Source(CRCNContainer* pContainer);
	virtual						~CRCNXAudio2Source();

	virtual	void				Start();
	virtual void				Stop();
	virtual void				Update();

	STDMETHOD_(void, OnBufferEnd)(void *pBufferContext);

	STDMETHOD_(void, OnStreamEnd)()										{}
	STDMETHOD_(void, OnVoiceProcessingPassEnd)()						{}
	STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32 SamplesRequired){}
	STDMETHOD_(void ,OnBufferStart)(void *pBufferContext)				{}
	STDMETHOD_(void, OnLoopEnd)(void *pBufferContext)					{}
	STDMETHOD_(void, OnVoiceError)(void *pBufferContext, HRESULT Error)	{}

protected:

	struct TBufferFillJobParamaters
	{
		CRCNXAudio2Source*		pSource;
		void*					pBuffer;
	};

	virtual void				Initialise(CRCNFile* pFile);
	static	void				FillBufferJob(CRCNJob* pJob, void* pParameters);
	virtual void				ResetBuffer();
	virtual void				StreamCallback(void* pBuffer, const TBufferFillResult& tResult, unsigned int uParam);

	WAVEFORMATEX*				m_pXHeader;
	IXAudio2SourceVoice*		m_pVoice;
	void*						m_pBufferContext;

};

#endif	// _RCN_RCNXAUDIO2SOURCE_H_