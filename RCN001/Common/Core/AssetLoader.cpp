/*---------------------------------------------------------------------------*
Project:		RCN
File:			AssetLoader.cpp
Description:	Performs loading of assets

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "AssetLoader.h"

// -----------------------------------------
// CLASS DEFINITIONS

CAssetLoader::CAssetLoader()
{
	m_iJobCount = 0;
	m_pJobQueue = RCNNew CRCNJobQueue();
}

CAssetLoader::~CAssetLoader()
{
	RCNDelete m_pJobQueue;
}

void CAssetLoader::BeginLoad(FNJobFunction pfnJobFunction, void* pJobParameters)
{
	m_pLoadingJob[m_iJobCount].SetJobFunction(pfnJobFunction, pJobParameters);
	m_pJobQueue->InsertJob(&m_pLoadingJob[m_iJobCount]);
	m_iJobCount++;

	m_pLoadingJob[m_iJobCount].SetJobFunction(EndLoadFunc, NULL);
	m_pJobQueue->InsertJob(&m_pLoadingJob[m_iJobCount]);
	m_iJobCount++;
}

void CAssetLoader::EndLoadFunc(CRCNJob* pJob, void* pParameters)
{
	g_pApplication->GetAssetLoader()->EndLoad();
}

void CAssetLoader::EndLoad()
{
	m_iJobCount = 0;
}

bool CAssetLoader::IsJobQueueEmpty()
{
	return m_iJobCount == 0;
}