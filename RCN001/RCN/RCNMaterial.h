/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNMaterial.h
Description:    Provides a platform/API-agnostic material interface

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNMATERIAL_H_
#define _RCN_RCNMATERIAL_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNMaterial
{
public:

					CRCNMaterial();
					~CRCNMaterial();

	void			SetDiffuseColour(unsigned int iColour);
	void			SetAmbientColour(unsigned int iColour);
	void			SetSpecularColour(unsigned int iColour);
	void			SetEmissiveColour(unsigned int iColour);
	void			SetPower(float fPower);

	unsigned int	GetDiffuseColour();
	unsigned int	GetAmbientColour();
	unsigned int	GetSpecularColour();
	unsigned int	GetEmissiveColour();
	float			GetPower();

#if defined DIRECTX
					operator D3DMATERIAL9* () const;
	CRCNMaterial&	operator = (const D3DMATERIAL9& Material);
#endif

private:

#if defined DIRECTX
	D3DMATERIAL9*	m_tD3DMaterial;
#endif
};

#endif	// _RCN_RCNMATERIAL_H_