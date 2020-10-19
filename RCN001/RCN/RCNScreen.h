/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNScreen.h
Description:    Provides a common screen interface

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNSCREEN_H_
#define _RCN_RCNSCREEN_H_

// -----------------------------------------
// INCLUDES

#include "RCNInput.h"

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNScreen
{
public:

	enum EScreenState
	{
		EScreenState_Undefined = -1,
		EScreenState_Initialised,
		EScreenState_FadingIn,
		EScreenState_FadingOut,
		EScreenState_Finalising,
		EScreenState_Count
	};

						CRCNScreen(const char* pScreenName);
	virtual				~CRCNScreen();

	virtual void		Process();
	virtual void		ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState);
	virtual void		Render2D(float fInterpolation);
	virtual void		Render3D(float fInterpolation);

	virtual void		SetPendingScreen(const char* cScreenName);
	virtual void		SetScreenState(EScreenState eScreenState);

protected:

	EScreenState		m_eScreenState;

private:

	static const int	ms_iMaxScreenNameLength = 256;
	char				m_cScreenName[ms_iMaxScreenNameLength];
	char				m_cPendingScreenName[ms_iMaxScreenNameLength];
};

#endif	// _RCN_RCNSCREEN_H_