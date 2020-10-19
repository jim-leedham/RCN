/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNTerrainBlock.h
Description:    A single terrain block generated from a height-map for use with
				RCNTerrain. Uses Vertex/Index buffers for rendering.

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNTERRAINBLOCK_H_
#define _RCN_RCNTERRAINBLOCK_H_

// -----------------------------------------
// INCLUDES

#include "RCN3D.h"

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNTerrainBlock
{
public:

						CRCNTerrainBlock(int iOriginX, int iOriginZ, int iExtentX, int iExtentZ);
						~CRCNTerrainBlock();

	void				CreateHeightmap();
	void				ValidateLevelOfDetail(double dX, double dZ);
	void				CreateRenderBuffers();

	const CRCNTerrainBlock*	GetChild(int iIndex);
	int					GetNumberOfChildren();

	int					GetOriginX();
	int					GetOriginZ();

	void				ValidateEdges();
	void				ValidateHeightmaps();

	void				Render(const CRCNVector& vPosition);

private:

	bool				LODTooLow(double dX, double dZ);
	bool				LODTooHigh(double dX, double dZ);

	void				SubdivideBlock();
	void				AbsorbChildren();

	void				ExtractHeightmap(const float* pParentHeights, int iOriginX, int iOriginZ);

	void				DeleteRenderBuffers();

	void				CreateGroundRenderBuffers();
	void				CreateWaterRenderBuffers();

	int					m_iOriginX;
	int					m_iOriginZ;
	int					m_iExtentX;
	int					m_iExtentZ;

	// stepping of outside edges
	int					m_iXMinimumStep;
	int					m_iZMinimumStep;
	int					m_iXMaximumStep;
	int					m_iZMaximumStep;

	double				m_dMinimumHeight;
	double				m_dMaximumHeight;

	float*				m_fHeights;

	CRCNTerrainBlock*	m_pChildren[4];

	TRCNVertexBuffer*	m_pGroundVertexBuffer;
	int					m_iGroundVertexCount;

	TRCNIndexBuffer*	m_pGroundIndexBuffer;
	int					m_iGroundPrimitiveCount;

	TRCNVertexBuffer*	m_pWaterVertexBuffer;
	TRCNIndexBuffer*	m_pWaterIndexBuffer;

	bool				m_bPendingRebuildHeightmap;
	bool				m_bPendingRebuildRenderBuffers;

	friend class		CRCNTerrain;
};

#endif	// _RCN_RCNTERRAINBLOCK_H_