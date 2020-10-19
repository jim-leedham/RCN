/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCN3D.cpp
Description:    Provides classes and functions for common 3D mathematical uses.
				Includes Vector, Vertex and Matrix implementations.

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCN3D.h"

// -----------------------------------------
// STATIC DEFINITIONS

CRCNVector	CRCNVector::s_ZeroVector = CRCNVector(0.0f, 0.0f, 0.0f);

// -----------------------------------------
// CLASS DEFINITIONS

CRCNVector::CRCNVector()
{

}

CRCNVector::CRCNVector(float fX, float fY, float fZ)
{
	this->fX = fX;
	this->fY = fY;
	this->fZ = fZ;
}

CRCNVector::CRCNVector(const CRCNVector& vVector)
{
	this->fX = vVector.fX;
	this->fY = vVector.fY;
	this->fZ = vVector.fZ;
}

float CRCNVector::Magnitude(const CRCNVector& vA)
{
	return RCNSquareRoot(MagnitudeSquared(vA));
}

float CRCNVector::MagnitudeSquared(const CRCNVector& vA)
{
	return vA.fX * vA.fX + vA.fY * vA.fY + vA.fZ * vA.fZ;
}

float CRCNVector::DotProduct(const CRCNVector& vA, const CRCNVector& vB)
{
	return vA.fX * vB.fX + vA.fY * vB.fY + vA.fZ * vB.fZ;
}

float CRCNVector::Theta(const CRCNVector& vA, const CRCNVector& vB)
{
	return RCNACos(DotProduct(vA, vB) / (RCNSquareRoot(MagnitudeSquared(vA) * MagnitudeSquared(vB))));
}

CRCNVector CRCNVector::CrossProduct(const CRCNVector& vA, const CRCNVector& vB)
{
	return CRCNVector((vA.fY * vB.fZ) - (vA.fZ * vB.fY),
					  (vA.fZ * vB.fX) - (vA.fX * vB.fZ),
					  (vA.fX * vB.fY) - (vA.fY * vB.fX));
}

CRCNVector CRCNVector::operator - (const CRCNVector& vRHS) const
{
	return CRCNVector(this->fX - vRHS.fX, this->fY - vRHS.fY, this->fZ - vRHS.fZ);
}

void CRCNVector::operator -= (const CRCNVector& vRHS)
{
	this->fX -= vRHS.fX; 
	this->fY -= vRHS.fY; 
	this->fZ -= vRHS.fZ;
}

CRCNVector CRCNVector::operator + (const CRCNVector& vRHS) const
{
	return CRCNVector(this->fX + vRHS.fX, this->fY + vRHS.fY, this->fZ + vRHS.fZ);
}

void CRCNVector::operator += (const CRCNVector& vRHS)
{
	this->fX += vRHS.fX; 
	this->fY += vRHS.fY; 
	this->fZ += vRHS.fZ;
}

bool CRCNVector::operator == (const CRCNVector& vRHS) const
{
	return ((this->fX == vRHS.fX) && (this->fY == vRHS.fY) && (this->fZ == vRHS.fZ));
}

bool CRCNVector::operator != (const CRCNVector& vRHS) const
{
	return ! operator == (vRHS);
}

CRCNVector CRCNVector::operator * (const float fRHS) const
{
	return CRCNVector(this->fX * fRHS, this->fY * fRHS, this->fZ * fRHS);
}

CRCNVector CRCNVector::operator - () const
{
	return CRCNVector(-fX, -fY, -fZ);
}

CRCNVector::~CRCNVector()
{

}

#if defined DIRECTX
CRCNVector::operator D3DXVECTOR3() const
{
	return (D3DXVECTOR3)&fX;
}
#endif

CRCNVertex::CRCNVertex()
{
	fX = 0.0f;
	fY = 0.0f;
	fZ = 0.0f;
	vNormal.fX = 0.0f;
	vNormal.fY = 0.0f;
	vNormal.fZ = 0.0f;
	fU = 0.0f;
	fV = 0.0f;
}

CRCNVertex::CRCNVertex(float fX, float fY, float fZ, const CRCNVector& vNormal, float fU, float fV)
{
	this->fX = fX;
	this->fY = fY;
	this->fZ = fZ;
	this->vNormal = CRCNVector(vNormal);
	this->fU = fU;
	this->fV = fV;
}

CRCNVertex::CRCNVertex(float fX, float fY, float fZ, float fNX, float fNY, float fNZ, float fU, float fV)
{
	this->fX = fX;
	this->fY = fY;
	this->fZ = fZ;
	this->vNormal = CRCNVector(fNX, fNY, fNZ);
	this->fU = fU;
	this->fV = fV;
}

CRCNVertex::~CRCNVertex()
{

}

CRCNMatrix::CRCNMatrix()
{

}

CRCNMatrix::CRCNMatrix(float f00, float f01, float f02, float f03, 
					   float f10, float f11, float f12, float f13, 
					   float f20, float f21, float f22, float f23, 
					   float f30, float f31, float f32, float f33)
{
	fM[0][0] = f00; fM[0][1] = f01; fM[0][2] = f02; fM[0][3] = f03;
	fM[1][0] = f10; fM[1][1] = f11; fM[1][2] = f12; fM[1][3] = f13;
	fM[2][0] = f20; fM[2][1] = f21; fM[2][2] = f22; fM[2][3] = f23;
	fM[3][0] = f30; fM[3][1] = f31; fM[3][2] = f32; fM[3][3] = f33;
}

CRCNMatrix::~CRCNMatrix()
{

}

CRCNMatrix CRCNMatrix::MakeIdentityMatrix()
{
	return CRCNMatrix(1.0f, 0.0f, 0.0f, 0.0f, 
					  0.0f, 1.0f, 0.0f, 0.0f,
					  0.0f, 0.0f, 1.0f, 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f);
}

CRCNMatrix CRCNMatrix::MakeTranslationMatrix(float fX, float fY, float fZ)
{
	CRCNMatrix mMatrix = MakeIdentityMatrix();
	mMatrix.fM[3][0] = fX;
	mMatrix.fM[3][1] = fY;
	mMatrix.fM[3][2] = fZ;
	return mMatrix;
}

CRCNMatrix CRCNMatrix::MakeTranslationMatrix(const CRCNVector& vPosition)
{
	return MakeTranslationMatrix(vPosition.fX, vPosition.fY, vPosition.fZ);
}

CRCNMatrix CRCNMatrix::MakeScaleMatrix(float fX, float fY, float fZ)
{
	CRCNMatrix mMatrix = MakeIdentityMatrix();
	mMatrix.fM[0][0] = fX;
	mMatrix.fM[1][1] = fY;
	mMatrix.fM[2][2] = fZ;
	return mMatrix;
}

CRCNMatrix CRCNMatrix::MakeScaleMatrix(float fScale)
{
	return MakeScaleMatrix(fScale, fScale, fScale);
}

CRCNMatrix CRCNMatrix::MakeXRotationMatrix(float fRadians)
{
	CRCNMatrix mMatrix = MakeIdentityMatrix();
	mMatrix.fM[1][1] = cosf(fRadians);
	mMatrix.fM[1][2] = sinf(fRadians);
	mMatrix.fM[2][1] = -mMatrix.fM[1][2];
	mMatrix.fM[2][2] = mMatrix.fM[1][1];
	return mMatrix;
}

CRCNMatrix CRCNMatrix::MakeYRotationMatrix(float fRadians)
{
	CRCNMatrix mMatrix = MakeIdentityMatrix();
	mMatrix.fM[0][0] = cosf(fRadians);
	mMatrix.fM[2][0] = sinf(fRadians);
	mMatrix.fM[0][2] = -mMatrix.fM[2][0];
	mMatrix.fM[2][2] = mMatrix.fM[0][0];
	return mMatrix;
}

CRCNMatrix CRCNMatrix::MakeZRotationMatrix(float fRadians)
{
	CRCNMatrix mMatrix = MakeIdentityMatrix();
	mMatrix.fM[0][0] = cosf(fRadians);
	mMatrix.fM[0][1] = sinf(fRadians);
	mMatrix.fM[1][0] = -mMatrix.fM[0][1];
	mMatrix.fM[1][1] = mMatrix.fM[0][0];
	return mMatrix;
}

CRCNMatrix CRCNMatrix::MakeOrientationMatrix32(const CRCNVector& vDir, const CRCNVector *pUpDirection)
{
	// TODO
	return MakeIdentityMatrix();
}

CRCNMatrix CRCNMatrix::MakeProjectionMatrix32(float fFOV, float fNearPlane, float fFarPlane)
{
	// TODO
	return MakeIdentityMatrix();
}

CRCNMatrix CRCNMatrix::MakeViewMatrix32(const CRCNVector& vPosition, const CRCNVector& vLookAt, const CRCNVector& vUp)
{
	// TODO
	return MakeIdentityMatrix();
}

CRCNMatrix CRCNMatrix::Multiply(const CRCNMatrix& m1, const CRCNMatrix& m2)
{
	CRCNMatrix m3(0.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 0.0f);

	float* fM1 = (float*)&m1;
	float* fM2 = (float*)&m2;
	float* fM3 = (float*)&m3;

	int i, j, k;
	for(i = 0; i < 4; i++) 
	{
		for(j = 0; j < 4; j++) 
		{
			for(k = 0; k < 4; k++)
			{
				fM3[4 * i + j] += fM1[4 * k + j] * fM2[4 * i + k];
			}
		}
	}

	return m3;
}

#if defined DIRECTX
CRCNMatrix::operator D3DXMATRIX*() const
{
	return (D3DXMATRIX*)fM;
}
#endif