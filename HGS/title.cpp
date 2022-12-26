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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_TIMER							(1200)							//フェードするまでの時間

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
int g_nCounterFadeTitle;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTitle::CTitle()
{
	
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
	int nIndex = CTexture::LoadTexture("data/TEXTURE/タイトル.png");
	pPolygon[0].SetTextIndex(nIndex);
	pPolygon[0].SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	pPolygon[0].SetDiagonalLine(200.0f, 200.0f);
	pPolygon[0].SetPolygon();

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
void CTitle::Update()
{
	//カウンタ加算
	//g_nCounterFadeTitle++;

	//ドミノ更新
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

	//ランキングへの自動遷移
	/*if (g_nCounterFadeTitle > FADE_TIMER)
	{
		CManager * pManager = GetManager();
		pManager->NextMode(TYPE_RESULT);
	}*/
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CTitle::Draw()
{
	//ドミノ描画
	DrawDomino();

	//UiDraw
	m_pUi->Draw();
}