/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNTypes.h
Description:    Provides a variable list of data types based on the host platform
				and available graphics API

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNTYPES_H_
#define _RCN_RCNTYPES_H_

// NOTE: For now the only graphics API we support is DirectX. However, in the future
// we may extend support to other APIs such as OpenGL. All RCN code should be written
// with sufficient abstraction that such extension should be relatively simple and
// not cause any errors. To achieve this abstraction, any necessary API or platform
// specific types should be redefined to an RCN equivalent here.

#if defined WIN32

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

#define DIRECTX

// DIRECTX / DIRECT3D 
//struct IDirect3DDevice9;
//struct IDirect3D9;
//struct IDirect3DVertexBuffer9;
//struct IDirect3DIndexBuffer9;
//struct IDirect3DTexture9;
//struct ID3DXSprite;
//struct ID3DXMesh;
//struct ID3DXFont;

typedef IDirect3D9				TRCNRenderInterface;
typedef IDirect3DDevice9		TRCNRenderDevice;
typedef IDirect3DVertexBuffer9	TRCNVertexBuffer;
typedef IDirect3DIndexBuffer9	TRCNIndexBuffer;
typedef IDirect3DTexture9		TRCNTexture;
typedef ID3DXSprite				TRCNXSpriteInterface; 
typedef ID3DXMesh				TRCNXMesh;
typedef ID3DXFont				TRCNXFont;

//struct D3DXMATRIX;
//struct D3DXVECTOR3;

typedef struct _D3DLIGHT9		D3DLIGHT9;
typedef struct _D3DMATERIAL9	D3DMATERIAL9;

// WINDOWS
//struct tagRECT;
//struct tagRAWINPUT;

typedef tagRECT					TRCNRect;
typedef tagRAWINPUT				TRCNRawInput;
#endif

#endif	// _RCN_RCNTYPES_H_