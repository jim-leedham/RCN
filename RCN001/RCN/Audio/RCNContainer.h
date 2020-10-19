/*---------------------------------------------------------------------------*
Project:		RCN
File:			RCNContainer.h
Description:    Stores a variety of audio source parameters as read in from XML

*---------------------------------------------------------------------------*/

#ifndef _RCN_RCNCONTAINER_H_
#define _RCN_RCNCONTAINER_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CRCNContainer
{
public:

						CRCNContainer();
						~CRCNContainer();
protected:

	void				LoadXML(tinyxml2::XMLElement* pContainerElement);
	const char*			GetName();

	friend class		CRCNAudioController;

private:

	static const int	ms_iMaximumNameLength = 64;
	char				m_pName[ms_iMaximumNameLength];

	int					m_eFormat;
	bool				m_bStream;
	bool				m_bLoop;
	float				m_fVolume;

	static const int	ms_iMaximumSourceLength = 64;
	char				m_pSource[ms_iMaximumSourceLength];

	friend class		CRCNAudioSource;
	friend class		AUDIOSOURCE;

};

#endif	// _RCN_RCNCONTAINER_H_