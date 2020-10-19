/*---------------------------------------------------------------------------*
Project:		RCN
File:			ModelDefinitions.h
Description:    Global definitions and enumerations relating to models

*---------------------------------------------------------------------------*/

#ifndef _COMMON_CORE_MODELDEFINITIONS_H_
#define _COMMON_CORE_MODELDEFINITIONS_H_

// NOTE: Ideally we would auto-generate this file. Perhaps if our assets grow
// significantly it may become worthwhile. For now it seems over-engineering.

enum EModelID
{
	EModelID_None = -1,
	EModelID_Plane,
	EModelID_Count
};

struct TModelData
{
	const char*			pModelDirectory;
	const char*			pFilename;
	EModelID			eModelID;
	CRCNModel::EType	eModelType;

};

static TModelData g_tModels[EModelID_Count] =
{
	{ MODEL_FOLDER, "plane.x", EModelID_Plane, CRCNModel::EType_TexturedMesh	},
};

#endif	// _COMMON_CORE_MODELDEFINITIONS_H_