/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNInput.cpp
Description:    Handles all keyboard and mouse input and mediates passing
				of such data to the game

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNInput.h"

// -----------------------------------------
// STATIC DEFINITIONS

long					CRCNInput::m_iMouseX;
long					CRCNInput::m_iMouseY;
CRCNInput::TInputData	CRCNInput::m_eInputStates[CRCNInput::EInput_Count];

// -----------------------------------------
// CLASS DEFINITIONS

void CRCNInput::SetInputs(TRCNRawInput* pRawInput)
{
#if defined WIN32
	if(pRawInput->header.dwType== RIM_TYPEMOUSE)
	{
		// NOTE: Even though RawInput can give us mouse X and Y, we're using
		// GetCursorPos instead because it provides screen-space coordinates,
		// whereas RawInput provides relative differences regardless of mouse
		// sensitivity or starting position, so would require more calculation
		// to obtain usable values.

		POINT tMousePos;
		GetCursorPos(&tMousePos);
		ScreenToClient(((AppPC*)g_pApplication)->GetHWND(), &tMousePos);
		m_iMouseX	= tMousePos.x;
		m_iMouseY	= tMousePos.y;

		bool bButtonDown;
		bool bButtonUp;

		bButtonDown = (pRawInput->data.mouse.ulButtons & RI_MOUSE_LEFT_BUTTON_DOWN) != 0;
		bButtonUp	= (pRawInput->data.mouse.ulButtons & RI_MOUSE_LEFT_BUTTON_UP) != 0;
		if(bButtonDown || bButtonUp)
		{
			m_eInputStates[EInput_LeftMouseButton].bChanged = m_eInputStates[EInput_LeftMouseButton].bDown != bButtonDown;
			m_eInputStates[EInput_LeftMouseButton].bDown = bButtonDown;
		}

		bButtonDown = (pRawInput->data.mouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_DOWN) != 0;
		bButtonUp	= (pRawInput->data.mouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_UP) != 0;
		if(bButtonDown || bButtonUp)
		{
			m_eInputStates[EInput_RightMouseButton].bChanged = m_eInputStates[EInput_RightMouseButton].bDown != bButtonDown;
			m_eInputStates[EInput_RightMouseButton].bDown = bButtonDown;
		}

		bButtonDown = (pRawInput->data.mouse.ulButtons & RI_MOUSE_MIDDLE_BUTTON_DOWN) != 0;
		bButtonUp	= (pRawInput->data.mouse.ulButtons & RI_MOUSE_MIDDLE_BUTTON_DOWN) != 0;
		if(bButtonDown || bButtonUp)
		{
			m_eInputStates[EInput_MiddleMouseButton].bChanged = m_eInputStates[EInput_MiddleMouseButton].bDown != bButtonDown;
			m_eInputStates[EInput_MiddleMouseButton].bDown = bButtonDown;
		}
	}
	if(pRawInput->header.dwType== RIM_TYPEKEYBOARD)
	{
		USHORT iKeyCode	= pRawInput->data.keyboard.VKey;
		bool bKeyUp	= pRawInput->data.keyboard.Flags & RI_KEY_BREAK;
		EInput eInput	= EInput_NotSet;

		switch(iKeyCode)
		{
		case VK_UP:		eInput = EInput_Up;		break;
		case VK_DOWN:	eInput = EInput_Down;	break;
		case VK_LEFT:	eInput = EInput_Left;	break;
		case VK_RIGHT:	eInput = EInput_Right;	break;
		case '1':		eInput = EInput_1;		break;
		case '2':		eInput = EInput_2;		break;
		case '3':		eInput = EInput_3;		break;
		case '4':		eInput = EInput_4;		break;
		case 'W':		eInput = EInput_W;		break;
		case 'A':		eInput = EInput_A;		break;
		case 'S':		eInput = EInput_S;		break;
		case 'D':		eInput = EInput_D;		break;
		}

		if(eInput != EInput_NotSet && m_eInputStates[eInput].bDown == bKeyUp)
		{
			m_eInputStates[eInput].bChanged		= true;
			m_eInputStates[eInput].bDown		= !bKeyUp;
		}
	}
#endif
}

void CRCNInput::ProcessInputs()
{
	for(int iInput = 0; iInput < EInput_Count; iInput++)
	{
		if(m_eInputStates[iInput].bChanged)
		{
			g_pApplication->ProcessInput((EInput)iInput, m_eInputStates[iInput].bDown ? EInputState_Pressed : EInputState_Released);
			m_eInputStates[iInput].bChanged = false;
		}
		else if(m_eInputStates[iInput].bDown)
		{
			g_pApplication->ProcessInput((EInput)iInput, EInputState_Held);
		}
	}
}

int CRCNInput::GetMouseX()
{
	return m_iMouseX;
}

int CRCNInput::GetMouseY()
{
	return m_iMouseY;
}

CRCNInput::TInputData::TInputData()
{
	bDown		= false;
	bChanged	= false;
}
