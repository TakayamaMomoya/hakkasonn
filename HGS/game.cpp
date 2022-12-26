//=============================================================================
//
// game.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "manager.h"
#include "game.h"
#include "input.h"
#include "texture.h"
#include "ui.h"
#include "domino.h"
#include "sound.h"
#include "hand.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DOMINO_SPACE			(DOMINO_WIDTH * 2.2f)					//ドミノ同士の間隔
#define SCROLL_SPEED			(22.0f)					//スクロールスピード
#define MAX_TIME (3)
#define TIMELIMIT (10)
#define CLOSSKEY (4)

//*****************************************************************************
void ManageScroll(void);

//*****************************************************************************
// グローバル宣言
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
// コンストラクタ
//*****************************************************************************
CGame::CGame()
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CGame::~CGame()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CGame::Init()
{
	//変数初期化
	g_posWorld = {0.0f,0.0f,0.0f};
	g_gameState = GAMESTATE_PUSH;

	//ドミノ初期化
	InitDomino();

	//ハンド初期化
	InitHand();

	//各数値初期化
	g_PushState.NowTargetButton = TARGETBUTTON_NONE;
	g_PushState.nPushCount = 0;
	g_PushState.nPushLimitTime = 0;
	g_PushState.nColorCount = 0;
	g_PushState.nTotalLimitTime = TIMELIMIT * 60;

	//乱数の種を取得
	DWORD time = timeGetTime();
	srand((unsigned int)time);

	m_pButton = new C2DPolygon;
	if (FAILED(m_pButton->Init()))
	{
		return -1;
	}
	int nTex = CTexture::LoadTexture("data\\TEXTURE\\ボタン.png");
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
	pPolygon[2].SetUVSize(D3DXVECTOR2(0.5f, 0.5f));
	pPolygon[2].SetUVMove(D3DXVECTOR2(0.0005f, 0.0f));
	pPolygon[2].SetPolygon();

	nIndex = CTexture::LoadTexture("data/TEXTURE/sea.png");
	pPolygon[3].SetTextIndex(nIndex);
	pPolygon[3].SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	pPolygon[3].SetDiagonalLine(SCREEN_WIDTH, SCREEN_HEIGHT);
	pPolygon[3].SetUVMove(D3DXVECTOR2(0.0005f, 0.0f));
	pPolygon[3].SetPolygon();


	//石橋
	m_pstone_bridge = new C2DPolygon;
	if (FAILED(m_pstone_bridge->Init()))
	{
		return -1;
	}
	nIndex = CTexture::LoadTexture("data\\TEXTURE\\stone_bridge.png");
	m_pstone_bridge->SetTextIndex(nIndex);
	m_pstone_bridge->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - 400.0f, 0.0f));
	m_pstone_bridge->SetDiagonalLine(SCREEN_WIDTH, 500.0f);
	m_pstone_bridge->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pstone_bridge->SetUVSize(D3DXVECTOR2(0.3f, 0.5f));
	m_pstone_bridge->SetUVMove(D3DXVECTOR2(0.001f, 0.0f));
	m_pstone_bridge->SetPolygon();


	CManager::GetSound()->Play(CSound::SOUND_BGM_GAME);

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CGame::Uninit()
{
	CManager::GetSound()->Stop(CSound::SOUND_BGM_GAME);

	//ドミノ終了
	UninitDomino();

	//ハンド終了
	UninitHand();

	if (m_pButton != nullptr)
	{
		m_pButton->Uninit();
		delete m_pButton;
		m_pButton = nullptr;
	}
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
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CGame::Update()
{
	m_pBg->Update();
	m_pstone_bridge->Update();

	if (g_PushState.nColorCount <= 0)
	{
		//色のリセット
		m_pButton->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		g_PushState.nColorCount = 0;
	}
	else
	{
		//色の変更
		m_pButton->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}

	//制限時間減少
	g_PushState.nPushLimitTime--;
	g_PushState.nTotalLimitTime--;
	g_PushState.nColorCount--;

	if (g_PushState.nPushLimitTime <= 0)
	{//次のボタンまでの時間が０になったとき

		//目標ボタンをランダムに設定
		int nRandButton = rand() % (TARGETBUTTON_MAX - 1);
		int nRandTime = (rand() % MAX_TIME + 1) * 60;

		g_PushState.NowTargetButton = (TARGETBUTTON)(nRandButton + 1);
		g_PushState.nPushLimitTime = nRandTime;
	}

	//GetInput
	CInput *pInput = CInput::GetKey();


	if (g_gameState == GAMESTATE_PUSH && g_PushState.nPushCount < 999)
	{
		//===============================================
		//目標ボタンが押されているかの判定
		if (g_PushState.NowTargetButton == TARGETBUTTON_UP)
		{
			m_pButton->SetPos(POS_UP);
			m_pButton->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(180)));
			if (pInput->Trigger(KEY_UP) == true)
			{
				g_PushState.nPushCount++;
				g_PushState.nColorCount = 3;

				SetDomino(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + g_PushState.nPushCount * DOMINO_SPACE, 0, 0.0f));
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

				SetDomino(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + g_PushState.nPushCount * DOMINO_SPACE, 0, 0.0f));
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

				SetDomino(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + g_PushState.nPushCount * DOMINO_SPACE, 0, 0.0f));
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

				SetDomino(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + g_PushState.nPushCount * DOMINO_SPACE, 0.0f, 0.0f));
			}
		}
	}

	m_pButton->Update();

	//ドミノ情報取得
	Domino *pDomino = GetDomino();

	//ドミノ更新
	UpdateDomino();

	//スクロールの管理
	ManageScroll();

	if (g_PushState.nTotalLimitTime <= 0 && g_gameState == GAMESTATE_PUSH)
	{//制限時間がなくなったときドミノを倒しはじめる

		pDomino->state = DOMINOSTATE_DOWN;

		SetGameState(GAMESTATE_DOWN);
	}

	if (g_gameState == GAMESTATE_END)
	{//ゲーム終了なら決定ボタンで遷移

		if (pInput->Trigger(KEY_DECISION))
		{
			CManager * pManager = GetManager();
			pManager->NextMode(TYPE_RESULT);
		}
	}
}


//*****************************************************************************
// スクロール管理
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
// 描画処理
//*****************************************************************************
void CGame::Draw()
{
	//背景描画
	m_pBg->Draw();

	if (g_gameState == GAMESTATE_PUSH)
	{//ボタン描画
		m_pButton->Draw();
	}
	
	//ドミノ描画
	DrawDomino();

	//石橋
	m_pstone_bridge->Draw();
}

//*****************************************************************************
// 世界の原点位置
//*****************************************************************************
D3DXVECTOR3 *GetPosWorld(void)
{
	return &g_posWorld;
}

//*****************************************************************************
// ゲーム状態取得
//*****************************************************************************
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//*****************************************************************************
// ゲーム状態設定処理
//*****************************************************************************
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
}