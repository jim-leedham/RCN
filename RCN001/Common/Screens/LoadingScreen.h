#ifndef _COMMON_SCREENS_LOADINGSCREEN_H_
#define _COMMON_SCREENS_LOADINGSCREEN_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CLoadingScreen : public CRCNScreen
{
public:
						CLoadingScreen(const char* pScreenName);
	virtual				~CLoadingScreen();

	virtual void		Process();
	virtual void		Render2D(float fInterpolation);
	virtual void		SetScreenState(EScreenState eScreenState);

private:

	double				m_dDisplayTime;
	CRCNTexture*		Background;

	static const int	m_iFadeInTime	= 500;
	static const int	m_iHoldTime		= 1000;
	static const int	m_iFadeOutTime	= 500;
};

#endif	// _COMMON_SCREENS_LOADINGSCREEN_H_