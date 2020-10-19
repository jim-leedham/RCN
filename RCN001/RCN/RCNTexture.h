/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNTexture.h
Description:    Provides a platform/API-agnostic texture interface

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNTEXTURE_H_
#define _RCN_RCNTEXTURE_H_

// -----------------------------------------
// INCLUDES

#include "RCN3D.h"

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNTexture
{
public:

				CRCNTexture();
				CRCNTexture(const char* sFilename);
				~CRCNTexture();

	void		Initialise();

	void		Load(const char* sFilename);

	void		Render();

	CRCNVector	GetCenter();
	CRCNVector	GetPosition();
	float		GetTransparency();
	int			GetWidth();
	int			GetHeight();
	int			GetX();
	int			GetY();

	void		SetPosition(CRCNVector vPosition);
	void		SetTransparency(float fTransparency);
	void		SetDimensions(int iWidth, int iHeight);
	void		SetRenderCoordinates(int iX, int iY);

#if defined DIRECTX
				operator IDirect3DTexture9* () const;
#endif

private:

	TRCNTexture**	m_pTexture;

	CRCNVector		m_vCenter;
	CRCNVector		m_vPosition;
	float			m_fTransparency;
	int				m_iWidth;
	int				m_iHeight;
	int				m_iX;
	int				m_iY;
};

#endif	// _RCN_RCNTEXTURE_H_