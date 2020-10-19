/*---------------------------------------------------------------------------*
Project:		RCN
File:			TextureManager.cpp
Description:    Handles all loading and referencing of textures

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "TextureManager.h"

// -----------------------------------------
// CLASS DEFINITIONS

CTextureManager::CTextureManager()
{
	for(int i = 0; i < ETexture_Count; i++)
		m_pTextures[i] = NULL;
}

CTextureManager::~CTextureManager()
{
	for(int i = 0; i < ETexture_Count; i++)
		RCNDelete m_pTextures[i];
}

void CTextureManager::LoadTexture(ETextureID eTexture)
{
	RCNAssertStr(ETexture_None < eTexture && eTexture < ETexture_Count, "Specified texture out of range");
	
	if(m_pTextures[eTexture])
		return;

	m_pTextures[eTexture] = RCNNew CRCNTexture();
	m_pTextures[eTexture]->SetDimensions(g_tTextures[eTexture].iWidth, g_tTextures[eTexture].iHeight);
	m_pTextures[eTexture]->Load(g_tTextures[eTexture].pFilename);
}

void CTextureManager::LoadTextures(ETextureID eFirst, ETextureID eLast)
{
	RCNAssertStr(ETexture_None < eFirst && eLast < ETexture_Count, "Specified texture out of range");

	for(int i = eFirst; i <= eLast; i++)
	{
		if(m_pTextures[i])
			continue;

		m_pTextures[i] = RCNNew CRCNTexture();
		m_pTextures[i]->SetDimensions(g_tTextures[i].iWidth, g_tTextures[i].iHeight);
		m_pTextures[i]->Load(g_tTextures[i].pFilename);
	}
}

CRCNTexture* CTextureManager::GetTexture(ETextureID eTexture)
{
	RCNAssertStr(ETexture_None < eTexture && eTexture < ETexture_Count, "Specified texture out of range");
	RCNAssertStr(m_pTextures[eTexture] != NULL, "Texture not loaded");

	return m_pTextures[eTexture];
}
