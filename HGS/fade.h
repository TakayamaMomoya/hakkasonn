//=============================================================================
//
// fade.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _FADE_H_			//���̃}�N����`������Ȃ�������
#define _FADE_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class C2DPolygon;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CFade : public CObject
{
private:
	
public:
	CFade();
	~CFade() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void FadeOutIn();
	void FadeOut() { m_bFadeUse = true; }
	bool CurrentFadeState() { return m_bFadeEnd; }

private:
	C2DPolygon *m_pFade;
	D3DXCOLOR m_FadeColor;
	bool m_bFadeUse;
	bool m_bFadeEnd;
};

#endif