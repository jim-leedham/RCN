/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNCube.cpp
Description:    An indexed primitive cube implementation derived from RCNModel

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNCube.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNCube::CRCNCube()
{
	m_tVertexBuffer = NULL;
	m_tIndexBuffer = NULL;

	CRCNVertex pVertices[] =
	{
		CRCNVertex(-3.0f, -3.0f, 3.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f),			// side 1
		CRCNVertex(3.0f, -3.0f, 3.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f),
		CRCNVertex(-3.0f, 3.0f, 3.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f),
		CRCNVertex(3.0f, 3.0f, 3.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f),

		CRCNVertex(-3.0f, -3.0f, -3.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f),		// side 2
		CRCNVertex(-3.0f, 3.0f, -3.0f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f),
		CRCNVertex(3.0f, -3.0f, -3.0f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f),
		CRCNVertex(3.0f, 3.0f, -3.0f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f),

		CRCNVertex(-3.0f, 3.0f, -3.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f),			// side 3
		CRCNVertex(-3.0f, 3.0f, 3.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f),
		CRCNVertex(3.0f, 3.0f, -3.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f),
		CRCNVertex(3.0f, 3.0f, 3.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f),

		CRCNVertex(-3.0f, -3.0f, -3.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f),		// side 4
		CRCNVertex(3.0f, -3.0f, -3.0f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f),
		CRCNVertex(-3.0f, -3.0f, 3.0f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f),
		CRCNVertex(3.0f, -3.0f, 3.0f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f),

		CRCNVertex(3.0f, -3.0f, -3.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f),			// side 5
		CRCNVertex(3.0f, 3.0f, -3.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f),
		CRCNVertex(3.0f, -3.0f, 3.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f),
		CRCNVertex(3.0f, 3.0f, 3.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f),

		CRCNVertex(-3.0f, -3.0f, -3.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f),		// side 6
		CRCNVertex(-3.0f, -3.0f, 3.0f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f),
		CRCNVertex(-3.0f, 3.0f, -3.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f),
		CRCNVertex(-3.0f, 3.0f, 3.0f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f),
	};

	CRCNRenderer::CreateVertexBuffer(24, m_tVertexBuffer, pVertices);

	short pIndices[] =
	{
		0, 1, 2,		// side 1
		2, 1, 3,
		4, 5, 6,		// side 2
		6, 5, 7,
		8, 9, 10,		// side 3
		10, 9, 11,
		12, 13, 14,		// side 4
		14, 13, 15,
		16, 17, 18,		// side 5
		18, 17, 19,
		20, 21, 22,		// side 6
		22, 21, 23,
	};

	CRCNRenderer::CreateIndexBuffer(36, m_tIndexBuffer, pIndices);

	m_eType = EType_IndexedPrimitive;
	m_iNumberOfVertices = 24;
	m_iPrimitiveCount = 12;

	m_pMaterials = RCNNew CRCNMaterial();
	m_pMaterials->SetDiffuseColour(0xffffffff);
	m_pMaterials->SetAmbientColour(0xffffffff);
	m_iNumberOfMaterials = 1;

	static int iTexture = 0;
	if(iTexture % 2 == 0)
		m_pTextures = RCNNew CRCNTexture("Data/Textures/wood.png");
	else
		m_pTextures = RCNNew CRCNTexture("Data/Textures/bricks.png");
	iTexture++;
}

CRCNCube::~CRCNCube()
{
	CRCNRenderer::ReleaseVertexBuffer(m_tVertexBuffer);
	CRCNRenderer::ReleaseIndexBuffer(m_tIndexBuffer);
}
