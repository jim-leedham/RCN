/*---------------------------------------------------------------------------*
Project:		RCN
File:			App.cpp
Description:    Handles the main program flow

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"

#include "App.h"

#include "Core/AssetLoader.h"
#include "Core/TextureManager.h"
#include "Core/ModelManager.h"

#include "Game/Game.h"

#include "Screens/BootstrapScreen.h"
#include "Screens/MenuScreen.h"
#include "Screens/LoadingScreen.h"
#include "Screens/GameScreen.h"

// -----------------------------------------
// STATIC DEFINITIONS

const char* CApp::m_sBootstrapScreenName	= "BootstapScreen";
const char* CApp::m_sMenuScreenName			= "MenuScreen";
const char* CApp::m_sLoadingScreenName		= "LoadingScreen";
const char* CApp::m_sGameScreenName			= "GameScreen";

// -----------------------------------------
// CLASS DEFINITIONS

CApp::CApp(void)
{
	m_eAppState = EAppState_Undefined;
}

CApp::~CApp(void)
{
}

void CApp::Start(void)
{
	m_eAppState = EAppState_InitialisationComplete;

	CRCNTimer::Initialise();
	CRCNRenderer::Initialise();
	CRCNLighting::Initialise();
	CRCNAudioController::Initialise("Data/Audio/Audio.XML");
	CRCNFileSystem::Initialise();

	m_pTextureManager	= RCNNew CTextureManager();
	m_pModelManager		= RCNNew CModelManager();
	m_pAssetLoader		= RCNNew CAssetLoader();

	// Load any assets that are needed for the bootstrap screen
	m_pTextureManager->LoadTexture(ETexture_BootstrapScreenBackground);
	SetAppState(EAppState_BootstrapAssetLoadingComplete);

	CRCNScreenController::Initialise(ScreenInstantiationCallback);

	m_pGame = RCNNew CGame();

	m_bRunning = true;
}

void CApp::MainLoop(void)
{
	// This game loop uses a constant Process() speed of 30 times per second which is independent of the Render() speed.
	// The Render() speed is variable; it will decrease if necessary to maintain a constant Process() speed, but it can go 
	// as high as the player's machine allows. The Render() function takes an interpolation value to improve visual quality 
	// when the Render() speed exceeds the Process() speed. More details: http://www.koonsolo.com/news/dewitters-gameloop/

	const int iTicksPerSecond	= 30;
	const int iSkipTicks		= 1000 / iTicksPerSecond;
	const int iMaxFrameSkip		= 10;

	m_dNextGameTick = CRCNTimer::GetTicks();
	m_dPreviousTick = m_dNextGameTick;

	double currentTick = 0.0;
	int loops;
	float fInterpolation;

	while(m_bRunning)
	{
		if(!PlatformMainLoop())
			return;

		loops = 0;
		currentTick = CRCNTimer::GetTicks();
		while (currentTick > m_dNextGameTick && loops < iMaxFrameSkip)
		{
			m_dTimeStep = currentTick - m_dPreviousTick;
			Process();

			m_dPreviousTick = currentTick;
			m_dNextGameTick += iSkipTicks;
			loops++;
		}

		fInterpolation = (float)((double)(currentTick + iSkipTicks - m_dNextGameTick) / (double)iSkipTicks);
		Render(fInterpolation);
	}
}

void CApp::Process()
{
	m_pGame->Process();
}

void CApp::Render(float fInterpolation)
{
	m_pGame->Render(fInterpolation);
}

void CApp::ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState)
{
	m_pGame->ProcessInput(eInput, eInputState);
}

void CApp::OnExitSizeMove()
{
	// Specific to platforms where this game can be run in windowed mode. This function is called
	// whenever the user has released the window having previously clicked/dragged to move or resize it.
	// It's important to reset the game loop timer now to avoid a sudden spike in m_dTimeStep.

	m_dNextGameTick = CRCNTimer::GetTicks();
	m_dPreviousTick = m_dNextGameTick;
}

void CApp::End(void)
{
	RCNDelete m_pGame;

	CRCNScreenController::Shutdown();
	CRCNRenderer::Shutdown();
	CRCNLighting::Shutdown();
	CRCNAudioController::Shutdown();
	CRCNFileSystem::Shutdown();

	RCNDelete m_pModelManager;
	RCNDelete m_pTextureManager;
	RCNDelete m_pAssetLoader;
}

void CApp::Quit()
{
	if(!m_pAssetLoader->IsJobQueueEmpty())
	{
		CRCNThread::RCNSleep(100);
	}
	m_bRunning = false;
}

CRCNScreen* CApp::ScreenInstantiationCallback(const char* sScreenName)
{
	CRCNScreen* pScreen = NULL;

	if(strcmp(sScreenName, "") == 0)
	{
		pScreen = RCNNew CBootstrapScreen(m_sBootstrapScreenName);
	}
	else if(strcmp(sScreenName, m_sMenuScreenName) == 0)
	{
		pScreen = RCNNew CMenuScreen(sScreenName);
	}
	else if(strcmp(sScreenName, m_sLoadingScreenName) == 0)
	{
		pScreen = RCNNew CLoadingScreen(sScreenName);
	}
	else if(strcmp(sScreenName, m_sGameScreenName) == 0)
	{
		pScreen = RCNNew CGameScreen(sScreenName);
	}

	if(pScreen)
	{
		return pScreen;
	}
	else
	{
		RCNAssertStr(false, "Cannot instantiate screen. Screen name not recognised.");
		return NULL;
	}
}

void CApp::MainMenuLoadingFunc(CRCNJob* pJob, void* pParameters)
{
	g_pApplication->GetTextureManager()->LoadTextures(ETexture_MenuScreenBackground, ETexture_LoadingScreenBackground);
	g_pApplication->SetAppState(EAppState_MainMenuAssetLoadingComplete);
}

void CApp::GameLoadingFunc(CRCNJob* pJob, void* pParameters)
{
	g_pApplication->GetTextureManager()->LoadTexture(ETexture_GameScreenBackground);	
	g_pApplication->GetModelManager()->LoadModel(EModelID_Plane);
	g_pApplication->GetGame()->LoadLevel();
	g_pApplication->SetAppState(EAppState_GameLoadingComplete);
}