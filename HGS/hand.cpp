//==================================================================================================
//
//Hand.cpp
//Author:髙山桃也
//
//==================================================================================================

#include "main.h"
#include "hand.h"
#include "manager.h"
#include "game.h"
#include "input.h"

//マクロ定義
#define HAND_TEX				""									//テクスチャファイル名
#define HAND_HEIGHT			(100)									//高さ
#define HAND_WIDTH			(100)									//幅

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureHand = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHand = NULL;						//頂点バッファへのポインタ
Hand g_hand;														//構造体
int g_nHandNum;														//敵の総数

//プロトタイプ宣言
void UpdateHandPos(Hand *pHand);
void UpdateHandPolygon(Hand *pHand, int nCntHand);
void ManageStateHand(Hand *pHand);

//==================================================================================================
//初期化処理
//==================================================================================================
void InitHand(void)
{
	//変数宣言
	int nCntHand;

	//変数初期化
	g_nHandNum = 0;

	//デバイス取得
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		HAND_TEX,
		&g_pTextureHand);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHand,
		NULL);

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHand->Lock(0, 0, (void**)&pVtx, 0);

	//情報取得
	Hand *pHand = GetHand();

	//情報の設定
	pHand->pos = { 0.0f ,0.0f,0.0f };
	pHand->posWorld = { 0.0f ,0.0f,0.0f };
	pHand->col = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
	pHand->state = HANDSTATE_NORMAL;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = pHand->col;
	pVtx[1].col = pHand->col;
	pVtx[2].col = pHand->col;
	pVtx[3].col = pHand->col;

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffHand->Unlock();
}

//==================================================================================================
//終了処理
//==================================================================================================
void UninitHand(void)
{
	if (g_pTextureHand != NULL)
	{//テクスチャへのポインタ破棄
		g_pTextureHand->Release();
		g_pTextureHand = NULL;
	}


	if (g_pVtxBuffHand != NULL)
	{//頂点バッファへのポインタ破棄
		g_pVtxBuffHand->Release();
		g_pVtxBuffHand = NULL;
	}
}

//==================================================================================================
//更新処理
//==================================================================================================
void UpdateHand(void)
{
}

//==================================================================================================
//位置更新処理
//==================================================================================================
void UpdateHandPos(Hand *pHand)
{
	//相対位置設定
	pHand->pos = *GetPosWorld() + pHand->posWorld;
}

//==================================================================================================
//ポリゴン更新処理
//==================================================================================================
void UpdateHandPolygon(Hand *pHand, int nCntHand)
{
}

//==================================================================================================
//状態管理
//==================================================================================================
void ManageStateHand(Hand *pHand)
{
}

//==================================================================================================
//描画処理
//==================================================================================================
void DrawHand(void)
{
}

//==================================================================================================
//ポインタ取得処理
//==================================================================================================
Hand *GetHand(void)
{
}