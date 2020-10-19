#ifndef _COMMON_SCREENS_BOOTSTRAPSCREEN_H_
#define _COMMON_SCREENS_BOOTSTRAPSCREEN_H_

// -----------------------------------------
// CLASS DECLARATIONS

class CBootstrapScreen : public CRCNScreen
{
public:
						CBootstrapScreen(const char* pScreenName);
	virtual				~CBootstrapScreen();

	virtual void		Process();
	virtual void		Render2D(float fInterpolation);

	virtual void		SetScreenState(EScreenState eScreenState);

private:

	double				m_dDisplayTime;
	CRCNTexture*		m_pBackground;

	static const int	m_iFadeInTime	= 1500;
	static const int	m_iHoldTime		= 1000;
	static const int	m_iFadeOutTime	= 1500;
};

#endif	// _COMMON_SCREENS_BOOTSTRAPSCREEN_H_