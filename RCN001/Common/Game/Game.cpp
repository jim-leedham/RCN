 /*---------------------------------------------------------------------------*
Project:		RCN
File:			Game.cpp
Description:    Handles the main game flow

TODO:			- SHORT TERM
				+ The Texture and Model Managers need better ways to free assets. Need to ensure that re-entering the game will re-load the assets correctly.
				+ Audio: Integrate the RCNAudio project
				+ Add exception throwing/handling
				
				- LONG TERM
				+ Animation loader/manager
				+ Language manager
				+ Various documentation improvements
				+ Custom heap. Ask Windows to alloc a large chunk at game start and manually handle all allocations within that chunk. 
				  This will avoid multiple kernel accesses during the game and let us track memory fragmentation better.

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "Game.h"
#include "Level.h"
#include "Demo.h"
#include "Core/Player.h"

// -----------------------------------------
// CLASS DEFINITIONS

CGame::CGame(void)
{
	m_pCamera = RCNNew CRCNCamera();

	// Set view matrix parameters
	m_pCamera->SetPosition(CRCNVector(0.0f, 8.0f, 25.0f));
	m_pCamera->SetLookAt(CRCNVector(0.0f, 0.0f, 0.0f));
	m_pCamera->SetUp(CRCNVector(0.0f, 1.0f, 0.0f));

	// Set projection matrix parameters
	m_pCamera->SetFieldOfView(45.0f);
	m_pCamera->SetNearPlane(1.0f);
	m_pCamera->SetFarPlane(100000.0f);

	m_pLevel = NULL;

	m_pPlayer = RCNNew CPlayer();
	m_pPlayer->SetPosition(CRCNVector(0.0f, 0.0f, 0.0f));
}

CGame::~CGame(void)
{
	RCNDelete m_pPlayer;
	RCNDelete m_pCamera;
	UnloadLevel();
}

void CGame::Process()
{
	CRCNScreenController::Process();

	if(m_pLevel)
		m_pLevel->Process();

	m_pPlayer->Process();

	CRCNInput::ProcessInputs();

	if(m_pLevel != NULL && m_pPlayer->HasPendingMovement())
	{
		if(m_pLevel->ValidatePlayerMovement(m_pPlayer->GetPendingPosition(), m_pPlayer->GetPendingMovement()) == false)
		{
			// NOTE: Usually this means the level has detected a collision
			m_pPlayer->InvalidatePendingMovement();
		}
	}

	CRCNAudioController::Process();
}

void CGame::ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState)
{
	CRCNScreenController::ProcessInput(eInput, eInputState);

	m_pPlayer->ProcessInput(eInput, eInputState);

	if(m_pLevel != NULL)
		m_pLevel->ProcessInput(eInput, eInputState);
}


void CGame::Render(float fInterpolation)
{
	m_pCamera->SetPosition(m_pPlayer->GetPositionThisFrame(fInterpolation));
	m_pCamera->SetLookAt(m_pCamera->GetPosition() + m_pPlayer->GetDirectionThisFrame(fInterpolation));

	CRCNRenderer::SetCamera(m_pCamera);

	CRCNRenderer::BeginRendering();

	CRCNRenderer::Clear(true, true, 0xff000000);

	if(m_pLevel != NULL)
		m_pLevel->Render(fInterpolation);

	CRCNScreenController::Render3D(fInterpolation);

	CRCNRenderer::Begin2DRendering();

	CRCNScreenController::Render2D(fInterpolation);

	CRCNRenderer::End2DRendering();

	CRCNRenderer::EndRendering();
}

void CGame::LoadLevel()
{
	// NOTE: In a complete game, the level we load here would be determined
	// by a selection made on the Main Menu screen, or by querying the player's
	// save data. For now we only have one level:

	RCNAssertStr(m_pLevel == NULL, "Level appears to have already been loaded");
	m_pLevel = RCNNew CDemo();

}

void CGame::UnloadLevel()
{
	if(m_pLevel != NULL)
	{
		RCNDelete m_pLevel;
		m_pLevel = NULL;
	}
}
