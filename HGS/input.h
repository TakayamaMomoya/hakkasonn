//=============================================================================
//
// ���͏��� [input.h]
// Author : KOZUNA HIROHITO
// 
//=============================================================================

#ifndef _INPUT_H_		//���̃}�N����`������Ȃ�������
#define _INPUT_H_		//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "inputkeydata.h"
#include "DirectInput.h"

//----------------------------------------------------------------------------
//�O���錾
//----------------------------------------------------------------------------
class CInputKeyboard;
class CInputJoyPad;
class CInputMouse;

//----------------------------------------------------------------------------
//�N���X��`
//----------------------------------------------------------------------------
class CInput
{
public:

	CInput();
	~CInput();

	//*�A�v���P�[�V���������ɏ������
	static CInput *Create();							//���͏����n�̃N���G�C�g�AInit�̑O�ɏ���
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		//���͏����S���̏�����
	void Uninit();										//���͏����S���̏I������
	void Update();										//���͏����S���̍X�V����
														//*

														//*�C���v�b�g���K�v�Ȏ��Ăяo��
	static CInput *GetKey() { return m_pInput; }		//�v���C��g���K�[�Ȃǂ̃A�h���X�̎擾

	bool Press(STAN_DART_INPUT_KEY key);				//�����v���X
	bool Trigger(STAN_DART_INPUT_KEY key);				//�����g���K�[
	bool Press(int nKey);								//�L�[�{�[�h�v���X
	bool Trigger(int nkey);								//�L�[�{�[�h�g���K�[
	bool Release(int nkey);								//�L�[�{�[�h�����[�X
	bool Press(DirectJoypad key, int nNum = 0);			//�W���C�p�b�g�v���X
	bool Trigger(DirectJoypad key, int nNum = 0);		//�W���C�p�b�g�g���K�[
	bool Press(MOUSE_KEY key);							//�}�E�X�v���X
	bool Trigger(MOUSE_KEY key);						//�}�E�X�g���K�[
	bool Release(MOUSE_KEY key);						//�}�E�X�����[�X

	D3DXVECTOR3 VectorMoveKey(int nNum);								//�\���L�[���̃x�N�g���擾
	D3DXVECTOR3 VectorMoveKeyAll();								//�\���L�[���̃x�N�g���擾
	D3DXVECTOR3 VectorMoveJoyStick(int nNum = 0, bool bleftandright = false); //�W���C�X�e�B�b�N�̃x�N�g���擾
	D3DXVECTOR3 VectorMoveJoyStickAll(bool bleftandright = false); //�W���C�X�e�B�b�N�̃x�N�g���擾

	D3DXVECTOR3 GetMouseCursor(void);		// �}�E�X�J�[�\���̃X�N���[�����W�̎擾
	int GetMouseWheel(void);				// �}�E�X�̃z�C�[���̓�������
	D3DXVECTOR3 GetMouseMove(void);			// �}�E�X�̈ړ��ʂ��o�͏���

	InputType GetOldInputType() { return m_nOldInputType; }		//�Ō�ɐG�����f�o�C�X
																//*

private:

	CInputKeyboard *m_pKeyboard;	//�L�[�{�[�h�̏��
	CInputJoyPad *m_pJoyPad;		//�W���C�p�b�h�̏��
	CInputMouse *m_pMouse;			//�}�E�X�̏��
	static CInput *m_pInput;		//���̃N���X�̏��
	InputType m_nOldInputType;		//�Ō�ɐG�����f�o�C�X
};

#endif

