//=============================================================================
//
// game.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include "result.h"
#include "ui.h"
#include "texture.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "ranking.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CResult::CResult()
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CResult::~CResult()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CResult::Init()
{
	InitRanking();
	//nullptr
	m_pUi = nullptr;

	//GetUiData
	m_pUi = new CUI;

	//UiInit
	if (FAILED(m_pUi->Init()))
	{
		return -1;
	}

	//GetUiPointer
	C2DPolygon *pPolygon = m_pUi->CreateUi(1);

	//SetUiData
	int nIndex = CTexture::LoadTexture("data/TEXTURE/���U���g.png");
	pPolygon[0].SetTextIndex(nIndex);
	pPolygon[0].SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	pPolygon[0].SetDiagonalLine(200.0f, 200.0f);
	pPolygon[0].SetPolygon();

	CManager::GetSound()->Play(CSound::SOUND_BGM_RESULT);

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CResult::Uninit()
{
	CManager::GetSound()->Stop(CSound::SOUND_BGM_RESULT);
	UninitRanking();
	//UiRelease
	if (m_pUi != nullptr)
	{
		//UiUninit
		m_pUi->Uninit();
		delete m_pUi;
		m_pUi = nullptr;
	}
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CResult::Update()
{
	//UiUpdate
	m_pUi->Update();

	//GetInput
	CInput *pInput = CInput::GetKey();

	UpdateRanking();

	//EndResult
	if (pInput->Trigger(KEY_DECISION))
	{
		CManager * pManager = GetManager();
		pManager->NextMode(TYPE_TITLE);
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CResult::Draw()
{
	//UiDraw
	m_pUi->Draw();
	DrawRanking();
}