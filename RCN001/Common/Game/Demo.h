/*---------------------------------------------------------------------------*
Project:		RCN
File:			Demo.h
Description:    A basic demo level containing a few models, background music,
				text rendering and player movement

*---------------------------------------------------------------------------*/

#ifndef _COMMON_GAME_DEMO_H_
#define _COMMON_GAME_DEMO_H_

// -----------------------------------------
// INCLUDES

#include "Level.h"

// -----------------------------------------
// FORWARD DECLARATIONS

// -----------------------------------------
// CLASS DECLARATIONS

class CDemo : public ILevel
{
public:
					CDemo();
					~CDemo();

	void			Process();
	void			ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState);

	void			Render(float fInterpolation);

	bool			ValidatePlayerMovement(CRCNVector vPendingPlayerPosition, CRCNVector vPendingPlayerMovement);

private:

	CRCNVector		m_vCube1Position;
	CRCNVector		m_vCube2Position;
	CRCNVector		m_vPlanePosition;

	CRCNCube*		m_pCube1;
	CRCNCube*		m_pCube2;
	CRCNModel*		m_pPlane;

	bool			m_bPendingTranslation;
	CRCNVector		m_vPendingTranslation;
};

#endif	// _COMMON_GAME_DEMO_H_