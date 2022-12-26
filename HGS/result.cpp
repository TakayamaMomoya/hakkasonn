//=============================================================================
//
// game.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
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
// マクロ定義
//*****************************************************************************
#define FADE_TIMER					(300)			//フェードまでの時間


//*****************************************************************************
// グローバル変数
//*****************************************************************************
int g_nCntFadeResult;								//フェードまでのカウンター

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CResult::CResult()
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CResult::~CResult()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CResult::Init()
{
	//変数初期化
	g_nCntFadeResult = 0;

	//ドミノ初期化
	InitDomino();

	//ドミノ召喚
	for (int nCntDomino = 0; nCntDomino < MAX_DOMINO; nCntDomino++)
	{
		SetDomino(D3DXVECTOR3(SCREEN_WIDTH * 0.3f + nCntDomino * DOMINO_WIDTH * 2.5f, SCREEN_HEIGHT * 0.82f, 0.0f));
	}

	//情報取得
	Domino *pDomino = GetDomino();

	//最初のドミノを倒す
	pDomino->state = DOMINOSTATE_DOWN;

	//ランキング初期化
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
	int nIndex = CTexture::LoadTexture("data/TEXTURE/リザルト.png");
	pPolygon[0].SetTextIndex(nIndex);
	pPolygon[0].SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	pPolygon[0].SetDiagonalLine(200.0f, 200.0f);
	pPolygon[0].SetPolygon();

	CManager::GetSound()->Play(CSound::SOUND_BGM_RESULT);

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CResult::Uninit()
{
	CManager::GetSound()->Stop(CSound::SOUND_BGM_RESULT);

	//ランキング終了
	UninitRanking();

	//ドミノ終了
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
// 更新処理
//*****************************************************************************
void CResult::Update()
{
	//カウンター加算
	g_nCntFadeResult++;

	//UiUpdate
	m_pUi->Update();

	//GetInput
	CInput *pInput = CInput::GetKey();

	//ランキング更新
	UpdateRanking();

	//ドミノ更新
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
// 描画処理
//*****************************************************************************
void CResult::Draw()
{
	//UiDraw
	m_pUi->Draw();

	//ドミノ描画
	DrawDomino();

	//ランキング描画
	DrawRanking();
}