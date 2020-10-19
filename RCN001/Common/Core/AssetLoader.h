/*---------------------------------------------------------------------------*
Project:		RCN
File:			AssetLoader.h
Description:	Performs loading of assets

*---------------------------------------------------------------------------*/

#ifndef _COMMON_CORE_ASSETLOADER_H_
#define _COMMON_CORE_ASSETLOADER_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CAssetLoader
{
public:
						CAssetLoader();
						~CAssetLoader();

	void				BeginLoad(FNJobFunction pfnJobFunction, void* pJobParameters); // Start or append a job to the current list of load functions
	void				EndLoad();

	static void			EndLoadFunc(CRCNJob* pJob, void* pParameters);

	bool				IsJobQueueEmpty();

private:

	CRCNJobQueue*		m_pJobQueue;

	static const int	m_iMaxLoadingJobs	= 2;
	CRCNJob				m_pLoadingJob[m_iMaxLoadingJobs];
	int					m_iJobCount;
};

#endif	// _COMMON_CORE_ASSETLOADER_H_