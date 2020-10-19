/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNButton.cpp
Description:    Provides a common button interface. Can be assigned any given
				texture for its default and OnClick states. Can be assigned
				a callback function to be called when leaving the OnClick state.

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNButton.h"
#include "RCNTexture.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNButton::CRCNButton()
{
	m_pDefaultTexture		= NULL;
	m_pCurrentTexture		= NULL;
	m_pOnClickTexture		= NULL;
	m_vPosition				= CRCNVector::s_ZeroVector;
	m_pfnOnReleaseFunction	= NULL;
}

CRCNButton::~CRCNButton()
{

}

void CRCNButton::Process()
{

}

void CRCNButton::ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState)
{
	switch(eInput)
	{
	case CRCNInput::EInput_LeftMouseButton:
		if(eInputState == CRCNInput::EInputState_Pressed)
		{
			if((m_vPosition.fX <= CRCNInput::GetMouseX() && CRCNInput::GetMouseX() <= (m_vPosition.fX + m_pCurrentTexture->GetWidth())) &&
				(m_vPosition.fY <= CRCNInput::GetMouseY() && CRCNInput::GetMouseY() <= (m_vPosition.fY + m_pCurrentTexture->GetHeight())))
			{
				m_pCurrentTexture = m_pOnClickTexture;
			}
		}
		else if(eInputState == CRCNInput::EInputState_Released)
		{
			if((m_vPosition.fX <= CRCNInput::GetMouseX() && CRCNInput::GetMouseX() <= (m_vPosition.fX + m_pCurrentTexture->GetWidth())) &&
				(m_vPosition.fY <= CRCNInput::GetMouseY() && CRCNInput::GetMouseY() <= (m_vPosition.fY + m_pCurrentTexture->GetHeight())))
			{
				m_pCurrentTexture = m_pDefaultTexture;
				m_pfnOnReleaseFunction();
			}
		}
		break;
	}
}

void CRCNButton::Render(float fInterpolation)
{
	RCNAssertStr(m_pCurrentTexture != NULL, "No default texture set for this button");
	m_pCurrentTexture->Render();
}

void CRCNButton::SetDefaultTexture(CRCNTexture* pTexture)
{
	m_pDefaultTexture = pTexture;
	m_pCurrentTexture = m_pDefaultTexture;
}

void CRCNButton::SetOnClickTexture(CRCNTexture* pTexture)
{
	m_pOnClickTexture = pTexture;
}

void CRCNButton::SetPosition(CRCNVector vPosition)
{
	m_vPosition = vPosition;

	if(m_pDefaultTexture)
		m_pDefaultTexture->SetPosition(m_vPosition);

	if(m_pOnClickTexture)
		m_pOnClickTexture->SetPosition(m_vPosition);
}

void CRCNButton::SetTransparency(float fTransparency)
{
	if(m_pDefaultTexture)
		m_pDefaultTexture->SetTransparency(fTransparency);

	if(m_pOnClickTexture)
		m_pOnClickTexture->SetTransparency(fTransparency);
}

void CRCNButton::SetOnReleaseFunction(FNButtonFunction pfnFunction)
{
	m_pfnOnReleaseFunction = pfnFunction;
}
