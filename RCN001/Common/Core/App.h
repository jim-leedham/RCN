/*---------------------------------------------------------------------------*
Project:		RCN
File:			App.h
Description:    Handles the main program flow

*---------------------------------------------------------------------------*/

#ifndef _COMMON_CORE_APP_H_
#define _COMMON_CORE_APP_H_

// -----------------------------------------
// FORWARD DECLARATIONS

class CGame;
class CAssetLoader;
class CTextureManager;
class CModelManager;

// -----------------------------------------
// CLASS DECLARATIONS

class CApp
{
public:

	typedef enum EAppState
	{
		EAppState_Undefined = 0,
		EAppState_InitialisationComplete,
		EAppState_BootstrapAssetLoadingComplete,
		EAppState_MainMenuAssetLoadingComplete,
		EAppState_GameLoadingComplete,
		EAppState_Count
	};

					CApp();
	virtual			~CApp();

	virtual void 				Start();
	virtual void				MainLoop();
	virtual void				ProcessInput(CRCNInput::EInput eInput, CRCNInput::EInputState eInputState);
	virtual void 				End();
	
	virtual bool 				PlatformMainLoop() = 0;

	inline CGame*				GetGame()			const		{ return m_pGame;			}
	inline EAppState			GetAppState()		const		{ return m_eAppState;		}
	inline CTextureManager*		GetTextureManager()	const		{ return m_pTextureManager; }
	inline CModelManager*		GetModelManager()	const		{ return m_pModelManager;	}
	inline double				GetTimeStep()		const		{ return m_dTimeStep;		}
	inline CAssetLoader*		GetAssetLoader()	const		{ return m_pAssetLoader;	}

	void						SetAppState(EAppState eAppState) { m_eAppState = eAppState; }

	void						Quit();

	static void					MainMenuLoadingFunc(CRCNJob* pJob, void* pParameters);
	static void					GameLoadingFunc(CRCNJob* pJob, void* pParameters);

	void						OnExitSizeMove();

	static const char*			m_sBootstrapScreenName;
	static const char*			m_sMenuScreenName;
	static const char*			m_sLoadingScreenName;
	static const char*			m_sGameScreenName;

private:

	static CRCNScreen*			ScreenInstantiationCallback(const char* pScreenName);

	void						Process();
	void						Render(float fInterpolation);

	CGame*						m_pGame;
	bool						m_bRunning;
	EAppState					m_eAppState;

	CAssetLoader*				m_pAssetLoader;
	CTextureManager*			m_pTextureManager;
	CModelManager*				m_pModelManager;

	double						m_dTimeStep;
	double						m_dNextGameTick;
	double						m_dPreviousTick;
};	

extern CApp *g_pApplication;

#endif	// _COMMON_CORE_APP_H_