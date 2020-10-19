/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNCamera.h
Description:    Provides a common camera interface

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNCAMERA_H_
#define _RCN_RCNCAMERA_H_

// -----------------------------------------
// INCLUDES

#include "RCN3D.h"

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNCamera
{
public:

	CRCNCamera();
	~CRCNCamera();

	void					SetPosition(CRCNVector& vPosition);
	void					SetLookAt(CRCNVector& vLookUp);
	void					SetUp(CRCNVector& vUp);
	void					SetFieldOfView(float fFieldOfView);
	void					SetNearPlane(float fNearPlane);
	void					SetFarPlane(float fFarPlane);

	const CRCNVector		GetPosition()		const		{ return m_vPosition;	}
	const CRCNVector*		GetLookAt()			const		{ return &m_vLookAt;	}
	const CRCNVector*		GetUp()				const		{ return &m_vUp;		}
	float					GetFieldOfView()	const		{ return m_fFieldOfView;}
	float					GetNearPlane()		const		{ return m_fNearPlane;	}
	float					GetFarPlane()		const		{ return m_fFarPlane;	}

private:

	CRCNVector		m_vPosition;
	CRCNVector		m_vLookAt;
	CRCNVector		m_vUp;

	float			m_fFieldOfView;
	float			m_fNearPlane;
	float			m_fFarPlane;
};

#endif	// _RCN_RCNCAMERA_H_