/*---------------------------------------------------------------------------*
Project:		RCN
File:			AppPC.cpp
Description:    App for the PC platform

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "AppMain.h"

extern CApp* g_pApplication;

LRESULT CALLBACK WindowProc(HWND hWndMsg, 
						 UINT uMsg, 
						 WPARAM wParam, 
						 LPARAM lParam)
{
	AppPC* pApp  =  (AppPC*)g_pApplication;

	if(pApp)
		pApp->HandleInput(hWndMsg, uMsg, wParam, lParam);

	return DefWindowProc (hWndMsg, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, 
				   HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, 
				   int nShowCmd)
{
	//Put executable file path in to cModuleFilename, this will be argv[0]
	char cModuleFilename[MAX_PATH] = {0};
	GetModuleFileNameA(0, cModuleFilename, MAX_PATH);

	//Count args
	int iArgc = 1;
	char* pCmdLine = 0;
	if(lpCmdLine[0] != 0)
	{
		char* pCmdLine = lpCmdLine;
		while(*pCmdLine)
		{
			if(*pCmdLine ==' ')
			{
				++iArgc;
			}
			++pCmdLine;
		}
		++iArgc;
	}
	//Allocate buffer for argv pointers
	char **pArgv = (char**)malloc( iArgc * sizeof( char* ) );

	//Set argv[0]
	pArgv[0] = cModuleFilename;

	//Set argv[N]
	iArgc = 1;
	if(lpCmdLine[0] != 0)
	{
		pCmdLine = lpCmdLine;
		pArgv[1] = pCmdLine;
		while(*pCmdLine)
		{
			if(*pCmdLine == ' ')
			{
				*pCmdLine = '\0';	//Put NULL into original Win32 args string, this saves having to alloc each argv string
				++pCmdLine;
				++iArgc;
				pArgv[iArgc] = pCmdLine;
			}
			else
			{
				++pCmdLine;
			}
		}
		++iArgc;
	}

	// Call main passing argc and argv
	int iAppMainReturnVal = AppMain(iArgc, pArgv, (unsigned int)hInstance);
	
	//Free allocated memory
	free(pArgv);

	return iAppMainReturnVal;
}

// -----------------------------------------
// CLASS DEFINITIONS

AppPC::AppPC(unsigned int uParam)
{
	m_hInstance = (HINSTANCE)uParam;
	WNDCLASSEXA wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = m_hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "RCN";

	RegisterClassExA(&wc);

	RECT windowRect = {0, 0, s_iDefaultScreenWidth, s_iDefaultScreenHeight};    // set the size, but not the position
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);					// adjust the size

	// Create the render window
	m_hWnd = CreateWindowEx(NULL,
		"RCN",
		"RECON 001",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		50, 50,
		windowRect.right - windowRect.left, 
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		m_hInstance,
		NULL);

	ShowWindow(m_hWnd, SW_SHOW);

	RegisterInputDevices();
}

void AppPC::RegisterInputDevices()
{
	RAWINPUTDEVICE tInputDevices[2];

	// Keyboard
	tInputDevices[0].usUsagePage	= 1;
	tInputDevices[0].usUsage		= 6;
	tInputDevices[0].dwFlags		= 0;
	tInputDevices[0].hwndTarget		= m_hWnd;

	// Mouse
	tInputDevices[1].usUsagePage	= 1;
	tInputDevices[1].usUsage		= 2;
	tInputDevices[1].dwFlags		= 0;
	tInputDevices[1].hwndTarget		= m_hWnd;

	RegisterRawInputDevices(tInputDevices, 2, sizeof(RAWINPUTDEVICE));    // register the device
}

void AppPC::HandleInput(HWND hWndMsg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case VK_ESCAPE:
		g_pApplication->Quit();
		break;
	}

	switch(uMsg)
	{
	case WM_INPUT:
		{
			UINT iBufferSize;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &iBufferSize, sizeof(RAWINPUTHEADER));
			RCNAssertStr(iBufferSize <= s_iInputBufferSize, "Byte buffer required by Raw Input exceeds m_InputBuffer size");
			
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, (LPVOID)m_InputBuffer, &iBufferSize, sizeof (RAWINPUTHEADER));

			CRCNInput::SetInputs((RAWINPUT*)m_InputBuffer);
		}
		break;
	case WM_EXITSIZEMOVE:
		CApp::OnExitSizeMove();
		break;
	case WM_CLOSE:
	case WM_QUIT:
	case WM_DESTROY:
		g_pApplication->Quit();
		break;
	}
}

void AppPC::Start()
{
	CApp::Start();
}

bool AppPC::PlatformMainLoop()
{
	MSG msg;

	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

void AppPC::End()
{
	CApp::End();
}