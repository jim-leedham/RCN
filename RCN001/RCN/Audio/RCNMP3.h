/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNMP3.h
Description:    An MP3 validation and decoding utility. Uses the mpg123 third
				party library.

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNMP3_H_
#define _RCN_RCNMP3_H_

#include "ThirdParty/mpg123/mpg123.h"

class CRCNFile;

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNMP3
{
public:

					CRCNMP3(unsigned int iAverageBytesPerSecond);
					~CRCNMP3();

	void			Decode(const unsigned char*& pData, unsigned int& uDataSize);

	static bool		IsMP3(CRCNFile* pFile, unsigned int* pNumChannel = NULL, unsigned int* pFrequency = NULL, unsigned int* pAverageBitrate = NULL);

private:

	CRCNMP3();							// Prevent void construction
	CRCNMP3(const CRCNMP3&);            // Prevent copy construction
	CRCNMP3& operator=(const CRCNMP3&); // Prevent assignment

	static CRCNCriticalSection& GetCriticalSection();

	struct TDecodeBuffer
	{
		TDecodeBuffer()
		{
			m_pOriginalData			= NULL;
			m_iOriginalDataSize		= 0;
			m_pDecodedData			= NULL;
			m_DecodedDataSize		= 0;
			m_DecodedDataCapacity	= 0;
		}

		const unsigned char*	m_pOriginalData;
		unsigned int			m_iOriginalDataSize;
		unsigned char*			m_pDecodedData;
		unsigned int			m_DecodedDataSize;
		unsigned int			m_DecodedDataCapacity;
	};

	static const bool			ms_bPrintDebugInfo = false;
	mpg123_handle*				m_pMPG123Handle;
	static const int			ms_iNumberOfDecodeBuffers = 2;
	TDecodeBuffer				m_pDecodeBuffer[ms_iNumberOfDecodeBuffers];
};

#endif	// _RCN_RCNMP3_H_