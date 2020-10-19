/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNCamera.cpp
Description:    Provides a common camera interface

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNCamera.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNCamera::CRCNCamera()
{

}

CRCNCamera::~CRCNCamera()
{

}

void CRCNCamera::SetPosition(CRCNVector& vPosition)
{
	m_vPosition = vPosition;
}

void CRCNCamera::SetLookAt(CRCNVector& vLookUp)
{
	m_vLookAt = vLookUp;
}

void CRCNCamera::SetUp(CRCNVector& vUp)
{
	m_vUp = vUp;
}

void CRCNCamera::SetFieldOfView(float fFieldOfView)
{
	m_fFieldOfView = fFieldOfView;
}

void CRCNCamera::SetNearPlane(float fNearPlane)
{
	m_fNearPlane = fNearPlane;
}

void CRCNCamera::SetFarPlane(float fFarPlane)
{
	m_fFarPlane = fFarPlane;
}
