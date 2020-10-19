/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNMaterial.cpp
Description:    Provides a platform/API-agnostic material interface

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNMaterial::CRCNMaterial()
{
#if defined DIRECTX
	m_tD3DMaterial = RCNNew D3DMATERIAL9();
#endif
}

CRCNMaterial::~CRCNMaterial()
{
#if defined DIRECTX
	RCNDelete m_tD3DMaterial;
#endif
}

#if defined DIRECTX
CRCNMaterial::operator D3DMATERIAL9*() const
{
	return (D3DMATERIAL9*)m_tD3DMaterial;
}

CRCNMaterial& CRCNMaterial::operator =(const D3DMATERIAL9& Material)
{
	*m_tD3DMaterial = Material;
	return *this;
}
#endif

void CRCNMaterial::SetDiffuseColour(unsigned int iColour)
{
#if defined DIRECTX
	m_tD3DMaterial->Diffuse = D3DXCOLOR(iColour);
#endif
}

void CRCNMaterial::SetAmbientColour(unsigned int iColour)
{
#if defined DIRECTX
	m_tD3DMaterial->Ambient = D3DXCOLOR(iColour);
#endif
}

void CRCNMaterial::SetSpecularColour(unsigned int iColour)
{
#if defined DIRECTX
	m_tD3DMaterial->Specular = D3DXCOLOR(iColour);
#endif
}

void CRCNMaterial::SetEmissiveColour(unsigned int iColour)
{
#if defined DIRECTX
	m_tD3DMaterial->Emissive = D3DXCOLOR(iColour);
#endif
}

void CRCNMaterial::SetPower(float fPower)
{
#if defined DIRECTX
	m_tD3DMaterial->Power = fPower;
#endif
}

unsigned int CRCNMaterial::GetDiffuseColour()
{
#if defined DIRECTX
	return (D3DXCOLOR)m_tD3DMaterial->Diffuse;
#endif
}

unsigned int CRCNMaterial::GetAmbientColour()
{
#if defined DIRECTX
	return (D3DXCOLOR)m_tD3DMaterial->Ambient;
#endif
}

unsigned int CRCNMaterial::GetSpecularColour()
{
#if defined DIRECTX
	return (D3DXCOLOR)m_tD3DMaterial->Specular;
#endif
}

unsigned int CRCNMaterial::GetEmissiveColour()
{
#if defined DIRECTX
	return (D3DXCOLOR)m_tD3DMaterial->Emissive;
#endif
}

float CRCNMaterial::GetPower()
{
#if defined DIRECTX
	return m_tD3DMaterial->Power;
#endif
}
