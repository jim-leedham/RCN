/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNFileSystem.cpp
Description:    Provides a platform/API-agnostic file system interface

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNFileSystem.h"
#include "RCNFile.h"

// -----------------------------------------
// CLASS DEFINITIONS

char CRCNFileSystem::ms_pCurrentDirectory[ms_iMaxDirectoryLength];

void CRCNFileSystem::Initialise()
{
#if defined (RCNWINDOWS)
	GetCurrentDirectory(sizeof(ms_pCurrentDirectory), ms_pCurrentDirectory);
#endif
}

void CRCNFileSystem::Shutdown()
{

}

CRCNFile* CRCNFileSystem::Open(const char* pFilename, EAccessMode eAccessMode)
{
	RCNAssertStr(strlen(ms_pCurrentDirectory) + 1 + strlen(pFilename) + 1 < ms_iMaxAbsoluteFilenameLength, "Filename exceeds allowed length");
	char pAbsoluteFilename[ms_iMaxAbsoluteFilenameLength];

	strcpy_s(pAbsoluteFilename, ms_pCurrentDirectory);
	strcat_s(pAbsoluteFilename, "\\");
	strcat_s(pAbsoluteFilename, pFilename);

	CRCNFile* pFile = RCNNew CRCNFile(pAbsoluteFilename, eAccessMode);
	
	return pFile;
}
