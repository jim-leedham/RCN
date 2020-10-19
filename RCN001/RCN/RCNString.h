/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNString.h
Description:    Provides a common string interface

				WARNING: Performs heap allocation and should therefore only be
				used when the size of the string cannot be known at build time
				and it is insufficient to use a large, fixed size char array.

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNSTRING_H_
#define _RCN_RCNSTRING_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNString
{
public:
							CRCNString();
							CRCNString(const char* sString);
							CRCNString(const CRCNString& rString);
	virtual					~CRCNString();

	void					SetCharacters(const char* sString);
	const char*				GetCharacters() const;

	int						GetLength();

	void					Render();

	void					SetRenderCoordinates(int iX, int iY);
	int						GetRenderX();
	int						GetRenderY();

private:

	char*					m_sString;					
	int						m_iLength;

	int						m_iX;
	int						m_iY;
};

#endif	// _RCN_RCNSTRING_H_