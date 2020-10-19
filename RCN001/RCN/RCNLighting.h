/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNLighting.h
Description:    Handles setting up of lights

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNLIGHTING_H_
#define _RCN_RCNLIGHTING_H_

// -----------------------------------------
// FORWARD DECLARATIONS

class CRCNLight;

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNLighting
{
public:

						CRCNLighting();
						~CRCNLighting();

	static void			Initialise();
	static void			Shutdown();

	static void			EnableLight(int iIndex);
	static void			DisableLight(int iIndex);

	static CRCNLight*	GetLight(int iIndex);

	static bool			IsLightEnabled(int iIndex);

private:

	static const int	ms_iMaxLights = 8;
	static CRCNLight*	ms_tLights[ms_iMaxLights];
};

#endif	// _RCN_RCNLIGHTING_H_