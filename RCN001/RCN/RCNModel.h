/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNModel.h
Description:    A common interface for models. Can describe either an indexed
				primitive or textured mesh.

				TODO: Add support for animated models.

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNMODEL_H_
#define _RCN_RCNMODEL_H_

// -----------------------------------------
// INCLUDES

#include "RCN3D.h"

// -----------------------------------------
// FORWARD DECLARATIONS

class CRCNMaterial;
class CRCNTexture;

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNModel
{
public:

	enum EType
	{
		EType_IndexedPrimitive,
		EType_TexturedMesh,
	};

	CRCNModel();
	~CRCNModel();

	void Render();
	void Load(const char* sFileDirectory, const char* sFilename, EType eType);
	void SetTransform(CRCNMatrix& mTransform);

	// Common
	EType						GetType()				const	{ return m_eType;				}
	const CRCNMatrix*			GetTransform()			const	{ return &m_mTransform;			}
	const CRCNMaterial*			GetMaterials()			const	{ return m_pMaterials;			}
	const CRCNTexture*			GetTextures()			const	{ return m_pTextures;			}
	int							GetNumberOfMaterials()	const	{ return m_iNumberOfMaterials;	}

	// IndexedPrimitive
	TRCNVertexBuffer*			GetVertexBuffer()		const	{ return m_tVertexBuffer;		}
	TRCNIndexBuffer*			GetIndexBuffer()		const	{ return m_tIndexBuffer;		}
	int							GetNumberOfVertices()	const	{ return m_iNumberOfVertices;	}
	int							GetPrimitiveCount()		const	{ return m_iPrimitiveCount;		}

	// TexturedMesh	// TODO: Replace this with a pointer cast (see RCNVector/Matrix)
	TRCNXMesh*					GetXMesh()				const	{ return m_pXMesh;				}

protected:

	EType				m_eType;
	CRCNMatrix			m_mTransform;
	CRCNMaterial*		m_pMaterials;
	CRCNTexture*		m_pTextures;
	unsigned long		m_iNumberOfMaterials;

	TRCNVertexBuffer*	m_tVertexBuffer;
	TRCNIndexBuffer*	m_tIndexBuffer;
	int					m_iNumberOfVertices;
	int					m_iPrimitiveCount;

	TRCNXMesh*			m_pXMesh;
};

#endif	// _RCN_RCNMODEL_H_