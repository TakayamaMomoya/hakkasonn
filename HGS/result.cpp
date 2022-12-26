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

	//GetUiData
	m_pBg = new CUI;

	//UiInit
	if (FAILED(m_pBg->Init()))
	{
		return -1;
	}

	//GetUiPointer
	C2DPolygon *pPolygon = m_pBg->CreateUi(4);

	//SetUiData
	int nIndex = CTexture::LoadTexture("data/TEXTURE/sky.png");
	pPolygon[0].SetTextIndex(nIndex);
	pPolygon[0].SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	pPolygon[0].SetDiagonalLine(SCREEN_WIDTH, SCREEN_HEIGHT);
	pPolygon[0].SetPolygon();

	nIndex = 0;
	pPolygon[1].SetTextIndex(nIndex);
	pPolygon[1].SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f + 30.0f, 0.0f));
	pPolygon[1].SetDiagonalLine(SCREEN_WIDTH, 20.0f);
	pPolygon[1].SetColor(D3DXCOLOR(0.4f, 1.0f, 1.0f, 1.0f));
	pPolygon[1].SetPolygon();

	nIndex = CTexture::LoadTexture("data/TEXTURE/Cloud.png");
	pPolygon[2].SetTextIndex(nIndex);
	pPolygon[2].SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 100.0f, 0.0f));
	pPolygon[2].SetDiagonalLine(SCREEN_WIDTH, 100.0f);
	pPolygon[2].SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pPolygon[2].SetUVSize(D3DXVECTOR2(0.5f, 0.5f));
	pPolygon[2].SetUVMove(D3DXVECTOR2(0.0005f, 0.0f));
	pPolygon[2].SetPolygon();

	nIndex = CTexture::LoadTexture("data/TEXTURE/sea.png");
	pPolygon[3].SetTextIndex(nIndex);
	pPolygon[3].SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	pPolygon[3].SetDiagonalLine(SCREEN_WIDTH, SCREEN_HEIGHT);
	pPolygon[3].SetUVMove(D3DXVECTOR2(0.0005f, 0.0f));
	pPolygon[3].SetPolygon();


	//�΋�
	m_pstone_bridge = new C2DPolygon;
	if (FAILED(m_pstone_bridge->Init()))
	{
		return -1;
	}
	nIndex = CTexture::LoadTexture("data\\TEXTURE\\stone_bridge.png");
	m_pstone_bridge->SetTextIndex(nIndex);
	m_pstone_bridge->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - 250.0f, 0.0f));
	m_pstone_bridge->SetDiagonalLine(SCREEN_WIDTH, 500.0f);
	m_pstone_bridge->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pstone_bridge->SetUVSize(D3DXVECTOR2(0.3f, 0.5f));
	m_pstone_bridge->SetUVMove(D3DXVECTOR2(0.0041f, 0.0f));
	m_pstone_bridge->SetPolygon();

	m_pFlowObject = new C2DPolygon;
	if (FAILED(m_pFlowObject->Init()))
	{
		return -1;
	}
	m_pFlowObject->SetPos(D3DXVECTOR3(SCREEN_WIDTH + 100.0f, SCREEN_HEIGHT * 0.5f + 100.0f, 0.0f));
	m_pFlowObject->SetDiagonalLine(200.0f, 200.0f);
	m_pFlowObject->SetPolygon();
	m_pFlowObject->SetMove(D3DXVECTOR3(-3.0f, 0.0f, 0.0f));

	m_nText[0] = CTexture::LoadTexture("data\\TEXTURE\\�Y����01.png");
	m_nText[1] = CTexture::LoadTexture("data\\TEXTURE\\�Y����02.png");
	m_nText[2] = CTexture::LoadTexture("data\\TEXTURE\\�Y����03.png");
	m_nText[3] = CTexture::LoadTexture("data\\TEXTURE\\�Y����04.png");
	m_nText[4] = CTexture::LoadTexture("data\\TEXTURE\\�Y����05.png");
	m_nText[5] = CTexture::LoadTexture("data\\TEXTURE\\�Y����06.png");
	m_nText[6] = CTexture::LoadTexture("data\\TEXTURE\\�Y����07.png");
	m_nText[7] = CTexture::LoadTexture("data\\TEXTURE\\�Y����08.png");
	m_nText[8] = CTexture::LoadTexture("data\\TEXTURE\\�Y����09.png");
	m_nText[9] = CTexture::LoadTexture("data\\TEXTURE\\�Y����10.png");
	m_nText[10] = CTexture::LoadTexture("data\\TEXTURE\\�Y����11.png");
	m_nText[11] = CTexture::LoadTexture("data\\TEXTURE\\�Y����12.png");
	m_nText[12] = CTexture::LoadTexture("data\\TEXTURE\\�Y����13.png");
	m_nText[13] = CTexture::LoadTexture("data\\TEXTURE\\�Y����14.png");
	m_nText[14] = CTexture::LoadTexture("data\\TEXTURE\\�Y����15.png");
	m_nText[15] = CTexture::LoadTexture("data\\TEXTURE\\�Y����16.png");
	m_nText[16] = CTexture::LoadTexture("data\\TEXTURE\\�Y����17.png");


	m_pFlowObject->SetTextIndex(m_nText[rand() % TEXT_MAX]);

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
	if (m_pstone_bridge != nullptr)
	{
		m_pstone_bridge->Uninit();
		delete m_pstone_bridge;
		m_pstone_bridge = nullptr;
	}
	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		delete m_pBg;
		m_pBg = nullptr;
	}
	if (m_pFlowObject != nullptr)
	{
		m_pFlowObject->Uninit();
		delete m_pFlowObject;
		m_pFlowObject = nullptr;
	}
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CResult::Update()
{
	//�J�E���^�[���Z
	g_nCntFadeResult++;

	m_pFlowObject->Update();
	m_pBg->Update();
	m_pstone_bridge->Update();

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
	//�w�i�`��
	m_pBg->Draw();

	//�Y����
	m_pFlowObject->Draw();

	//�h�~�m�`��
	DrawDomino();

	//�΋�
	m_pstone_bridge->Draw();

	//�����L���O�`��
	DrawRanking();
}