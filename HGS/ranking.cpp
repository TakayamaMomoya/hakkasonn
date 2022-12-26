#include <stdio.h>
#include <stdlib.h>
#include "Ranking.h"
#include "manager.h"
#include "renderer.h"
#include "domino.h"
#include "game.h"

//マクロ定義
#define MAX_NUMBER (5)
#define NUM_PLACERANKING (3)
#define INTERVAL (30)

//プロトタイプ宣言

int Sort();
//グローバル変数
int g_nRanking[MAX_NUMBER];
int g_nRankingPoint[MAX_NUMBER];
int nNumber[MAX_NUMBER];
bool g_bDisp = false;
int g_nDispCount;
int g_nTarget = -1;

LPDIRECT3DTEXTURE9 g_pTextureRanking = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;
D3DXVECTOR3 g_posPcoreRanking;

//======================================
//初期化処理
//======================================
void InitRanking(void)
{

	//デバイスの取得
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();
	int nCntRanking;


	for (int nCnt = 0; nCnt < MAX_NUMBER; nCnt++)
	{
		g_nRanking[nCnt] = 0;
	}
	g_bDisp = true;
	g_nDispCount = 0;
	g_nTarget = -1;

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"Data\\TEXTURE\\number_pop.png",
		&g_pTextureRanking);

	//スコアの情報の初期化

	g_posPcoreRanking = D3DXVECTOR3(100.0f, 200.0f / 2, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACERANKING * MAX_NUMBER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_NUMBER; nCnt++)
	{
		for (nCntRanking = 0; nCntRanking < NUM_PLACERANKING; nCntRanking++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(((g_posPcoreRanking.x + 70 * nCntRanking) - 40), (g_posPcoreRanking.y + 150 * nCnt - 80), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(((g_posPcoreRanking.x + 70 * nCntRanking) + 40), (g_posPcoreRanking.y + 150 * nCnt - 80), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(((g_posPcoreRanking.x + 70 * nCntRanking) - 40), (g_posPcoreRanking.y + 150 * nCnt + 80), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(((g_posPcoreRanking.x + 70 * nCntRanking) + 40), (g_posPcoreRanking.y + 150 * nCnt + 80), 0.0f);

			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;
		}
	}
	g_pVtxBuffRanking->Unlock();
	Sort();
}

//======================================
//終了処理
//======================================
void UninitRanking(void)
{
	if (g_pTextureRanking != NULL)
	{
		g_pTextureRanking->Release();
		g_pTextureRanking = NULL;
	}
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}
//======================================
//更新処理
//======================================
void UpdateRanking(void)
{
	int nCntRanking;
	int aTexU[MAX_NUMBER][NUM_PLACERANKING];
	VERTEX_2D *pVtx;

	g_nDispCount++;
	if (g_nDispCount >= INTERVAL)
	{//カウンターがインターバルを超えたとき表示を切り替える
		g_nDispCount = 0;
		if (g_bDisp == true)
		{
			g_bDisp = false;
		}
		else
		{
			g_bDisp = true;
		}
	}

	for (int nCnt = 0; nCnt < MAX_NUMBER; nCnt++)
	{
		g_nRankingPoint[nCnt] = nNumber[nCnt];
		g_nRanking[nCnt] += (g_nRankingPoint[nCnt] - g_nRanking[nCnt]) / 20;
		g_nRanking[nCnt]++;
		if (g_nRanking[nCnt] >= g_nRankingPoint[nCnt])
		{
			g_nRanking[nCnt] = g_nRankingPoint[nCnt];
		}

		aTexU[nCnt][0] = g_nRanking[nCnt] % 1000 / 100;
		aTexU[nCnt][1] = g_nRanking[nCnt] % 100 / 10;
		aTexU[nCnt][2] = g_nRanking[nCnt] % 10 / 1;
	}


	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_NUMBER; nCnt++)
	{
		for (int nCntRanking = 0; nCntRanking < NUM_PLACERANKING; nCntRanking++)
		{
			pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCnt][nCntRanking]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCnt][nCntRanking]) + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCnt][nCntRanking]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCnt][nCntRanking]) + 0.1f, 1.0f);

			pVtx += 4;
		}
	}

	g_pVtxBuffRanking->Unlock();
}

//======================================
//描画処理
//======================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntRanking;

	//デバイスの取得
	CManager *pManager = GetManager();

	pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));


	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRanking);
	for (int nCnt = 0; nCnt < MAX_NUMBER; nCnt++)
	{
		for (nCntRanking = 0; nCntRanking < NUM_PLACERANKING; nCntRanking++)
		{
			if (nCnt == g_nTarget)
			{
				if (g_bDisp == true)
				{
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCnt * NUM_PLACERANKING + nCntRanking )* 4, 2);
				}
			}
			else
			{
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCnt * NUM_PLACERANKING + nCntRanking) * 4, 2);
			}
		}
	}
	

}
//======================================
//設定処理
//======================================
void SetRanking(int nRanking)
{

	int nCntRanking;
	VERTEX_2D *pVtx;
	int aTexU[MAX_NUMBER][NUM_PLACERANKING];
	for (int nCnt1 = 0; nCnt1 < MAX_NUMBER; nCnt1++)
	{
		g_nRanking[nCnt1] = nNumber[nCnt1];

		aTexU[nCnt1][0] = g_nRanking[nCnt1] % 1000 / 100;
		aTexU[nCnt1][1] = g_nRanking[nCnt1] % 100 / 10;
		aTexU[nCnt1][2] = g_nRanking[nCnt1] % 10 / 1;
	}								
	

	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	
	
	g_pVtxBuffRanking->Unlock();
}

//======================================
//入力処理
//======================================
int Sort()
{
	PUSHSTATE*pPushState = GetPushState();
	FILE*pFile;
	int nCount;
	int nCheck = 0;
	int nData;
	int nTemp;

	int nAdd = pPushState->nPushCount;
	
	//読み込み
	pFile = fopen("Ranking.txt", "r");
	if (pFile != NULL)
	{
		for (nCount = 0; nCount < MAX_NUMBER; nCount++)
		{
			fscanf(pFile, "%d\n", &nNumber[nCount]);
		}
		fclose(pFile);
	}
	else
	{
		for (nCount = 0; nCount < MAX_NUMBER; nCount++)
		{
			nNumber[nCount] = 0;
		}
	
	}
	

	for (nCheck = 0; nCheck < MAX_NUMBER - 1; nCheck++)
	{
		nData = nCheck;
		for (nCount = (nCheck + 1); nCount < MAX_NUMBER; nCount++)
		{
			if (nNumber[nData] < nNumber[nCount])
			{
				nData = nCount;
			}
		}
		nTemp = nNumber[nCheck];

		nNumber[nCheck] = nNumber[nData];
		nNumber[nData] = nTemp;
	}

    //今回のスコアと比較
	scanf("%d", &nAdd);
	if (nAdd > nNumber[MAX_NUMBER - 1])
	{
		nNumber[MAX_NUMBER - 1] = nAdd;
	}

	for (nCount = 0; nCount < MAX_NUMBER - 1; nCount++)
	{
		for (nCheck = (nCount + 1); nCheck < MAX_NUMBER; nCheck++)
		{
			nTemp = nNumber[nCheck];
			if (nNumber[nCount] < nNumber[nCheck])
			{
				nNumber[nCheck] = nNumber[nCount];
				nNumber[nCount] = nTemp;
			}
		}
	}
	
	//書き込み
	pFile = fopen("Ranking.txt", "w");
	if (pFile != NULL)
	{
		for (nCount = 0; nCount < MAX_NUMBER; nCount++)
		{
			fprintf(pFile, "%d\n", nNumber[nCount]);
		}
		fclose(pFile);
	}

	for (int nCnt = 0; nCnt < MAX_NUMBER; nCnt++)
	{
		if (nNumber[nCnt] == pPushState->nPushCount)
		{
			g_nTarget = nCnt;
		}
	}

	return 0;
}