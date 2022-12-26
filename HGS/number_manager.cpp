//=============================================================================
//
// number_manager.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "number_manager.h"
#include "number.h"
#include "texture.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CNumber_Manager::CNumber_Manager()
{
	
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CNumber_Manager::~CNumber_Manager()
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CNumber_Manager::Init()
{
	m_nTextNum = CTexture::LoadTexture("data\\TEXTURE\\number_pop.png");
	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CNumber_Manager::Uninit()
{
	//イテレーターループ
	for (auto itr = m_NumberList.begin(); itr != m_NumberList.end();)
	{
		//イテレーターから瓦礫のポインタの代入
		CNumber* pNumber = *itr;

		//瓦礫ポインタの解放
		if (pNumber != nullptr)
		{
			//終了処理
			pNumber->Uninit();
			delete pNumber;
			pNumber = nullptr;
		}

		//次のイテレーターの代入、現在のイテレーターを破棄
		itr = m_NumberList.erase(itr);
	}
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CNumber_Manager::Update()
{
	//イテレーターループ
	for (auto itr = m_NumberList.begin(); itr != m_NumberList.end();)
	{
		//イテレーターからナンバーのポインタの代入
		CNumber* pNumber = *itr;

		//ポインタの解放
		if (pNumber == nullptr)
		{
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_NumberList.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//更新処理
		pNumber->AddNumberPos(D3DXVECTOR3(-22.0f,0.0f,0.0f));

		//終了条件処理
		if (pNumber->GetPos().x < -300.0f)
		{
			//終了処理
			pNumber->Uninit();
			delete pNumber;
			pNumber = nullptr;

			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_NumberList.erase(itr);

			//以下の処理を無視する
			continue;
		}

		//イテレーターを進める
		itr++;
	}
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CNumber_Manager::Draw()
{
	//イテレーターループ
	for (auto itr = m_NumberList.begin(); itr != m_NumberList.end(); itr++)
	{
		//イテレーターからナンバーのポインタの代入
		CNumber* pNumber = *itr;

		//ポインタの解放
		if (pNumber != nullptr)
		{
			//描画処理
			pNumber->Draw();
		}
	}
}

//ナンバーのセット
void CNumber_Manager::SetNumber(D3DXVECTOR3 pos, int nNum)
{
	CNumber* pNumber = nullptr;

	pNumber = new CNumber;

	pNumber->SetTextNum(m_nTextNum);

	if (FAILED(pNumber->Init()))
	{
		return;
	}

	pNumber->SetNumber(pos,3,50.0f, nNum);

	

	//リストに瓦礫情報を追加
	m_NumberList.push_back(pNumber);
}
