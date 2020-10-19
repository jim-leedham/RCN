/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNContainer.h
Description:    Stores a variety of audio source parameters as read in from XML

*---------------------------------------------------------------------------*/

#include "stdheader.h"
#include "RCNContainer.h"
#include "RCNAudioSource.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNContainer::CRCNContainer()
{
	
}

CRCNContainer::~CRCNContainer()
{
	
}

void CRCNContainer::LoadXML(tinyxml2::XMLElement* pContainerElement)
{
	tinyxml2::XMLElement* pInnerElement;

	pInnerElement = pContainerElement->FirstChildElement("Name");
	RCNAssertStr(pInnerElement, "Container element fails to include a Name");
	RCNAssertStr(strlen(pInnerElement->GetText()) < ms_iMaximumNameLength, "Name element exceeds allowed length");
	strcpy_s(m_pName, pInnerElement->GetText());

	m_eFormat = CRCNAudioSource::EFormat::EFormat_MP3;	// Currently the only supported format

	pInnerElement = pContainerElement->FirstChildElement("Stream");
	m_bStream = true;
	if(pInnerElement->GetText())
	{
		if(stricmp(pInnerElement->GetText(), "false") == 0)
			m_bStream = false;
	}

	pInnerElement = pContainerElement->FirstChildElement("Loop");
	m_bLoop = false;
	if(pInnerElement->GetText())
	{
		if(stricmp(pInnerElement->GetText(), "true") == 0)
			m_bLoop = true;
	}

	pInnerElement = pContainerElement->FirstChildElement("Volume");
	m_fVolume = 1.0f;
	if(pInnerElement->GetText())
	{
		m_fVolume = (float)atof(pInnerElement->GetText());
	}

	pInnerElement = pContainerElement->FirstChildElement("Source");
	RCNAssertStr(pInnerElement, "Container element fails to include a Source");
	RCNAssertStr(strlen(pInnerElement->GetText()) < ms_iMaximumSourceLength, "Source element exceeds allowed length");
	strcpy_s(m_pSource, pInnerElement->GetText());
}

const char* CRCNContainer::GetName()
{
	return m_pName;
}

