// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "GameScreen.h"
#include "Core/TextureManager.h"
#include "Game/Game.h"

// -----------------------------------------
// CLASS DEFINITIONS

CGameScreen::CGameScreen(const char* cScreenName) : CRCNScreen(cScreenName)
{
	m_pBackground = g_pApplication->GetTextureManager()->GetTexture(ETexture_GameScreenBackground);

	m_pQuitButton = RCNNew CRCNButton();
	m_pQuitButton->SetDefaultTexture(g_pApplication->GetTextureManager()->GetTexture(ETexture_MenuScreenQuitButtonOnClick));
	m_pQuitButton->SetOnClickTexture(g_pApplication->GetTextureManager()->GetTexture(ETexture_MenuScreenQuitButtonOnClick));
	m_pQuitButton->SetPosition(CRCNVector(30.0f, 30.0f, 0.0f));
	m_pQuitButton->SetOnReleaseFunction(QuitButtonReleaseFunc);

	m_sString1.SetCharacters("This is a demonstration of the RECON engine");
	m_sString1.SetRenderCoordinates(500, 600);

	m_sString2.SetCharacters("[1][2][3] keys control lighting. [W][A][S][D] keys control movements");
	m_sString2.SetRenderCoordinates(400, 630);

	m_pMusic = CRCNAudioController::Create("RCNMusic_InGame");
	m_pMusic->SetVolume(CRCNAudioSource::ms_fMinimumVolume);
	m_pMusic->SetFade(CRCNAudioSource::ms_fMaximumVolume, 2000);
	m_pMusic->Play();

	SetScreenState(EScreenState_Initialised);
}

CGameScreen::~CGameScreen()
{
	RCNDelete m_pQuitButton;

	m_pMusic->Release(CRCNAudioSource::ERelease::EImmediate);
}

void CGameScreen::Process()
{
	m_pQuitButton->Process();

	CRCNScreen::Process();
}

void CGameScreen::ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState)
{
	m_pQuitButton->ProcessInput(eInput, eInputState);
}

void CGameScreen::Render2D(float fInterpolation)
{
	m_pBackground->Render();
	m_pQuitButton->Render(fInterpolation);

	m_sString1.Render();
	m_sString2.Render();
}

void CGameScreen::QuitButtonReleaseFunc()
{
	CRCNAudioSource* pSFX = CRCNAudioController::Create("RCNSFX_OnClick");
	pSFX->Play();
	pSFX->Release(CRCNAudioSource::ERelease::EOnceFinished);

	g_pApplication->GetGame()->UnloadLevel();
	CRCNScreenController::GetCurrentScreen()->SetPendingScreen(g_pApplication->m_sMenuScreenName);
	CRCNScreenController::GetCurrentScreen()->SetScreenState(EScreenState_Finalising);
}
