/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNLight.h
Description:    Provides a platform/API-agnostic light interface

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNLIGHT_H_
#define _RCN_RCNLIGHT_H_

// -----------------------------------------
// INCLUDES

#include "RCN3D.h"

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNLight
{
public:

	CRCNLight();
	~CRCNLight();

	enum EType
	{
		EType_Point,
		EType_Spot,
		EType_Directional,
	};

	void SetType(EType eType);
	void SetDiffuseColour(unsigned int iColour);
	void SetSpecularColour(unsigned int iColour);
	void SetAmbientColour(unsigned int iColour);
	void SetPosition(CRCNVector& vPosition);
	void SetDirection(CRCNVector& vDirection);
	void SetRange(float fRange);
	void SetFalloff(float fFalloff);
	void SetConstantAttenuation(float fConstantAttenuation);
	void SetLinearAttenuation(float fLinearAttenuation);
	void SetQuadraticAttenuation(float fQuadraticAttenuation);
	void SetInnerConeAngle(float fAngle);
	void SetOuterConeAngle(float fAngle);

#if defined DIRECTX
	operator D3DLIGHT9* () const;
#endif

private:

#if defined DIRECTX
	D3DLIGHT9*		m_tD3DLight;
#endif
};

#endif	// _RCN_RCNLIGHT_H_