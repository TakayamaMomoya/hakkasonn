//=============================================================================
//
// number_manager.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _NUMBER_MANAGER_H_			//このマクロ定義がされなかったら
#define _NUMBER_MANAGER_H_			//2重インクルード防止のマクロ定義
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include <list>

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CNumber_Manager
{

public:
	CNumber_Manager();
	~CNumber_Manager();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void SetNumber(D3DXVECTOR3 pos, int nNum);

private:
	std::list<CNumber*> m_NumberList;
	int m_nTextNum;
};
#endif

