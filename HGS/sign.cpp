//==================================================================================================
//
//Sign.cpp
//Author:ûüRç
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

//}Nè`
#define SIGN_TEX				""										//eNX`t@C¼
#define NUM_COL					(4)										//FÌíÞ

//O[oÏé¾
LPDIRECT3DTEXTURE9 g_apTextureSign[SIGNTYPE_MAX] = {};				//eNX`ÖÌ|C^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSign = NULL;						//¸_obt@ÖÌ|C^
Sign g_aSign[MAX_SIGN];												//\¢Ì
int g_nSignNum;														//
const char *c_apFilenameBlock[SIGNTYPE_MAX] =
{
	"data\\TEXTURE\\ÅÂ.png",
	"data\\TEXTURE\\ÅÂ.png",
	"data\\TEXTURE\\ÅÂ.png"
};																	//eNX`t@C¼

//vg^Cvé¾
void UpdateSignPos(Sign *pSign);
void UpdateSignPolygon(Sign *pSign, int nCntSign);

//==================================================================================================
//ú»
//==================================================================================================
void InitSign(void)
{
	//Ïé¾
	int nCntSign;

	//Ïú»
	g_nSignNum = 0;

	//foCXæ¾
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//eNX`ÇÝÝ
	for (int nCntTex = 0;nCntTex < SIGNTYPE_MAX;nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameBlock[nCntTex],
			&g_apTextureSign[nCntTex]);
	}

	//¸_obt@Ì¶¬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SIGN,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSign,
		NULL);

	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffSign->Lock(0, 0, (void**)&pVtx, 0);

	//îñæ¾
	Sign *pSign = GetSign();

	for (nCntSign = 0; nCntSign < MAX_SIGN; nCntSign++, pSign++)
	{//SÄÌú»

	    //îñÌÝè
		pSign->pos = { 0.0f ,0.0f,0.0f };
		pSign->rot = { 0.0f ,0.0f,0.0f };
		pSign->posWorld = { 0.0f ,0.0f,0.0f };
		pSign->col = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
		pSign->type = SIGNTYPE_10;
		pSign->width = 0;
		pSign->height = 0;
		pSign->bUse = false;

		//¸_ÀWÌÝè
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);

		//rhwÌÝè
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//¸_J[ÌÝè
		pVtx[0].col = pSign->col;
		pVtx[1].col = pSign->col;
		pVtx[2].col = pSign->col;
		pVtx[3].col = pSign->col;

		//eNX`ÀW
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//¸_f[^Ì|C^ð4Âªißé
		pVtx += 4;
	}

	//¸_obt@ðAbN
	g_pVtxBuffSign->Unlock();
}

//==================================================================================================
//I¹
//==================================================================================================
void UninitSign(void)
{
	for (int nCnt = 0; nCnt < SIGNTYPE_MAX; nCnt++)
	{
		if (g_apTextureSign[nCnt] != NULL)
		{//eNX`ÖÌ|C^jü
			g_apTextureSign[nCnt]->Release();
			g_apTextureSign[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffSign != NULL)
	{//¸_obt@ÖÌ|C^jü
		g_pVtxBuffSign->Release();
		g_pVtxBuffSign = NULL;
	}
}

//==================================================================================================
//XV
//==================================================================================================
void UpdateSign(void)
{
	//|C^é¾
	Sign *pSign = GetSign();

	//CvbgÌ|C^ðé¾
	CInput *pInput = CInput::GetKey();

	for (int nCntSign = 0; nCntSign < MAX_SIGN; nCntSign++, pSign++)
	{//SÄð`FbN·é
		if (pSign->bUse)
		{//gpµÄ¢éóÔÈç

			//ÊuXV
			UpdateSignPos(pSign);

			//|SXV
			UpdateSignPolygon(pSign, nCntSign);
		}
	}
}

//==================================================================================================
//ÊuXV
//==================================================================================================
void UpdateSignPos(Sign *pSign)
{
	//ÎÊuÝè
	pSign->pos = *GetPosWorld() + pSign->posWorld;
}

//==================================================================================================
//|SXV
//==================================================================================================
void UpdateSignPolygon(Sign *pSign, int nCntSign)
{
	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffSign->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntSign;

	//¸_ÀWÌÝè
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

	//¸_obt@ðAbN
	g_pVtxBuffSign->Unlock();
}

//==================================================================================================
//Ýè
//==================================================================================================
void SetSign(D3DXVECTOR3 pos, SIGNTYPE type, int nDominoNumber)
{
	float fSize = 50.0f;
	float fPosy = -100.0f; 
	//io[}l[W[Ìæ¾
	CManager *pManager = GetManager();
	CGame* pGame = (CGame*)pManager->GetGameObject();
	CNumber_Manager* pNumber_Manager = pGame->GetNumber_Manager();

	//îñæ¾
	Sign *pSign = GetSign();

	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffSign->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSign = 0; nCntSign < MAX_SIGN; nCntSign++, pSign++)
	{//SÄð`FbN·é
		if (pSign->bUse == false)
		{
			switch (type)
			{
			case SIGNTYPE_10:
				fPosy = -100.0f;
				fSize = 50.0f;
				pSign->width = 150;
				pSign->height = 150;
				
				break;
			case SIGNTYPE_50:
				fPosy = -130.0f;
				fSize = 60.0f;
				pSign->width = 200;
				pSign->height = 200;

				break;
			case SIGNTYPE_100:
				fPosy = -160.0f;
				fSize = 70.0f;
				pSign->width = 250;
				pSign->height = 250;

				break;
			}


			//øó¯æè
			pSign->posWorld = pos;
			pSign->pos = pos;

			//gpóÔÉ·é
			pSign->bUse = true;

			//¸_ÀWÌÝè
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

			//io[Ì¶¬
			pNumber_Manager->SetNumber(D3DXVECTOR3(pos.x, pos.y + fPosy, 0.0f), fSize, nDominoNumber);
			break;
		}

		pVtx += 4;
	}

	//¸_obt@ðAbN
	g_pVtxBuffSign->Unlock();
}

//==================================================================================================
//`æ
//==================================================================================================
void DrawSign(void)
{
	//foCXæ¾
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//|C^é¾
	Sign *pSign = GetSign();

	//Ïé¾
	int nCntSign;

	//¸_obt@ðf[^Xg[ÉÝè
	pDevice->SetStreamSource(0, g_pVtxBuffSign, 0, sizeof(VERTEX_2D));

	//¸_tH[}bgÌÝè
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntSign = 0; nCntSign < MAX_SIGN; nCntSign++, pSign++)
	{//SÄð`FbN·é
		if (pSign->bUse == true)
		{//gpµÄ¢éóÔÈç

			//eNX`Ýè
			pDevice->SetTexture(0, g_apTextureSign[0]);

			//|SÌ`æ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSign * 4, 2);
		}
	}
}

//==================================================================================================
//|C^æ¾
//==================================================================================================
Sign *GetSign(void)
{
	return &g_aSign[0];
}

//==================================================================================================
//
//==================================================================================================
int GetSignNum(void)
{
	return g_nSignNum;
}