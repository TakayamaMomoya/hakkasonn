//==================================================================================================
//
//Sign.h
//Author:���R����
//
//==================================================================================================

#ifndef _SIGN_H_
#define _SIGN_H_

#include "main.h"

//�}�N����`
#define MAX_SIGN			(80)									//�ő吔
#define SIGN_HEIGHT			(300)									//����
#define SIGN_WIDTH			(300)									//��

//���
typedef enum
{
	SIGNTYPE_10 = 0,		//�P�O��
	SIGNTYPE_50,		//�T�O��
	SIGNTYPE_100,		//�P�O�O��
	SIGNTYPE_MAX
}SIGNTYPE;

//�\���̒�`
typedef struct
{
	D3DXVECTOR3 pos;			//���݈ʒu
	D3DXVECTOR3 posWorld;		//���_����̑��Έʒu
	D3DXVECTOR3 rot;			//����
	D3DXCOLOR col;				//�F
	SIGNTYPE type;				//���
	bool bUse;					//�g�p���Ă��邩�ǂ���
	float width;				//��
	float height;				//����
}Sign;

//�v���g�^�C�v�錾
void InitSign(void);
void UninitSign(void);
void UpdateSign(void);
void DrawSign(void);
void SetSign(D3DXVECTOR3 pos,SIGNTYPE type);
Sign *GetSign(void);
int GetSignNum(void);

#endif