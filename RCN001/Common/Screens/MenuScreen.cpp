// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "MenuScreen.h"
#include "Core/TextureManager.h"
#include "Core/AssetLoader.h"

// -----------------------------------------
// CLASS DEFINITIONS

CMenuScreen::CMenuScreen(const char* cScreenName) : CRCNScreen(cScreenName)
{
	m_dDisplayTime = 0.0;

	m_pBackground = g_pApplication->GetTextureManager()->GetTexture(ETexture_MenuScreenBackground);
	m_pBackground->SetTransparency(0.0f);

	m_pPlayButton = RCNNew CRCNButton();
	m_pPlayButton->SetDefaultTexture(g_pApplication->GetTextureManager()->GetTexture(ETexture_MenuScreenPlayButton));
	m_pPlayButton->SetOnClickTexture(g_pApplication->GetTextureManager()->GetTexture(ETexture_MenuScreenPlayButtonOnClick));
	m_pPlayButton->SetPosition(CRCNVector(500.0f, 420.0f, 0.0f));
	m_pPlayButton->SetTransparency(0.0f);
	m_pPlayButton->SetOnReleaseFunction(PlayButtonReleaseFunc);

	m_pQuitButton = RCNNew CRCNButton();
	m_pQuitButton->SetDefaultTexture(g_pApplication->GetTextureManager()->GetTexture(ETexture_MenuScreenQuitButton));
	m_pQuitButton->SetOnClickTexture(g_pApplication->GetTextureManager()->GetTexture(ETexture_MenuScreenQuitButtonOnClick));
	m_pQuitButton->SetPosition(CRCNVector(700.0f, 420.0f, 0.0f));
	m_pQuitButton->SetTransparency(0.0f);
	m_pQuitButton->SetOnReleaseFunction(QuitButtonReleaseFunc);

	m_pMusic = CRCNAudioController::Create("RCNMusic_Menu");
	m_pMusic->SetVolume(CRCNAudioSource::ms_fMinimumVolume);
	m_pMusic->SetFade(CRCNAudioSource::ms_fMaximumVolume, 2000);
	m_pMusic->Play();

	SetScreenState(EScreenState_FadingIn);
}

CMenuScreen::~CMenuScreen()
{
	// NOTE: Ideally we would now tell the TextureManager to delete any Menu assets.
	// However, unless memory consumption becomes a problem it seems unnecessary.

	// It's helpful to reset these properties on the textures we used, in case another part of the game uses them later
	m_pPlayButton->SetTransparency(1.0f);
	m_pQuitButton->SetTransparency(1.0f);

	m_pBackground = NULL;
	RCNDelete m_pPlayButton;
	RCNDelete m_pQuitButton;

	m_pMusic->SetFade(CRCNAudioSource::ms_fMinimumVolume, 2000);
	m_pMusic->Release(CRCNAudioSource::ERelease::EOnceFaded);
}

void CMenuScreen::Process()
{
	m_dDisplayTime += g_pApplication->GetTimeStep();

	if(m_eScreenState == EScreenState_FadingIn)
	{
		if(m_dDisplayTime < m_iFadeInTime)
		{
			m_pBackground->SetTransparency((float)m_dDisplayTime / m_iFadeInTime);
			m_pPlayButton->SetTransparency((float)m_dDisplayTime / m_iFadeInTime);
			m_pQuitButton->SetTransparency((float)m_dDisplayTime / m_iFadeInTime);
			return;
		}
		else
		{
			m_pBackground->SetTransparency(1.0f);
			m_pPlayButton->SetTransparency(1.0f);
			m_pQuitButton->SetTransparency(1.0f);
			SetScreenState(EScreenState_Initialised);
		}
	}

	if(m_eScreenState == EScreenState_Initialised)
	{
		m_pPlayButton->Process();
		m_pQuitButton->Process();
	}

	if(m_eScreenState == EScreenState_FadingOut)
	{
		if(m_dDisplayTime < m_iFadeOutTime)
		{
			m_pBackground->SetTransparency(1.0f - ((float)m_dDisplayTime / m_iFadeOutTime));
			m_pPlayButton->SetTransparency(1.0f - ((float)m_dDisplayTime / m_iFadeOutTime));
			m_pQuitButton->SetTransparency(1.0f - ((float)m_dDisplayTime / m_iFadeOutTime));
			return;
		}
		else
		{
			CRCNScreenController::GetCurrentScreen()->SetPendingScreen(g_pApplication->m_sLoadingScreenName);
			CRCNScreenController::GetCurrentScreen()->SetScreenState(EScreenState_Finalising);
		}
	}

	CRCNScreen::Process();
}

void CMenuScreen::ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState)
{
	m_pPlayButton->ProcessInput(eInput, eInputState);
	m_pQuitButton->ProcessInput(eInput, eInputState);
}

void CMenuScreen::Render2D(float fInterpolation)
{
	m_pBackground->Render();
	m_pPlayButton->Render(fInterpolation);
	m_pQuitButton->Render(fInterpolation);
}

void CMenuScreen::SetScreenState(EScreenState eScreenState)
{
	m_dDisplayTime = 0.0;
	CRCNScreen::SetScreenState(eScreenState);
}

void CMenuScreen::PlayButtonReleaseFunc()
{
	CRCNAudioSource* pSFX = CRCNAudioController::Create("RCNSFX_OnClick");
	pSFX->Play();
	pSFX->Release(CRCNAudioSource::ERelease::EOnceFinished);

	CRCNScreenController::GetCurrentScreen()->SetScreenState(EScreenState_FadingOut);
}

void CMenuScreen::QuitButtonReleaseFunc()
{
	g_pApplication->Quit();
}