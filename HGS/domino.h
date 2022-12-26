//==================================================================================================
//
//Domino.h
//Author:髙山桃也
//
//==================================================================================================

#ifndef _DOMINO_H_
#define _DOMINO_H_

#include "main.h"

//マクロ定義
#define MAX_DOMINO			(128)											//敵の最大数

static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

//種類
typedef enum
{
	DOMINOTYPE_NORMAL = 0,		//通常敵
	DOMINOTYPE_MAX
}DOMINOTYPE;

//状態
typedef enum
{
	DOMINOSTATE_NORMAL = 0,		//通常状態
	DOMINOSTATE_DOWN,			//倒れてる状態
	DOMINOSTATE_END,			//倒れ切った状態
	DOMINOSTATE_MAX
}DOMINOSTATE;

//構造体定義
typedef struct
{
	D3DXVECTOR3 pos;			//現在位置
	D3DXVECTOR3 posWorld;		//原点からの相対位置
	D3DXVECTOR3 rot;			//向き
	D3DXCOLOR col;				//色
	DOMINOTYPE type;			//種類
	DOMINOSTATE state;			//状態
	bool bUse;					//使用しているかどうか
	float fAngle;				//対角線の角度
	float fLength;				//対角線の長さ
}Domino;

//プロトタイプ宣言
void InitDomino(void);
void UninitDomino(void);
void UpdateDomino(void);
void DrawDomino(void);
void SetDomino(D3DXVECTOR3 pos);
Domino *GetDomino(void);
int GetDominoNum(void);

#endif