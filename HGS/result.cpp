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
	//UiUpdate
	m_pUi->Update();

	//GetInput
	CInput *pInput = CInput::GetKey();

	//EndResult
	if (pInput->Trigger(KEY_DECISION))
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
}