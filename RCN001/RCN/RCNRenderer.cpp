/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNRenderer.cpp
Description:    Provides a platform/API-agnostic rendering interface. Supports
				rendering indexed primitives, textured meshes, as well as 
				individual textures, strings and models.

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNRenderer.h"
#include "RCN3D.h"
#include "RCNTexture.h"
#include "RCNMaterial.h"
#include "RCNCamera.h"
#include "RCNModel.h"
#include "RCNString.h"

#if defined DIRECTX
#define RCNFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )
#endif

// -----------------------------------------
// STATIC DEFINITIONS

TRCNRenderInterface*	CRCNRenderer::m_pRenderInterface;		
TRCNRenderDevice*		CRCNRenderer::m_pRenderDevice;			
TRCNXSpriteInterface*	CRCNRenderer::m_pXSpriteInterface;		
TRCNXFont*				CRCNRenderer::m_pXFont;
TRCNRect				CRCNRenderer::m_pTextbox;
TRCNRect				CRCNRenderer::m_pRect;
CRCNCamera*				CRCNRenderer::m_pCamera;

// -----------------------------------------
// CLASS DEFINITIONS

CRCNRenderer::CRCNRenderer()
{
}

CRCNRenderer::~CRCNRenderer()
{
}

void CRCNRenderer::Initialise()
{
#if defined DIRECTX
	HWND hWnd = ((AppPC*)g_pApplication)->GetHWND();

	m_pRenderInterface = Direct3DCreate9(D3D_SDK_VERSION);    // Create the Direct3D interface

	D3DPRESENT_PARAMETERS d3dpp;    // Create a struct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp));				// Clear out the struct for use
	d3dpp.Windowed = true;							// Program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// Discard old frames
	d3dpp.hDeviceWindow = hWnd;						// Set the window to be used by Direct3D
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;		// Set the back buffer format to 32-bit
	d3dpp.BackBufferWidth = ((AppPC*)g_pApplication)->GetDefaultScreenWidth();		// Set the width of the buffer
	d3dpp.BackBufferHeight = ((AppPC*)g_pApplication)->GetDefaultScreenHeight();    // Set the height of the buffer

	// Prepare the z-buffer
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// Create a device class using this information and the info from the D3dDevice struct
	m_pRenderInterface->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&d3dpp,
		&m_pRenderDevice); 

	// Initialise the device for 3D render (should probably be a separate function)
	// We're using the Z-axis so enable it now
	m_pRenderDevice->SetRenderState(D3DRS_ZENABLE, true);

	m_pRenderDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 8);
	m_pRenderDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_pRenderDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pRenderDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// Initialising lighting (should probably be a separate function)
	// Enable lighting
	m_pRenderDevice->SetRenderState(D3DRS_LIGHTING, TRUE);  
	// Set ambient light. Remember, dark gray resembles a darkish environment, whereas a lighter gray (perhaps 200, 200, 200) resembles a well-lit, probably outdoor, environment.
	m_pRenderDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));
	// Set Direct3D normalising our normals, otherwise objects will get brighter/darker as they scale
	m_pRenderDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// Initialise sprite (2D) interface
	D3DXCreateSprite(m_pRenderDevice, &m_pXSpriteInterface);    // create the Direct3D Sprite object

	// Initialise font object (should really offer this via a separate function with its own parameters, but this'll do for now
	D3DXCreateFontA(m_pRenderDevice,		// the Direct3D Device
		20, 0,							// font size twenty with the default width
		FW_NORMAL,						// normal font weight
		1,								// no mipmap levels
		FALSE,							// not italic
		DEFAULT_CHARSET,				// default character set
		OUT_DEFAULT_PRECIS,				// default precision
		DEFAULT_QUALITY,				// default quality
		DEFAULT_PITCH || FF_DONTCARE,   // more defaults...
		"Arial",						// typeface "Arial"
		&m_pXFont);						// address of the font object created above

	SetRect(&m_pTextbox, 0, 0, d3dpp.BackBufferWidth, d3dpp.BackBufferHeight);

#endif
}

void CRCNRenderer::Shutdown()
{
#if defined DIRECTX
	m_pRenderDevice->Release();    // close and release the D3D device
	m_pRenderInterface->Release();			// close and release Direct3D
	m_pXSpriteInterface->Release();
	m_pXFont->Release();
#endif
}

void CRCNRenderer::CreateVertexBuffer(int iNumberOfVertices, TRCNVertexBuffer*& pVertexBuffer, CRCNVertex* pVertices)
{
#if defined DIRECTX
	m_pRenderDevice->CreateVertexBuffer(iNumberOfVertices*sizeof(CRCNVertex), 0, RCNFVF, D3DPOOL_MANAGED, &pVertexBuffer, NULL);
	// Now copy the specified number of vertices into the buffer
	void* pVoid;
	pVertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, pVertices, iNumberOfVertices*sizeof(CRCNVertex));
	pVertexBuffer->Unlock();
#endif
}

void CRCNRenderer::CreateIndexBuffer(int iNumberOfIndices, TRCNIndexBuffer*& pIndexBuffer, short* pIndices)
{
#if defined DIRECTX
	m_pRenderDevice->CreateIndexBuffer(iNumberOfIndices*sizeof(short), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIndexBuffer, NULL);
	// Now copy the specified number of indices into the buffer
	void* pVoid;
	pIndexBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, pIndices, iNumberOfIndices*sizeof(short));
	pIndexBuffer->Unlock();
#endif
}

void CRCNRenderer::Clear(bool bScreen, bool bZBuffer, unsigned int iColour)
{
#if defined DIRECTX
	if(bScreen)
		m_pRenderDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR(iColour), 1.0f, 0);

	if(bZBuffer)
		m_pRenderDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR(iColour), 1.0f, 0);
#endif
}

void CRCNRenderer::BeginRendering()
{
#if defined DIRECTX
	m_pRenderDevice->BeginScene();
	m_pRenderDevice->SetFVF(RCNFVF);
#endif
}

void CRCNRenderer::EndRendering()
{
#if defined DIRECTX
	m_pRenderDevice->EndScene();    // ends the 3D scene
	m_pRenderDevice->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
#endif
}

void CRCNRenderer::Begin2DRendering()
{
#if defined DIRECTX
	m_pXSpriteInterface->Begin(D3DXSPRITE_ALPHABLEND);
#endif
}

void CRCNRenderer::End2DRendering()
{
#if defined DIRECTX
	m_pXSpriteInterface->End();
#endif
}

void CRCNRenderer::ReleaseVertexBuffer(TRCNVertexBuffer* tVertexBuffer)
{
#if defined DIRECTX
	tVertexBuffer->Release();
#endif
}

void CRCNRenderer::ReleaseIndexBuffer(TRCNIndexBuffer* tIndexBuffer)

{
#if defined DIRECTX
	tIndexBuffer->Release();
#endif
}

void CRCNRenderer::DrawIndexedPrimitive(TRCNVertexBuffer* tVertexBuffer, TRCNIndexBuffer* tIndexBuffer, unsigned int iNumberOfVertices, int iNumberOfPrimitives)
{
#if defined DIRECTX
	m_pRenderDevice->SetStreamSource(0, tVertexBuffer, 0, sizeof(CRCNVertex));
	m_pRenderDevice->SetIndices(tIndexBuffer);
	m_pRenderDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, iNumberOfVertices, 0, iNumberOfPrimitives);
#endif
}

void CRCNRenderer::SetCamera(CRCNCamera* pCamera)
{
	// TODO: Instead pass a CRCNMatrix, which contains an = operator override to D3DXMATRIX
	m_pCamera = pCamera;
#if defined DIRECTX
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3 (pCamera->GetPosition().fX, pCamera->GetPosition().fY, pCamera->GetPosition().fZ),
		&D3DXVECTOR3 (pCamera->GetLookAt()->fX, pCamera->GetLookAt()->fY, pCamera->GetLookAt()->fZ),
		&D3DXVECTOR3 (pCamera->GetUp()->fX, pCamera->GetUp()->fY, pCamera->GetUp()->fZ));
	m_pRenderDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(pCamera->GetFieldOfView()),
		(FLOAT)((AppPC*)g_pApplication)->GetDefaultScreenWidth() / (FLOAT)((AppPC*)g_pApplication)->GetDefaultScreenHeight(),
		pCamera->GetNearPlane(),
		pCamera->GetFarPlane());
	m_pRenderDevice->SetTransform(D3DTS_PROJECTION, &matProjection);
#endif
}

void CRCNRenderer::Render(CRCNModel* pModel)
{
#if defined DIRECTX
	m_pRenderDevice->SetTransform(D3DTS_WORLD, static_cast<D3DXMATRIX*>(*pModel->GetTransform()));

	for(int i = 0; i < pModel->GetNumberOfMaterials(); i++)    // loop through each subset
	{
		m_pRenderDevice->SetMaterial(static_cast<D3DMATERIAL9*>(pModel->GetMaterials()[i]));    // set the appropriate material for the subset
		if(pModel->GetTextures()[i] != NULL)    // if the subset has a texture (if texture is not NULL)
			m_pRenderDevice->SetTexture(0, static_cast<LPDIRECT3DTEXTURE9>(pModel->GetTextures()[i]));    // ...then set the texture

		if(pModel->GetType() == CRCNModel::EType_TexturedMesh)
			pModel->GetXMesh()->DrawSubset(i);    // draw the subset
	}
#endif

	if(pModel->GetType() == CRCNModel::EType_IndexedPrimitive)
		DrawIndexedPrimitive(pModel->GetVertexBuffer(), pModel->GetIndexBuffer(), pModel->GetNumberOfVertices(), pModel->GetPrimitiveCount());
}


void CRCNRenderer::Render(CRCNTexture* pTexture)
{
#if defined DIRECTX
	SetRect(&m_pRect, pTexture->GetX(), pTexture->GetY(), pTexture->GetX() + pTexture->GetWidth(), pTexture->GetY() + pTexture->GetHeight());
	m_pXSpriteInterface->Draw(
		static_cast<LPDIRECT3DTEXTURE9>(*pTexture), 
		&m_pRect, 
		&static_cast<D3DXVECTOR3>(pTexture->GetCenter()), 
		&static_cast<D3DXVECTOR3>(pTexture->GetPosition()), 
		D3DCOLOR_ARGB(int(pTexture->GetTransparency() * 255), 255, 255, 255));
#endif
}

void CRCNRenderer::Render(CRCNString* pString)
{
#if defined DIRECTX
	SetRect(&m_pTextbox, pString->GetRenderX(), pString->GetRenderY(), ((AppPC*)g_pApplication)->GetDefaultScreenWidth(), ((AppPC*)g_pApplication)->GetDefaultScreenHeight());
	m_pXFont->DrawTextA(NULL, pString->GetCharacters(), pString->GetLength(), &m_pTextbox, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 255));
#endif
}

void CRCNRenderer::Render(CRCNTexture* pTexture, TRCNVertexBuffer* pVertexBuffer, int iVertexCount, TRCNIndexBuffer* pIndexBuffer, int iPrimitiveCount)
{
	if(pTexture != NULL)
		m_pRenderDevice->SetTexture(0, static_cast<LPDIRECT3DTEXTURE9>(*pTexture));
	DrawIndexedPrimitive(pVertexBuffer, pIndexBuffer, iVertexCount, iPrimitiveCount);
}

void CRCNRenderer::CreateTexture(const char* sFilename, TRCNTexture*& tTexture, CRCNTexture* pTexture)
{
#if defined DIRECTX		
	HRESULT iReturnValue = D3DXCreateTextureFromFileExA(
		m_pRenderDevice,			// D3D Device
		sFilename,					// Filename
		pTexture->GetWidth(),		// Width
		pTexture->GetHeight(),		// Height
		D3DX_DEFAULT,				// Mipmapping
		NULL,						// Usage
		D3DFMT_A8B8G8R8,			// Format (32-bit)
		D3DPOOL_MANAGED,			// Memory handling
		D3DX_DEFAULT,				// Filtering
		D3DX_DEFAULT,				// Mip filtering
		D3DCOLOR_XRGB(255, 0, 255),	// Color key (pink)
		NULL,						// Image info struct
		NULL,						// Palette entry
		&tTexture);

	RCNAssertStr(iReturnValue == D3D_OK, "Create texture failed");
#endif
}

void CRCNRenderer::ReleaseTexture(TRCNTexture* tTexture)
{
#if defined DIRECTX	
	tTexture->Release();
#endif
}

void CRCNRenderer::LoadMesh(const char* sFileDirectory, const char* sFilename, TRCNXMesh*& pXMesh, CRCNMaterial*& pMaterials, CRCNTexture*& pTextures, unsigned long* iNumberOfMaterials)
{
#if defined DIRECTX
	LPD3DXBUFFER pTemporaryMaterialBuffer;

	char filename[128];
	strcpy_s(filename, sFileDirectory);
	strcat_s(filename, sFilename);

	D3DXLoadMeshFromXA(filename,    // load this file
		D3DXMESH_SYSTEMMEM,			// load the mesh into system memory
		m_pRenderDevice,			// the Direct3D Device
		NULL,						// we aren't using adjacency
		&pTemporaryMaterialBuffer,	// put the materials here
		NULL,						// we aren't using effect instances
		iNumberOfMaterials,			// the number of materials in this model
		&pXMesh);					// put the mesh here

	// retrieve the pointer to the buffer containing the material information
	D3DXMATERIAL* tempMaterials = (D3DXMATERIAL*)pTemporaryMaterialBuffer->GetBufferPointer();

	// create a new material buffer for each material in the mesh
	pMaterials = RCNNew CRCNMaterial[*iNumberOfMaterials];
	pTextures = RCNNew CRCNTexture[*iNumberOfMaterials];

	for(DWORD i = 0; i < *iNumberOfMaterials; i++)    // for each material...
	{
		pMaterials[i] = tempMaterials[i].MatD3D;
		pMaterials[i].SetAmbientColour(pMaterials[i].GetDiffuseColour());


		if(tempMaterials[i].pTextureFilename)
		{
			strcpy_s(filename, sFileDirectory);
			strcat_s(filename, tempMaterials[i].pTextureFilename);

			pTextures[i].Load(filename);
		}
	}

	pTemporaryMaterialBuffer->Release();
#endif
}

void CRCNRenderer::ReleaseMesh(TRCNXMesh*& pXMesh)
{
#if defined DIRECTX
	pXMesh->Release();
#endif
}

bool CRCNRenderer::WithinDrawDistance(const CRCNVector& vPosition, float fRadiusSquared)
{
	CRCNVector vDistanceToCamera = vPosition - m_pCamera->GetPosition();

	if(CRCNVector::MagnitudeSquared(vDistanceToCamera) > fRadiusSquared + (m_pCamera->GetFarPlane() * m_pCamera->GetFarPlane()))
		return false;

	return true;
}

