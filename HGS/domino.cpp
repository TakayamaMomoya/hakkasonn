//==================================================================================================
//
//Domino.cpp
//Author:髙山桃也
//
//==================================================================================================

#include "main.h"
#include "domino.h"
#include <stdlib.h>
#include "manager.h"
#include "game.h"
#include "input.h"

//マクロ定義
#define DOMINO_TEX				""										//テクスチャファイル名
#define DOMINO_HEIGHT			(100)									//高さ
#define DOMINO_WIDTH			(20)									//幅
#define ROLL_SPEED				(-0.16f)									//回転スピード
#define ROLL_LIMIT				(D3DX_PI * 0.35f)						//倒れきる角度

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureDomino[DOMINOTYPE_MAX] = {};				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDomino = NULL;						//頂点バッファへのポインタ
Domino g_aDomino[MAX_DOMINO];											//構造体
int g_nDominoNum;													//敵の総数

//プロトタイプ宣言
void UpdateDominoPos(Domino *pDomino);
void UpdateDominoPolygon(Domino *pDomino, int nCntDomino);
void ManageStateDomino(Domino *pDomino);

//==================================================================================================
//初期化処理
//==================================================================================================
void InitDomino(void)
{
	//変数宣言
	int nCntDomino;

	//変数初期化
	g_nDominoNum = 0;

	//デバイス取得
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//テクスチャ読み込み
	for (int nCntTex = 0;nCntTex < DOMINOTYPE_MAX;nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			DOMINO_TEX,
			&g_apTextureDomino[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DOMINO,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDomino,
		NULL);

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDomino->Lock(0, 0, (void**)&pVtx, 0);

	//情報取得
	Domino *pDomino = GetDomino();

	for (nCntDomino = 0; nCntDomino < MAX_DOMINO; nCntDomino++, pDomino++)
	{//全ての初期化

	    //情報の設定
		pDomino->pos = { 0.0f ,0.0f,0.0f };
		pDomino->rot = { 0.0f ,0.0f,0.0f };
		pDomino->posWorld = { 0.0f ,0.0f,0.0f };
		pDomino->col = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
		pDomino->state = DOMINOSTATE_NORMAL;
		pDomino->fAngle = 0;
		pDomino->fLength = 0;
		pDomino->bUse = false;

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
		pVtx[0].col = pDomino->col;
		pVtx[1].col = pDomino->col;
		pVtx[2].col = pDomino->col;
		pVtx[3].col = pDomino->col;

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffDomino->Unlock();
}

//==================================================================================================
//終了処理
//==================================================================================================
void UninitDomino(void)
{
	for (int nCnt = 0; nCnt < DOMINOTYPE_MAX; nCnt++)
	{
		if (g_apTextureDomino[nCnt] != NULL)
		{//テクスチャへのポインタ破棄
			g_apTextureDomino[nCnt]->Release();
			g_apTextureDomino[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffDomino != NULL)
	{//頂点バッファへのポインタ破棄
		g_pVtxBuffDomino->Release();
		g_pVtxBuffDomino = NULL;
	}
}

//==================================================================================================
//更新処理
//==================================================================================================
void UpdateDomino(void)
{
	//ポインタ宣言
	Domino *pDomino = GetDomino();

	//インプットのポインタを宣言
	CInput *pInput = CInput::GetKey();

	if (pDomino->state == DOMINOSTATE_NORMAL)
	{//最初のドミノが倒れる
		pDomino->state = DOMINOSTATE_DOWN;
	}

	for (int nCntDomino = 0; nCntDomino < MAX_DOMINO; nCntDomino++, pDomino++)
	{//全てをチェックする
		if (pDomino->bUse)
		{//使用している状態なら

			//位置更新処理
			UpdateDominoPos(pDomino);

			//ポリゴン更新処理
			UpdateDominoPolygon(pDomino,nCntDomino);

			if (GetGameState() == GAMESTATE_DOWN)
			{
				//状態更新処理
				ManageStateDomino(pDomino);
			}
		}
	}
}

//==================================================================================================
//位置更新処理
//==================================================================================================
void UpdateDominoPos(Domino *pDomino)
{
	//相対位置設定
	pDomino->pos = *GetPosWorld() + pDomino->posWorld;
}

//==================================================================================================
//ポリゴン更新処理
//==================================================================================================
void UpdateDominoPolygon(Domino *pDomino, int nCntDomino)
{
	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDomino->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntDomino;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3
	(
		pDomino->pos.x + sinf(pDomino->rot.z - D3DX_PI + pDomino->fAngle) * pDomino->fLength, 
		pDomino->pos.y + cosf(pDomino->rot.z - D3DX_PI + pDomino->fAngle) * pDomino->fLength,
		0.0f
	);

	pVtx[1].pos = D3DXVECTOR3
	(
		pDomino->pos.x + sinf(pDomino->rot.z + D3DX_PI - pDomino->fAngle) * pDomino->fLength,
		pDomino->pos.y + cosf(pDomino->rot.z + D3DX_PI - pDomino->fAngle) * pDomino->fLength,
		0.0f
	);

	pVtx[2].pos = D3DXVECTOR3
	(
		pDomino->pos.x + sinf(pDomino->rot.z - D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
		pDomino->pos.y + cosf(pDomino->rot.z - D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
		0.0f
	);

	pVtx[3].pos = D3DXVECTOR3
	(
		pDomino->pos.x + sinf(pDomino->rot.z + D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
		pDomino->pos.y + cosf(pDomino->rot.z + D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
		0.0f
	);

	//頂点バッファをアンロック
	g_pVtxBuffDomino->Unlock();
}

//==================================================================================================
//状態管理
//==================================================================================================
void ManageStateDomino(Domino *pDomino)
{
	switch (pDomino->state)
	{
	case DOMINOSTATE_NORMAL:
		break;
	case DOMINOSTATE_DOWN:

		pDomino->rot.z += ROLL_SPEED;

		if (pDomino->rot.z < -ROLL_LIMIT && pDomino[1].bUse)
		{
			pDomino->state = DOMINOSTATE_END;
			pDomino[1].state = DOMINOSTATE_DOWN;
		}
		else if (pDomino->rot.z < -D3DX_PI * 0.5f && pDomino[1].bUse == false)
		{
			pDomino->rot.z = -D3DX_PI * 0.5f;
			pDomino->state = DOMINOSTATE_END;
			SetGameState(GAMESTATE_END);
		}

		break;
	case DOMINOSTATE_END:
		break;
	}
}

//==================================================================================================
//設定処理
//==================================================================================================
void SetDomino(D3DXVECTOR3 pos)
{
	//情報取得
	Domino *pDomino = GetDomino();

	//頂点情報のポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDomino->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDomino = 0; nCntDomino < MAX_DOMINO; nCntDomino++, pDomino++)
	{//全てをチェックする
		if (pDomino->bUse == false)
		{
			//対角線の角度設定
			pDomino->fAngle = atan2f(DOMINO_WIDTH,DOMINO_HEIGHT);

			//対角線の長さ設定
			pDomino->fLength = sqrtf((DOMINO_WIDTH * 0.5f) * (DOMINO_WIDTH * 0.5f) + (DOMINO_HEIGHT) * (DOMINO_HEIGHT)) * 0.5f;

			//引数受け取り
			pDomino->posWorld = pos;
			pDomino->pos = pos;

			//使用状態にする
			pDomino->bUse = true;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3
			(
				pDomino->pos.x + sinf(pDomino->rot.z - D3DX_PI + pDomino->fAngle) * pDomino->fLength,
				pDomino->pos.y + cosf(pDomino->rot.z - D3DX_PI + pDomino->fAngle) * pDomino->fLength,
				0.0f
			);

			pVtx[1].pos = D3DXVECTOR3
			(
				pDomino->pos.x + sinf(pDomino->rot.z + D3DX_PI - pDomino->fAngle) * pDomino->fLength,
				pDomino->pos.y + cosf(pDomino->rot.z + D3DX_PI - pDomino->fAngle) * pDomino->fLength,
				0.0f
			);

			pVtx[2].pos = D3DXVECTOR3
			(
				pDomino->pos.x + sinf(pDomino->rot.z - D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
				pDomino->pos.y + cosf(pDomino->rot.z - D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
				0.0f
			);

			pVtx[3].pos = D3DXVECTOR3
			(
				pDomino->pos.x + sinf(pDomino->rot.z + D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
				pDomino->pos.y + cosf(pDomino->rot.z + D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
				0.0f
			);

			g_nDominoNum++;

			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffDomino->Unlock();
}

//==================================================================================================
//描画処理
//==================================================================================================
void DrawDomino(void)
{
	//デバイス取得
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//ポインタ宣言
	Domino *pDomino = GetDomino();

	//変数宣言
	int nCntDomino;

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffDomino, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntDomino = 0; nCntDomino < MAX_DOMINO; nCntDomino++, pDomino++)
	{//全てをチェックする
		if (pDomino->bUse == true)
		{//使用している状態なら

			//テクスチャ設定
			pDevice->SetTexture(0, g_apTextureDomino[0]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDomino * 4, 2);
		}
	}
}

//==================================================================================================
//ポインタ取得処理
//==================================================================================================
Domino *GetDomino(void)
{
	return &g_aDomino[0];
}

//==================================================================================================
//敵数処理
//==================================================================================================
int GetDominoNum(void)
{
	return g_nDominoNum;
}