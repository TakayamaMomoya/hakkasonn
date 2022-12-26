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
#include "domino.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FADE_TIMER					(300)			//�t�F�[�h�܂ł̎���


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
int g_nCntFadeResult;								//�t�F�[�h�܂ł̃J�E���^�[

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
	//�ϐ�������
	g_nCntFadeResult = 0;

	//�h�~�m������
	InitDomino();

	//�h�~�m����
	for (int nCntDomino = 0; nCntDomino < MAX_DOMINO; nCntDomino++)
	{
		SetDomino(D3DXVECTOR3(SCREEN_WIDTH * 0.3f + nCntDomino * DOMINO_WIDTH * 2.5f, SCREEN_HEIGHT * 0.82f, 0.0f));
	}

	//���擾
	Domino *pDomino = GetDomino();

	//�ŏ��̃h�~�m��|��
	pDomino->state = DOMINOSTATE_DOWN;

	//�����L���O������
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

	//�����L���O�I��
	UninitRanking();

	//�h�~�m�I��
	UninitDomino();

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
	//�J�E���^�[���Z
	g_nCntFadeResult++;

	//UiUpdate
	m_pUi->Update();

	//GetInput
	CInput *pInput = CInput::GetKey();

	//�����L���O�X�V
	UpdateRanking();

	//�h�~�m�X�V
	UpdateTitleDomino();

	//EndResult
	if (pInput->Trigger(KEY_DECISION))
	{
		CManager * pManager = GetManager();
		pManager->NextMode(TYPE_TITLE);
	}

	if (FADE_TIMER < g_nCntFadeResult)
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

	//�h�~�m�`��
	DrawDomino();

	//�����L���O�`��
	DrawRanking();
}