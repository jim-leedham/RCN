/*---------------------------------------------------------------------------*
Project:		RCN
File:			stdheader.h
Description:    Common includes to avoid large block of includes in every file

*---------------------------------------------------------------------------*/

#ifndef _COMMON_STDHEADER_H_
#define _COMMON_STDHEADER_H_

// NOTE: The order of include statements is important. All RCN classes assume RCNTypes 
// has already been included and do not include it themselves. The order of all
// other RCN include statements is alphabetical, although no RCN class should assume 
// this and should explicitly forward declare or include any other RCN class it needs.
// Game classes may assume all RCN classes have already been included and should
// not need to forward declare or include them.

// RCN (types)
#include "RCNTypes.h"

// RCN (other)
#include "Audio/RCNAudio.h"
#include "Audio/RCNAudioSource.h"
#include "RCN3D.h"
#include "RCNAssert.h"
#include "RCNButton.h"
#include "RCNCamera.h"
#include "RCNCriticalSection.h"
#include "RCNCube.h"
#include "RCNFileSystem.h"
#include "RCNInput.h"
#include "RCNJob.h"
#include "RCNJobQueue.h"
#include "RCNLight.h"
#include "RCNLighting.h"
#include "RCNLinkedList.h"
#include "RCNMaterial.h"
#include "RCNMemUtil.h"
#include "RCNModel.h"
#include "RCNNoise.h"
#include "RCNRenderer.h"
#include "RCNScreen.h"
#include "RCNScreenController.h"
#include "RCNSemaphore.h"
#include "RCNString.h"
#include "RCNTerrain.h"
#include "RCNTerrainBlock.h"
#include "RCNTexture.h"
#include "RCNThread.h"
#include "RCNTimer.h"
#include "RCNUtil.h"

// RCN (third party)
#include "ThirdParty/TinyXML2/tinyxml2.h"

// Core
#include "AppPlatform.h"
#include "Core/Definitions.h"

// Game

#endif	// _COMMON_STDHEADER_H_