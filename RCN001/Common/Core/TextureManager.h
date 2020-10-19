/*---------------------------------------------------------------------------*
Project:		RCN
File:			TextureManager.h
Description:    Handles all loading and referencing of textures

*---------------------------------------------------------------------------*/

#ifndef _COMMON_CORE_TEXTUREMANAGER_H_
#define _COMMON_CORE_TEXTUREMANAGER_H_

#include "TextureDefinitions.h"

// -----------------------------------------
// CLASS DECLARATIONS

class CTextureManager
{
public:
					CTextureManager();
					~CTextureManager();

	void			LoadTexture(ETextureID eTexture);
	void			LoadTextures(ETextureID eFirst, ETextureID eLast);
	CRCNTexture*	GetTexture(ETextureID eTexture);

private:

	CRCNTexture*	m_pTextures[ETexture_Count];
};

#endif	// _COMMON_CORE_TEXTUREMANAGER_H_