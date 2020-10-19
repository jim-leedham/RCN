/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNInput.h
Description:    Handles all keyboard and mouse input and mediates passing
				of such data to the game

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNINPUT_H_
#define _RCN_RCNINPUT_H_

// -----------------------------------------
// INCLUDES

#include "RCNTypes.h"

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNInput
{
public:

	enum EInput
	{
		EInput_NotSet = -1,

		EInput_LeftMouseButton,
		EInput_RightMouseButton,
		EInput_MiddleMouseButton,

		EInput_Left,
		EInput_Right,
		EInput_Up,
		EInput_Down,
		EInput_1,
		EInput_2,
		EInput_3,
		EInput_4,
		EInput_W,
		EInput_A,
		EInput_S,
		EInput_D,

		EInput_Count,
	};

	enum EInputState
	{
		EInputState_Pressed,
		EInputState_Held,
		EInputState_Released,
	};

	struct TInputData
	{
				TInputData();

		bool	bDown;
		bool	bChanged;
	};

	static void SetInputs(TRCNRawInput* pRawInput);
	static void ProcessInputs();

	static int	GetMouseX();
	static int	GetMouseY();

private:

	static long				m_iMouseX;
	static long				m_iMouseY;
	static TInputData		m_eInputStates[EInput_Count];
};

#endif	// _RCN_RCNINPUT_H_