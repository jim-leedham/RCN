/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNLight.cpp
Description:    A common light interface

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNLight::CRCNLight()
{
#if defined DIRECTX
	m_tD3DLight = RCNNew D3DLIGHT9();
#endif
}

CRCNLight::~CRCNLight()
{
#if defined DIRECTX
	RCNDelete m_tD3DLight;
#endif
}

void CRCNLight::SetType(EType eType)
{
#if defined DIRECTX
	switch(eType)
	{
	case EType_Point:		m_tD3DLight->Type = D3DLIGHT_POINT;		break;
	case EType_Spot:		m_tD3DLight->Type = D3DLIGHT_SPOT;		break;
	case EType_Directional: m_tD3DLight->Type = D3DLIGHT_DIRECTIONAL; break;
	default:
		RCNAssertStr(false, "Invalid type");
	}
#endif
}

void CRCNLight::SetDiffuseColour(unsigned int iColour)
{
#if defined DIRECTX
	m_tD3DLight->Diffuse = D3DXCOLOR(iColour);
#endif
}

void CRCNLight::SetSpecularColour(unsigned int iColour)
{
#if defined DIRECTX
	m_tD3DLight->Specular = D3DXCOLOR(iColour);
#endif
}

void CRCNLight::SetAmbientColour(unsigned int iColour)
{
#if defined DIRECTX
	m_tD3DLight->Ambient = D3DXCOLOR(iColour);
#endif
}

void CRCNLight::SetPosition(CRCNVector& vPosition)
{
#if defined DIRECTX
	m_tD3DLight->Position = vPosition;
#endif
}

void CRCNLight::SetDirection(CRCNVector& vDirection)
{
#if defined DIRECTX
	m_tD3DLight->Direction = vDirection;
#endif
}

void CRCNLight::SetRange(float fRange)
{
#if defined DIRECTX
	m_tD3DLight->Range = fRange;
#endif
}

void CRCNLight::SetFalloff(float fFalloff)
{
#if defined DIRECTX
	m_tD3DLight->Falloff = fFalloff;
#endif
}

void CRCNLight::SetConstantAttenuation(float fConstantAttenuation)
{
#if defined DIRECTX
	m_tD3DLight->Attenuation0 = fConstantAttenuation;
#endif
}

void CRCNLight::SetLinearAttenuation(float fLinearAttenuation)
{
#if defined DIRECTX
	m_tD3DLight->Attenuation1 = fLinearAttenuation;
#endif
}

void CRCNLight::SetQuadraticAttenuation(float fQuadraticAttenuation)
{
#if defined DIRECTX
	m_tD3DLight->Attenuation2 = fQuadraticAttenuation;
#endif
}

void CRCNLight::SetInnerConeAngle(float fAngle)
{
#if defined DIRECTX
	m_tD3DLight->Theta = fAngle;
#endif
}

void CRCNLight::SetOuterConeAngle(float fAngle)
{
#if defined DIRECTX
	m_tD3DLight->Phi = fAngle;
#endif
}

#if defined DIRECTX
CRCNLight::operator D3DLIGHT9*() const
{
	return (D3DLIGHT9*)m_tD3DLight;
}
#endif