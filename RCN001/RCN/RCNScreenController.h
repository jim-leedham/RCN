/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNScreenController.h
Description:    Mediates all screen operations. Maintains a stack of screen 
				objects and controls their rendering and processing.

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNSCREENCONTROLLER_H_
#define _RCN_RCNSCREENCONTROLLER_H_

// -----------------------------------------
// INCLUDES

#include "RCNInput.h"

// -----------------------------------------
// FORWARD DECLARATIONS

class CRCNScreen;

// -----------------------------------------
// CLASS DECLARATIONS

typedef struct TScreenStack
{
	TScreenStack()
	{
		m_iScreenStackPosition = 0;
	}

	static const int ms_iMaxScreens = 8;

	void Push(CRCNScreen* pScreen)
	{
		RCNAssertStr(m_iScreenStackPosition < ms_iMaxScreens, "Cannot add screen. Consider increasing ms_iMaxScreens");

		m_pScreens[m_iScreenStackPosition] = pScreen;
		m_iScreenStackPosition++;
	}

	CRCNScreen* Pop()
	{
		RCNAssertStr(m_iScreenStackPosition > 0, "No screen to pop.");

		CRCNScreen* pScreen = m_pScreens[m_iScreenStackPosition-1];
		m_pScreens[m_iScreenStackPosition-1] = NULL;
		m_iScreenStackPosition--;

		return pScreen;
	}

	CRCNScreen* Peek()
	{
		RCNAssertStr(m_iScreenStackPosition > 0, "No screen to peek.");

		return m_pScreens[m_iScreenStackPosition-1];
	}

	CRCNScreen*		m_pScreens[ms_iMaxScreens];
	int				m_iScreenStackPosition;

} TScreenStack;

typedef CRCNScreen* (*FNScreenInstantiationFunction)(const char* pScreenName);

class CRCNScreenController
{
public:

						CRCNScreenController();
						~CRCNScreenController();

	static void			Initialise(FNScreenInstantiationFunction fnScreenInstantiationFunction);
	static void			GoToScreen(const char* pScreenName);
	static void			ReplaceScreen(const char* cScreenName);

	static void			Process();
	static void			ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState);
	static void			Render2D(float fInterpolation);
	static void			Render3D(float fInterpolation);
	static void			Shutdown();

	static CRCNScreen*	GetCurrentScreen();
private:

	static FNScreenInstantiationFunction		m_pfnScreenInstantiationFunction;
	static TScreenStack							m_tScreenStack;
};

#endif	// _RCN_RCNSCREENCONTROLLER_H_