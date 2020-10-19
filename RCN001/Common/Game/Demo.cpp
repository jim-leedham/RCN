/*---------------------------------------------------------------------------*
Project:		RCN
File:			Demo.cpp
Description:    A basic demo level containing a few models, background music,
				text rendering and player movement

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "Demo.h"
#include "Core/ModelManager.h"

// -----------------------------------------
// CLASS DEFINITIONS

CDemo::CDemo()
{
	m_vCube1Position = CRCNVector(-10.0f, -2.0f, 25.0f);
	m_vCube2Position = CRCNVector( 10.0f,  0.0f, 35.0f);
	m_vPlanePosition = CRCNVector( 0.0f,  4.0f,  25.0f);

	m_pCube1 = RCNNew CRCNCube();
	m_pCube1->SetTransform(CRCNMatrix::MakeTranslationMatrix(m_vCube1Position));

	m_pCube2 = RCNNew CRCNCube();
	m_pCube2->SetTransform(CRCNMatrix::MakeTranslationMatrix(m_vCube2Position));

	m_pPlane = g_pApplication->GetModelManager()->GetModel(EModelID_Plane);
	m_pPlane->SetTransform(CRCNMatrix::Multiply(CRCNMatrix::MakeTranslationMatrix(m_vPlanePosition), CRCNMatrix::MakeYRotationMatrix(180.0f)));

	CRCNLighting::GetLight(0)->SetType(CRCNLight::EType_Point);
	CRCNLighting::GetLight(0)->SetDiffuseColour(0xff888888);
	CRCNLighting::GetLight(0)->SetPosition(CRCNVector(0.0f, 3.0f, 25.0f));		// Just under the plane
	CRCNLighting::GetLight(0)->SetRange(100.0f);
	CRCNLighting::GetLight(0)->SetConstantAttenuation(0.0f);
	CRCNLighting::GetLight(0)->SetLinearAttenuation(0.125f);
	CRCNLighting::GetLight(0)->SetQuadraticAttenuation(0.0f);

	CRCNLighting::GetLight(1)->SetType(CRCNLight::EType_Directional);
	CRCNLighting::GetLight(1)->SetDiffuseColour(0xff888888);
	CRCNLighting::GetLight(1)->SetDirection(CRCNVector(0.0f, -1.0f, 1.0f));

	CRCNLighting::GetLight(2)->SetType(CRCNLight::EType_Spot);
	CRCNLighting::GetLight(2)->SetDiffuseColour(0xff888888);
	CRCNLighting::GetLight(2)->SetPosition(CRCNVector(0.0f, 0.0f, 0.0f));
	CRCNLighting::GetLight(2)->SetDirection(CRCNVector(-10.0f, -2.0f, 25.0f));	// Pointed at m_pCube1
	CRCNLighting::GetLight(2)->SetRange(100.0f);
	CRCNLighting::GetLight(2)->SetFalloff(1.0f);
	CRCNLighting::GetLight(2)->SetConstantAttenuation(0.0f);
	CRCNLighting::GetLight(2)->SetLinearAttenuation(0.125f);
	CRCNLighting::GetLight(2)->SetQuadraticAttenuation(0.0f);
	CRCNLighting::GetLight(2)->SetOuterConeAngle(40.0f * DEG_TO_RAD);
	CRCNLighting::GetLight(2)->SetInnerConeAngle(20.0f * DEG_TO_RAD);

	CRCNLighting::EnableLight(0);
}

CDemo::~CDemo()
{
	RCNDelete m_pCube1;
	RCNDelete m_pCube2;

	g_pApplication->GetModelManager()->ReleaseModel(EModelID_Plane);
}

void CDemo::Process()
{

}

void CDemo::ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState)
{
	switch(eInput)
	{
	case CRCNInput::EInput_1:
		if(eInputState == CRCNInput::EInputState_Pressed)
		{
			if(CRCNLighting::IsLightEnabled(0))
				CRCNLighting::DisableLight(0);
			else
				CRCNLighting::EnableLight(0);
		}
		break;
	case CRCNInput::EInput_2:
		if(eInputState == CRCNInput::EInputState_Pressed)
		{
			if(CRCNLighting::IsLightEnabled(1))
				CRCNLighting::DisableLight(1);
			else
				CRCNLighting::EnableLight(1);
		}
		break;
	case CRCNInput::EInput_3:
		if(eInputState == CRCNInput::EInputState_Pressed)
		{
			if(CRCNLighting::IsLightEnabled(2))
				CRCNLighting::DisableLight(2);
			else
				CRCNLighting::EnableLight(2);
		}
		break;
	}
}

void CDemo::Render(float fInterpolation)
{
	m_pCube1->Render();
	m_pCube2->Render();
	m_pPlane->Render();
}

bool CDemo::ValidatePlayerMovement(CRCNVector vPendingPlayerPosition, CRCNVector vPendingPlayerMovement)
{
	// The function exists to give the game a chance to perform some collision detection.
	// Since we do not have any real geometry in this demo, we will simply do radius-based 
	// collision detection on the two cubes.

	if(CRCNVector::MagnitudeSquared(m_vCube1Position - vPendingPlayerPosition) < 25)
		return false;

	if(CRCNVector::MagnitudeSquared(m_vCube2Position - vPendingPlayerPosition) < 25)
		return false;

	return true;
}
