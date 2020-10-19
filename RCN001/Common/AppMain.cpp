/*---------------------------------------------------------------------------*
Project:		RCN
File:			AppMain.cpp
Description:    Runs the app for the appropriate platform

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "AppMain.h"

#if defined WIN32
#define APPPLATFORMCLASS AppPC
#endif

CApp *g_pApplication = 0;

int AppMain(int argc, char **argv, unsigned int uParam)
{
	CRCNMemUtil::EnableAllocationGuards();
	CRCNMemUtil::EnableAllocationTracking();

	g_pApplication = RCNNew APPPLATFORMCLASS(uParam);

	// Now is a good time to handle any command line options

	g_pApplication->Start();
	g_pApplication->MainLoop();
	g_pApplication->End();

	RCNDelete g_pApplication;

	CRCNMemUtil::Shutdown();

	return 0;
}