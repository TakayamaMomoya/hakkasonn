//=============================================================================
//
// Result.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _RESULT_H_			//このマクロ定義がされなかったら
#define _RESULT_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CUI;
class C2DPolygon;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CResult : public CObject
{
private:
	static const int TEXT_MAX = 17;
public:
	CResult();
	~CResult() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	C2DPolygon* m_pSea;
	CUI* m_pBg;
	C2DPolygon* m_pstone_bridge;
	C2DPolygon* m_pFlowObject;
	int m_nText[TEXT_MAX];
};

#endif