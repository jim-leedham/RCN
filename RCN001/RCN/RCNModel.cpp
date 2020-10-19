/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNModel.cpp
Description:    A common interface for models. Can describe either an indexed
				primitive or textured mesh.

				TODO: Add support for animated models.

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNModel.h"
#include "RCNMaterial.h"
#include "RCNTexture.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNModel::CRCNModel()
{
	m_mTransform = CRCNMatrix::MakeIdentityMatrix();
	m_pXMesh = NULL;
	m_pMaterials = NULL;
	m_pTextures = NULL;
}

CRCNModel::~CRCNModel()
{
	if(m_pXMesh)
		CRCNRenderer::ReleaseMesh(m_pXMesh);
	
	if(m_pMaterials)
	{
		if(m_iNumberOfMaterials > 1)
			RCNDelete [] m_pMaterials;
		else
			RCNDelete m_pMaterials;
	}

	if(m_pTextures)
	{
		if(m_iNumberOfMaterials > 1)
			RCNDelete [] m_pTextures;
		else
			RCNDelete m_pTextures;
	}
}

void CRCNModel::Render()
{
	CRCNRenderer::Render(this);
}

void CRCNModel::SetTransform(CRCNMatrix& mTransform)
{
	m_mTransform = mTransform;
}

void CRCNModel::Load(const char* sFileDirectory, const char* sFilename, EType eType)
{
	switch(eType)
	{
	case EType_IndexedPrimitive:
		RCNAssertStr(false, "No support for loading indexed primitive models. Construct the vertex/index buffers manually");
		break;
	case EType_TexturedMesh:
		CRCNRenderer::LoadMesh(sFileDirectory, sFilename, m_pXMesh, m_pMaterials, m_pTextures, &m_iNumberOfMaterials);
		m_eType = eType;
		break;
	default:
		RCNAssertStr(false, "Invalid model type");
	}
}
