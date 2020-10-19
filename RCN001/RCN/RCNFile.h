/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNFile.h
Description:    Provides a platform/API-agnostic file interface

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNFILE_H_
#define _RCN_RCNFILE_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNFileAsyncEvent;

class CRCNFile
{
public:

							CRCNFile(const char* pFilename, int iAccessMode);
							~CRCNFile();

	signed long long		Seek(signed long long iDistanceToMove, int eSeekMethod);
	signed long long		Read(void* pBuffer, signed long long iBytesToRead);
	void					Write();
	void					Close();

	bool					IsOpen();
	unsigned long long		GetLength();

	void					ReadAsync(void* pBuffer, signed long long iPos, signed long long iCount, void* pParameters, CRCNFileAsyncEvent* pEvent);

private:

#if defined (RCNWINDOWS)
	HANDLE					m_pFile;
#endif
	static const int		m_iMaxFilenameLength = 512;
	char					m_pFilename[m_iMaxFilenameLength];
	bool					m_bOpen;
	unsigned long long		m_uLength;
	unsigned long long		m_uLastModified;
};

// Event data for completion of async file operations
class TRCNFileAsyncEventData
{
public:

	CRCNFile*			pFile;		// File that the event is for
	void*				pBuffer;	// Buffer that we were reading/writing into/from
	signed long long	iPos;		// Position in file we were accessing
	signed long long	iCount;		// Number of bytes we were asked to read/write
	const void*			pUserData;	// User data associated with this op
	bool				bWrite;		// True for if it was a write operation, false for read
	signed long long	iResult;	// Result of file operation (Return value of Seek()/Read()/Write())
};

class CRCNFileAsyncEvent;

typedef void (*FNAsyncEventCallback)(CRCNFileAsyncEvent* pEvent, const TRCNFileAsyncEventData* pData, void* pParameters);

class CRCNFileAsyncEvent
{
public:

	void					AddCallback(FNAsyncEventCallback pFNCallack, void* pParameters);
	void					Complete(const TRCNFileAsyncEventData* pData);

private:

	FNAsyncEventCallback	m_pFNCallback;
	void*					m_pParameters;
};

#endif	// _RCN_RCNFILE_H_