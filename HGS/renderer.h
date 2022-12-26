//=============================================================================
//
// renderer.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _RENDERER_H_			//���̃}�N����`������Ȃ�������
#define _RENDERER_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw(const int nFps);
	LPDIRECT3DDEVICE9 GetDeviceRenderer() { return m_pD3DDevice; }

#ifdef _DEBUG
	void DrawFPS(const int nFps);
#endif // _DEBUG
private:

	// Direct3D�I�u�W�F�N�g
	LPDIRECT3D9 m_pD3D = nullptr;
	// Device�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice = nullptr;

#ifdef _DEBUG
	// �t�H���g
	LPD3DXFONT m_pFont = nullptr;
#endif // _DEBUG

};
#endif