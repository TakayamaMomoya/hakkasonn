//=============================================================================
//
// game.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "manager.h"
#include "title.h"
#include "input.h"
#include "texture.h"
#include "ui.h"
#include "sound.h"
#include "domino.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FADE_TIMER							(1200)							//�t�F�[�h����܂ł̎���

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
int g_nCounterFadeTitle;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTitle::CTitle()
{
	
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTitle::~CTitle()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CTitle::Init()
{
	//�ϐ�������
	g_nCounterFadeTitle = 0;

	//�h�~�m������
	InitDomino();

	//�h�~�m����
	for (int nCntDomino = 0;nCntDomino < MAX_DOMINO;nCntDomino++)
	{
		SetDomino(D3DXVECTOR3(SCREEN_WIDTH * 0.3f + nCntDomino * DOMINO_WIDTH * 2.5f, SCREEN_HEIGHT * 0.82f, 0.0f));
	}

	//���擾
	Domino *pDomino = GetDomino();

	//�ŏ��̃h�~�m��|��
	pDomino->state = DOMINOSTATE_DOWN;

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
	int nIndex = CTexture::LoadTexture("data/TEXTURE/�^�C�g��.png");
	pPolygon[0].SetTextIndex(nIndex);
	pPolygon[0].SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	pPolygon[0].SetDiagonalLine(200.0f, 200.0f);
	pPolygon[0].SetPolygon();

	//�T�E���h�̍Đ�
	CManager::GetSound()->Play(CSound::SOUND_BGM_TITLE);

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CTitle::Uninit()
{
	//�T�E���h�̒�~
	CManager::GetSound()->Stop(CSound::SOUND_BGM_TITLE);

	//�h�~�m�j��
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
void CTitle::Update()
{
	//�J�E���^���Z
	//g_nCounterFadeTitle++;

	//�h�~�m�X�V
	UpdateTitleDomino();

	//UiUpdate
	m_pUi->Update();

	//GetInput
	CInput *pInput = CInput::GetKey();

	//EndTitle
	if (pInput->Trigger(KEY_DECISION))
	{
		CManager * pManager = GetManager();
		pManager->NextMode(TYPE_GAME);
	}

	//�����L���O�ւ̎����J��
	/*if (g_nCounterFadeTitle > FADE_TIMER)
	{
		CManager * pManager = GetManager();
		pManager->NextMode(TYPE_RESULT);
	}*/
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CTitle::Draw()
{
	//�h�~�m�`��
	DrawDomino();

	//UiDraw
	m_pUi->Draw();
}