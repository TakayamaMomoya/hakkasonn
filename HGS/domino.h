//==================================================================================================
//
//Domino.h
//Author:���R����
//
//==================================================================================================

#ifndef _DOMINO_H_
#define _DOMINO_H_

#include "main.h"

//�}�N����`
#define MAX_DOMINO			(128)											//�G�̍ő吔

static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

//���
typedef enum
{
	DOMINOTYPE_NORMAL = 0,		//�ʏ�G
	DOMINOTYPE_MAX
}DOMINOTYPE;

//���
typedef enum
{
	DOMINOSTATE_NORMAL = 0,		//�ʏ���
	DOMINOSTATE_DOWN,			//�|��Ă���
	DOMINOSTATE_END,			//�|��؂������
	DOMINOSTATE_MAX
}DOMINOSTATE;

//�\���̒�`
typedef struct
{
	D3DXVECTOR3 pos;			//���݈ʒu
	D3DXVECTOR3 posWorld;		//���_����̑��Έʒu
	D3DXVECTOR3 rot;			//����
	D3DXCOLOR col;				//�F
	DOMINOTYPE type;			//���
	DOMINOSTATE state;			//���
	bool bUse;					//�g�p���Ă��邩�ǂ���
	float fAngle;				//�Ίp���̊p�x
	float fLength;				//�Ίp���̒���
}Domino;

//�v���g�^�C�v�錾
void InitDomino(void);
void UninitDomino(void);
void UpdateDomino(void);
void DrawDomino(void);
void SetDomino(D3DXVECTOR3 pos);
Domino *GetDomino(void);
int GetDominoNum(void);

#endif