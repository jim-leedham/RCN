/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNJob.h
Description:    A common job interface

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNJOB_H_
#define _RCN_RCNJOB_H_

// -----------------------------------------
// FORWARD DECLARATIONS

class CRCNJob;

typedef void (*FNJobFunction)(CRCNJob* pJob, void* pJobParameters);

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNJob
{
public:

	CRCNJob();
	CRCNJob(FNJobFunction pfnJobFunction, void* pJobParameters);
	virtual ~CRCNJob();
	
	void SetJobFunction(FNJobFunction pfnJobFunction, void* pJobParameters);

private:

	FNJobFunction	m_pfnJobFunction;
	void*			m_pJobParameters;

	friend class CRCNJobQueue;
};

#endif	// _RCN_RCNJOB_H_