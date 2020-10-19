/*---------------------------------------------------------------------------*
Project:		RCN
File:			AppPlatform.h
Description:    App for the PC platform

*---------------------------------------------------------------------------*/

#ifndef _PC_APPPLATFORM_H_
#define _PC_APPPLATFORM_H_

#include "Core\App.h"

// -----------------------------------------
// CLASS DECLARATIONS

class AppPC : public CApp
{
public:

	AppPC(unsigned int uParam);

	void				HandleInput(HWND hWndMsg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void		Start();
	virtual bool		PlatformMainLoop();
	virtual void		End();

	inline HINSTANCE	GetHINSTANCE() const	{ return m_hInstance;	}
	inline HWND			GetHWND() const			{ return m_hWnd;		}

	void				RegisterInputDevices();

	const int			GetDefaultScreenWidth()		{ return s_iDefaultScreenWidth;		}
	const int			GetDefaultScreenHeight()	{ return s_iDefaultScreenHeight;	}

private:

	HINSTANCE			m_hInstance;
	HWND				m_hWnd;

	static const int	s_iInputBufferSize = 40;	// NOTE: Should be sufficient for keyboard or mouse data
	BYTE				m_InputBuffer[s_iInputBufferSize];

	static const int	s_iDefaultScreenWidth = 1280;
	static const int	s_iDefaultScreenHeight = 720;
};

#endif	// _PC_APPPLATFORM_H_