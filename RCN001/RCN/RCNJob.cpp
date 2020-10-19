/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNJob.cpp
Description:    A common job interface

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNJob.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNJob::CRCNJob()
{
	m_pfnJobFunction = NULL;
	m_pJobParameters = NULL;
}

CRCNJob::CRCNJob(FNJobFunction pfnJobFunction, void* pJobParameters)
{
	SetJobFunction(pfnJobFunction, pJobParameters);
}

CRCNJob::~CRCNJob()
{
}

void CRCNJob::SetJobFunction(FNJobFunction pfnJobFunction, void* pJobParameters)
{
	RCNAssertStr(pfnJobFunction, "Job function cannot be null");
	m_pfnJobFunction = pfnJobFunction;
	m_pJobParameters = pJobParameters;
}