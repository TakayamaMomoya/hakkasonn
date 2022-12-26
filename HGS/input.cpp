//=============================================================================
//
// ���͏��� [input.cpp]
// Author1 : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "input.h"
#include "DirectInput.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "inputmouse.h"

//-----------------------------------------------------------------------------
//�ÓI�����o�ϐ��錾
//-----------------------------------------------------------------------------
CInput *CInput::m_pInput = nullptr;//���̃N���X�̏��

//*************************************************************************************
//�R���X�g���N�^
//*************************************************************************************
CInput::CInput()
{
	m_pKeyboard = nullptr;		//�L�[�{�[�h�̏��
	m_pJoyPad = nullptr;		//�W���C�p�b�h�̏��
	m_pMouse = nullptr;			//�}�E�X�̏��
	m_nOldInputType = INPUT_TYPE_KEYBOARD;
}

//*************************************************************************************
//�f�X�g���N�^
//*************************************************************************************
CInput::~CInput()
{
}

//*************************************************************************************
//������
//*************************************************************************************
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(CDirectInput::Create(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�̐���
	m_pKeyboard = new CInputKeyboard;

	//�L�[�{�[�h�̏���������
	if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�W���C�p�b�h�̐���
	m_pJoyPad = new CInputJoyPad;

	//�W���C�p�b�h�̏���������
	if (FAILED(m_pJoyPad->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�}�E�X�̐���
	m_pMouse = new CInputMouse;

	//�}�E�X�̏���������
	if (FAILED(m_pMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//*************************************************************************************
//�I������
//*************************************************************************************
void CInput::Uninit()
{
	//�L�[�{�[�h�̔j��
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	//�W���C�p�b�h�̔j��
	if (m_pJoyPad != nullptr)
	{
		m_pJoyPad->Uninit();
		delete m_pJoyPad;
		m_pJoyPad = nullptr;
	}

	//�}�E�X�̔j��
	if (m_pMouse != nullptr)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = nullptr;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	CDirectInput::Break();

	//���Ȕj��
	if (m_pInput != nullptr)
	{
		delete m_pInput;
		m_pInput = nullptr;
	}
}

//*************************************************************************************
//�X�V����
//*************************************************************************************
void CInput::Update()
{
	//�L�[�{�[�h�̍X�V
	m_pKeyboard->Update();
	//�W���C�p�b�h�̍X�V
	m_pJoyPad->Update();
	//�}�E�X�̍X�V
	m_pMouse->Update();

	//�Ō�ɐG�����f�o�C�X
	if (m_pJoyPad->GetPressAll())
	{
		m_nOldInputType = INPUT_TYPE_JOYPAD;
	}
	else if (m_pKeyboard->GetKeyboardAllPress())
	{
		m_nOldInputType = INPUT_TYPE_KEYBOARD;
	}
	else if (m_pMouse->GetPressAll())
	{
		m_nOldInputType = INPUT_TYPE_MOUSE;
	}
}

//*************************************************************************************
//�C���v�b�g�̐���
//*************************************************************************************
CInput *CInput::Create()
{
	//Input�̎��Ȑ���
	m_pInput = new CInput;
	return m_pInput;
}

//*************************************************************************************
//�v���X����(����)
//*************************************************************************************
bool CInput::Press(STAN_DART_INPUT_KEY key)
{
	switch (key)
	{
	case KEY_UP:
		if (m_pKeyboard->GetKeyboardPress(DIK_W)
			|| m_pKeyboard->GetKeyboardPress(DIK_UP)
			|| m_pJoyPad->GetPressAll(JOYPAD_UP))
		{
			return true;
		}
		break;
	case KEY_UP_LEFT:
		if ((m_pKeyboard->GetKeyboardPress(DIK_W) && m_pKeyboard->GetKeyboardPress(DIK_A))
			|| (m_pKeyboard->GetKeyboardPress(DIK_UP) && m_pKeyboard->GetKeyboardPress(DIK_LEFT))
			|| m_pJoyPad->GetPressAll(JOYPAD_UP_LEFT))
		{
			return true;
		}
		break;
	case KEY_UP_RIGHT:
		if ((m_pKeyboard->GetKeyboardPress(DIK_W) && m_pKeyboard->GetKeyboardPress(DIK_D))
			|| (m_pKeyboard->GetKeyboardPress(DIK_UP) && m_pKeyboard->GetKeyboardPress(DIK_RIGHT))
			|| m_pJoyPad->GetPressAll(JOYPAD_UP_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DOWN:
		if (m_pKeyboard->GetKeyboardPress(DIK_S)
			|| m_pKeyboard->GetKeyboardPress(DIK_DOWN)
			|| m_pJoyPad->GetPressAll(JOYPAD_DOWN))
		{
			return true;
		}
		break;
	case KEY_DOWN_LEFT:
		if ((m_pKeyboard->GetKeyboardPress(DIK_S) && m_pKeyboard->GetKeyboardPress(DIK_A))
			|| (m_pKeyboard->GetKeyboardPress(DIK_DOWN) && m_pKeyboard->GetKeyboardPress(DIK_LEFT))
			|| m_pJoyPad->GetPressAll(JOYPAD_DOWN_LEFT))
		{
			return true;
		}
		break;
	case KEY_DOWN_RIGHT:
		if ((m_pKeyboard->GetKeyboardPress(DIK_S) && m_pKeyboard->GetKeyboardPress(DIK_D))
			|| (m_pKeyboard->GetKeyboardPress(DIK_DOWN) && m_pKeyboard->GetKeyboardPress(DIK_RIGHT))
			|| m_pJoyPad->GetPressAll(JOYPAD_DOWN_RIGHT))
		{
			return true;
		}
		break;
	case KEY_LEFT:
		if (m_pKeyboard->GetKeyboardPress(DIK_A)
			|| m_pKeyboard->GetKeyboardPress(DIK_LEFT)
			|| m_pJoyPad->GetPressAll(JOYPAD_LEFT))
		{
			return true;
		}
		break;
	case KEY_RIGHT:
		if (m_pKeyboard->GetKeyboardPress(DIK_D)
			|| m_pKeyboard->GetKeyboardPress(DIK_RIGHT)
			|| m_pJoyPad->GetPressAll(JOYPAD_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DECISION:
		if (m_pKeyboard->GetKeyboardPress(DIK_RETURN)
			|| m_pJoyPad->GetPressAll(JOYPAD_A))
		{
			return true;
		}
		break;
	case KEY_SHOT:
		if (m_pKeyboard->GetKeyboardPress(DIK_SPACE)
			|| m_pJoyPad->GetPressAll(JOYPAD_R1))
		{
			return true;
		}
		break;
	case KEY_BACK:
		if (m_pKeyboard->GetKeyboardPress(DIK_BACKSPACE)
			|| m_pKeyboard->GetKeyboardPress(DIK_B)
			|| m_pJoyPad->GetPressAll(JOYPAD_BACK)
			|| m_pJoyPad->GetPressAll(JOYPAD_B))
		{
			return true;
		}
		break;
	case KEY_SHIFT:
		if (m_pKeyboard->GetKeyboardPress(DIK_RSHIFT)
			|| m_pKeyboard->GetKeyboardPress(DIK_LSHIFT)
			|| m_pJoyPad->GetPressAll(JOYPAD_L1))
		{
			return true;
		}
		break;
	case KEY_MOVE:
		if (Press(KEY_UP) || Press(KEY_DOWN) || Press(KEY_LEFT) || Press(KEY_RIGHT))
		{
			return true;
		}
		break;
	case KEY_PAUSE:
		if (m_pKeyboard->GetKeyboardPress(DIK_P)
			|| m_pJoyPad->GetPressAll(JOYPAD_START))
		{
			return true;
		}
		break;
	case KEY_ALL:
		if (m_pKeyboard->GetKeyboardAllPress()
			|| m_pJoyPad->GetPressAll()
			|| m_pMouse->GetPressAll())
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

//*************************************************************************************
//�g���K�[����(����)
//*************************************************************************************
bool CInput::Trigger(STAN_DART_INPUT_KEY key)
{
	switch (key)
	{
	case KEY_UP:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_W)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_UP)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_UP))
		{
			return true;
		}
		break;
	case KEY_UP_LEFT:
		if ((m_pKeyboard->GetKeyboardTrigger(DIK_W) && m_pKeyboard->GetKeyboardTrigger(DIK_A))
			|| (m_pKeyboard->GetKeyboardTrigger(DIK_UP) && m_pKeyboard->GetKeyboardTrigger(DIK_LEFT))
			|| m_pJoyPad->GetTriggerAll(JOYPAD_UP_LEFT))
		{
			return true;
		}
		break;
	case KEY_UP_RIGHT:
		if ((m_pKeyboard->GetKeyboardTrigger(DIK_W) && m_pKeyboard->GetKeyboardTrigger(DIK_D))
			|| (m_pKeyboard->GetKeyboardTrigger(DIK_UP) && m_pKeyboard->GetKeyboardTrigger(DIK_RIGHT))
			|| m_pJoyPad->GetTriggerAll(JOYPAD_UP_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DOWN:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_S)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_DOWN)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_DOWN))
		{
			return true;
		}
		break;
	case KEY_DOWN_LEFT:
		if ((m_pKeyboard->GetKeyboardTrigger(DIK_S) && m_pKeyboard->GetKeyboardTrigger(DIK_A))
			|| (m_pKeyboard->GetKeyboardTrigger(DIK_DOWN) && m_pKeyboard->GetKeyboardTrigger(DIK_LEFT))
			|| m_pJoyPad->GetTriggerAll(JOYPAD_DOWN_LEFT))
		{
			return true;
		}
		break;
	case KEY_DOWN_RIGHT:
		if ((m_pKeyboard->GetKeyboardTrigger(DIK_S) && m_pKeyboard->GetKeyboardTrigger(DIK_D))
			|| (m_pKeyboard->GetKeyboardTrigger(DIK_DOWN) && m_pKeyboard->GetKeyboardTrigger(DIK_RIGHT))
			|| m_pJoyPad->GetTriggerAll(JOYPAD_DOWN_RIGHT))
		{
			return true;
		}
		break;
	case KEY_LEFT:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_A)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_LEFT)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_LEFT))
		{
			return true;
		}
		break;
	case KEY_RIGHT:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_D)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_RIGHT)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_RIGHT))
		{
			return true;
		}
		break;
	case KEY_DECISION:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_RETURN)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_A))
		{
			return true;
		}
		break;
	case KEY_SHOT:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_SPACE)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_R1))
		{
			return true;
		}
		break;
	case KEY_BACK:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_BACKSPACE)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_B)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_BACK)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_B))
		{
			return true;
		}
		break;
	case KEY_SHIFT:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_RSHIFT)
			|| m_pKeyboard->GetKeyboardTrigger(DIK_LSHIFT)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_A))
		{
			return true;
		}
		break;
	case KEY_MOVE:
		if (Trigger(KEY_UP) || Trigger(KEY_DOWN) || Trigger(KEY_LEFT) || Trigger(KEY_RIGHT))
		{
			return true;
		}
		break;
	case KEY_PAUSE:
		if (m_pKeyboard->GetKeyboardTrigger(DIK_P)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_START)
			|| m_pJoyPad->GetTriggerAll(JOYPAD_HOME))
		{
			return true;
		}
		break;
	case KEY_ALL:
		if (m_pKeyboard->GetKeyboardAllTrigger()
			|| m_pJoyPad->GetTriggerAll()
			|| m_pMouse->GetTriggerAll())
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

//*************************************************************************************
//�v���X����(�L�[�{�[�h)
//*************************************************************************************
bool CInput::Press(int nKey)
{
	return m_pKeyboard->GetKeyboardPress(nKey);
}

//*************************************************************************************
//�g���K�[����(�L�[�{�[�h)
//*************************************************************************************
bool CInput::Trigger(int nkey)
{
	return m_pKeyboard->GetKeyboardTrigger(nkey);
}

//*************************************************************************************
//�����[�X����(�L�[�{�[�h)
//*************************************************************************************
bool CInput::Release(int nkey)
{
	return m_pKeyboard->GetKeyboardRelease(nkey);
}

//*************************************************************************************
//�v���X����(�W���C�p�b�h)
//*************************************************************************************
bool CInput::Press(DirectJoypad key, int nNum)
{
	return m_pJoyPad->GetPress(key, nNum);
}

//*************************************************************************************
//�g���K�[����(�W���C�p�b�h)
//*************************************************************************************
bool CInput::Trigger(DirectJoypad key, int nNum)
{
	return m_pJoyPad->GetTrigger(key, nNum);
}

//*************************************************************************************
//�v���X����(�}�E�X)
//*************************************************************************************
bool CInput::Press(MOUSE_KEY Key)
{
	return m_pMouse->GetPress(Key);
}

//*************************************************************************************
//�g���K�[����(�}�E�X)
//*************************************************************************************
bool CInput::Trigger(MOUSE_KEY Key)
{
	return m_pMouse->GetTrigger(Key);
}

//*************************************************************************************
//�����[�X����(�}�E�X)
//*************************************************************************************
bool CInput::Release(MOUSE_KEY Key)
{
	return m_pMouse->GetRelease(Key);
}

//*************************************************************************************
//�\���L�[�̃x�N�g��
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveKey(int nNum)
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	if (m_pJoyPad->GetJoyPadNumMax() <= nNum)
	{
		return VectorMove;
	}

	int nCheck = m_pJoyPad->GetCross(nNum);

	if (nCheck > 360 || nCheck < 0)
	{//�\���L�[�̊p�x���w��O��������O��Ԃ�
		return VectorMove;
	}

	float fRot = D3DXToRadian(nCheck);

	VectorMove.x = sinf(fRot);
	VectorMove.y = -cosf(fRot);

	//�����P�̃x�N�g���ϊ�
	D3DXVec3Normalize(&VectorMove, &VectorMove);

	return VectorMove;
}

D3DXVECTOR3 CInput::VectorMoveKeyAll()
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nJoyPad = m_pJoyPad->GetJoyPadNumMax();
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	for (int nCnt = 0; nCnt < nJoyPad; nCnt++)
	{
		int nCheck = m_pJoyPad->GetCross(nCnt);

		if (nCheck > 360 || nCheck < 0)
		{//�\���L�[�̊p�x���w��O��������O��Ԃ�
			return VectorMove;
		}

		float fRot = D3DXToRadian(nCheck);

		VectorMove.x = sinf(fRot);
		VectorMove.y = -cosf(fRot);

		//�����P�̃x�N�g���ϊ�
		D3DXVec3Normalize(&VectorMove, &VectorMove);

		if (VectorMove != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			return VectorMove;
		}

	}
	return VectorMove;
}

//*************************************************************************************
//�W���C�X�e�B�b�N�̃x�N�g��
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveJoyStick(int nNum, bool bleftandright)
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	VectorMove = m_pJoyPad->GetJoyStickData(nNum, bleftandright);
	//�����P�̃x�N�g���ϊ�
	D3DXVec3Normalize(&VectorMove, &VectorMove);
	return VectorMove;
}

//*************************************************************************************
//�W���C�X�e�B�b�N�̃x�N�g��
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveJoyStickAll(bool bleftandright)
{
	D3DXVECTOR3 VectorNull = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		VectorMove = m_pJoyPad->GetJoyStickData(nCnt, bleftandright);
		//�����P�̃x�N�g���ϊ�
		D3DXVec3Normalize(&VectorMove, &VectorMove);
		if (VectorNull != VectorMove)
		{
			return  VectorMove;
		}
	}

	return VectorNull;
}

//*************************************************************************************
// �}�E�X�J�[�\���̃X�N���[�����W�̎擾
//*************************************************************************************
D3DXVECTOR3 CInput::GetMouseCursor(void)
{
	return m_pMouse->GetMouseCursor();
}

//*************************************************************************************
// �}�E�X�̃z�C�[���̓�������
//*************************************************************************************
int CInput::GetMouseWheel(void)
{
	return m_pMouse->GetMouseWheel();
}

//*************************************************************************************
// �}�E�X�̈ړ��ʂ��o�͏���
//*************************************************************************************
D3DXVECTOR3 CInput::GetMouseMove(void)
{
	return m_pMouse->GetMouseMove();
}
