/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNScreenController.cpp
Description:    Mediates all screen operations. Maintains a stack of screen 
				objects and controls their rendering and processing.

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNScreenController.h"
#include "RCNScreen.h"

// -----------------------------------------
// STATIC DEFINITIONS

FNScreenInstantiationFunction CRCNScreenController::m_pfnScreenInstantiationFunction;
TScreenStack CRCNScreenController::m_tScreenStack;

// -----------------------------------------
// CLASS DEFINITIONS

CRCNScreenController::CRCNScreenController()
{

}

CRCNScreenController::~CRCNScreenController()
{

}

void CRCNScreenController::Initialise(FNScreenInstantiationFunction fnScreenInstantiationFunction)
{
	m_pfnScreenInstantiationFunction = fnScreenInstantiationFunction;

	GoToScreen("");
}

void CRCNScreenController::GoToScreen(const char* pScreenName)
{
	CRCNScreen* pScreen = m_pfnScreenInstantiationFunction(pScreenName);

	m_tScreenStack.Push(pScreen);
}

void CRCNScreenController::ReplaceScreen(const char* cScreenName)
{
	CRCNScreen* pScreen = m_pfnScreenInstantiationFunction(cScreenName);
	
	RCNDelete m_tScreenStack.Pop();
	m_tScreenStack.Push(pScreen);
}

void CRCNScreenController::Process()
{
	m_tScreenStack.Peek()->Process();
}

void CRCNScreenController::ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState)
{
	m_tScreenStack.Peek()->ProcessInput(eInput, eInputState);
}

void CRCNScreenController::Render2D(float fInterpolation)
{
	m_tScreenStack.Peek()->Render2D(fInterpolation);
}

void CRCNScreenController::Render3D(float fInterpolation)
{
	m_tScreenStack.Peek()->Render3D(fInterpolation);
}

void CRCNScreenController::Shutdown()
{
	while(m_tScreenStack.m_iScreenStackPosition > 0)
	{
		RCNDelete m_tScreenStack.Pop();
	}
}

CRCNScreen* CRCNScreenController::GetCurrentScreen()
{
	return m_tScreenStack.Peek();
}
