/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNFile.cpp
Description:    Provides a platform/API-agnostic file interface

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNFile.h"
#include "RCNFileSystem.h"

#if defined (RCNWINDOWS)
#include <sys/stat.h>
#endif

// -----------------------------------------
// CLASS DEFINITIONS

CRCNFile::CRCNFile(const char* pFilename, int eAccessMode)
{
#if defined (RCNWINDOWS)

	// Create file handle
	m_pFile = INVALID_HANDLE_VALUE;
	m_bOpen = false;

	DWORD iAccessMode			= 0;
	DWORD iCreationDisposition	= OPEN_EXISTING;
	DWORD iFlagsAndAttributes	= FILE_ATTRIBUTE_NORMAL;
	DWORD iShareMode			= 0;

	switch(eAccessMode)
	{
	case CRCNFileSystem::EAccessMode::EReadOnly:
		iAccessMode				= GENERIC_READ;
		iCreationDisposition	= OPEN_EXISTING;
		iShareMode				= FILE_SHARE_READ;
		break;
	case CRCNFileSystem::EAccessMode::EWriteOnly:
		iAccessMode				= GENERIC_WRITE;
		iCreationDisposition	= CREATE_ALWAYS;
		break;
	case CRCNFileSystem::EAccessMode::EReadWrite:
		iAccessMode				= GENERIC_READ | GENERIC_WRITE;
		iCreationDisposition	= OPEN_EXISTING;
		break;
	default:
		RCNAssertStr(false, "Unknown access mode");
	}

	m_pFile = CreateFile(pFilename, iAccessMode, iShareMode, NULL, iCreationDisposition, iFlagsAndAttributes, NULL);

	RCNAssertStr(m_pFile != INVALID_HANDLE_VALUE, "Failed to open file");	// TODO: Better error logging here!

	// Cache file attributes
	if(m_pFile != INVALID_HANDLE_VALUE)
	{
		m_bOpen	= true;

		struct _stat64 tFileStats;
		int iResult = _stat64(pFilename, &tFileStats);
		RCNAssertStr(iResult == 0, "Failed to retrieve file attributes");

		if(iResult == 0)
		{
			m_uLength		= tFileStats.st_size;
			m_uLastModified	= tFileStats.st_mtime;
		}
		else
		{
			Close();
		}
	}

#endif

	// Store filename
	RCNAssertStr(strlen(pFilename) < m_iMaxFilenameLength, "Filename exceeds maximum length");
	strcpy_s(m_pFilename, pFilename);

}

CRCNFile::~CRCNFile()
{
	Close();
}

signed long long CRCNFile::Seek(signed long long iDistanceToMove, int eSeekMethod)
{
	RCNAssertStr(m_bOpen, "File is not open");
	if(!m_bOpen)
		return -1;

#if defined (RCNWINDOWS)
	DWORD iSeekMethod;
	switch(eSeekMethod)
	{
	case CRCNFileSystem::EStart:
		iSeekMethod = FILE_BEGIN;
		break;
	case CRCNFileSystem::EEnd:
		iSeekMethod = FILE_END;
		break;
	case CRCNFileSystem::ECurrent:
		iSeekMethod = FILE_CURRENT;
		break;
	default:
		RCNAssertStr(false, "Unknown seek method");
	}

	return SetFilePointer(m_pFile, (DWORD)iDistanceToMove, NULL, iSeekMethod);
#endif
}

signed long long CRCNFile::Read(void* pBuffer, signed long long iBytesToRead)
{
	RCNAssertStr(m_bOpen, "File is not open");
	RCNAssertStr(iBytesToRead > 0, "iBytesToRead must be greater than zero");

	if(!m_bOpen || iBytesToRead < 1)
		return -1;

#if defined (RCNWINDOWS)
	DWORD iNumBytesRead = 0;
	int iSuccess = ReadFile(m_pFile, pBuffer, (DWORD)iBytesToRead, &iNumBytesRead, NULL);
#endif

	RCNAssertStr(iSuccess != 0, "Failed to read file");

	if(iSuccess != 0)
		return iNumBytesRead;

	return -1;
}

void CRCNFile::Write()
{
	// TODO
}

void CRCNFile::Close()
{
	if(m_bOpen)
	{
#if defined (RCNWINDOWS)
		CloseHandle(m_pFile);
#endif
		m_bOpen = false;
	}
}

bool CRCNFile::IsOpen()
{
	return m_bOpen;
}

unsigned long long CRCNFile::GetLength()
{
	return m_uLength;
}

void CRCNFile::ReadAsync(void* pBuffer, signed long long iPos, signed long long iCount, void* pParameters, CRCNFileAsyncEvent* pEvent)
{
	if(!m_bOpen)
		return;

	TRCNFileAsyncEventData tData;
	tData.pFile = this;
	tData.pBuffer = pBuffer;
	tData.iPos = iPos;
	tData.iCount = iCount;
	tData.pUserData = pParameters;
	tData.bWrite = false;
	if(iCount != 0)
	{
		signed long long iResult = Seek(iPos, CRCNFileSystem::EStart);
		if(iResult != iPos)
		{
			// TODO: Error handling
			if(iResult >= 0)
				iResult = -1;
		}
		else
		{
			iResult = Read(pBuffer, iCount);
		}
		tData.iResult = iResult;
	}
	else
		tData.iResult = 0;

	pEvent->Complete(&tData);
}

void CRCNFileAsyncEvent::AddCallback(FNAsyncEventCallback pFNCallack, void* pParameters)
{
	m_pFNCallback = pFNCallack;
	m_pParameters = pParameters;
}

void CRCNFileAsyncEvent::Complete(const TRCNFileAsyncEventData* pData)
{
	m_pFNCallback(this, pData, m_pParameters);
}
