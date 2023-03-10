//==================================================================================================
//
//Hand.cpp
//Author:ûüRç
//
//==================================================================================================

#include "main.h"
#include "hand.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "domino.h"

//}Nè`
#define HAND_TEX			"data\\TEXTURE\\è.png"					//eNX`t@C¼
#define HAND_HEIGHT			(300)									//³
#define HAND_WIDTH			(300)									//
#define NUM_POS_DEST		(3)										//ÚWn_Ì
#define HAND_FACT			(0.1f)									//nhÌÚ®W
#define LIMIT_DIFF			(50.0f)									//ÌL[É¤Âé·ªl

//O[oÏé¾
LPDIRECT3DTEXTURE9 g_pTextureHand = NULL;							//eNX`ÖÌ|C^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHand = NULL;						//¸_obt@ÖÌ|C^
Hand g_hand;														//\¢Ì
D3DXVECTOR3 g_aPosDestHand[NUM_POS_DEST];							//ÚWn_

//vg^Cvé¾
void UpdateHandPos(Hand *pHand);
void UpdateHandPolygon(Hand *pHand);
void ManageStateHand(Hand *pHand);

//==================================================================================================
//ú»
//==================================================================================================
void InitHand(void)
{
	//foCXæ¾
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//eNX`ÇÝÝ
	D3DXCreateTextureFromFile(pDevice,
		HAND_TEX,
		&g_pTextureHand);

	//¸_obt@Ì¶¬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHand,
		NULL);

	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffHand->Lock(0, 0, (void**)&pVtx, 0);

	//îñæ¾
	Hand *pHand = GetHand();

	//ÚWn_Ýè
	g_aPosDestHand[0] = { SCREEN_WIDTH * 0.2f,SCREEN_HEIGHT * 0.7f,0.0f };
	g_aPosDestHand[1] = { SCREEN_WIDTH * 0.0f,SCREEN_HEIGHT * 0.7f,0.0f };
	g_aPosDestHand[2] = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT * 0.7f,0.0f };

	//îñÌÝè
	pHand->pos = { g_aPosDestHand[0].x ,g_aPosDestHand[0].y,0.0f };
	pHand->posWorld = { g_aPosDestHand[0].x ,g_aPosDestHand[0].y,0.0f };
	pHand->col = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
	pHand->state = HANDSTATE_NORMAL;
	pHand->nMovekey = 0;

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
	pVtx[0].col = pHand->col;
	pVtx[1].col = pHand->col;
	pVtx[2].col = pHand->col;
	pVtx[3].col = pHand->col;

	//eNX`ÀW
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//¸_obt@ðAbN
	g_pVtxBuffHand->Unlock();
}

//==================================================================================================
//I¹
//==================================================================================================
void UninitHand(void)
{
	if (g_pTextureHand != NULL)
	{//eNX`ÖÌ|C^jü
		g_pTextureHand->Release();
		g_pTextureHand = NULL;
	}

	if (g_pVtxBuffHand != NULL)
	{//¸_obt@ÖÌ|C^jü
		g_pVtxBuffHand->Release();
		g_pVtxBuffHand = NULL;
	}
}

//==================================================================================================
//XV
//==================================================================================================
void UpdateHand(void)
{
	//îñæ¾
	Hand *pHand = GetHand();

	if (pHand->state == HANDSTATE_PUSH)
	{
		//ÊuXV
		UpdateHandPos(pHand);
	}

	//|SXV
	UpdateHandPolygon(pHand);
}

//==================================================================================================
//ÊuXV
//==================================================================================================
void UpdateHandPos(Hand *pHand)
{
	//·ªÌæ¾
	D3DXVECTOR3 posDiff = (g_aPosDestHand[pHand->nMovekey] - pHand->posWorld);
	posDiff.x = posDiff.x * posDiff.x;
	posDiff.y = posDiff.y * posDiff.y;

	//ÚWÖÚ®
	pHand->posWorld += (g_aPosDestHand[pHand->nMovekey] - pHand->posWorld) * HAND_FACT;

	if (posDiff.x + posDiff.y < LIMIT_DIFF * LIMIT_DIFF)
	{//·ªªêèÈºÅÌL[Ö
		if (pHand->nMovekey < NUM_POS_DEST - 1)
		{//L[ðißé
			pHand->nMovekey++;
		}
		else if(GetGameState() == GAMESTATE_PUSH)
		{//Q[óÔðÏX
			SetGameState(GAMESTATE_DOWN);
		}
	}

	//ÎÊuÝè
	pHand->pos = *GetPosWorld() + pHand->posWorld;
}

//==================================================================================================
//|SXV
//==================================================================================================
void UpdateHandPolygon(Hand *pHand)
{
	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffHand->Lock(0, 0, (void**)&pVtx, 0);

	//¸_ÀWÌÝè
	pVtx[0].pos = D3DXVECTOR3(pHand->pos.x - HAND_WIDTH * 0.5f, pHand->pos.y - HAND_HEIGHT * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pHand->pos.x + HAND_WIDTH * 0.5f, pHand->pos.y - HAND_HEIGHT * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pHand->pos.x - HAND_WIDTH * 0.5f, pHand->pos.y + HAND_HEIGHT * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pHand->pos.x + HAND_WIDTH * 0.5f, pHand->pos.y + HAND_HEIGHT * 0.5f, 0.0f);

	//¸_obt@ðAbN
	g_pVtxBuffHand->Unlock();
}

//==================================================================================================
//óÔÇ
//==================================================================================================
void ManageStateHand(Hand *pHand)
{
}

//==================================================================================================
//`æ
//==================================================================================================
void DrawHand(void)
{
	//foCXæ¾
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//¸_obt@ðf[^Xg[ÉÝè
	pDevice->SetStreamSource(0, g_pVtxBuffHand, 0, sizeof(VERTEX_2D));

	//¸_tH[}bgÌÝè
	pDevice->SetFVF(FVF_VERTEX_2D);

	//eNX`Ýè
	pDevice->SetTexture(0, g_pTextureHand);

	//|SÌ`æ
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==================================================================================================
//|C^æ¾
//==================================================================================================
Hand *GetHand(void)
{
	return &g_hand;
}