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
#include "Score.h"
#include "sign.h"
#include "sound.h"
#include "number_manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DOMINO_SPACE			(DOMINO_WIDTH * 2.2f)					//ドミノ同士の間隔
#define SCROLL_SPEED			(11.0f)					//スクロールスピード
#define MAX_TIME (3)
#define TIMELIMIT (2)
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

	//スコア初期化
	InitScore();

	//看板初期化
	InitSign();

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
	m_pstone_bridge->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - 250.0f, 0.0f));
	m_pstone_bridge->SetDiagonalLine(SCREEN_WIDTH, 500.0f);
	m_pstone_bridge->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pstone_bridge->SetUVSize(D3DXVECTOR2(0.3f, 0.5f));
	m_pstone_bridge->SetUVMove(D3DXVECTOR2(0.008f, 0.0f));
	m_pstone_bridge->SetPolygon();


	CManager::GetSound()->Play(CSound::SOUND_BGM_GAME);

	m_pFlowObject = new C2DPolygon;
	if (FAILED(m_pFlowObject->Init()))
	{
		return -1;
	}
	m_pFlowObject->SetPos(D3DXVECTOR3(SCREEN_WIDTH + 100.0f, SCREEN_HEIGHT * 0.5f + 100.0f, 0.0f));
	m_pFlowObject->SetDiagonalLine(200.0f, 200.0f);
	m_pFlowObject->SetPolygon();
	m_pFlowObject->SetMove(D3DXVECTOR3(-3.0f,0.0f,0.0f));

	m_nText[0] = CTexture::LoadTexture("data\\TEXTURE\\漂流物01.png");
	m_nText[1] = CTexture::LoadTexture("data\\TEXTURE\\漂流物02.png");
	m_nText[2] = CTexture::LoadTexture("data\\TEXTURE\\漂流物03.png");
	m_nText[3] = CTexture::LoadTexture("data\\TEXTURE\\漂流物04.png");
	m_nText[4] = CTexture::LoadTexture("data\\TEXTURE\\漂流物05.png");
	m_nText[5] = CTexture::LoadTexture("data\\TEXTURE\\漂流物06.png");
	m_nText[6] = CTexture::LoadTexture("data\\TEXTURE\\漂流物07.png");
	m_nText[7] = CTexture::LoadTexture("data\\TEXTURE\\漂流物08.png");
	m_nText[8] = CTexture::LoadTexture("data\\TEXTURE\\漂流物09.png");
	m_nText[9] = CTexture::LoadTexture("data\\TEXTURE\\漂流物010.png");
	m_nText[10] = CTexture::LoadTexture("data\\TEXTURE\\漂流物11.png");
	m_nText[11] = CTexture::LoadTexture("data\\TEXTURE\\漂流物12.png");
	m_nText[12] = CTexture::LoadTexture("data\\TEXTURE\\漂流物13.png");
	m_nText[13] = CTexture::LoadTexture("data\\TEXTURE\\漂流物14.png");
	m_nText[14] = CTexture::LoadTexture("data\\TEXTURE\\漂流物15.png");
	m_nText[15] = CTexture::LoadTexture("data\\TEXTURE\\漂流物16.png");
	m_nText[16] = CTexture::LoadTexture("data\\TEXTURE\\漂流物17.png");

	
	m_pFlowObject->SetTextIndex(m_nText[rand() % TEXT_MAX]);


	m_pNumber_Manager = new CNumber_Manager;
	if (FAILED(m_pNumber_Manager->Init()))
	{
		return -1;
	}

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

	//スコア破棄
	UninitScore();

	//看板破棄
	UninitSign();

	g_posWorld = { 0.0f,0.0f,0.0f };

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
	if (m_pFlowObject != nullptr)
	{
		m_pFlowObject->Uninit();
		delete m_pFlowObject;
		m_pFlowObject = nullptr;
	}

	if (m_pNumber_Manager != nullptr)
	{
		m_pNumber_Manager->Uninit();
		delete m_pNumber_Manager;
		m_pNumber_Manager = nullptr;
	}
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CGame::Update()
{
	m_pFlowObject->Update();
	m_pBg->Update();
	if (g_gameState == GAMESTATE_DOWN)
	{
		m_pstone_bridge->Update();
		m_pNumber_Manager->Update();
	}
	if (g_gameState == GAMESTATE_END)
	{
		//スコア更新
		UpdateScore();
	}

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

				SetDomino(D3DXVECTOR3(SCREEN_WIDTH * 0.3f + g_PushState.nPushCount * DOMINO_SPACE, 0, 0.0f));

				//サウンド(SE)の再生
				CManager::GetSound()->Play(CSound::SOUND_SE_MASH_BOTTON);
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

				SetDomino(D3DXVECTOR3(SCREEN_WIDTH * 0.3f + g_PushState.nPushCount * DOMINO_SPACE, 0, 0.0f));

				//サウンド(SE)の再生
				CManager::GetSound()->Play(CSound::SOUND_SE_MASH_BOTTON);
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

				SetDomino(D3DXVECTOR3(SCREEN_WIDTH * 0.3f + g_PushState.nPushCount * DOMINO_SPACE, 0, 0.0f));

				//サウンド(SE)の再生
				CManager::GetSound()->Play(CSound::SOUND_SE_MASH_BOTTON);
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

				SetDomino(D3DXVECTOR3(SCREEN_WIDTH * 0.3f + g_PushState.nPushCount * DOMINO_SPACE, 0.0f, 0.0f));

				//サウンド(SE)の再生
				CManager::GetSound()->Play(CSound::SOUND_SE_MASH_BOTTON);
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

	//ハンド更新
	UpdateHand();

	//看板更新
	UpdateSign();

	if (g_PushState.nTotalLimitTime <= 0 && g_gameState == GAMESTATE_PUSH)
	{//制限時間がなくなったときドミノを倒しはじめる

		//情報取得
		Hand *pHand = GetHand();

		pDomino->state = DOMINOSTATE_DOWN;

		pHand->state = HANDSTATE_PUSH;
	}

	if (g_gameState == GAMESTATE_DOWN && GetDominoNum() == 0)
	{//ドミノを一個も出さずに倒した場合
		SetGameState(GAMESTATE_END);
	}

	if (m_pFlowObject->GetPos().x < -300.0f)
	{
		m_pFlowObject->SetPos(D3DXVECTOR3(SCREEN_WIDTH + 100.0f, SCREEN_HEIGHT * 0.5f + 100.0f, 0.0f));
		m_pFlowObject->SetTextIndex(m_nText[rand() % TEXT_MAX]);
	}


	if (g_gameState == GAMESTATE_END)
	{//ゲーム終了なら決定ボタンで遷移
		SetScore(g_PushState.nPushCount);
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

	//漂流物
	m_pFlowObject->Draw();

	if (g_gameState == GAMESTATE_PUSH)
	{//ボタン描画
		m_pButton->Draw();
	}
	
	//ドミノ描画
	DrawDomino();

	//ハンド描画
	DrawHand();

	//看板描画
	DrawSign();

	//ナンバーマネージャー
	m_pNumber_Manager->Draw();

	//石橋
	m_pstone_bridge->Draw();
	if (g_gameState == GAMESTATE_END)
	{
		//スコア描画
		DrawScore();
	}
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
//*****************************************************************************
// スコアなどの取得
//*****************************************************************************
PUSHSTATE*GetPushState()
{
	return &g_PushState;
}