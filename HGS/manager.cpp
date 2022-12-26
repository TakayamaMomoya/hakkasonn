//=============================================================================
//
// manager.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "input.h"
#include "texture.h"
#include "fade.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CManager::CManager()
{
	m_pGame = nullptr;
	m_pInput = nullptr;
	m_pRenderer = nullptr;
	m_pFade = nullptr;
	m_nScore = 0;
	m_nFadeCnt = 0;
	m_nAirframe = 0;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CManager::~CManager()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, bool bWindow)
{
	m_bChange = false;
	m_Mode = TYPE_GAME;
	CTexture::Create();

	m_pInput = CInput::Create();
	//���͏����̏���������
	if (FAILED(m_pInput->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	m_pRenderer = new CRenderer;

	// ����������
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return -1;
	}

	m_pGame = new CGame;
	if (FAILED(m_pGame->Init()))
	{
		return -1;
	}

	m_pFade = new CFade;
	if (FAILED(m_pFade->Init()))
	{
		return -1;
	}

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CManager::Uninit()
{
	//���͏����̏I������
	m_pInput->Uninit();

	CTexture::Uninit();

	if (m_pGame != nullptr)
	{
		// �I������
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
	}

	if (m_pRenderer != nullptr)
	{
		// �I������
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pFade != nullptr)
	{
		// �I������
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CManager::Update()
{
	m_nFadeCnt--;
	if (m_bChange && m_nFadeCnt < 0)
	{
		m_nFadeCnt = 0;
		m_pFade->FadeOut();
		if (m_pFade->CurrentFadeState())
		{
			m_bChange = false;
			ChangeMode(m_Mode);
		}
	}

	if (m_pInput != nullptr && !m_pFade->CurrentFadeState())
	{
		//���͏����̍X�V����
		m_pInput->Update();
	}

	if (m_pGame != nullptr)
	{
		m_pGame->Update();
	}

	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update();
	}

	if (m_pFade != nullptr)
	{
		m_pFade->Update();
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CManager::Draw(int nFPS)
{
	m_pRenderer->Draw(nFPS);
}

LPDIRECT3DDEVICE9 CManager::GetDeviceManager()
{
	return m_pRenderer->GetDeviceRenderer();
}

//*****************************************************************************
// ���[�h�̕ύX
//*****************************************************************************
void CManager::ChangeMode(MODE_TYPE type)
{
	if (m_pGame != nullptr)
	{// �I������
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
		CTexture::Uninit();
	}

	switch (type)
	{
	case TYPE_TITLE:
		m_pGame = new CTitle;
		break;
	case TYPE_GAME:
		m_pGame = new CGame;
		break;
	case TYPE_RESULT:
		m_pGame = new CResult;
		break;
	case TYPE_MAX:
		break;
	default:
		break;
	}

	CTexture::Create();
	if (FAILED(m_pGame->Init()))
	{
		return;
	}
}

//*****************************************************************************
// ���̃��[�h�̐ݒ�
//*****************************************************************************
void CManager::NextMode(MODE_TYPE type,int nFadeCnt)
{
	m_nFadeCnt = nFadeCnt;
	m_bChange = true;
	m_Mode = type;
}