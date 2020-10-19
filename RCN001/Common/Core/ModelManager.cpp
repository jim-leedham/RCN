/*---------------------------------------------------------------------------*
Project:		RCN
File:			ModelManager.cpp
Description:    Handles all loading and referencing of models

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "ModelManager.h"

// -----------------------------------------
// CLASS DEFINITIONS

CModelManager::CModelManager()
{
	for(int i = 0; i < EModelID_Count; i++)
		m_pModels[i] = NULL;
}

CModelManager::~CModelManager()
{
	for(int i = 0; i < EModelID_Count; i++)
	{
		if(m_pModels[i] != NULL)
			RCNDelete m_pModels[i];
	}
}

void CModelManager::LoadModel(EModelID eModelID)
{
	RCNAssertStr(EModelID_None < eModelID && eModelID < EModelID_Count, "Specified model out of range");
	m_pModels[eModelID] = RCNNew CRCNModel();
	m_pModels[eModelID]->Load(g_tModels[eModelID].pModelDirectory, g_tModels[eModelID].pFilename, g_tModels[eModelID].eModelType);
}

void CModelManager::LoadModels(EModelID eFirst, EModelID eLast)
{
	RCNAssertStr(EModelID_None < eFirst && eLast < EModelID_Count, "Specified model out of range");

	for(int i = eFirst; i <= eLast; i++)
	{
		m_pModels[i] = RCNNew CRCNModel();
		m_pModels[i]->Load(g_tModels[i].pModelDirectory, g_tModels[i].pFilename, g_tModels[i].eModelType);
	}
}

void CModelManager::ReleaseModel(EModelID eModelID)
{
	RCNAssertStr(EModelID_None < eModelID && eModelID < EModelID_Count, "Specified model out of range");
	RCNAssertStr(m_pModels[eModelID] != NULL, "Model not loaded");

	RCNDelete m_pModels[eModelID];
	m_pModels[eModelID] = NULL;
}

CRCNModel* CModelManager::GetModel(EModelID eModelID)
{
	RCNAssertStr(EModelID_None < eModelID && eModelID < EModelID_Count, "Specified model out of range");
	RCNAssertStr(m_pModels[eModelID] != NULL, "Model not loaded");

	return m_pModels[eModelID];
}
