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
#include "game.h"
#include "input.h"
#include "texture.h"
#include "ui.h"
#include "domino.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DOMINO_SPACE			(50)					//�h�~�m���m�̊Ԋu
#define SCROLL_SPEED			(8.0f)					//�X�N���[���X�s�[�h
#define MAX_TIME (3)
#define TIMELIMIT (10)
#define CLOSSKEY (4)

//*****************************************************************************
void ManageScroll(void);

//*****************************************************************************
// �O���[�o���錾
//*****************************************************************************
PUSHSTATE g_PushState;
static const D3DXVECTOR3 POS = D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f);
static const D3DXVECTOR3 POS_UP = D3DXVECTOR3(POS.x, POS.y - 70.0f, 0.0f);
static const D3DXVECTOR3 POS_DOWN = D3DXVECTOR3(POS.x, POS.y + 70.0f, 0.0f);
static const D3DXVECTOR3 POS_RIGHT = D3DXVECTOR3(POS.x + 70.0f, POS.y, 0.0f);
static const D3DXVECTOR3 POS_LEFT = D3DXVECTOR3(POS.x - 70.0f, POS.y, 0.0f);
D3DXVECTOR3 g_posWorld;
GAMESTATE g_gameState;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CGame::CGame()
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CGame::~CGame()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CGame::Init()
{
	//�ϐ�������
	g_posWorld = {0.0f,0.0f,0.0f};
	g_gameState = GAMESTATE_PUSH;

	//�h�~�m������
	InitDomino();

	//�e���l������
	g_PushState.NowTargetButton = TARGETBUTTON_NONE;
	g_PushState.nPushCount = 0;
	g_PushState.nPushLimitTime = 0;
	g_PushState.nColorCount = 0;
	g_PushState.nTotalLimitTime = TIMELIMIT * 60;

	//�����̎���擾
	DWORD time = timeGetTime();
	srand((unsigned int)time);

	m_pButton = new C2DPolygon;
	if (FAILED(m_pButton->Init()))
	{
		return -1;
	}
	int nTex = CTexture::LoadTexture("data\\TEXTURE\\�{�^��.png");
	m_pButton->SetTextIndex(nTex);
	m_pButton->SetUp(POS_UP, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pButton->SetDiagonalLine(100.0f,100.0f);
	m_pButton->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pButton->SetPolygon();

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
	pPolygon[1].SetColor(D3DXCOLOR(0.4f,1.0f,1.0f,1.0f));
	pPolygon[1].SetPolygon();

	nIndex = CTexture::LoadTexture("data/TEXTURE/Cloud.png");
	pPolygon[2].SetTextIndex(nIndex);
	pPolygon[2].SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 100.0f, 0.0f));
	pPolygon[2].SetDiagonalLine(SCREEN_WIDTH, 100.0f);
	pPolygon[2].SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pPolygon[2].SetUVMove(D3DXVECTOR2(0.0005f, 0.0f));
	pPolygon[2].SetPolygon();

	nIndex = CTexture::LoadTexture("data/TEXTURE/sea.png");
	pPolygon[3].SetTextIndex(nIndex);
	pPolygon[3].SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	pPolygon[3].SetDiagonalLine(SCREEN_WIDTH, SCREEN_HEIGHT);
	pPolygon[3].SetUVMove(D3DXVECTOR2(0.0005f, 0.0f));
	pPolygon[3].SetPolygon();

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CGame::Uninit()
{
	//�h�~�m�I��
	UninitDomino();

	if (m_pButton != nullptr)
	{
		m_pButton->Uninit();
		delete m_pButton;
		m_pButton = nullptr;
	}
	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		delete m_pBg;
		m_pBg = nullptr;
	}
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CGame::Update()
{
	m_pBg->Update();

	if (g_PushState.nColorCount <= 0)
	{
		//�F�̃��Z�b�g
		m_pButton->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		g_PushState.nColorCount = 0;
	}
	else
	{
		//�F�̕ύX
		m_pButton->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}

	//�������Ԍ���
	g_PushState.nPushLimitTime--;
	g_PushState.nTotalLimitTime--;
	g_PushState.nColorCount--;
	if (g_PushState.nPushLimitTime <= 0)
	{//���̃{�^���܂ł̎��Ԃ��O�ɂȂ����Ƃ�

	 //�ڕW�{�^���������_���ɐݒ�
		int nRandButton = rand() % (TARGETBUTTON_MAX - 1);
		int nRandTime = (rand() % MAX_TIME + 2) * 60;

		g_PushState.NowTargetButton = (TARGETBUTTON)(nRandButton + 1);
		g_PushState.nPushLimitTime = nRandTime;
	}

	//GetInput
	CInput *pInput = CInput::GetKey();

	//===============================================
	//�ڕW�{�^����������Ă��邩�̔���
	if (g_PushState.NowTargetButton == TARGETBUTTON_UP)
	{
		m_pButton->SetPos(POS_UP);
		m_pButton->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(180)));
		if (pInput->Trigger(KEY_UP) == true)
		{
			g_PushState.nPushCount++;
			g_PushState.nColorCount = 3;
		}
	}
	else if (g_PushState.NowTargetButton == TARGETBUTTON_DOWN)
	{
		m_pButton->SetPos(POS_DOWN);
		m_pButton->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(0)));
		if (pInput->Trigger(KEY_DOWN) == true)
		{
			g_PushState.nPushCount++;
			g_PushState.nColorCount = 3;
		}
	}
	else if (g_PushState.NowTargetButton == TARGETBUTTON_RIGHT)
	{
		m_pButton->SetPos(POS_RIGHT);
		m_pButton->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(90)));
		if (pInput->Trigger(KEY_RIGHT) == true)
		{
			g_PushState.nPushCount++;
			g_PushState.nColorCount = 3;
		}
	}
	else if (g_PushState.NowTargetButton == TARGETBUTTON_LEFT)
	{
		m_pButton->SetPos(POS_LEFT);
		m_pButton->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(270)));
		if (pInput->Trigger(KEY_LEFT) == true)
		{
			g_PushState.nPushCount++;
			g_PushState.nColorCount = 3;
		}
	}

	m_pButton->Update();

	//�h�~�m���擾
	Domino *pDomino = GetDomino();

	//�h�~�m�X�V
	UpdateDomino();

	//�X�N���[���̊Ǘ�
	ManageScroll();

	if (g_PushState.nTotalLimitTime <= 0 && g_gameState == GAMESTATE_PUSH)
	{//�������Ԃ��Ȃ��Ȃ����Ƃ�
		for (int nCntDomino = 0; nCntDomino < g_PushState.nPushCount; nCntDomino++)
		{//�h�~�m����
			SetDomino(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + nCntDomino * DOMINO_SPACE, SCREEN_HEIGHT * 0.5f, 0.0f));
		}
		SetGameState(GAMESTATE_DOWN);
	}

	if (g_gameState == GAMESTATE_END)
	{
		if (pInput->Trigger(KEY_DECISION))
		{
			CManager * pManager = GetManager();
			pManager->NextMode(TYPE_RESULT);
		}
	}

}

//*****************************************************************************
// �X�N���[���Ǘ�
//*****************************************************************************
void ManageScroll(void)
{
	switch (g_gameState)
	{
	case GAMESTATE_DOWN:

		g_posWorld.x -= SCROLL_SPEED;

		break;
	default:
		break;
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CGame::Draw()
{
	//�w�i�`��
	m_pBg->Draw();

	if (g_gameState == GAMESTATE_PUSH)
	{//�{�^���`��
		m_pButton->Draw();
	}
	
	//�h�~�m�`��
	DrawDomino();
}

//*****************************************************************************
// ���E�̌��_�ʒu
//*****************************************************************************
D3DXVECTOR3 *GetPosWorld(void)
{
	return &g_posWorld;
}

//*****************************************************************************
// �Q�[����Ԏ擾
//*****************************************************************************
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//*****************************************************************************
// �Q�[����Ԑݒ菈��
//*****************************************************************************
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
}