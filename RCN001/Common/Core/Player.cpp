/*---------------------------------------------------------------------------*
Project:		RCN
File:			Player.cpp
Description:    Holds all data about the player

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "Player.h"

// -----------------------------------------
// STATIC DEFINITIONS

const float CPlayer::m_fTurnSpeed = 0.1f;
const float CPlayer::m_fWalkSpeed = 3.0f;	

// -----------------------------------------
// CLASS DEFINITIONS

CPlayer::CPlayer()
{
	m_vDimensions		= CRCNVector(1.0f, 1.0f, 1.0f);
	m_vPosition			= CRCNVector::s_ZeroVector;
	m_fFacing			= 0.0f;
	m_vDirection		= CRCNVector(0.0f, 0.0f, -1.0f);
	m_vDirection.fX		= RCNSin(m_fFacing);
	m_vDirection.fZ		= RCNCos(m_fFacing);
	m_vPendingMovement	= CRCNVector::s_ZeroVector;
	m_vPendingDirection	= CRCNVector::s_ZeroVector;
}

CPlayer::~CPlayer()
{
}

void CPlayer::SetPosition(const CRCNVector& vPosition)
{
	m_vPosition = vPosition;
}

float CPlayer::GetHeight()
{
	return m_vDimensions.fY;
}

CRCNVector CPlayer::GetPosition()
{
	return m_vPosition;
}

CRCNVector CPlayer::GetDirection()
{
	return m_vDirection;
}

void CPlayer::ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState)
{
	switch(eInput)
	{
	case CRCNInput::EInput_W:
		if(eInputState == CRCNInput::EInputState_Held)
		{
			m_vPendingMovement = m_vDirection * m_fWalkSpeed;
		}
		break;
	case CRCNInput::EInput_S:
		if(eInputState == CRCNInput::EInputState_Held)
		{
			m_vPendingMovement = -m_vDirection * m_fWalkSpeed;
		}
		break;
	case CRCNInput::EInput_A:
		if(eInputState == CRCNInput::EInputState_Held)
		{
			m_fFacing -= m_fTurnSpeed;
			m_vPendingDirection.fX = RCNSin(m_fFacing);
			m_vPendingDirection.fZ = RCNCos(m_fFacing);

			if(m_vPendingDirection.fX < m_vDirection.fX)
				m_vPendingDirection.fX = -(m_vDirection.fX - m_vPendingDirection.fX);
			else
				m_vPendingDirection.fX = m_vPendingDirection.fX - m_vDirection.fX;

			if(m_vPendingDirection.fZ < m_vDirection.fZ)
				m_vPendingDirection.fZ = -(m_vDirection.fZ - m_vPendingDirection.fZ);
			else
				m_vPendingDirection.fZ = m_vPendingDirection.fZ - m_vDirection.fZ;
		}
		break;
	case CRCNInput::EInput_D:
		if(eInputState == CRCNInput::EInputState_Held)
		{
			m_fFacing += m_fTurnSpeed;
			m_vPendingDirection.fX = RCNSin(m_fFacing);
			m_vPendingDirection.fZ = RCNCos(m_fFacing);

			if(m_vPendingDirection.fX < m_vDirection.fX)
				m_vPendingDirection.fX = -(m_vDirection.fX - m_vPendingDirection.fX);
			else
				m_vPendingDirection.fX = m_vPendingDirection.fX - m_vDirection.fX;

			if(m_vPendingDirection.fZ < m_vDirection.fZ)
				m_vPendingDirection.fZ = -(m_vDirection.fZ - m_vPendingDirection.fZ);
			else
				m_vPendingDirection.fZ = m_vPendingDirection.fZ - m_vDirection.fZ;
		}
		break;
	}
}

void CPlayer::Process()
{
	if(m_vPendingMovement != CRCNVector::s_ZeroVector)
	{
		m_vPosition += m_vPendingMovement;
		m_vPendingMovement = CRCNVector::s_ZeroVector;
	}

	if(m_vPendingDirection != CRCNVector::s_ZeroVector)
	{
		m_vDirection += m_vPendingDirection;
		m_vPendingDirection = CRCNVector::s_ZeroVector;
	}
}

CRCNVector CPlayer::GetPositionThisFrame(float fInterpolation)
{
	return m_vPosition + (m_vPendingMovement * fInterpolation);
}

CRCNVector CPlayer::GetDirectionThisFrame(float fInterpolation)
{
	return m_vDirection + (m_vPendingDirection * fInterpolation);
}

bool CPlayer::HasPendingMovement()
{
	return m_vPendingMovement != CRCNVector::s_ZeroVector;
}

CRCNVector CPlayer::GetPendingMovement()
{
	return m_vPendingMovement;
}

CRCNVector CPlayer::GetPendingPosition()
{
	return m_vPosition + m_vPendingMovement;
}

void CPlayer::InvalidatePendingMovement()
{
	m_vPendingMovement = CRCNVector::s_ZeroVector;
}
