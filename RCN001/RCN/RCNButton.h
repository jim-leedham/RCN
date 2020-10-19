/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNButton.h
Description:    Provides a common button interface. Can be assigned any given
				texture for its default and OnClick states. Can be assigned
				a callback function to be called when leaving the OnClick state.

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNBUTTON_H_
#define _RCN_RCNBUTTON_H_

// -----------------------------------------
// INCLUDES

#include "RCNInput.h"
#include "RCN3D.h"

// -----------------------------------------
// FORWARD DECLARATIONS

class CRCNTexture;

// -----------------------------------------
// CLASS DECLARATIONS

typedef void (*FNButtonFunction)();

class CRCNButton
{
public:

						CRCNButton();
						~CRCNButton();

	void				Process();
	void				ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState);

	void				Render(float fInterpolation);

	void				SetDefaultTexture(CRCNTexture* pTexture);
	void				SetOnClickTexture(CRCNTexture* pTexture);
	void				SetPosition(CRCNVector vPosition);
	void				SetTransparency(float fTransparency);
	void				SetOnReleaseFunction(FNButtonFunction pfnFunction);

private:

	CRCNTexture*		m_pDefaultTexture;
	CRCNTexture*		m_pOnClickTexture;
	CRCNTexture*		m_pCurrentTexture;

	CRCNVector			m_vPosition;

	FNButtonFunction	m_pfnOnReleaseFunction;
};

#endif	// _RCN_RCNBUTTON_H_