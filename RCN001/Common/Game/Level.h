/*---------------------------------------------------------------------------*
Project:		RCN
File:			Level.h
Description:    Provides an interface for in-game levels

*---------------------------------------------------------------------------*/

#ifndef _COMMON_GAME_LEVEL_H_
#define _COMMON_GAME_LEVEL_H_

// -----------------------------------------
// FORWARD DECLARATIONS

// -----------------------------------------
// CLASS DECLARATIONS

class ILevel
{
public:
							ILevel();
	virtual					~ILevel();

	virtual void			Process()																						= 0;
	virtual void			ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState)						= 0;

	virtual void			Render(float fInterpolation)																	= 0;

	virtual bool			ValidatePlayerMovement(CRCNVector vPendingPlayerPosition, CRCNVector vPendingPlayerMovement)	= 0;

private:

};

#endif	// _COMMON_GAME_LEVEL_H_