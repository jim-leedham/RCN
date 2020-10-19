
// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "LoadingScreen.h"
#include "Core/TextureManager.h"
#include "Core/AssetLoader.h"

// -----------------------------------------
// CLASS DEFINITIONS

CLoadingScreen::CLoadingScreen(const char* pScreenName) : CRCNScreen(pScreenName)
{
	m_dDisplayTime = 0.0;

	Background = g_pApplication->GetTextureManager()->GetTexture(ETexture_LoadingScreenBackground);
	Background->SetTransparency(0.0f);

	g_pApplication->GetAssetLoader()->BeginLoad(g_pApplication->GameLoadingFunc, NULL);

	SetScreenState(EScreenState_FadingIn);
}

CLoadingScreen::~CLoadingScreen()
{
	Background = NULL;
}

void CLoadingScreen::Process()
{
	m_dDisplayTime += g_pApplication->GetTimeStep();

	if(m_eScreenState == EScreenState_FadingIn)
	{
		if(m_dDisplayTime < m_iFadeInTime)
		{
			Background->SetTransparency((float)m_dDisplayTime / m_iFadeInTime);
			return;
		}
		else
		{
			Background->SetTransparency(1.0f);
			SetScreenState(EScreenState_Initialised);
		}
	}

	if(m_eScreenState == EScreenState_Initialised && m_dDisplayTime > m_iHoldTime && g_pApplication->GetAppState() >= CApp::EAppState_GameLoadingComplete)
	{
		SetScreenState(EScreenState_FadingOut);
	}

	if(m_eScreenState == EScreenState_FadingOut)
	{
		if(m_dDisplayTime < m_iFadeOutTime)
		{
			Background->SetTransparency(1.0f - ((float)m_dDisplayTime / m_iFadeOutTime));
			return;
		}
		else
		{
			SetPendingScreen(g_pApplication->m_sGameScreenName);
			SetScreenState(EScreenState_Finalising);
		}
	}

	CRCNScreen::Process();
}

void CLoadingScreen::Render2D(float fInterpolation)
{
	Background->Render();
}

void CLoadingScreen::SetScreenState(EScreenState eScreenState)
{
	m_dDisplayTime = 0.0;
	CRCNScreen::SetScreenState(eScreenState);
}
