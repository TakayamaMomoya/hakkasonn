//==================================================================================================
//
//Hand.h
//Author:���R����
//
//==================================================================================================

#ifndef _HAND_H_
#define _HAND_H_

#include "main.h"

//���
typedef enum
{
	HANDSTATE_NORMAL = 0,		//�ʏ���
	HANDSTATE_MAX
}HANDSTATE;

//�\���̒�`
typedef struct
{
	D3DXVECTOR3 pos;			//���݈ʒu
	D3DXVECTOR3 posWorld;		//���_����̑��Έʒu
	D3DXCOLOR col;				//�F
	HANDSTATE state;			//���
}Hand;

//�v���g�^�C�v�錾
void InitHand(void);
void UninitHand(void);
void UpdateHand(void);
void DrawHand(void);
void SetHand(D3DXVECTOR3 pos);
Hand *GetHand(void);

#endif