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
#include "title.h"
#include "input.h"
#include "texture.h"
#include "ui.h"
#include "sound.h"
#include "domino.h"
#include "2dpolygon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_TIMER							(600)							//フェードするまでの時間

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
int g_nCounterFadeTitle;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTitle::CTitle()
{
	m_fPressKeyPolygonColorA = 0.0f;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTitle::~CTitle()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CTitle::Init()
{
	//変数初期化
	g_nCounterFadeTitle = 0;

	//ドミノ初期化
	InitDomino();

	//ドミノ召喚
	for (int nCntDomino = 0;nCntDomino < MAX_DOMINO;nCntDomino++)
	{
		SetDomino(D3DXVECTOR3(SCREEN_WIDTH * 0.3f + nCntDomino * DOMINO_WIDTH * 2.5f, SCREEN_HEIGHT * 0.82f, 0.0f));
	}

	//情報取得
	Domino *pDomino = GetDomino();

	//最初のドミノを倒す
	pDomino->state = DOMINOSTATE_DOWN;

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

	m_nText[0] = CTexture::LoadTexture("data\\TEXTURE\\漂流物01.png");
	m_nText[1] = CTexture::LoadTexture("data\\TEXTURE\\漂流物02.png");
	m_nText[2] = CTexture::LoadTexture("data\\TEXTURE\\漂流物03.png");
	m_nText[3] = CTexture::LoadTexture("data\\TEXTURE\\漂流物04.png");
	m_nText[4] = CTexture::LoadTexture("data\\TEXTURE\\漂流物05.png");
	m_nText[5] = CTexture::LoadTexture("data\\TEXTURE\\漂流物06.png");
	m_nText[6] = CTexture::LoadTexture("data\\TEXTURE\\漂流物07.png");
	m_nText[7] = CTexture::LoadTexture("data\\TEXTURE\\漂流物08.png");
	m_nText[8] = CTexture::LoadTexture("data\\TEXTURE\\漂流物09.png");
	m_nText[9] = CTexture::LoadTexture("data\\TEXTURE\\漂流物10.png");
	m_nText[10] = CTexture::LoadTexture("data\\TEXTURE\\漂流物11.png");
	m_nText[11] = CTexture::LoadTexture("data\\TEXTURE\\漂流物12.png");
	m_nText[12] = CTexture::LoadTexture("data\\TEXTURE\\漂流物13.png");
	m_nText[13] = CTexture::LoadTexture("data\\TEXTURE\\漂流物14.png");
	m_nText[14] = CTexture::LoadTexture("data\\TEXTURE\\漂流物15.png");
	m_nText[15] = CTexture::LoadTexture("data\\TEXTURE\\漂流物16.png");
	m_nText[16] = CTexture::LoadTexture("data\\TEXTURE\\漂流物17.png");


	m_pFlowObject->SetTextIndex(m_nText[rand() % TEXT_MAX]);


	m_pTitle = new C2DPolygon;
	if (FAILED(m_pTitle->Init()))
	{
		return -1;
	}
	m_pTitle->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 100.0f, 0.0f));
	m_pTitle->SetDiagonalLine(400.0f, 400.0f);
	m_pTitle->SetPolygon();
	m_pTitle->SetTextIndex(CTexture::LoadTexture("data\\TEXTURE\\Titlelogo.png"));

	m_pPressKeyPolygon = new C2DPolygon;

	if (FAILED(m_pPressKeyPolygon->Init()))
	{
		return -1;
	}

	nIndex = CTexture::LoadTexture("data\\TEXTURE\\PressKey.png");
	m_pPressKeyPolygon->SetTextIndex(nIndex);
	m_pPressKeyPolygon->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - 200.0f, 0.0f));
	m_pPressKeyPolygon->SetDiagonalLine(600.0f, 600.0f);
	m_pPressKeyPolygon->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fPressKeyPolygonColorA));
	m_pPressKeyPolygon->SetFadeSpeed(0.01f);
	m_pPressKeyPolygon->SetPolygon();

	//サウンドの再生
	CManager::GetSound()->Play(CSound::SOUND_BGM_TITLE);

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CTitle::Uninit()
{
	//サウンドの停止
	CManager::GetSound()->Stop(CSound::SOUND_BGM_TITLE);

	//ドミノ破棄
	UninitDomino();

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
	if (m_pTitle != nullptr)
	{
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = nullptr;
	}
	if (m_pPressKeyPolygon != nullptr)
	{
		m_pPressKeyPolygon->Uninit();
		delete m_pPressKeyPolygon;
		m_pPressKeyPolygon = nullptr;
	}
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CTitle::Update()
{
	//カウンタ加算
	g_nCounterFadeTitle++;

	//ドミノ更新
	UpdateTitleDomino();

	m_pFlowObject->Update();
	m_pBg->Update();
	m_pstone_bridge->Update();
	m_pPressKeyPolygon->SetFlashing(10);
	m_pPressKeyPolygon->Update();

	//GetInput
	CInput *pInput = CInput::GetKey();

	//EndTitle
	if (pInput->Trigger(KEY_DECISION))
	{
		CManager * pManager = GetManager();
		pManager->NextMode(TYPE_GAME);
	}

	//ランキングへの自動遷移
	if (g_nCounterFadeTitle > FADE_TIMER)
	{
		CManager * pManager = GetManager();
		pManager->NextMode(TYPE_RESULT);
	}
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CTitle::Draw()
{
	//背景描画
	m_pBg->Draw();

	//漂流物
	m_pFlowObject->Draw();

	//ドミノ描画
	DrawDomino();

	//石橋
	m_pstone_bridge->Draw();

	m_pTitle->Draw();

	m_pPressKeyPolygon->Draw();
}