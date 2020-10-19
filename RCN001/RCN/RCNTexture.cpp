/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNTexture.cpp
Description:    Provides a platform/API-agnostic texture interface

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNTexture.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNTexture::CRCNTexture()
{
	Initialise();
}

CRCNTexture::CRCNTexture(const char* sFilename)
{
	Initialise();
	Load(sFilename);	
}

CRCNTexture::~CRCNTexture()
{
	if(m_pTexture != NULL)
	{
		CRCNRenderer::ReleaseTexture(*m_pTexture);
		RCNDelete m_pTexture;
	}
}

void CRCNTexture::Initialise()
{
	m_pTexture = NULL;
	m_vCenter = CRCNVector(0.0f, 0.0f, 0.0f);	// Default: Upper-left corner of the texture
	m_vPosition = CRCNVector(0.0f, 0.0f, 0.0f);
	m_fTransparency = 1.0f;
	m_iWidth = 0;
	m_iHeight = 0;
	m_iX = 0;
	m_iY = 0;
}

void CRCNTexture::Load(const char* sFilename)
{
#if defined DIRECTX
	m_pTexture = RCNNew LPDIRECT3DTEXTURE9();
#endif
	CRCNRenderer::CreateTexture(sFilename, *m_pTexture, this);
}

void CRCNTexture::Render()
{
	CRCNRenderer::Render(this);
}

CRCNVector CRCNTexture::GetCenter()
{
	return m_vCenter;
}

CRCNVector CRCNTexture::GetPosition()
{
	return m_vPosition;
}

float CRCNTexture::GetTransparency()
{
	return m_fTransparency;
}

int CRCNTexture::GetWidth()
{
	return m_iWidth;
}

int CRCNTexture::GetHeight()
{
	return m_iHeight;
}

int CRCNTexture::GetX()
{
	return m_iX;
}

int CRCNTexture::GetY()
{
	return m_iY;
}

#if defined DIRECTX
CRCNTexture::operator IDirect3DTexture9*() const
{
	if(m_pTexture)
		return (IDirect3DTexture9*)*m_pTexture;
	else
		return NULL;
}
#endif

void CRCNTexture::SetPosition(CRCNVector vPosition)
{
	m_vPosition = vPosition;
}

void CRCNTexture::SetTransparency(float fTransparency)
{
	RCNAssertStr(0.0f <= fTransparency && fTransparency <= 1.0f, "Transparency must be between 0.0 and 1.0");
	m_fTransparency = fTransparency;
}

void CRCNTexture::SetDimensions(int iWidth, int iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
}

void CRCNTexture::SetRenderCoordinates(int iX, int iY)
{
	m_iX = iX;
	m_iY = iY;
}
