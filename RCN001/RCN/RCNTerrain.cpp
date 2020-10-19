/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNTerrain.cpp
Description:    Provides a system for auto-generating terrain. Uses height-maps
				built using RCNNoise. Can create a grid of TerrainBlocks and
				wrap them around the maximum draw distance such that the player 
				cannot reach the edge. Can re-sample any TerrainBlock to a given
				level of detail such that TerrainBlocks closer to the player are
				more detailed. Based on:
				http://www.sea-of-memes.com/LetsCode28/LetsCode28.html

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNTerrain.h"
#include "RCNTerrainBlock.h"
#include "RCNTexture.h"
#include "RCNNoise.h"

// -----------------------------------------
// STATIC DEFINITIONS

const double		CRCNTerrain::ms_dWaterHeight			= 150.0;
const int			CRCNTerrain::ms_iMaximumBlockExtent		= 65536;
const int			CRCNTerrain::ms_iMinimumBlockExtent		= 64;
const int			CRCNTerrain::ms_iBlockExtentDivisor		= 64;

int					CRCNTerrain::ms_iMaximumBlockCount;
CRCNTerrainBlock*	CRCNTerrain::ms_pBlocks[3][3];
CRCNTerrainBlock*	CRCNTerrain::ms_pInvalidBlockList[100];		 // TODO: Work out theoretical maximum this number actually might be (100 is way too big!)
int					CRCNTerrain::ms_iInvalidBlockCount;

CRCNTexture*		CRCNTerrain::ms_pGroundTexture;
bool				CRCNTerrain::ms_ReadyToRender;

// -----------------------------------------
// CLASS DEFINITIONS

void CRCNTerrain::Initialise()
{
	CRCNNoise::Initialise();
	ms_ReadyToRender = false;
}

void CRCNTerrain::Shutdown()
{
	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			RCNDelete ms_pBlocks[x][z];
			ms_pBlocks[x][z] = NULL;
		}
	}
}

float CRCNTerrain::GetHeightAtPosition(const CRCNVector& vPosition)
{
	return (float)CRCNTerrain::GetHeightAtPosition((double)vPosition.fX, (double)vPosition.fZ);
}

double CRCNTerrain::GetHeightAtPosition(double dX, double dZ)
{
	double dHeight	= GetYAtXZ(dX / 40000.0, dZ / 40000.0);
	dHeight			= 70.0f * exp(5.0f * fabs(dHeight));
	dHeight		   += 20.0f * GetYAtXZ(dX / 1333.0f, dZ / 1333.0f);

	return dHeight;
}

float CRCNTerrain::GetYAtXZ(double dX, double dZ)
{
	int iOctaves = 10;
	float fScale = 1.0f;
	float fAmplitude = 1.0f;

	float fValue = 0.0;
	for (int i = 0; i < iOctaves; i++)
	{
		float fNoise = (float)CRCNNoise::GetNoise(dX * fScale, dZ * fScale);
		fValue		+= fNoise * fAmplitude;
		fScale		*= 2.0;
		fAmplitude	*= 0.5;
	}
	return fValue;
}

void CRCNTerrain::GenerateBlocks()
{			
	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			CRCNTerrainBlock* pBlock = RCNNew CRCNTerrainBlock(
				((x - 1) * ms_iMaximumBlockExtent) - (ms_iMaximumBlockExtent / 2),	// Block origin X
				((z - 1) * ms_iMaximumBlockExtent) - (ms_iMaximumBlockExtent / 2),	// Block origin Z
				ms_iMaximumBlockExtent,												// Block extent X
				ms_iMaximumBlockExtent);											// Block extent Z

			pBlock->CreateHeightmap();
			ms_pBlocks[x][z] = pBlock;
		}
	}
}

double CRCNTerrain::GetWaterHeight()
{
	return ms_dWaterHeight;
}

void CRCNTerrain::ValidateLevelOfDetail(const CRCNVector& vPosition)
{
	int iBlockCount = 0;
	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			ms_pBlocks[x][z]->ValidateLevelOfDetail(vPosition.fX, vPosition.fZ);
			iBlockCount += ms_pBlocks[x][z]->GetNumberOfChildren();
		}
	}
	ms_iMaximumBlockCount = max(ms_iMaximumBlockCount, iBlockCount);
}

void CRCNTerrain::CreateRenderBuffers()
{
	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			ms_pBlocks[x][z]->CreateRenderBuffers();
		}
	}
	ms_ReadyToRender = true;
}

void CRCNTerrain::ValidateGrid(const CRCNVector& vPosition)
{
	// if we're in rightmost cell, extend to right
	if (vPosition.fX > ms_pBlocks[2][0]->GetOriginX())
	{
		for (int z = 0; z <= 2; z++)
		{
			// RCNDelete first cell, shift others left
			RCNDelete ms_pBlocks[0][z];
			ms_pBlocks[0][z] = ms_pBlocks[1][z];
			ms_pBlocks[1][z] = ms_pBlocks[2][z];

			// add RCNNew terrain cell at right
			int originX = ms_pBlocks[1][z]->GetOriginX() + ms_iMaximumBlockExtent;
			int originZ = ms_pBlocks[1][z]->GetOriginZ();
			CRCNTerrainBlock* block = RCNNew CRCNTerrainBlock(originX, originZ, ms_iMaximumBlockExtent, ms_iMaximumBlockExtent);

			ms_pBlocks[2][z] = block;
		}
	}

	// if we're in leftmost cell, extend to left
	else if (vPosition.fX < ms_pBlocks[1][0]->GetOriginX())
	{
		for (int z = 0; z <= 2; z++)
		{
			// RCNDelete last cell, shift others right
			RCNDelete ms_pBlocks[2][z];
			ms_pBlocks[2][z] = ms_pBlocks[1][z];
			ms_pBlocks[1][z] = ms_pBlocks[0][z];

			// add RCNNew terrain cell at right
			int originX = ms_pBlocks[1][z]->GetOriginX() - ms_iMaximumBlockExtent;
			int originZ = ms_pBlocks[1][z]->GetOriginZ();
			CRCNTerrainBlock* block = RCNNew CRCNTerrainBlock(originX, originZ, ms_iMaximumBlockExtent, ms_iMaximumBlockExtent);

			ms_pBlocks[0][z] = block;
		}
	}

	// if we're in bottommost cell, extend to bottom
	if (vPosition.fZ > ms_pBlocks[0][2]->GetOriginZ())
	{
		for (int x = 0; x <= 2; x++)
		{
			// RCNDelete top cell, shift others up
			RCNDelete ms_pBlocks[x][0];
			ms_pBlocks[x][0] = ms_pBlocks[x][1];
			ms_pBlocks[x][1] = ms_pBlocks[x][2];

			// add RCNNew terrain cell at bottom
			int originX = ms_pBlocks[x][1]->GetOriginX();
			int originZ = ms_pBlocks[x][1]->GetOriginZ() + ms_iMaximumBlockExtent;
			CRCNTerrainBlock* block = RCNNew CRCNTerrainBlock(originX, originZ, ms_iMaximumBlockExtent, ms_iMaximumBlockExtent);

			ms_pBlocks[x][2] = block;
		}
	}

	// if we're in topmost cell, extend to top
	else if (vPosition.fZ < ms_pBlocks[0][1]->GetOriginZ())
	{
		for (int x = 0; x <= 2; x++)
		{
			// RCNDelete bottom cell, shift others down
			RCNDelete ms_pBlocks[x][2];
			ms_pBlocks[x][2] = ms_pBlocks[x][1];
			ms_pBlocks[x][1] = ms_pBlocks[x][0];

			// add RCNNew terrain cell at bottom
			int originX = ms_pBlocks[x][1]->GetOriginX();
			int originZ = ms_pBlocks[x][1]->GetOriginZ() - ms_iMaximumBlockExtent;
			CRCNTerrainBlock* block = RCNNew CRCNTerrainBlock(originX, originZ, ms_iMaximumBlockExtent, ms_iMaximumBlockExtent);

			ms_pBlocks[x][0] = block;
		}
	}
}

void CRCNTerrain::ValidateBlockEdges()
{
	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			ms_pBlocks[x][z]->ValidateEdges();
		}
	}
}

void CRCNTerrain::ValidateHeightmaps()
{
	for(int i = 0; i < 100; i++)
		ms_pInvalidBlockList[i] = NULL;

	ms_iInvalidBlockCount = 0;

	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			ms_pBlocks[x][z]->ValidateHeightmaps();
		}
	}

	// Bubble sort the blocks of smallest extent to the front
	for(int i = 0; i < ms_iInvalidBlockCount; i++)
	{
		if(ms_pInvalidBlockList[i] != NULL)
		{
			int j = i;
			while(j - 1 > -1 && ms_pInvalidBlockList[j - 1]->m_iExtentX > ms_pInvalidBlockList[j]->m_iExtentX)
			{
				CRCNTerrainBlock* pTemp		= ms_pInvalidBlockList[j - 1];
				ms_pInvalidBlockList[j - 1] = ms_pInvalidBlockList[j];
				ms_pInvalidBlockList[j]		= pTemp;
				j--;
			}
		}
	}

	for(int i = 0; i < ms_iInvalidBlockCount; i++)
	{
		ms_pInvalidBlockList[i]->CreateHeightmap();
		ms_pInvalidBlockList[i]->m_bPendingRebuildRenderBuffers = true;
	}

	CreateRenderBuffers();
}

void CRCNTerrain::Render(const CRCNVector& vPosition)
{
	if(ms_ReadyToRender == false)
		return;

	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			ms_pBlocks[x][z]->Render(vPosition);
		}
	}
}

void CRCNTerrain::SetGroundTexture(CRCNTexture* pGroundTexture)
{
	ms_pGroundTexture = pGroundTexture;
}
