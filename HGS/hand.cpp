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

//�}�N����`
#define HAND_TEX				""									//�e�N�X�`���t�@�C����
#define HAND_HEIGHT			(100)									//����
#define HAND_WIDTH			(100)									//��

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureHand = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHand = NULL;						//���_�o�b�t�@�ւ̃|�C���^
Hand g_hand;														//�\����
int g_nHandNum;														//�G�̑���

//�v���g�^�C�v�錾
void UpdateHandPos(Hand *pHand);
void UpdateHandPolygon(Hand *pHand, int nCntHand);
void ManageStateHand(Hand *pHand);

//==================================================================================================
//����������
//==================================================================================================
void InitHand(void)
{
	//�ϐ��錾
	int nCntHand;

	//�ϐ�������
	g_nHandNum = 0;

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

	//���̐ݒ�
	pHand->pos = { 0.0f ,0.0f,0.0f };
	pHand->posWorld = { 0.0f ,0.0f,0.0f };
	pHand->col = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
	pHand->state = HANDSTATE_NORMAL;

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
}

//==================================================================================================
//�ʒu�X�V����
//==================================================================================================
void UpdateHandPos(Hand *pHand)
{
	//���Έʒu�ݒ�
	pHand->pos = *GetPosWorld() + pHand->posWorld;
}

//==================================================================================================
//�|���S���X�V����
//==================================================================================================
void UpdateHandPolygon(Hand *pHand, int nCntHand)
{
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
}

//==================================================================================================
//�|�C���^�擾����
//==================================================================================================
Hand *GetHand(void)
{
}