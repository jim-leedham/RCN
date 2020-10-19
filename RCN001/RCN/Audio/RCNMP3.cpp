/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNMP3.h
Description:    An MP3 validation and decoding utility. Uses the mpg123 third
				party library.

*---------------------------------------------------------------------------*/

#include "stdheader.h"
#include "RCNMP3.h"

#include "RCNFile.h"
#include "RCNFileSystem.h"
#include "RCNCriticalSection.h"

// -----------------------------------------
// CLASS DEFINITIONS

bool CRCNMP3::IsMP3(CRCNFile* pFile, unsigned int* pNumChannel, unsigned int* pFrequency, unsigned int* pAverageBitrate)
{
	unsigned char pTemp[8192];
	pFile->Seek(0, CRCNFileSystem::ESeekMethod::EStart);

	// MPG123 Init is not thread safe
	GetCriticalSection().Lock();
	mpg123_init();
	GetCriticalSection().Unlock();

	int iReturnValue = 0;
	mpg123_handle* pMPG123Handle = mpg123_new(NULL, &iReturnValue);
	if(pMPG123Handle == NULL)
		return false;

	mpg123_param(pMPG123Handle, MPG123_VERBOSE, ms_bPrintDebugInfo ? 2 : 0, 0);

	if(!ms_bPrintDebugInfo)
		mpg123_param(pMPG123Handle, MPG123_ADD_FLAGS, MPG123_QUIET, 0.0);

	mpg123_open_feed(pMPG123Handle);

	unsigned char pOutputBuffer[16384];
	const unsigned int iOutputBufferSize = sizeof(pOutputBuffer);

	int Encoding;
	int NumChannel;
	long Frequency;

	bool bFormatDetermined = false;

	while(true)
	{
		unsigned int iBytesRead = (unsigned int)pFile->Read(pTemp,sizeof(pTemp));
		RCNAssert(iBytesRead > 0);

		size_t OutputLength = 0;
		iReturnValue = mpg123_decode(pMPG123Handle, pTemp, iBytesRead, pOutputBuffer, iOutputBufferSize, &OutputLength);
		if(iReturnValue == MPG123_NEW_FORMAT)
		{
			mpg123_getformat(pMPG123Handle, &Frequency, &NumChannel, &Encoding);

			if(ms_bPrintDebugInfo)
				RCNPrintf("New format: %li Hz, %i channels, encoding value %i\n", Frequency, NumChannel, Encoding);

			if(pNumChannel)
				*pNumChannel = NumChannel;
			if(pFrequency)
				*pFrequency = Frequency;
			
			break;
		}
		
		if(ms_bPrintDebugInfo)
			RCNPrintf("Consume... %u Bytes\n", iBytesRead);

		// Consume rest of decoded data
		while(iReturnValue != MPG123_ERR && iReturnValue != MPG123_NEED_MORE)
			iReturnValue = mpg123_decode(pMPG123Handle, NULL, 0, pOutputBuffer, iOutputBufferSize, &OutputLength);

		if(iReturnValue==MPG123_ERR || !iBytesRead)
			return false;
	}

	if(pAverageBitrate)
	{
		off_t Length = mpg123_length(pMPG123Handle);

		if(Length)
		{
			float EstimatedNumSamples = (float)Length;
			float EstimatedDuration = EstimatedNumSamples / (float)Frequency;

			float FileSize = (float)pFile->GetLength();

			*pAverageBitrate = (unsigned int)(FileSize * 8.0f / EstimatedDuration);
		}
		else
		{
			// Fallback to 128kbps
			*pAverageBitrate = 128*1024;

			if(ms_bPrintDebugInfo)
				RCNPrintf("Guessing this mp3 is 128kbps\n");
		}
	}

	pFile->Seek(0, CRCNFileSystem::ESeekMethod::EStart);

	mpg123_delete(pMPG123Handle);

	// MPG123 Exit is not thread safe
	GetCriticalSection().Lock();
	mpg123_exit();
	GetCriticalSection().Unlock();

	return iReturnValue == MPG123_OK || iReturnValue == MPG123_NEW_FORMAT || iReturnValue == MPG123_NEED_MORE || iReturnValue == MPG123_DONE;
}

CRCNCriticalSection& CRCNMP3::GetCriticalSection()
{
	static CRCNCriticalSection criticalSection;
	return criticalSection;
}

CRCNMP3::CRCNMP3(unsigned int iAverageBytesPerSecond)
{
	// Allocate decode buffers, 1.5 seconds long - this will grow if too small
	for(int i = 0; i < ms_iNumberOfDecodeBuffers; i++)
	{
		m_pDecodeBuffer[i].m_DecodedDataCapacity = ((iAverageBytesPerSecond + (iAverageBytesPerSecond >> 1)) + 1023) & ~1023;
		m_pDecodeBuffer[i].m_pDecodedData = RCNNew unsigned char[m_pDecodeBuffer[i].m_DecodedDataCapacity];
		m_pDecodeBuffer[i].m_DecodedDataSize = 0;
	}

	// MPG123 Init is not thread safe
	GetCriticalSection().Lock();
	mpg123_init();
	GetCriticalSection().Unlock();

	int iRet = 0;
	m_pMPG123Handle = mpg123_new(NULL, &iRet);
	RCNAssert(m_pMPG123Handle);
	if(m_pMPG123Handle == NULL)
		return;

	mpg123_param(m_pMPG123Handle, MPG123_VERBOSE, ms_bPrintDebugInfo ? 2 : 0, 0);

	if(!ms_bPrintDebugInfo)
		mpg123_param(m_pMPG123Handle, MPG123_ADD_FLAGS, MPG123_QUIET, 0.0);

	mpg123_open_feed(m_pMPG123Handle);
}

CRCNMP3::~CRCNMP3()
{
	// Release decode buffers
	for(int i = 0; i < ms_iNumberOfDecodeBuffers; i++)
	{
		if(m_pDecodeBuffer[i].m_pDecodedData)
			RCNDelete [] m_pDecodeBuffer[i].m_pDecodedData;
	}

	if(m_pMPG123Handle)
	{
		mpg123_delete(m_pMPG123Handle);
		m_pMPG123Handle = NULL;
	}
}

void CRCNMP3::Decode(const unsigned char*& pData, unsigned int& uDataSize)
{
	if(ms_bPrintDebugInfo)
		RCNPrintf("Decode %d bytes @ %p\n", uDataSize, pData);

	TDecodeBuffer &rDecodeBuffer = ( m_pDecodeBuffer[0].m_pOriginalData == NULL || m_pDecodeBuffer[0].m_pOriginalData == pData) ? m_pDecodeBuffer[0] : m_pDecodeBuffer[1];

	rDecodeBuffer.m_pOriginalData		= pData;
	rDecodeBuffer.m_iOriginalDataSize	= uDataSize;

	unsigned char* pDest		= rDecodeBuffer.m_pDecodedData;
	unsigned int iDestRemain	= rDecodeBuffer.m_DecodedDataCapacity;

	size_t iBytesRead = 0;
	int iRet = mpg123_decode(m_pMPG123Handle, rDecodeBuffer.m_pOriginalData, rDecodeBuffer.m_iOriginalDataSize, pDest, iDestRemain, &iBytesRead);
	if(iRet == MPG123_NEW_FORMAT)
	{
		long rate;
		int channels, enc;
		mpg123_getformat(m_pMPG123Handle, &rate, &channels, &enc);

		if(ms_bPrintDebugInfo)
			RCNPrintf("New format: %li Hz, %i channels, encoding value %i\n", rate, channels, enc);
	}

	pDest += iBytesRead;
	iDestRemain -= iBytesRead;

	while(iRet != MPG123_ERR && iRet != MPG123_NEED_MORE)
	{
		if(!iDestRemain)
		{
			// Increase buffer size by about 50%

			unsigned int PrevCapacity	= rDecodeBuffer.m_DecodedDataCapacity;
			unsigned char *pPrevBuffer	= rDecodeBuffer.m_pDecodedData;

			unsigned int NewCapacity = ((PrevCapacity + (PrevCapacity >> 1)) + 1023) & ~1023;
			unsigned char *pNewDecodeBuffer = RCNNew unsigned char[NewCapacity];

			if(ms_bPrintDebugInfo)
				RCNPrintf("%p resize from %u to %u\n", &rDecodeBuffer, PrevCapacity, NewCapacity);

			RCNAssert(pNewDecodeBuffer);
			memcpy(pNewDecodeBuffer, pPrevBuffer, PrevCapacity);

			RCNDelete [] pPrevBuffer;

			rDecodeBuffer.m_pDecodedData		= pNewDecodeBuffer;
			rDecodeBuffer.m_DecodedDataCapacity = NewCapacity;

			iDestRemain = NewCapacity - PrevCapacity;
			pDest = pNewDecodeBuffer + PrevCapacity;
		}

		iRet = mpg123_decode(m_pMPG123Handle, NULL, 0, pDest, iDestRemain, &iBytesRead);

		pDest += iBytesRead;
		iDestRemain -= iBytesRead;
	}

	rDecodeBuffer.m_DecodedDataSize = pDest - rDecodeBuffer.m_pDecodedData;

	RCNAssert(iRet == MPG123_OK || iRet == MPG123_DONE || iRet == MPG123_NEED_MORE);

	pData = rDecodeBuffer.m_pDecodedData;
	uDataSize = rDecodeBuffer.m_DecodedDataSize;

	if(ms_bPrintDebugInfo)
		RCNPrintf("Playing %6d bytes %06x\n", uDataSize, pData);
}
