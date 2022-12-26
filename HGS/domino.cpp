//==================================================================================================
//
//Domino.cpp
//Author:���R����
//
//==================================================================================================

#include "main.h"
#include "domino.h"
#include <stdlib.h>
#include "manager.h"
#include "game.h"
#include "input.h"

//�}�N����`
#define DOMINO_TEX				""										//�e�N�X�`���t�@�C����
#define DOMINO_HEIGHT			(100)									//����
#define DOMINO_WIDTH			(20)									//��
#define ROLL_SPEED				(-0.16f)									//��]�X�s�[�h
#define ROLL_LIMIT				(D3DX_PI * 0.35f)						//�|�ꂫ��p�x

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureDomino[DOMINOTYPE_MAX] = {};				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDomino = NULL;						//���_�o�b�t�@�ւ̃|�C���^
Domino g_aDomino[MAX_DOMINO];											//�\����
int g_nDominoNum;													//�G�̑���

//�v���g�^�C�v�錾
void UpdateDominoPos(Domino *pDomino);
void UpdateDominoPolygon(Domino *pDomino, int nCntDomino);
void ManageStateDomino(Domino *pDomino);

//==================================================================================================
//����������
//==================================================================================================
void InitDomino(void)
{
	//�ϐ��錾
	int nCntDomino;

	//�ϐ�������
	g_nDominoNum = 0;

	//�f�o�C�X�擾
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//�e�N�X�`���ǂݍ���
	for (int nCntTex = 0;nCntTex < DOMINOTYPE_MAX;nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			DOMINO_TEX,
			&g_apTextureDomino[nCntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DOMINO,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDomino,
		NULL);

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffDomino->Lock(0, 0, (void**)&pVtx, 0);

	//���擾
	Domino *pDomino = GetDomino();

	for (nCntDomino = 0; nCntDomino < MAX_DOMINO; nCntDomino++, pDomino++)
	{//�S�Ă̏�����

	    //���̐ݒ�
		pDomino->pos = { 0.0f ,0.0f,0.0f };
		pDomino->rot = { 0.0f ,0.0f,0.0f };
		pDomino->posWorld = { 0.0f ,0.0f,0.0f };
		pDomino->col = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
		pDomino->state = DOMINOSTATE_NORMAL;
		pDomino->fAngle = 0;
		pDomino->fLength = 0;
		pDomino->bUse = false;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = pDomino->col;
		pVtx[1].col = pDomino->col;
		pVtx[2].col = pDomino->col;
		pVtx[3].col = pDomino->col;

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffDomino->Unlock();
}

//==================================================================================================
//�I������
//==================================================================================================
void UninitDomino(void)
{
	for (int nCnt = 0; nCnt < DOMINOTYPE_MAX; nCnt++)
	{
		if (g_apTextureDomino[nCnt] != NULL)
		{//�e�N�X�`���ւ̃|�C���^�j��
			g_apTextureDomino[nCnt]->Release();
			g_apTextureDomino[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffDomino != NULL)
	{//���_�o�b�t�@�ւ̃|�C���^�j��
		g_pVtxBuffDomino->Release();
		g_pVtxBuffDomino = NULL;
	}
}

//==================================================================================================
//�X�V����
//==================================================================================================
void UpdateDomino(void)
{
	//�|�C���^�錾
	Domino *pDomino = GetDomino();

	//�C���v�b�g�̃|�C���^��錾
	CInput *pInput = CInput::GetKey();

	if (pDomino->state == DOMINOSTATE_NORMAL)
	{//�ŏ��̃h�~�m���|���
		pDomino->state = DOMINOSTATE_DOWN;
	}

	for (int nCntDomino = 0; nCntDomino < MAX_DOMINO; nCntDomino++, pDomino++)
	{//�S�Ă��`�F�b�N����
		if (pDomino->bUse)
		{//�g�p���Ă����ԂȂ�

			//�ʒu�X�V����
			UpdateDominoPos(pDomino);

			//�|���S���X�V����
			UpdateDominoPolygon(pDomino,nCntDomino);

			if (GetGameState() == GAMESTATE_DOWN)
			{
				//��ԍX�V����
				ManageStateDomino(pDomino);
			}
		}
	}
}

//==================================================================================================
//�ʒu�X�V����
//==================================================================================================
void UpdateDominoPos(Domino *pDomino)
{
	//���Έʒu�ݒ�
	pDomino->pos = *GetPosWorld() + pDomino->posWorld;
}

//==================================================================================================
//�|���S���X�V����
//==================================================================================================
void UpdateDominoPolygon(Domino *pDomino, int nCntDomino)
{
	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffDomino->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntDomino;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3
	(
		pDomino->pos.x + sinf(pDomino->rot.z - D3DX_PI + pDomino->fAngle) * pDomino->fLength, 
		pDomino->pos.y + cosf(pDomino->rot.z - D3DX_PI + pDomino->fAngle) * pDomino->fLength,
		0.0f
	);

	pVtx[1].pos = D3DXVECTOR3
	(
		pDomino->pos.x + sinf(pDomino->rot.z + D3DX_PI - pDomino->fAngle) * pDomino->fLength,
		pDomino->pos.y + cosf(pDomino->rot.z + D3DX_PI - pDomino->fAngle) * pDomino->fLength,
		0.0f
	);

	pVtx[2].pos = D3DXVECTOR3
	(
		pDomino->pos.x + sinf(pDomino->rot.z - D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
		pDomino->pos.y + cosf(pDomino->rot.z - D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
		0.0f
	);

	pVtx[3].pos = D3DXVECTOR3
	(
		pDomino->pos.x + sinf(pDomino->rot.z + D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
		pDomino->pos.y + cosf(pDomino->rot.z + D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
		0.0f
	);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffDomino->Unlock();
}

//==================================================================================================
//��ԊǗ�
//==================================================================================================
void ManageStateDomino(Domino *pDomino)
{
	switch (pDomino->state)
	{
	case DOMINOSTATE_NORMAL:
		break;
	case DOMINOSTATE_DOWN:

		pDomino->rot.z += ROLL_SPEED;

		if (pDomino->rot.z < -ROLL_LIMIT && pDomino[1].bUse)
		{
			pDomino->state = DOMINOSTATE_END;
			pDomino[1].state = DOMINOSTATE_DOWN;
		}
		else if (pDomino->rot.z < -D3DX_PI * 0.5f && pDomino[1].bUse == false)
		{
			pDomino->rot.z = -D3DX_PI * 0.5f;
			pDomino->state = DOMINOSTATE_END;
			SetGameState(GAMESTATE_END);
		}

		break;
	case DOMINOSTATE_END:
		break;
	}
}

//==================================================================================================
//�ݒ菈��
//==================================================================================================
void SetDomino(D3DXVECTOR3 pos)
{
	//���擾
	Domino *pDomino = GetDomino();

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffDomino->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDomino = 0; nCntDomino < MAX_DOMINO; nCntDomino++, pDomino++)
	{//�S�Ă��`�F�b�N����
		if (pDomino->bUse == false)
		{
			//�Ίp���̊p�x�ݒ�
			pDomino->fAngle = atan2f(DOMINO_WIDTH,DOMINO_HEIGHT);

			//�Ίp���̒����ݒ�
			pDomino->fLength = sqrtf((DOMINO_WIDTH * 0.5f) * (DOMINO_WIDTH * 0.5f) + (DOMINO_HEIGHT) * (DOMINO_HEIGHT)) * 0.5f;

			//�����󂯎��
			pDomino->posWorld = pos;
			pDomino->pos = pos;

			//�g�p��Ԃɂ���
			pDomino->bUse = true;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3
			(
				pDomino->pos.x + sinf(pDomino->rot.z - D3DX_PI + pDomino->fAngle) * pDomino->fLength,
				pDomino->pos.y + cosf(pDomino->rot.z - D3DX_PI + pDomino->fAngle) * pDomino->fLength,
				0.0f
			);

			pVtx[1].pos = D3DXVECTOR3
			(
				pDomino->pos.x + sinf(pDomino->rot.z + D3DX_PI - pDomino->fAngle) * pDomino->fLength,
				pDomino->pos.y + cosf(pDomino->rot.z + D3DX_PI - pDomino->fAngle) * pDomino->fLength,
				0.0f
			);

			pVtx[2].pos = D3DXVECTOR3
			(
				pDomino->pos.x + sinf(pDomino->rot.z - D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
				pDomino->pos.y + cosf(pDomino->rot.z - D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
				0.0f
			);

			pVtx[3].pos = D3DXVECTOR3
			(
				pDomino->pos.x + sinf(pDomino->rot.z + D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
				pDomino->pos.y + cosf(pDomino->rot.z + D3DX_PI * 0.5f) * DOMINO_WIDTH * 0.5f,
				0.0f
			);

			g_nDominoNum++;

			break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffDomino->Unlock();
}

//==================================================================================================
//�`�揈��
//==================================================================================================
void DrawDomino(void)
{
	//�f�o�C�X�擾
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//�|�C���^�錾
	Domino *pDomino = GetDomino();

	//�ϐ��錾
	int nCntDomino;

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffDomino, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntDomino = 0; nCntDomino < MAX_DOMINO; nCntDomino++, pDomino++)
	{//�S�Ă��`�F�b�N����
		if (pDomino->bUse == true)
		{//�g�p���Ă����ԂȂ�

			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_apTextureDomino[0]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDomino * 4, 2);
		}
	}
}

//==================================================================================================
//�|�C���^�擾����
//==================================================================================================
Domino *GetDomino(void)
{
	return &g_aDomino[0];
}

//==================================================================================================
//�G������
//==================================================================================================
int GetDominoNum(void)
{
	return g_nDominoNum;
}