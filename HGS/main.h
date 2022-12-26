//=============================================================================
//
// main.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _MAIN_H_			//���̃}�N����`������Ȃ�������
#define _MAIN_H_			//2�d�C���N���[�h�h�~�̃}�N����`


//*****************************************************************************
// ���C�u�����[�����N
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include <tchar.h> // _T
#include <d3dx9.h>

//*****************************************************************************
// �萔��`
//*****************************************************************************
namespace
{
	// �E�C���h�E�̃N���X��
	LPCTSTR CLASS_NAME = _T("AppClass");
	// �E�C���h�E�̃L���v�V������
	LPCTSTR WINDOW_NAME = _T("materialcode");
}

// �X�N���[���̕�
const int SCREEN_WIDTH = 1600;
// �X�N���[���̍���
const int SCREEN_HEIGHT = 900;

// �X�N���[���̕�
const int SCREEN_GAME_WIDTH = 720;
// �X�N���[���̍���
const int SCREEN_GAME_HEIGHT = 840;

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ���_�f�[�^
struct VERTEX_2D
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;		//�e�N�X�`�����W
};

// ���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;//���_���W
	D3DXVECTOR3 nor;//�@���x�N�g��
	D3DCOLOR col;//���_�J���[
	D3DXVECTOR2 tex;		//�e�N�X�`�����W
}VERTEX_3D;

class CRenderer;
class CManager;

CManager *GetManager();

#endif