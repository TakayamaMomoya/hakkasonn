//=============================================================================
//
// manager.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _MANAGER_H_			//���̃}�N����`������Ȃ�������
#define _MANAGER_H_			//2�d�C���N���[�h�h�~�̃}�N����`
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CInput;
class CObject;
class CRenderer;
class CFade;

//*****************************************************************************
// �񋓌^��`
//*****************************************************************************

enum MODE_TYPE
{
	TYPE_TITLE = 0,
	TYPE_GAME,
	TYPE_RESULT,
	TYPE_MAX
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CManager
{

public:
	

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance,HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw(int nFPS);
	LPDIRECT3DDEVICE9 GetDeviceManager();
	CObject *GetGameObject() { return m_pGame; }
	CFade *GetFadeObject() { return m_pFade; }
	void SetGemeObject(CObject *Object) {m_pGame = Object;}
	void ChangeMode(MODE_TYPE type);
	void NextMode(MODE_TYPE type, int nFadeCnt = 0);
	void SetScore(int nScore) { m_nScore = nScore; }
	int GetScore() { return m_nScore; }
	bool IsChangingMode() { return m_bChange; }
	int GetAirframe() { return m_nAirframe; }
	void SetAirframe(int nAirframe) { m_nAirframe = nAirframe; }

private:
	MODE_TYPE m_Mode;
	bool m_bChange;
	CRenderer *m_pRenderer;
	CObject *m_pGame;
	CInput *m_pInput;
	CFade *m_pFade;
	int m_nScore;
	int m_nFadeCnt;
	int m_nAirframe;
};
#endif

