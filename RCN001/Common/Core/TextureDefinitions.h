/*---------------------------------------------------------------------------*
Project:		RCN
File:			TextureDefinitions.h
Description:    Global definitions and enumerations relating to textures

*---------------------------------------------------------------------------*/

#ifndef _COMMON_CORE_TEXTUREDEFINITIONS_H_
#define _COMMON_CORE_TEXTUREDEFINITIONS_H_

// NOTE: Ideally we would auto-generate this file. Perhaps if our assets grow
// significantly it may become worthwhile. For now it seems over-engineering.

// NOTE: It would also be good to generate an Atlas containing many textures and
// this would simply define the top-left, bottom-right co-ordinates of each

enum ETextureID
{
	ETexture_None = -1,
	ETexture_BootstrapScreenBackground,
	ETexture_MenuScreenBackground,
	ETexture_MenuScreenPlayButton,
	ETexture_MenuScreenPlayButtonOnClick,
	ETexture_MenuScreenQuitButton,
	ETexture_MenuScreenQuitButtonOnClick,
	ETexture_LoadingScreenBackground,
	ETexture_GameScreenBackground,
	ETexture_Count
};

struct TTextureData
{
	const char* pFilename;
	ETextureID	eTexture;
	int			iWidth;
	int			iHeight;
};


static TTextureData g_tTextures[ETexture_Count] =
{
	// Bootstrap screen
	{ TEXTURE_FOLDER"BootstrapScreen.png",	ETexture_BootstrapScreenBackground,		1280,	720	},
	// Menu screen
	{ TEXTURE_FOLDER"MenuScreen.png",		ETexture_MenuScreenBackground,			1280,	720	},
	{ TEXTURE_FOLDER"Play.png",				ETexture_MenuScreenPlayButton,			104,	60	},
	{ TEXTURE_FOLDER"PlayOnClick.png",		ETexture_MenuScreenPlayButtonOnClick,	104,	60	},
	{ TEXTURE_FOLDER"Quit.png",				ETexture_MenuScreenQuitButton,			94,		60	},
	{ TEXTURE_FOLDER"QuitOnClick.png",		ETexture_MenuScreenQuitButtonOnClick,	94,		60	},
	{ TEXTURE_FOLDER"LoadingScreen.png",	ETexture_LoadingScreenBackground,		1280,	720	},
	// Game screen
	{ TEXTURE_FOLDER"GameScreen.png",		ETexture_GameScreenBackground,			1280,	720	},
};

#endif	// _COMMON_CORE_TEXTUREDEFINITIONS_H_