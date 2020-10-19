/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNScreen.cpp
Description:    Provides a common screen interface

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNScreen.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNScreen::CRCNScreen(const char* pScreenName)
{
	strncpy_s( m_cScreenName, pScreenName, ms_iMaxScreenNameLength );
}

CRCNScreen::~CRCNScreen()
{

}

void CRCNScreen::Process()
{
	if(m_eScreenState == EScreenState_Finalising)
	{
		RCNAssertStr(m_cPendingScreenName[0] != 0, "No screen name set!");
		CRCNScreenController::ReplaceScreen(m_cPendingScreenName);
	}
}

void CRCNScreen::ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState)
{

}

void CRCNScreen::Render2D(float fInterpolation)
{

}

void CRCNScreen::Render3D(float fInterpolation)
{

}

void CRCNScreen::SetPendingScreen(const char* cScreenName)
{
	strncpy_s( m_cPendingScreenName, cScreenName, ms_iMaxScreenNameLength );
}

void CRCNScreen::SetScreenState(EScreenState eScreenState)
{
	m_eScreenState = eScreenState;
}
