//==================================================================================================
//
//Sign.cpp
//Author:���R����
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

//�}�N����`
#define SIGN_TEX				""										//�e�N�X�`���t�@�C����
#define NUM_COL					(4)										//�F�̎��

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureSign[SIGNTYPE_MAX] = {};				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSign = NULL;						//���_�o�b�t�@�ւ̃|�C���^
Sign g_aSign[MAX_SIGN];												//�\����
int g_nSignNum;														//����
const char *c_apFilenameBlock[SIGNTYPE_MAX] =
{
	"data\\TEXTURE\\�Ŕ�.png",
	"data\\TEXTURE\\�Ŕ�.png",
	"data\\TEXTURE\\�Ŕ�.png"
};																	//�e�N�X�`���t�@�C����

//�v���g�^�C�v�錾
void UpdateSignPos(Sign *pSign);
void UpdateSignPolygon(Sign *pSign, int nCntSign);

//==================================================================================================
//����������
//==================================================================================================
void InitSign(void)
{
	//�ϐ��錾
	int nCntSign;

	//�ϐ�������
	g_nSignNum = 0;

	//�f�o�C�X�擾
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//�e�N�X�`���ǂݍ���
	for (int nCntTex = 0;nCntTex < SIGNTYPE_MAX;nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameBlock[nCntTex],
			&g_apTextureSign[nCntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SIGN,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSign,
		NULL);

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSign->Lock(0, 0, (void**)&pVtx, 0);

	//���擾
	Sign *pSign = GetSign();

	for (nCntSign = 0; nCntSign < MAX_SIGN; nCntSign++, pSign++)
	{//�S�Ă̏�����

	    //���̐ݒ�
		pSign->pos = { 0.0f ,0.0f,0.0f };
		pSign->rot = { 0.0f ,0.0f,0.0f };
		pSign->posWorld = { 0.0f ,0.0f,0.0f };
		pSign->col = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
		pSign->type = SIGNTYPE_10;
		pSign->width = 0;
		pSign->height = 0;
		pSign->bUse = false;

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
		pVtx[0].col = pSign->col;
		pVtx[1].col = pSign->col;
		pVtx[2].col = pSign->col;
		pVtx[3].col = pSign->col;

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffSign->Unlock();
}

//==================================================================================================
//�I������
//==================================================================================================
void UninitSign(void)
{
	for (int nCnt = 0; nCnt < SIGNTYPE_MAX; nCnt++)
	{
		if (g_apTextureSign[nCnt] != NULL)
		{//�e�N�X�`���ւ̃|�C���^�j��
			g_apTextureSign[nCnt]->Release();
			g_apTextureSign[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffSign != NULL)
	{//���_�o�b�t�@�ւ̃|�C���^�j��
		g_pVtxBuffSign->Release();
		g_pVtxBuffSign = NULL;
	}
}

//==================================================================================================
//�X�V����
//==================================================================================================
void UpdateSign(void)
{
	//�|�C���^�錾
	Sign *pSign = GetSign();

	//�C���v�b�g�̃|�C���^��錾
	CInput *pInput = CInput::GetKey();

	for (int nCntSign = 0; nCntSign < MAX_SIGN; nCntSign++, pSign++)
	{//�S�Ă��`�F�b�N����
		if (pSign->bUse)
		{//�g�p���Ă����ԂȂ�

			//�ʒu�X�V����
			UpdateSignPos(pSign);

			//�|���S���X�V����
			UpdateSignPolygon(pSign, nCntSign);
		}
	}
}

//==================================================================================================
//�ʒu�X�V����
//==================================================================================================
void UpdateSignPos(Sign *pSign)
{
	//���Έʒu�ݒ�
	pSign->pos = *GetPosWorld() + pSign->posWorld;
}

//==================================================================================================
//�|���S���X�V����
//==================================================================================================
void UpdateSignPolygon(Sign *pSign, int nCntSign)
{
	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSign->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntSign;

	//���_���W�̐ݒ�
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

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffSign->Unlock();
}

//==================================================================================================
//�ݒ菈��
//==================================================================================================
void SetSign(D3DXVECTOR3 pos, SIGNTYPE type, int nDominoNumber)
{
	float fSize = 0.0f;
	//�i���o�[�}�l�[�W���[�̎擾
	CManager *pManager = GetManager();
	CGame* pGame = (CGame*)pManager->GetGameObject();
	CNumber_Manager* pNumber_Manager = pGame->GetNumber_Manager();

	//���擾
	Sign *pSign = GetSign();

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSign->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSign = 0; nCntSign < MAX_SIGN; nCntSign++, pSign++)
	{//�S�Ă��`�F�b�N����
		if (pSign->bUse == false)
		{
			switch (type)
			{
			case SIGNTYPE_10:
				
				pSign->width = 150;
				pSign->height = 150;
				
				break;
			case SIGNTYPE_50:

				pSign->width = 200;
				pSign->height = 200;

				break;
			case SIGNTYPE_100:

				pSign->width = 250;
				pSign->height = 250;

				break;
			}


			//�����󂯎��
			pSign->posWorld = pos;
			pSign->pos = pos;

			//�g�p��Ԃɂ���
			pSign->bUse = true;

			//���_���W�̐ݒ�
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

			//�i���o�[�̐���
			pNumber_Manager->SetNumber(D3DXVECTOR3(pos.x,pos.y - 100.0f,0.0f), nDominoNumber);
			break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffSign->Unlock();
}

//==================================================================================================
//�`�揈��
//==================================================================================================
void DrawSign(void)
{
	//�f�o�C�X�擾
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//�|�C���^�錾
	Sign *pSign = GetSign();

	//�ϐ��錾
	int nCntSign;

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSign, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntSign = 0; nCntSign < MAX_SIGN; nCntSign++, pSign++)
	{//�S�Ă��`�F�b�N����
		if (pSign->bUse == true)
		{//�g�p���Ă����ԂȂ�

			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_apTextureSign[0]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSign * 4, 2);
		}
	}
}

//==================================================================================================
//�|�C���^�擾����
//==================================================================================================
Sign *GetSign(void)
{
	return &g_aSign[0];
}

//==================================================================================================
//������
//==================================================================================================
int GetSignNum(void)
{
	return g_nSignNum;
}