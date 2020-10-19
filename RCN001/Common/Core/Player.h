/*---------------------------------------------------------------------------*
Project:		RCN
File:			Player.h
Description:    Holds all data about the player

*---------------------------------------------------------------------------*/

#ifndef _COMMON_CORE_PLAYER_H_
#define _COMMON_CORE_PLAYER_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CPlayer
{
public:
						CPlayer();
						~CPlayer();

	void				SetPosition(const CRCNVector& vPosition);
	CRCNVector			GetPosition();

	CRCNVector			GetDirection();

	float				GetHeight();

	void				Process();
	void				ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState);

	CRCNVector			GetPositionThisFrame(float fInterpolation);
	CRCNVector			GetDirectionThisFrame(float fInterpolation);

	bool				HasPendingMovement();
	CRCNVector			GetPendingMovement();
	CRCNVector			GetPendingPosition();

	void				InvalidatePendingMovement();

private:

	static const float	m_fTurnSpeed;
	static const float	m_fWalkSpeed;

	float				m_fFacing;
	bool				m_bWalking;

	CRCNVector			m_vDirection;
	CRCNVector			m_vPosition;
	CRCNVector			m_vDimensions;

	CRCNVector			m_vPendingMovement;
	CRCNVector			m_vPendingDirection;
};

#endif	// _COMMON_CORE_PLAYER_H_