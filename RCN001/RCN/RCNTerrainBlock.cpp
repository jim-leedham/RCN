/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNTerrainBlock.cpp
Description:    A single terrain block generated from a height-map for use with
				RCNTerrain. Uses Vertex/Index buffers for rendering.

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNTerrainBlock.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNTerrainBlock::CRCNTerrainBlock(int iOriginX, int iOriginZ, int iExtentX, int iExtentZ)
{
	m_iOriginX = iOriginX;
	m_iOriginZ = iOriginZ;
	m_iExtentX = iExtentX;
	m_iExtentZ = iExtentZ;

	for (int i = 0; i < 4; i++)
		m_pChildren[i] = NULL;

	m_fHeights			= NULL;

	m_pGroundVertexBuffer		= NULL;
	m_pGroundIndexBuffer		= NULL;
	m_pWaterVertexBuffer		= NULL;
	m_pWaterIndexBuffer			= NULL;

	m_bPendingRebuildRenderBuffers	= true;
	m_bPendingRebuildHeightmap		= true;

	m_iXMinimumStep	= 1;
	m_iZMinimumStep	= 1;
	m_iXMaximumStep	= 1;
	m_iZMaximumStep	= 1;
}

CRCNTerrainBlock::~CRCNTerrainBlock()
{
	if(m_fHeights != NULL)
		RCNDelete [] m_fHeights;

	if(m_pWaterIndexBuffer != NULL)
	{
		CRCNRenderer::ReleaseIndexBuffer(m_pWaterIndexBuffer);
		m_pWaterIndexBuffer = NULL;
	}
}

void CRCNTerrainBlock::CreateHeightmap()
{
	m_dMinimumHeight = CRCNTerrain::ms_dWaterHeight;
	m_dMaximumHeight = CRCNTerrain::ms_dWaterHeight;

	// Extend this block into neighbours so we can compute normals (-1 to size + 1)
	int iHeightSize = CRCNTerrain::ms_iBlockExtentDivisor + 3;

	if (m_fHeights == NULL)
		m_fHeights = RCNNew float[iHeightSize * iHeightSize];

	int iStep = m_iExtentX / CRCNTerrain::ms_iBlockExtentDivisor;

	for(int z = -1; z <= CRCNTerrain::ms_iBlockExtentDivisor + 1; z++)
	{
		for (int x = -1; x <= CRCNTerrain::ms_iBlockExtentDivisor + 1; x++)
		{
			double tx = m_iOriginX + x * iStep;
			double tz = m_iOriginZ + z * iStep;

			double ht = CRCNTerrain::GetHeightAtPosition(tx, tz);
			m_fHeights[(z + 1) * iHeightSize + (x + 1)] = (float) ht;

			m_dMinimumHeight = min(m_dMinimumHeight, ht);
			m_dMaximumHeight = max(m_dMaximumHeight, ht);
		}
	}

	m_bPendingRebuildHeightmap = false;
}

void CRCNTerrainBlock::ValidateLevelOfDetail(double dX, double dZ)
{
	if(m_pChildren[0] == NULL)	// This is a leaf node
	{
		if(LODTooLow(dX, dZ))
		{
			SubdivideBlock();

			// see if children need split
			for (int i = 0; i < 4; i++)
			{
				m_pChildren[i]->ValidateLevelOfDetail(dX, dZ);
			}
		}
	}
	else
	{
		// recurse to children
		bool tooFine = true;
		for (int i = 0; i < 4; i++)
		{
			m_pChildren[i]->ValidateLevelOfDetail(dX, dZ);
			if (m_pChildren[i]->GetChild(0) != NULL || !m_pChildren[i]->LODTooHigh(dX, dZ))
				tooFine = false;
		}

		// if all four children tooFine, combine them
		if (tooFine)
			AbsorbChildren();
	}
}

bool CRCNTerrainBlock::LODTooLow(double dX, double dZ)
{
	if(m_iExtentX <= CRCNTerrain::ms_iMinimumBlockExtent)
		return false;

	double dist;

	// figure distance from eye to edges
	double ldx = dX - m_iOriginX;
	double hdx = (m_iOriginX + m_iExtentX) - dX;
	double ldz = dZ - m_iOriginZ;
	double hdz = (m_iOriginZ + m_iExtentZ) - dZ;

	// if inside this chunk
	if (ldx > 0 && hdx > 0 && ldz > 0 && hdz > 0)
		return true;  // go to max resolution

	// get min distance to chunk
	double dx, dz; 

	// if we're nearest edge, no x component
	if (ldx > 0 && hdx > 0)
		dx = 0.0;
	else dx = min(fabs(ldx), fabs(hdx));

	// if we're nearest edge, no z component
	if (ldz > 0 && hdz > 0)
		dz = 0.0;
	else dz = min(fabs(ldz), fabs(hdz));

	dist = sqrt(dx*dx+dz*dz);

	// don't divide by small distance if near edge
	dist = max(1.0, dist);

	// want unit of landscape to stay roughly same size with distance
	bool result = (m_iExtentX / dist) > 3.0;

	return result;
}

bool CRCNTerrainBlock::LODTooHigh(double dX, double dZ)
{
	double dist;

	// figure distance from eye to edges
	double ldx = dX - m_iOriginX;
	double hdx = (m_iOriginX + m_iExtentX) - dX;
	double ldz = dZ - m_iOriginZ;
	double hdz = (m_iOriginZ + m_iExtentZ) - dZ;

	// if inside this chunk
	if (ldx > 0 && hdx > 0 && ldz > 0 && hdz > 0)
		return false;  // don't drop resolution

	// get min distance to chunk
	double dx, dz; 

	// if we're nearest edge, no x component
	if (ldx > 0 && hdx > 0)
		dx = 0.0;
	else dx = min(fabs(ldx), fabs(hdx));

	// if we're nearest edge, no z component
	if (ldz > 0 && hdz > 0)
		dz = 0.0;
	else dz = min(fabs(ldz), fabs(hdz));

	dist = sqrt(dx*dx+dz*dz);

	// don't divide by small distance if near edge
	dist = max(1.0, dist);

	// want unit of landscape to stay roughly same size with distance
	bool result = (m_iExtentX / dist) < 1.0;

	return result;
}

void CRCNTerrainBlock::SubdivideBlock()
{
	DeleteRenderBuffers();

	int sizeX = m_iExtentX/2;
	int sizeZ = m_iExtentZ/2;

	CRCNTerrainBlock* pBlock;

	pBlock = RCNNew CRCNTerrainBlock(m_iOriginX, m_iOriginZ, sizeX, sizeZ);
	pBlock->ExtractHeightmap(m_fHeights, 0, 0);
	m_pChildren[0] = pBlock;

	pBlock = RCNNew CRCNTerrainBlock(m_iOriginX + sizeX, m_iOriginZ, sizeX, sizeZ);
	pBlock->ExtractHeightmap(m_fHeights, CRCNTerrain::ms_iBlockExtentDivisor / 2, 0);
	m_pChildren[1] = pBlock;

	pBlock = RCNNew CRCNTerrainBlock(m_iOriginX, m_iOriginZ + sizeZ, sizeX, sizeZ);
	pBlock->ExtractHeightmap(m_fHeights, 0, CRCNTerrain::ms_iBlockExtentDivisor / 2);
	m_pChildren[2] = pBlock;

	pBlock = RCNNew CRCNTerrainBlock(m_iOriginX + sizeX, m_iOriginZ + sizeZ, sizeX, sizeZ);
	pBlock->ExtractHeightmap(m_fHeights, CRCNTerrain::ms_iBlockExtentDivisor / 2, CRCNTerrain::ms_iBlockExtentDivisor / 2);
	m_pChildren[3] = pBlock;

	RCNDelete [] m_fHeights;
	m_fHeights = NULL;
}

void CRCNTerrainBlock::ExtractHeightmap( const float* pParentHeights, int iOriginX, int iOriginZ )
{
	int iStep = m_iExtentX / CRCNTerrain::ms_iBlockExtentDivisor;
	int iHeightSize = CRCNTerrain::ms_iBlockExtentDivisor + 3;  // -1 to size+1
	m_fHeights = RCNNew float[iHeightSize*iHeightSize];

	m_dMinimumHeight = CRCNTerrain::ms_dWaterHeight;
	m_dMaximumHeight = CRCNTerrain::ms_dWaterHeight;

	// copy height data from parent
	for (int z = 0; z <= CRCNTerrain::ms_iBlockExtentDivisor; z += 2)
	{
		for (int x = 0; x <= CRCNTerrain::ms_iBlockExtentDivisor; x += 2)
		{
			float ht = pParentHeights[(iOriginZ + z/2+1)*iHeightSize + (iOriginX + x/2+1)];
			m_fHeights[(z+1)*iHeightSize + (x+1)] = ht;

			m_dMinimumHeight = min(m_dMinimumHeight, ht);
			m_dMaximumHeight = max(m_dMaximumHeight, ht);
		}
	}

	// interpolate heights on odd cols of even rows
	for (int z = 0; z <= CRCNTerrain::ms_iBlockExtentDivisor; z+=2)
	{
		for (int x = 1; x <= CRCNTerrain::ms_iBlockExtentDivisor; x+=2)
		{
			float ht = m_fHeights[(z+1)*iHeightSize + (x+0)];
			ht += m_fHeights[(z+1)*iHeightSize + (x+2)];
			ht /= 2.0f;
			m_fHeights[(z+1)*iHeightSize + (x+1)] = ht;

			m_dMinimumHeight = min(m_dMinimumHeight, ht);
			m_dMaximumHeight = max(m_dMaximumHeight, ht);
		}
	}

	// interpolate heights on odd rows of even cols
	for (int z = 1; z <= CRCNTerrain::ms_iBlockExtentDivisor; z+=2)
	{
		for (int x = 0; x <= CRCNTerrain::ms_iBlockExtentDivisor; x+=2)
		{
			float ht = m_fHeights[(z+0)*iHeightSize + (x+1)];
			ht += m_fHeights[(z+2)*iHeightSize + (x+1)];
			ht /= 2.0f;

			m_fHeights[(z+1)*iHeightSize + (x+1)] = ht;

			m_dMinimumHeight = min(m_dMinimumHeight, ht);
			m_dMaximumHeight = max(m_dMaximumHeight, ht);
		}
	}

	// interpolate heights on odd rows of odd cols
	for (int z = 1; z <= CRCNTerrain::ms_iBlockExtentDivisor; z+=2)
	{
		for (int x = 1; x <= CRCNTerrain::ms_iBlockExtentDivisor; x+=2)
		{
			float ht = m_fHeights[(z+0)*iHeightSize + (x+0)];
			ht += m_fHeights[(z+0)*iHeightSize + (x+2)];
			ht += m_fHeights[(z+2)*iHeightSize + (x+0)];
			ht += m_fHeights[(z+2)*iHeightSize + (x+2)];
			ht /= 4.0f;

			m_fHeights[(z+1)*iHeightSize + (x+1)] = ht;

			m_dMinimumHeight = min(m_dMinimumHeight, ht);
			m_dMaximumHeight = max(m_dMaximumHeight, ht);
		}
	}

	// now we need edge values which were not in parent
	int coords[] = {-1, CRCNTerrain::ms_iBlockExtentDivisor, CRCNTerrain::ms_iBlockExtentDivisor+1};
	for (int x = -1; x <= CRCNTerrain::ms_iBlockExtentDivisor+1; x++)
	{
		for (int i = 0; i < 3; i++)
		{
			int z = coords[i];
			double tx = m_iOriginX + x * iStep;
			double tz = m_iOriginZ + z * iStep;

			double ht = CRCNTerrain::GetHeightAtPosition(tx, tz);
			m_fHeights[(z+1)*iHeightSize + (x+1)] = (float) ht;

			m_dMinimumHeight = min(m_dMinimumHeight, ht);
			m_dMaximumHeight = max(m_dMaximumHeight, ht);
		}
	}

	for (int z = -1; z <= CRCNTerrain::ms_iBlockExtentDivisor+1; z++)
	{
		for (int i = 0; i < 3; i++)
		{
			int x = coords[i];
			double tx = m_iOriginX + x * iStep;
			double tz = m_iOriginZ + z * iStep;

			double ht = CRCNTerrain::GetHeightAtPosition(tx, tz);
			m_fHeights[(z+1)*iHeightSize + (x+1)] = (float) ht;

			m_dMinimumHeight = min(m_dMinimumHeight, ht);
			m_dMaximumHeight = max(m_dMaximumHeight, ht);
		}
	}

	m_bPendingRebuildRenderBuffers = true;
}


void CRCNTerrainBlock::DeleteRenderBuffers()
{
	if(m_pGroundVertexBuffer != NULL)
	{
		CRCNRenderer::ReleaseVertexBuffer(m_pGroundVertexBuffer);
		m_pGroundVertexBuffer = NULL;
	}

	if(m_pGroundIndexBuffer != NULL)
	{
		CRCNRenderer::ReleaseIndexBuffer(m_pGroundIndexBuffer);
		m_pGroundIndexBuffer = NULL;
	}

	if(m_pWaterVertexBuffer != NULL)
	{
		CRCNRenderer::ReleaseVertexBuffer(m_pWaterVertexBuffer);
		m_pWaterVertexBuffer = NULL;
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_pChildren[i] != NULL)
			m_pChildren[i]->DeleteRenderBuffers();
	}
}

const CRCNTerrainBlock* CRCNTerrainBlock::GetChild(int iIndex)
{
	return m_pChildren[iIndex];
}

void CRCNTerrainBlock::AbsorbChildren()
{
	int step = m_iExtentX / CRCNTerrain::ms_iBlockExtentDivisor;
	int heightSize = CRCNTerrain::ms_iBlockExtentDivisor + 3;  // -1 to size+1
	m_fHeights = new float[heightSize*heightSize];

	// copy height data from each child
	const int HALF_TERRAIN = CRCNTerrain::ms_iBlockExtentDivisor / 2;

	for (int z = 0; z < 2; z++)
	{
		for (int x = 0; x < 2; x++)
		{
			int target = x * HALF_TERRAIN + z * HALF_TERRAIN * heightSize;
			target += 1 + heightSize;  // starting at cell 1,1
			int child = x + z*2;
			float* childHeights = m_pChildren[child]->m_fHeights;

			// copy every other height from child
			for (int childZ = 0; childZ < CRCNTerrain::ms_iBlockExtentDivisor; childZ += 2)
			{
				for (int childX = 0; childX < CRCNTerrain::ms_iBlockExtentDivisor; childX += 2)
				{
					m_fHeights[target++] = childHeights[1+childX + (1+childZ)*heightSize];
				}
				target += heightSize - HALF_TERRAIN;
			}
		}
	}

	// now we need edge values which were not in any child
	int coords[] = {-1, CRCNTerrain::ms_iBlockExtentDivisor, CRCNTerrain::ms_iBlockExtentDivisor+1};
	for (int x = -1; x <= CRCNTerrain::ms_iBlockExtentDivisor+1; x++)
	{
		for (int i = 0; i < 3; i++)
		{
			int z = coords[i];
			double tx = m_iOriginX+x*step;
			double tz = m_iOriginZ+z*step;

			double ht = CRCNTerrain::GetHeightAtPosition(tx, tz);
			m_fHeights[(z+1)*heightSize + (x+1)] = (float) ht;
		}
	}

	for (int z = -1; z <= CRCNTerrain::ms_iBlockExtentDivisor+1; z++)
	{
		for (int i = 0; i < 3; i++)
		{
			int x = coords[i];
			double tx = m_iOriginX+x*step;
			double tz = m_iOriginZ+z*step;

			double ht = CRCNTerrain::GetHeightAtPosition(tx, tz);
			m_fHeights[(z+1)*heightSize + (x+1)] = (float) ht;
		}
	}

	// max and min heights should already be set from when this was leaf chunk

	// if any child was a temporary heightmap, this chunk needs rebuild.

	// delete the children
	for (int i = 0; i < 4; i++)
	{
		if (m_pChildren[i]->m_bPendingRebuildHeightmap)
			m_bPendingRebuildHeightmap = true;

		RCNDelete m_pChildren[i];
		m_pChildren[i] = NULL;
	}

	m_bPendingRebuildRenderBuffers = true;
}

void CRCNTerrainBlock::CreateRenderBuffers()
{
	// if this is a leaf node
	if (m_pChildren[0] == NULL)
	{
		if (m_pGroundVertexBuffer == NULL || m_bPendingRebuildRenderBuffers == true)
		{
			CreateGroundRenderBuffers();
		}

		
		if (m_pWaterVertexBuffer == NULL || m_bPendingRebuildRenderBuffers == true)
		{
			CreateWaterRenderBuffers();
			/*changed = true;*/
		}
		
		// buffers regenerated
		m_bPendingRebuildRenderBuffers = false;
	}
	else
	{
		// recurse to children, update min/max heights
		m_dMinimumHeight = INT_MAX;
		m_dMaximumHeight = -INT_MAX;
		for (int i = 0; i < 4; i++)
		{
			/*changed |=*/ m_pChildren[i]->CreateRenderBuffers();

			m_dMinimumHeight = min(m_dMinimumHeight, m_pChildren[i]->m_dMinimumHeight);
			m_dMaximumHeight = min(m_dMaximumHeight, m_pChildren[i]->m_dMaximumHeight);
		}
	}
}

void CRCNTerrainBlock::CreateGroundRenderBuffers()
{
	// create terrain for this chunk
	int step = m_iExtentX / CRCNTerrain::ms_iBlockExtentDivisor;
	int heightSize = CRCNTerrain::ms_iBlockExtentDivisor + 3;  // -1 to size+1

	if (m_fHeights == NULL)
		CreateHeightmap();

	// generate the vertex list.
	int iVertexSize = CRCNTerrain::ms_iBlockExtentDivisor+1;   // 0 to size
	m_iGroundVertexCount = iVertexSize * iVertexSize;

	CRCNVertex* vGroundVertices = RCNNew CRCNVertex[m_iGroundVertexCount];
	CRCNVertex* v;
	for (int z = 0; z < iVertexSize; z++)
	{
		for (int x = 0; x < iVertexSize; x++)
		{
			v = &vGroundVertices[(z * iVertexSize) + x];

			int index = (z+1)*heightSize + (x+1);
			float ht;

			// on the outside edges, interpolate the points to match low res neighbors
			// skip the points that are not interpolated
			if (x == 0 && (z % m_iXMinimumStep) != 0)
			{
				// first col.  steps are res of m_xminStep
				int topZ = m_iXMinimumStep*(z/m_iXMinimumStep);
				int topIndex = (topZ+1)*heightSize + (x+1);
				float topHt = m_fHeights[topIndex];
				float botHt = m_fHeights[topIndex+m_iXMinimumStep*heightSize];

				// interpolate between low res points
				ht = topHt + ((z%m_iXMinimumStep) * (botHt-topHt))/m_iXMinimumStep;
			}
			else if (x == iVertexSize-1 && (z % m_iXMaximumStep) != 0)
			{
				// last col.  steps are res of m_xmaxStep
				int topZ = m_iXMaximumStep*(z/m_iXMaximumStep);
				int topIndex = (topZ+1)*heightSize + (x+1);
				float topHt = m_fHeights[topIndex];
				float botHt = m_fHeights[topIndex+m_iXMaximumStep*heightSize];

				// interpolate between low res points
				ht = topHt + ((z%m_iXMaximumStep) * (botHt-topHt))/m_iXMaximumStep;
			}
			else if (z == 0 && (x % m_iZMinimumStep) != 0)
			{
				// first row.  steps are res of m_zminStep
				int leftX = m_iZMinimumStep*(x/m_iZMinimumStep);
				int leftIndex = (z+1)*heightSize + (leftX+1);
				float leftHt = m_fHeights[leftIndex];
				float rightHt = m_fHeights[leftIndex+m_iZMinimumStep];

				// interpolate between low res points
				ht = leftHt + ((x%m_iZMinimumStep) * (rightHt-leftHt))/m_iZMinimumStep;
			}
			else if (z == iVertexSize-1 && (x % m_iZMaximumStep) != 0)
			{
				// last row.  steps are res of m_zmaxStep
				int leftX = m_iZMaximumStep*(x/m_iZMaximumStep);
				int leftIndex = (z+1)*heightSize + (leftX+1);
				float leftHt = m_fHeights[leftIndex];
				float rightHt = m_fHeights[leftIndex+m_iZMaximumStep];

				// interpolate between low res points
				ht = leftHt + ((x%m_iZMaximumStep) * (rightHt-leftHt))/m_iZMaximumStep;
			}
			else
			{
				// interior point or unmodified edge point
				ht = m_fHeights[index];
			}

			v->fX = (float) (x*step);
			v->fY = ht;
			v->fZ = (float) (z*step);

			v->vNormal.fX = 0.0f;
			v->vNormal.fY = 0.0f;
			v->vNormal.fZ = 0.0f;

			float slope, len;
			slope = m_fHeights[index-1] - ht;
			len = sqrt(slope*slope+step*step);
			v->vNormal.fX += slope/len;
			v->vNormal.fY += step/len;

			slope = m_fHeights[index+1] - ht;
			len = sqrt(slope*slope+step*step);
			v->vNormal.fX += -slope/len;
			v->vNormal.fY += step/len;

			slope = m_fHeights[index-heightSize] - ht;
			len = sqrt(slope*slope+step*step);
			v->vNormal.fZ += slope/len;
			v->vNormal.fY += step/len;

			slope = m_fHeights[index+heightSize] - ht;
			len = sqrt(slope*slope+step*step);
			v->vNormal.fZ += -slope/len;
			v->vNormal.fY += step/len;

			v->vNormal.fX /= 4.0f;
			v->vNormal.fY /= 4.0f;
			v->vNormal.fZ /= 4.0f;

			// TODO: Sort these! OpenGL vs DirectX shenanigans
			//v->fU = (float) (x*step);
			//v->fV = (float) (z*step);

			// TODO: Colour
			/*
			if (ht > 3000)
			{
				v->setColor(133, 133, 133);  // gray rock
			}
			else if (ht > 300)  
			{
				v->setColor(62, 124, 85); // green grass
			}
			else if (ht > 100)  
			{
				v->setColor(241, 220, 177); // sand
			}
			else 
			{
				double inten = max(0, ht)/100.0;
				v->setColor((BYTE) (inten*179), (BYTE) (inten*148), (BYTE) (inten*117));   // dirt
			}
			*/
		}
	}

	// delete old buffer 
	if(m_pGroundVertexBuffer != NULL)
	{
		CRCNRenderer::ReleaseVertexBuffer(m_pGroundVertexBuffer);
		m_pGroundVertexBuffer = NULL;
	}

	CRCNRenderer::CreateVertexBuffer(m_iGroundVertexCount, m_pGroundVertexBuffer, vGroundVertices);

	RCNDelete [] vGroundVertices;

	short* iGroundIndices = RCNNew short[6*CRCNTerrain::ms_iBlockExtentDivisor*CRCNTerrain::ms_iBlockExtentDivisor];
	int iIndexCounter = 0;
	for (int z = 0; z < CRCNTerrain::ms_iBlockExtentDivisor; z++)
	{
		for (int x = 0; x < CRCNTerrain::ms_iBlockExtentDivisor; x++)
		{
			int index = z*iVertexSize+x;
			iGroundIndices[iIndexCounter++] = index;					// top		left
			iGroundIndices[iIndexCounter++] = index+iVertexSize;		// bottom	left
			iGroundIndices[iIndexCounter++] = index+1;					// top		right
			iGroundIndices[iIndexCounter++] = index+iVertexSize;		// bottom	left
			iGroundIndices[iIndexCounter++] = index+iVertexSize+1;		// bottom	right
			iGroundIndices[iIndexCounter++] = index+1;					// top		right
		}
	}

	// delete old indexes
	if(m_pGroundIndexBuffer != NULL)
	{
		CRCNRenderer::ReleaseIndexBuffer(m_pGroundIndexBuffer);
		m_pGroundIndexBuffer = NULL;
	}

	m_iGroundPrimitiveCount = iIndexCounter / 3;
	CRCNRenderer::CreateIndexBuffer(iIndexCounter, m_pGroundIndexBuffer, iGroundIndices);

	RCNDelete [] iGroundIndices;
}

void CRCNTerrainBlock::CreateWaterRenderBuffers()
{
	CRCNVertex vTopLeft,vTopRight,vBottomLeft,vBottomRight;

	// three points times two triangles
	CRCNVertex vWaterVertices[3 * 2];

	// low x, low z
	vBottomLeft.vNormal.fX = 0.0f;  vBottomLeft.vNormal.fY = 1.0f;    vBottomLeft.vNormal.fZ = 0.0f;
	vBottomLeft.fX = 0.0f;  vBottomLeft.fY = (float)CRCNTerrain::ms_dWaterHeight;  vBottomLeft.fZ = 0.0f;

	vBottomLeft.fU = 0.0f;  vBottomLeft.fV = 0.0f;

	// low x, high z
	vTopLeft.vNormal.fX = 0.0f;  vTopLeft.vNormal.fY = 1.0f; vTopLeft.vNormal.fZ = 0.0f;
	vTopLeft.fX = 0.0f;  vTopLeft.fY = (float)CRCNTerrain::ms_dWaterHeight;  vTopLeft.fZ = (float) m_iExtentZ;

	vTopLeft.fU = 0.0f;  vTopLeft.fV = (float) m_iExtentZ;

	// high x, low z
	vBottomRight.vNormal.fX = 0.0f;  vBottomRight.vNormal.fY = 1.0f; vBottomRight.vNormal.fZ = 0.0f;
	vBottomRight.fX = (float) m_iExtentX;  vBottomRight.fY = (float)CRCNTerrain::ms_dWaterHeight;  vBottomRight.fZ = 0.0f;

	vBottomRight.fU = (float) m_iExtentX;  vBottomRight.fV = 0.0f;

	// high x, high z
	vTopRight.vNormal.fX = 0.0f;  vTopRight.vNormal.fY = 1.0f; vTopRight.vNormal.fZ = 0.0f;
	vTopRight.fX = (float) m_iExtentX;  vTopRight.fY = (float)CRCNTerrain::ms_dWaterHeight;  vTopRight.fZ = (float) m_iExtentZ;

	vTopRight.fU = (float) m_iExtentX;  vTopRight.fV = (float) m_iExtentZ;

	vWaterVertices[0] = vTopLeft;
	vWaterVertices[1] = vTopRight;
	vWaterVertices[2] = vBottomLeft;
	vWaterVertices[3] = vBottomLeft;
	vWaterVertices[4] = vTopRight;
	vWaterVertices[5] = vBottomRight;

	// delete old indexes
	if(m_pWaterVertexBuffer != NULL)
	{
		CRCNRenderer::ReleaseVertexBuffer(m_pWaterVertexBuffer);
		m_pWaterVertexBuffer = NULL;
	}

	CRCNRenderer::CreateVertexBuffer(3 * 2, m_pWaterVertexBuffer, vWaterVertices);
}

int CRCNTerrainBlock::GetNumberOfChildren()
{

	if(m_pChildren[0] == NULL)
		return 1;

	int iCount = 0;

	for(int i = 0; i < 4; i++)
	{
		iCount += m_pChildren[i]->GetNumberOfChildren();
	}

	return iCount;
}

int CRCNTerrainBlock::GetOriginX()
{
	return m_iOriginX;
}

int CRCNTerrainBlock::GetOriginZ()
{
	return m_iOriginZ;
}

void CRCNTerrainBlock::ValidateHeightmaps()
{
	if (m_pChildren[0] == NULL)
	{
		if (m_bPendingRebuildHeightmap)
			CRCNTerrain::ms_pInvalidBlockList[CRCNTerrain::ms_iInvalidBlockCount++] = this;
	}
	else
	{
		// recurse to children
		for (int i = 0; i < 4; i++)
		{
			m_pChildren[i]->ValidateHeightmaps();
		}                          
	}
}

void CRCNTerrainBlock::ValidateEdges()
{
	if (m_pChildren[0] == NULL)
	{
		// TODO
		/*
		bool bUpdateChunk = false;

		// we only care about stepping in high res chunks next to lower res ones
		int xminStep = resolutionAt(m_iOriginX - 1, m_iOriginZ) / m_iExtentX;
		xminStep = max(1, xminStep);

		if (xminStep != chunk->m_xminStep)
		{
			chunk->m_xminStep = xminStep;
			bUpdateChunk = true;
		}

		int xmaxStep = resolutionAt(m_iOriginX + m_iExtentX, m_iOriginZ) / m_iExtentX;
		xmaxStep = max(1, xmaxStep);

		if (xmaxStep != chunk->m_xmaxStep)
		{
			chunk->m_xmaxStep = xmaxStep;
			bUpdateChunk = true;
		}

		int zminStep = resolutionAt(m_iOriginX, m_iOriginZ - 1) / m_iExtentZ;
		zminStep = max(1, zminStep);

		if (zminStep != chunk->m_zminStep)
		{
			chunk->m_zminStep = zminStep;
			bUpdateChunk = true;
		}

		int zmaxStep = resolutionAt(m_iOriginX, m_iOriginZ + m_iExtentZ) / m_iExtentZ;
		zmaxStep = max(1, zmaxStep);

		if (zmaxStep != chunk->m_zmaxStep)
		{
			chunk->m_zmaxStep = zmaxStep;
			bUpdateChunk = true;
		}

		if (bUpdateChunk)
			chunk->deleteBuffers();
			*/
	}
	else
	{
		// recurse to children
		for (int i = 0; i < 4; i++)
			m_pChildren[i]->ValidateEdges();
	}
}

void CRCNTerrainBlock::Render(const CRCNVector& vPosition)
{
	// Compute the sphere containing this block so we can do
	// a fast cull if not within viewing distance
	// (for now we'll just naively use the camera's far distance
	// as a cut off, but would be smarter to use the view frustum)
	// TODO: Extend CRCNCamera to hold frustum data

	CRCNVector vCenter;
	vCenter.fX = (float)(m_iOriginX + m_iExtentX / 2);
	vCenter.fZ = (float)(m_iOriginZ + m_iExtentZ / 2);
	vCenter.fY = (float)((m_dMinimumHeight + m_dMaximumHeight) / 2);
	CRCNVector vCorner((float)(m_iExtentX / 2), (float)(m_dMaximumHeight - vCenter.fY), (float)(m_iExtentZ / 2));

	float fRadiusSquared = CRCNVector::MagnitudeSquared(vCorner);

	if(CRCNRenderer::WithinDrawDistance(vCenter, fRadiusSquared) == false)
		return;

	if (m_pChildren[0] == NULL)
	{
		CRCNRenderer::Render(CRCNTerrain::ms_pGroundTexture, m_pGroundVertexBuffer, m_iGroundVertexCount, m_pGroundIndexBuffer, m_iGroundPrimitiveCount);
	}
	else
	{
		for(int i = 0; i < 4; i++)
			m_pChildren[i]->Render(vPosition);
	}
}
