/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNTerrain.h
Description:    Provides a system for auto-generating terrain. Uses height-maps
				built using RCNNoise. Can create a grid of TerrainBlocks and
				wrap them around the maximum draw distance such that the player 
				cannot reach the edge. Can re-sample any TerrainBlock to a given
				level of detail such that TerrainBlocks closer to the player are
				more detailed. Based on:
				http://www.sea-of-memes.com/LetsCode28/LetsCode28.html

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNTERRAIN_H_
#define _RCN_RCNTERRAIN_H_

// -----------------------------------------
// INCLUDES

#include "RCN3D.h"

// -----------------------------------------
// FORWARD DECLARATIONS

class CRCNTerrainBlock;
class CRCNTexture;

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNTerrain
{
public:

	static void					Initialise();
	static void					Shutdown();

	static float				GetHeightAtPosition(const CRCNVector& vPosition);
	static double				GetHeightAtPosition(double dX, double dZ);

	static void					GenerateBlocks();
	static void					ValidateLevelOfDetail(const CRCNVector& vPosition);
	static void					CreateRenderBuffers();

	static void					ValidateGrid(const CRCNVector& vPosition);
	static void					ValidateBlockEdges();
	static void					ValidateHeightmaps();

	static void					Render(const CRCNVector& vPosition);

	static double				GetWaterHeight();

	static void					SetGroundTexture(CRCNTexture* pGroundTexture);

	static const double			ms_dWaterHeight;
	static const int			ms_iBlockExtentDivisor;
	static const int			ms_iMaximumBlockExtent;
	static const int			ms_iMinimumBlockExtent;

private:

	static float				GetYAtXZ(double fX, double fZ);

	static CRCNTerrainBlock*	ms_pBlocks[3][3];
	static CRCNTerrainBlock*	ms_pInvalidBlockList[100];
	static int					ms_iInvalidBlockCount;

	static int					ms_iMaximumBlockCount;

	friend class				CRCNTerrainBlock;

	static CRCNTexture*			ms_pGroundTexture;

	static bool					ms_ReadyToRender;
};

#endif	// _RCN_RCNTERRAIN_H_