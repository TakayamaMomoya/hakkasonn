//==================================================================================================
//
//Sign.h
//Author:髙山桃也
//
//==================================================================================================

#ifndef _SIGN_H_
#define _SIGN_H_

#include "main.h"

//マクロ定義
#define MAX_SIGN			(80)									//最大数
#define SIGN_HEIGHT			(300)									//高さ
#define SIGN_WIDTH			(300)									//幅

//種類
typedef enum
{
	SIGNTYPE_10 = 0,		//１０個目
	SIGNTYPE_50,		//５０個目
	SIGNTYPE_100,		//１００個目
	SIGNTYPE_MAX
}SIGNTYPE;

//構造体定義
typedef struct
{
	D3DXVECTOR3 pos;			//現在位置
	D3DXVECTOR3 posWorld;		//原点からの相対位置
	D3DXVECTOR3 rot;			//向き
	D3DXCOLOR col;				//色
	SIGNTYPE type;				//種類
	bool bUse;					//使用しているかどうか
	float width;				//幅
	float height;				//高さ
}Sign;

//プロトタイプ宣言
void InitSign(void);
void UninitSign(void);
void UpdateSign(void);
void DrawSign(void);
void SetSign(D3DXVECTOR3 pos,SIGNTYPE type);
Sign *GetSign(void);
int GetSignNum(void);

#endif