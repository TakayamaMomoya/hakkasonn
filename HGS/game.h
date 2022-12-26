//=============================================================================
//
// game.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _GAME_H_			//このマクロ定義がされなかったら
#define _GAME_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLOSSKEY (4)

//*****************************************************************************
// 列挙型宣言
//*****************************************************************************
typedef enum
{
	TARGETBUTTON_NONE = 0,
	TARGETBUTTON_UP,
	TARGETBUTTON_DOWN,
	TARGETBUTTON_RIGHT,
	TARGETBUTTON_LEFT,
	TARGETBUTTON_MAX
}TARGETBUTTON;

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{
	TARGETBUTTON NowTargetButton; //押すボタン
	int nPushCount; //押した回数
	int nPushLimitTime; //次のボタンに移行するまでの時間
	int nTotalLimitTime; //ゲームの制限時間
	int nColorCount; //色の変化時間
}PUSHSTATE;

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class C2DPolygon;
class CUI;
class CNumber_Manager;

//*****************************************************************************
// 列挙型
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,						//何もしていない状態
	GAMESTATE_NORMAL,						//通常状態
	GAMESTATE_PUSH,							//連打状態
	GAMESTATE_DOWN,							//倒れる状態
	GAMESTATE_END,							//終了状態
	GAMESTATE_MAX
}GAMESTATE;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame : public CObject
{
private:
	static const int TEXT_MAX = 17;
public:
	CGame();
	~CGame() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	CNumber_Manager* GetNumber_Manager() { return m_pNumber_Manager; }

private:
	C2DPolygon* m_pSea;
	CUI* m_pBg;
	C2DPolygon* m_pButton;
	C2DPolygon* m_pstone_bridge;
	C2DPolygon* m_pFlowObject;
	int m_nText[TEXT_MAX];
	CNumber_Manager* m_pNumber_Manager;



};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

D3DXVECTOR3 *GetPosWorld(void);
GAMESTATE GetGameState(void);
void SetGameState(GAMESTATE state);
PUSHSTATE*GetPushState();

#endif