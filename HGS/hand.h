//==================================================================================================
//
//Hand.h
//Author:髙山桃也
//
//==================================================================================================

#ifndef _HAND_H_
#define _HAND_H_

#include "main.h"

static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

//状態
typedef enum
{
	HANDSTATE_NORMAL = 0,		//通常状態
	HANDSTATE_MAX
}HANDSTATE;

//構造体定義
typedef struct
{
	D3DXVECTOR3 pos;			//現在位置
	D3DXVECTOR3 posWorld;		//原点からの相対位置
	D3DXCOLOR col;				//色
	HANDSTATE state;			//状態
}Hand;

//プロトタイプ宣言
void InitHand(void);
void UninitHand(void);
void UpdateHand(void);
void DrawHand(void);
void SetHand(D3DXVECTOR3 pos);
Hand *GetHand(void);

#endif