/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNFileSystem.h
Description:    Provides a platform/API-agnostic file system interface

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNFILESYSTEM_H_
#define _RCN_RCNFILESYSTEM_H_

class CRCNFile;

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNFileSystem
{
public:

	enum EAccessMode
	{
		EReadOnly,
		EWriteOnly,
		EReadWrite
	};

	enum ESeekMethod
	{
		EStart,
		EEnd,
		ECurrent
	};

	static void			Initialise();
	static void			Shutdown();

	static CRCNFile*	Open(const char* pFilename, EAccessMode eAccessMode = EReadOnly);

private:

	static const int	ms_iMaxDirectoryLength = 256;
	static char			ms_pCurrentDirectory[ms_iMaxDirectoryLength];

	static const int	ms_iMaxAbsoluteFilenameLength = 512;
};

#endif	// _RCN_RCNFILESYSTEM_H_