
// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "BootstrapScreen.h"
#include "Core/TextureManager.h"
#include "Core/AssetLoader.h"

// -----------------------------------------
// CLASS DEFINITIONS

CBootstrapScreen::CBootstrapScreen(const char* pScreenName) : CRCNScreen(pScreenName)
{
	m_dDisplayTime = 0.0;

	m_pBackground = g_pApplication->GetTextureManager()->GetTexture(ETexture_BootstrapScreenBackground);
	m_pBackground->SetTransparency(0.0f);

	g_pApplication->GetAssetLoader()->BeginLoad(g_pApplication->MainMenuLoadingFunc, NULL);

	SetScreenState(EScreenState_FadingIn);
}

CBootstrapScreen::~CBootstrapScreen()
{
	// NOTE: Ideally we would now tell the TextureManager to delete any bootstrap assets.
	// However, unless memory consumption becomes a problem it seems unnecessary.
	m_pBackground = NULL;
}

void CBootstrapScreen::Process()
{
	m_dDisplayTime += g_pApplication->GetTimeStep();

	if(m_eScreenState == EScreenState_FadingIn)
	{
		if(m_dDisplayTime < m_iFadeInTime)
		{
			m_pBackground->SetTransparency((float)m_dDisplayTime / m_iFadeInTime);
			return;
		}
		else
		{
			m_pBackground->SetTransparency(1.0f);
			SetScreenState(EScreenState_Initialised);
		}
	}

	if(m_eScreenState == EScreenState_Initialised && m_dDisplayTime > m_iHoldTime && g_pApplication->GetAppState() >= CApp::EAppState_MainMenuAssetLoadingComplete)
	{
		SetScreenState(EScreenState_FadingOut);
	}

	if(m_eScreenState == EScreenState_FadingOut)
	{
		if(m_dDisplayTime < m_iFadeOutTime)
		{
			m_pBackground->SetTransparency(1.0f - ((float)m_dDisplayTime / m_iFadeOutTime));
			return;
		}
		else
		{
			SetPendingScreen(g_pApplication->m_sMenuScreenName);
			SetScreenState(EScreenState_Finalising);
		}
	}

	CRCNScreen::Process();
}

void CBootstrapScreen::Render2D(float fInterpolation)
{
	m_pBackground->Render();
}

void CBootstrapScreen::SetScreenState(EScreenState eScreenState)
{
	m_dDisplayTime = 0.0;
	CRCNScreen::SetScreenState(eScreenState);
}
