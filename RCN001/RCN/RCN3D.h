/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCN3D.h
Description:    Provides classes and functions for common 3D mathematical uses.
				Includes Vector, Vertex and Matrix implementations.

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCN3D_H_
#define _RCN_RCN3D_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNVector
{
public:

						CRCNVector();
						CRCNVector(float fX, float fY, float fZ);
						CRCNVector(const CRCNVector& vVector);
						~CRCNVector();

	static float		Magnitude(const CRCNVector& vA);
	static float		MagnitudeSquared(const CRCNVector& vA);

	static float		DotProduct(const CRCNVector& vA, const CRCNVector& vB);
	static float		Theta(const CRCNVector& vA, const CRCNVector& vB);

	static CRCNVector	CrossProduct(const CRCNVector& vA, const CRCNVector& vB);

	CRCNVector			operator -	(const CRCNVector& vRHS) const;
	void				operator -= (const CRCNVector& vRHS);

	CRCNVector			operator +	(const CRCNVector& vRHS) const;
	void				operator += (const CRCNVector& vRHS);

	bool				operator == (const CRCNVector& vRHS) const;
	bool				operator != (const CRCNVector& vRHS) const;

	CRCNVector			operator -	() const;

	CRCNVector			operator *	(const float fRHS) const;

#if defined DIRECTX
						operator D3DXVECTOR3 () const;
#endif
			
	float fX;
	float fY;
	float fZ;

	static CRCNVector	s_ZeroVector;

private:
};

class CRCNVertex
{
public:
	CRCNVertex();
	CRCNVertex(float fX, float fY, float fZ, const CRCNVector& vNormal, float fU, float fV);
	CRCNVertex(float fX, float fY, float fZ, float fNX, float fNY, float fNZ, float fU, float fV);
	~CRCNVertex();

	// NOTE: Size of this class must equal that defined by RCNFVF in RCNRenderer.cpp (at least while using DirectX)

	float fX;
	float fY;
	float fZ;
	CRCNVector vNormal;
	float fU;
	float fV;

private:
};

class CRCNMatrix
{
public:
	CRCNMatrix();
	CRCNMatrix(float f00, float f01, float f02, float f03,
			   float f10, float f11, float f12, float f13,
			   float f20, float f21, float f22, float f23,
			   float f30, float f31, float f32, float f33);

	~CRCNMatrix();

	static CRCNMatrix MakeIdentityMatrix();

	static CRCNMatrix MakeTranslationMatrix(float fX, float fY, float fZ);
	static CRCNMatrix MakeTranslationMatrix(const CRCNVector& vPosition);

	static CRCNMatrix MakeScaleMatrix(float fScale);
	static CRCNMatrix MakeScaleMatrix(float fX, float fY, float fZ);

	static CRCNMatrix MakeXRotationMatrix(float fRadians);
	static CRCNMatrix MakeYRotationMatrix(float fRadians);
	static CRCNMatrix MakeZRotationMatrix(float fRadians);

	static CRCNMatrix MakeOrientationMatrix32(const CRCNVector& vDir, const CRCNVector *pUpDirection);

	static CRCNMatrix MakeProjectionMatrix32(float fFOV, float fNearPlane, float fFarPlane);
	static CRCNMatrix MakeViewMatrix32(const CRCNVector& vPosition, const CRCNVector& vLookAt, const CRCNVector& vUp);

	static CRCNMatrix Multiply(const CRCNMatrix& m1, const CRCNMatrix& m2);

#if defined DIRECTX
	operator D3DXMATRIX* () const;
#endif

	float fM[4][4];

private:
};

#endif	// _RCN_RCN3D_H_