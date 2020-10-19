/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNLighting.cpp
Description:    Handles setting up of lights

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"

// -----------------------------------------
// STATIC DEFINITIONS

CRCNLight* CRCNLighting::ms_tLights[ms_iMaxLights];

// -----------------------------------------
// CLASS DEFINITIONS

CRCNLighting::CRCNLighting()
{
}

CRCNLighting::~CRCNLighting()
{
}

void CRCNLighting::Initialise()
{
	for(int i = 0; i < ms_iMaxLights; i++)
		ms_tLights[i] = NULL;
}

void CRCNLighting::Shutdown()
{
	for(int i = 0; i < ms_iMaxLights; i++)
	{
		if(ms_tLights[i] != NULL)
			RCNDelete ms_tLights[i];
	}
}

void CRCNLighting::EnableLight(int iIndex)
{
	RCNAssertStr(0 <= iIndex && iIndex < ms_iMaxLights, "Light index out of range");
	RCNAssertStr(ms_tLights[iIndex] != NULL, "Light has not been initialised");

#if defined DIRECTX
	CRCNRenderer::GetRenderDevice()->SetLight(iIndex, static_cast<D3DLIGHT9*>(*ms_tLights[iIndex]));
	CRCNRenderer::GetRenderDevice()->LightEnable(iIndex, true);
#endif
}

void CRCNLighting::DisableLight(int iIndex)
{
	RCNAssertStr(0 <= iIndex && iIndex < ms_iMaxLights, "Light index out of range");
	RCNAssertStr(ms_tLights[iIndex] != NULL, "Light has not been initialised");
	RCNAssertStr(IsLightEnabled(iIndex) == true, "Light is not enabled");

#if defined DIRECTX
	CRCNRenderer::GetRenderDevice()->LightEnable(iIndex, false);
#endif
}

CRCNLight* CRCNLighting::GetLight(int iIndex)
{
	RCNAssertStr(0 <= iIndex && iIndex < ms_iMaxLights, "Light index out of range");
	if(ms_tLights[iIndex] == NULL)
		ms_tLights[iIndex] = RCNNew CRCNLight();

	RCNAssertStr(ms_tLights[iIndex], "Invalid light ID"); return ms_tLights[iIndex];
}

bool CRCNLighting::IsLightEnabled(int iIndex)
{
	RCNAssertStr(0 <= iIndex && iIndex < ms_iMaxLights, "Light index out of range");
	RCNAssertStr(ms_tLights[iIndex] != NULL, "Light has not been initialised");

#if defined DIRECTX
	BOOL bEnabled = false;
	CRCNRenderer::GetRenderDevice()->GetLightEnable(iIndex, &bEnabled);
	return bEnabled > 0;
#endif
}