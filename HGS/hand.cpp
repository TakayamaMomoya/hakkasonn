//==================================================================================================
//
//Hand.cpp
//Author:���R����
//
//==================================================================================================

#include "main.h"
#include "hand.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "domino.h"

//�}�N����`
#define HAND_TEX			"data\\TEXTURE\\��.png"					//�e�N�X�`���t�@�C����
#define HAND_HEIGHT			(300)									//����
#define HAND_WIDTH			(300)									//��
#define NUM_POS_DEST		(3)										//�ڕW�n�_�̐�
#define HAND_FACT			(0.1f)									//�n���h�̈ړ��W��
#define LIMIT_DIFF			(50.0f)									//���̃L�[�ɂ��鍷���l

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureHand = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHand = NULL;						//���_�o�b�t�@�ւ̃|�C���^
Hand g_hand;														//�\����
D3DXVECTOR3 g_aPosDestHand[NUM_POS_DEST];							//�ڕW�n�_

//�v���g�^�C�v�錾
void UpdateHandPos(Hand *pHand);
void UpdateHandPolygon(Hand *pHand);
void ManageStateHand(Hand *pHand);

//==================================================================================================
//����������
//==================================================================================================
void InitHand(void)
{
	//�f�o�C�X�擾
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		HAND_TEX,
		&g_pTextureHand);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHand,
		NULL);

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHand->Lock(0, 0, (void**)&pVtx, 0);

	//���擾
	Hand *pHand = GetHand();

	//�ڕW�n�_�ݒ�
	g_aPosDestHand[0] = { SCREEN_WIDTH * 0.2f,SCREEN_HEIGHT * 0.55f,0.0f };
	g_aPosDestHand[1] = { SCREEN_WIDTH * 0.0f,SCREEN_HEIGHT * 0.55f,0.0f };
	g_aPosDestHand[2] = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT * 0.55f,0.0f };

	//���̐ݒ�
	pHand->pos = { g_aPosDestHand[0].x ,g_aPosDestHand[0].y,0.0f };
	pHand->posWorld = { g_aPosDestHand[0].x ,g_aPosDestHand[0].y,0.0f };
	pHand->col = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
	pHand->state = HANDSTATE_NORMAL;
	pHand->nMovekey = 0;

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
	pVtx[0].col = pHand->col;
	pVtx[1].col = pHand->col;
	pVtx[2].col = pHand->col;
	pVtx[3].col = pHand->col;

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffHand->Unlock();
}

//==================================================================================================
//�I������
//==================================================================================================
void UninitHand(void)
{
	if (g_pTextureHand != NULL)
	{//�e�N�X�`���ւ̃|�C���^�j��
		g_pTextureHand->Release();
		g_pTextureHand = NULL;
	}

	if (g_pVtxBuffHand != NULL)
	{//���_�o�b�t�@�ւ̃|�C���^�j��
		g_pVtxBuffHand->Release();
		g_pVtxBuffHand = NULL;
	}
}

//==================================================================================================
//�X�V����
//==================================================================================================
void UpdateHand(void)
{
	//���擾
	Hand *pHand = GetHand();

	if (pHand->state == HANDSTATE_PUSH)
	{
		//�ʒu�X�V����
		UpdateHandPos(pHand);
	}

	//�|���S���X�V����
	UpdateHandPolygon(pHand);
}

//==================================================================================================
//�ʒu�X�V����
//==================================================================================================
void UpdateHandPos(Hand *pHand)
{
	//�����̎擾
	D3DXVECTOR3 posDiff = (g_aPosDestHand[pHand->nMovekey] - pHand->posWorld);
	posDiff.x = posDiff.x * posDiff.x;
	posDiff.y = posDiff.y * posDiff.y;

	//�ڕW�ֈړ�
	pHand->posWorld += (g_aPosDestHand[pHand->nMovekey] - pHand->posWorld) * HAND_FACT;

	if (posDiff.x + posDiff.y < LIMIT_DIFF * LIMIT_DIFF)
	{//���������ȉ��Ŏ��̃L�[��
		if (pHand->nMovekey < NUM_POS_DEST - 1)
		{//�L�[��i�߂�
			pHand->nMovekey++;
		}
		else if(GetGameState() == GAMESTATE_PUSH)
		{//�Q�[����Ԃ�ύX
			SetGameState(GAMESTATE_DOWN);
		}
	}

	//���Έʒu�ݒ�
	pHand->pos = *GetPosWorld() + pHand->posWorld;
}

//==================================================================================================
//�|���S���X�V����
//==================================================================================================
void UpdateHandPolygon(Hand *pHand)
{
	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHand->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pHand->pos.x - HAND_WIDTH * 0.5f, pHand->pos.y - HAND_HEIGHT * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pHand->pos.x + HAND_WIDTH * 0.5f, pHand->pos.y - HAND_HEIGHT * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pHand->pos.x - HAND_WIDTH * 0.5f, pHand->pos.y + HAND_HEIGHT * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pHand->pos.x + HAND_WIDTH * 0.5f, pHand->pos.y + HAND_HEIGHT * 0.5f, 0.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffHand->Unlock();
}

//==================================================================================================
//��ԊǗ�
//==================================================================================================
void ManageStateHand(Hand *pHand)
{
}

//==================================================================================================
//�`�揈��
//==================================================================================================
void DrawHand(void)
{
	//�f�o�C�X�擾
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHand, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureHand);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==================================================================================================
//�|�C���^�擾����
//==================================================================================================
Hand *GetHand(void)
{
	return &g_hand;
}