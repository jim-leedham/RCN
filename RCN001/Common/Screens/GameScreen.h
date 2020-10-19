#ifndef _COMMON_SCREENS_GAMESCREEN_H_
#define _COMMON_SCREENS_GAMESCREEN_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CGameScreen : public CRCNScreen
{
public:
					CGameScreen(const char* cScreenName);
	virtual			~CGameScreen();

	virtual void	Process();
	virtual void	ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState);
	virtual void	Render2D(float fInterpolation);

	static void		QuitButtonReleaseFunc();

private:

	CRCNString		m_sString1;
	CRCNString		m_sString2;

	CRCNTexture*	m_pBackground;

	CRCNButton*		m_pQuitButton;

	CRCNAudioSource*	m_pMusic;
};

#endif	// _COMMON_SCREENS_GAMESCREEN_H_