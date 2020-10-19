/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNRenderer.h
Description:    Provides a platform/API-agnostic rendering interface. Supports
				rendering indexed primitives, textured meshes, as well as 
				individual textures, strings and models.

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNRENDERER_H_
#define _RCN_RCNRENDERER_H_

// -----------------------------------------
// FORWARD DECLARATIONS

class CRCNVertex;
class CRCNTexture;
class CRCNMaterial;
class CRCNCamera;
class CRCNModel;
class CRCNString;

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNRenderer
{
public:

									CRCNRenderer();
									~CRCNRenderer();

	static void						Initialise();
	static void						Shutdown();

	static TRCNRenderDevice*		GetRenderDevice()	{ return m_pRenderDevice; }

	static void						CreateVertexBuffer(int iNumberOfVertices, TRCNVertexBuffer*& pVertexBuffer, CRCNVertex* pVertices);
	static void						CreateIndexBuffer(int iNumberOfIndices, TRCNIndexBuffer*& pIndexBuffer, short* pIndices);

	static void						ReleaseVertexBuffer(TRCNVertexBuffer* m_tVertexBuffer);
	static void						ReleaseIndexBuffer(TRCNIndexBuffer* tIndexBuffer);

	static void						CreateTexture(const char* sFilename, TRCNTexture*& tTexture, CRCNTexture* pTexture);
	static void						ReleaseTexture(TRCNTexture* tTexture);

	static void						LoadMesh(const char* sFileDirectory, const char* sFilename, TRCNXMesh*& m_pXMesh, CRCNMaterial*& m_pMaterials, CRCNTexture*& m_pTextures, unsigned long* m_iNumberOfMaterials);
	static void						ReleaseMesh( TRCNXMesh*& pXMesh );

	static void						Clear(bool bScreen, bool bZBuffer, unsigned int iColour);

	static void						BeginRendering();
	static void						EndRendering();

	static void						Begin2DRendering();
	static void						End2DRendering();

	static void						SetCamera(CRCNCamera* pCamera);

	static void						Render(CRCNModel* pModel);
	static void						Render(CRCNTexture* pTexture);
	static void						Render(CRCNString* pString);
	static void						Render(CRCNTexture* pTexture, TRCNVertexBuffer* pVertexBuffer, int iVertexCount, TRCNIndexBuffer* pIndexBuffer, int iPrimitiveCount);

	static bool						WithinDrawDistance(const CRCNVector& vPosition, float fRadiusSquared);

private:

	static void						DrawIndexedPrimitive(TRCNVertexBuffer* tVertexBuffer, TRCNIndexBuffer* tIndexBuffer, unsigned int iNumberOfVertices, int iNumberOfPrimitives);

	static TRCNRenderInterface*		m_pRenderInterface;			// pointer to the Direct3D interface
	static TRCNRenderDevice*		m_pRenderDevice;			// pointer to the device class
	static TRCNXSpriteInterface*	m_pXSpriteInterface;		// pointer to the sprite interface
	static TRCNXFont*				m_pXFont;
	static TRCNRect					m_pTextbox;
	static TRCNRect					m_pRect;

	static CRCNCamera*				m_pCamera;
};

#endif	// _RCN_RCNRENDERER_H_