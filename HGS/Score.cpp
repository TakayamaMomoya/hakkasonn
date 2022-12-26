#include "Score.h"
#include "manager.h"
#include "renderer.h"
#include "domino.h"

//プロトタイプ宣言
#define NUM_PLACE (3)

int g_nScore;
int g_nScorePoint;

LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;
D3DXVECTOR3 g_posPcore;

//======================================
//初期化処理
//======================================
void InitScore(void)
{
	
	//デバイスの取得
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();
	int nCntScore;

	

	g_nScore = 0;
	g_nScorePoint = 0;
	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"Data\\TEXTURE\\number001.png",
		&g_pTextureScore);

	//スコアの情報の初期化
	
		g_posPcore = D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタ
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	
		for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(((g_posPcore.x + 120 * nCntScore) - 100), (g_posPcore.y - 200), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(((g_posPcore.x + 120 * nCntScore) + 100), (g_posPcore.y - 200), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(((g_posPcore.x + 120 * nCntScore) - 100), (g_posPcore.y + 200), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(((g_posPcore.x + 120 * nCntScore) + 100), (g_posPcore.y + 200), 0.0f);

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
	
	
	
	g_pVtxBuffScore->Unlock();
}

//======================================
//終了処理
//======================================
void UninitScore(void)
{
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}
//======================================
//更新処理
//======================================
void UpdateScore(void)
{
	int nCntScore;
	int aTexU[NUM_PLACE];
	VERTEX_2D *pVtx;
	
	g_nScore += (g_nScorePoint - g_nScore) / 20;
	g_nScore++;
	if (g_nScore >= g_nScorePoint)
	{
		g_nScore = g_nScorePoint;
	}

	aTexU[0] = g_nScore % 1000 / 100;
	aTexU[1] = g_nScore % 100 / 10;
	aTexU[2] = g_nScore % 10 / 1;

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]) + 0.1f, 1.0f);

		pVtx += 4;
	}
	
	g_pVtxBuffScore->Unlock();
}

//======================================
//描画処理
//======================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//デバイスの取得
	CManager *pManager = GetManager();

	 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));


	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}

}
//======================================
//設定処理
//======================================
void SetScore(int nScore)
{
	
	int nCntScore;
	VERTEX_2D *pVtx;
	int aTexU[NUM_PLACE];
	g_nScore = nScore;

	
	aTexU[0] = g_nScore % 1000 / 100;
	aTexU[1] = g_nScore % 100 / 10;
	aTexU[2] = g_nScore % 10 / 1;

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]) + 0.1f, 1.0f);
	
		pVtx += 4;
	}
	g_pVtxBuffScore->Unlock();
}


int GetScore()
{
	return g_nScorePoint;
}