#ifndef _COMMON_SCREENS_MENUSCREEN_H_
#define _COMMON_SCREENS_MENUSCREEN_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CMenuScreen : public CRCNScreen
{
public:
						CMenuScreen(const char* cScreenName);
	virtual				~CMenuScreen();

	virtual void		Process();
	virtual void		ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState);
	virtual void		Render2D(float fInterpolation);
	virtual void		SetScreenState(EScreenState eScreenState);

	static void			PlayButtonReleaseFunc();
	static void			QuitButtonReleaseFunc();

private:
	
	double				m_dDisplayTime;

	CRCNTexture*		m_pBackground;
	CRCNButton*			m_pPlayButton;
	CRCNButton*			m_pQuitButton;

	CRCNAudioSource*	m_pMusic;

	static const int	m_iFadeInTime	= 1500;
	static const int	m_iFadeOutTime	= 500;
};

#endif	// _COMMON_SCREENS_MENUSCREEN_H_