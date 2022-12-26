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
#include "domino.h"

//マクロ定義
#define HAND_TEX			"data\\TEXTURE\\手.png"					//テクスチャファイル名
#define HAND_HEIGHT			(300)									//高さ
#define HAND_WIDTH			(300)									//幅
#define NUM_POS_DEST		(3)										//目標地点の数
#define HAND_FACT			(0.1f)									//ハンドの移動係数
#define LIMIT_DIFF			(50.0f)									//次のキーにうつる差分値

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureHand = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHand = NULL;						//頂点バッファへのポインタ
Hand g_hand;														//構造体
D3DXVECTOR3 g_aPosDestHand[NUM_POS_DEST];							//目標地点

//プロトタイプ宣言
void UpdateHandPos(Hand *pHand);
void UpdateHandPolygon(Hand *pHand);
void ManageStateHand(Hand *pHand);

//==================================================================================================
//初期化処理
//==================================================================================================
void InitHand(void)
{
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

	//目標地点設定
	g_aPosDestHand[0] = { SCREEN_WIDTH * 0.2f,SCREEN_HEIGHT * 0.55f,0.0f };
	g_aPosDestHand[1] = { SCREEN_WIDTH * 0.0f,SCREEN_HEIGHT * 0.55f,0.0f };
	g_aPosDestHand[2] = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT * 0.55f,0.0f };

	//情報の設定
	pHand->pos = { g_aPosDestHand[0].x ,g_aPosDestHand[0].y,0.0f };
	pHand->posWorld = { g_aPosDestHand[0].x ,g_aPosDestHand[0].y,0.0f };
	pHand->col = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
	pHand->state = HANDSTATE_NORMAL;
	pHand->nMovekey = 0;

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
	//情報取得
	Hand *pHand = GetHand();

	if (pHand->state == HANDSTATE_PUSH)
	{
		//位置更新処理
		UpdateHandPos(pHand);
	}

	//ポリゴン更新処理
	UpdateHandPolygon(pHand);
}

//==================================================================================================
//位置更新処理
//==================================================================================================
void UpdateHandPos(Hand *pHand)
{
	//差分の取得
	D3DXVECTOR3 posDiff = (g_aPosDestHand[pHand->nMovekey] - pHand->posWorld);
	posDiff.x = posDiff.x * posDiff.x;
	posDiff.y = posDiff.y * posDiff.y;

	//目標へ移動
	pHand->posWorld += (g_aPosDestHand[pHand->nMovekey] - pHand->posWorld) * HAND_FACT;

	if (posDiff.x + posDiff.y < LIMIT_DIFF * LIMIT_DIFF)
	{//差分が一定以下で次のキーへ
		if (pHand->nMovekey < NUM_POS_DEST - 1)
		{//キーを進める
			pHand->nMovekey++;
		}
		else if(GetGameState() == GAMESTATE_PUSH)
		{//ゲーム状態を変更
			SetGameState(GAMESTATE_DOWN);
		}
	}

	//相対位置設定
	pHand->pos = *GetPosWorld() + pHand->posWorld;
}

//==================================================================================================
//ポリゴン更新処理
//==================================================================================================
void UpdateHandPolygon(Hand *pHand)
{
	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHand->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pHand->pos.x - HAND_WIDTH * 0.5f, pHand->pos.y - HAND_HEIGHT * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pHand->pos.x + HAND_WIDTH * 0.5f, pHand->pos.y - HAND_HEIGHT * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pHand->pos.x - HAND_WIDTH * 0.5f, pHand->pos.y + HAND_HEIGHT * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pHand->pos.x + HAND_WIDTH * 0.5f, pHand->pos.y + HAND_HEIGHT * 0.5f, 0.0f);

	//頂点バッファをアンロック
	g_pVtxBuffHand->Unlock();
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
	//デバイス取得
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHand, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureHand);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==================================================================================================
//ポインタ取得処理
//==================================================================================================
Hand *GetHand(void)
{
	return &g_hand;
}