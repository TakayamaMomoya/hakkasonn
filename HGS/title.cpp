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
	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CTitle::Uninit()
{
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
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CTitle::Draw()
{
	//UiDraw
	m_pUi->Draw();
}