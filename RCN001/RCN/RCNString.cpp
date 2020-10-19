/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNString.cpp
Description:    Provides a common string interface

				WARNING: Performs heap allocation and should therefore only be
				used when the size of the string cannot be known at build time
				and it is insufficient to use a large, fixed size char array.

*---------------------------------------------------------------------------*/

// -----------------------------------------
// INCLUDES

#include "stdheader.h"
#include "RCNString.h"

// -----------------------------------------
// CLASS DEFINITIONS

CRCNString::CRCNString()
{
	m_sString = NULL;
}

CRCNString::CRCNString(const char* sString)
{
	m_sString = NULL;
	SetCharacters(sString);
}

CRCNString::CRCNString(const CRCNString& rString)
{
	m_sString = NULL;
	SetCharacters(rString.GetCharacters());
}

CRCNString::~CRCNString()
{
	if(m_sString != NULL)
		RCNDelete [] m_sString;
}

void CRCNString::SetCharacters(const char* sString)
{
	if(m_sString)
		RCNDelete [] m_sString;

	m_iLength = strlen(sString) + 1;
	m_sString = RCNNew char[m_iLength]();
	strncpy_s(m_sString, m_iLength, sString, m_iLength);
}

const char* CRCNString::GetCharacters() const
{
	return m_sString;
}

int CRCNString::GetLength()
{
	return m_iLength;
}

void CRCNString::Render()
{
	CRCNRenderer::Render(this);
}

void CRCNString::SetRenderCoordinates(int iX, int iY)
{
	m_iX = iX;
	m_iY = iY;
}

int CRCNString::GetRenderX()
{
	return m_iX;
}

int CRCNString::GetRenderY()
{
	return m_iY;
}