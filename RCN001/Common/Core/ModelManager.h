/*---------------------------------------------------------------------------*
Project:		RCN
File:			ModelManager.h
Description:    Handles all loading and referencing of models

*---------------------------------------------------------------------------*/

#ifndef _COMMON_CORE_MODELMANAGER_H_
#define _COMMON_CORE_MODELMANAGER_H_

#include "ModelDefinitions.h"

// -----------------------------------------
// CLASS DECLARATIONS

class CModelManager
{
public:
					CModelManager();
					~CModelManager();

	void			LoadModel(EModelID eModelID);
	void			LoadModels(EModelID eFirst, EModelID eLast);

	void			ReleaseModel(EModelID eModelID);

	CRCNModel*		GetModel(EModelID eModelID);

private:

	CRCNModel*		m_pModels[EModelID_Count];
};

#endif	// _COMMON_CORE_MODELMANAGER_H_