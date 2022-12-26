//==================================================================================================
//
//Sign.cpp
//Author:髙山桃也
//
//==================================================================================================

#include "main.h"
#include "domino.h"
#include "sign.h"
#include <stdlib.h>
#include "manager.h"
#include "game.h"
#include "input.h"
#include "number_manager.h"

//マクロ定義
#define SIGN_TEX				""										//テクスチャファイル名
#define NUM_COL					(4)										//色の種類

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureSign[SIGNTYPE_MAX] = {};				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSign = NULL;						//頂点バッファへのポインタ
Sign g_aSign[MAX_SIGN];												//構造体
int g_nSignNum;														//総数
const char *c_apFilenameBlock[SIGNTYPE_MAX] =
{
	"data\\TEXTURE\\看板.png",
	"data\\TEXTURE\\看板.png",
	"data\\TEXTURE\\看板.png"
};																	//テクスチャファイル名

//プロトタイプ宣言
void UpdateSignPos(Sign *pSign);
void UpdateSignPolygon(Sign *pSign, int nCntSign);

//==================================================================================================
//初期化処理
//==================================================================================================
void InitSign(void)
{
	//変数宣言
	int nCntSign;

	//変数初期化
	g_nSignNum = 0;

	//デバイス取得
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//テクスチャ読み込み
	for (int nCntTex = 0;nCntTex < SIGNTYPE_MAX;nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameBlock[nCntTex],
			&g_apTextureSign[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SIGN,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSign,
		NULL);

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSign->Lock(0, 0, (void**)&pVtx, 0);

	//情報取得
	Sign *pSign = GetSign();

	for (nCntSign = 0; nCntSign < MAX_SIGN; nCntSign++, pSign++)
	{//全ての初期化

	    //情報の設定
		pSign->pos = { 0.0f ,0.0f,0.0f };
		pSign->rot = { 0.0f ,0.0f,0.0f };
		pSign->posWorld = { 0.0f ,0.0f,0.0f };
		pSign->col = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
		pSign->type = SIGNTYPE_10;
		pSign->width = 0;
		pSign->height = 0;
		pSign->bUse = false;

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
		pVtx[0].col = pSign->col;
		pVtx[1].col = pSign->col;
		pVtx[2].col = pSign->col;
		pVtx[3].col = pSign->col;

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffSign->Unlock();
}

//==================================================================================================
//終了処理
//==================================================================================================
void UninitSign(void)
{
	for (int nCnt = 0; nCnt < SIGNTYPE_MAX; nCnt++)
	{
		if (g_apTextureSign[nCnt] != NULL)
		{//テクスチャへのポインタ破棄
			g_apTextureSign[nCnt]->Release();
			g_apTextureSign[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffSign != NULL)
	{//頂点バッファへのポインタ破棄
		g_pVtxBuffSign->Release();
		g_pVtxBuffSign = NULL;
	}
}

//==================================================================================================
//更新処理
//==================================================================================================
void UpdateSign(void)
{
	//ポインタ宣言
	Sign *pSign = GetSign();

	//インプットのポインタを宣言
	CInput *pInput = CInput::GetKey();

	for (int nCntSign = 0; nCntSign < MAX_SIGN; nCntSign++, pSign++)
	{//全てをチェックする
		if (pSign->bUse)
		{//使用している状態なら

			//位置更新処理
			UpdateSignPos(pSign);

			//ポリゴン更新処理
			UpdateSignPolygon(pSign, nCntSign);
		}
	}
}

//==================================================================================================
//位置更新処理
//==================================================================================================
void UpdateSignPos(Sign *pSign)
{
	//相対位置設定
	pSign->pos = *GetPosWorld() + pSign->posWorld;
}

//==================================================================================================
//ポリゴン更新処理
//==================================================================================================
void UpdateSignPolygon(Sign *pSign, int nCntSign)
{
	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSign->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntSign;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3
	(
		pSign->pos.x - pSign->width * 0.5f,
		pSign->pos.y - pSign->height,
		0.0f
	);

	pVtx[1].pos = D3DXVECTOR3
	(
		pSign->pos.x + pSign->width * 0.5f,
		pSign->pos.y - pSign->height,
		0.0f
	);

	pVtx[2].pos = D3DXVECTOR3
	(
		pSign->pos.x - pSign->width * 0.5f,
		pSign->pos.y,
		0.0f
	);

	pVtx[3].pos = D3DXVECTOR3
	(
		pSign->pos.x + pSign->width * 0.5f,
		pSign->pos.y,
		0.0f
	);

	//頂点バッファをアンロック
	g_pVtxBuffSign->Unlock();
}

//==================================================================================================
//設定処理
//==================================================================================================
void SetSign(D3DXVECTOR3 pos, SIGNTYPE type, int nDominoNumber)
{
	float fSize = 0.0f;
	//ナンバーマネージャーの取得
	CManager *pManager = GetManager();
	CGame* pGame = (CGame*)pManager->GetGameObject();
	CNumber_Manager* pNumber_Manager = pGame->GetNumber_Manager();

	//情報取得
	Sign *pSign = GetSign();

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSign->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSign = 0; nCntSign < MAX_SIGN; nCntSign++, pSign++)
	{//全てをチェックする
		if (pSign->bUse == false)
		{
			switch (type)
			{
			case SIGNTYPE_10:
				
				pSign->width = 150;
				pSign->height = 150;
				
				break;
			case SIGNTYPE_50:

				pSign->width = 200;
				pSign->height = 200;

				break;
			case SIGNTYPE_100:

				pSign->width = 250;
				pSign->height = 250;

				break;
			}


			//引数受け取り
			pSign->posWorld = pos;
			pSign->pos = pos;

			//使用状態にする
			pSign->bUse = true;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3
			(
				pSign->pos.x - pSign->width * 0.5f,
				pSign->pos.y - pSign->height * 0.5f,
				0.0f
			);

			pVtx[1].pos = D3DXVECTOR3
			(
				pSign->pos.x + pSign->width * 0.5f,
				pSign->pos.y - pSign->height * 0.5f,
				0.0f
			);

			pVtx[2].pos = D3DXVECTOR3
			(
				pSign->pos.x - pSign->width * 0.5f,
				pSign->pos.y + pSign->height * 0.5f,
				0.0f
			);

			pVtx[3].pos = D3DXVECTOR3
			(
				pSign->pos.x + pSign->width * 0.5f,
				pSign->pos.y + pSign->height * 0.5f,
				0.0f
			);

			//ナンバーの生成
			pNumber_Manager->SetNumber(D3DXVECTOR3(pos.x,pos.y - 100.0f,0.0f), nDominoNumber);
			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffSign->Unlock();
}

//==================================================================================================
//描画処理
//==================================================================================================
void DrawSign(void)
{
	//デバイス取得
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//ポインタ宣言
	Sign *pSign = GetSign();

	//変数宣言
	int nCntSign;

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSign, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntSign = 0; nCntSign < MAX_SIGN; nCntSign++, pSign++)
	{//全てをチェックする
		if (pSign->bUse == true)
		{//使用している状態なら

			//テクスチャ設定
			pDevice->SetTexture(0, g_apTextureSign[0]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSign * 4, 2);
		}
	}
}

//==================================================================================================
//ポインタ取得処理
//==================================================================================================
Sign *GetSign(void)
{
	return &g_aSign[0];
}

//==================================================================================================
//数処理
//==================================================================================================
int GetSignNum(void)
{
	return g_nSignNum;
}