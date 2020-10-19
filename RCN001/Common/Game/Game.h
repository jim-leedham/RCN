/*---------------------------------------------------------------------------*
Project:		RCN
File:			Game.h
Description:    Handles the main game flow

*---------------------------------------------------------------------------*/

#ifndef _COMMON_GAME_GAME_H_
#define _COMMON_GAME_GAME_H_

// -----------------------------------------
// FORWARD DECLARATIONS

class ILevel;
class CPlayer;

// -----------------------------------------
// CLASS DECLARATIONS

class CGame
{
public:
	CGame();
	~CGame();

	void					Process();
	void					ProcessInput(CRCNInput::EInput iInput, CRCNInput::EInputState eInputState);

	void					Render(float fInterpolation);

	void					LoadLevel();
	void					UnloadLevel();

	inline	CRCNCamera*		GetCamera()		const		{ return m_pCamera;			}

private:

	CRCNCamera*				m_pCamera;
	ILevel*					m_pLevel;
	CPlayer*				m_pPlayer;
};

#endif	// _COMMON_GAME_GAME_H_