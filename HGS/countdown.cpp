#include "CountDown.h"
#include "manager.h"
#include "renderer.h"
#include "domino.h"
#include "tutorial.h"
#include "sound.h"

//�v���g�^�C�v�錾
#define NUM_PLACECOUNT (1)
#define MAX_COUNTDOWN (3)

int g_nCountDown;
int g_nCountDownPoint;
int g_nCountDownFrame;
bool g_bUse;

LPDIRECT3DTEXTURE9 g_pTextureCountDown = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCountDown = NULL;
D3DXVECTOR3 g_posPcoreCount;

//======================================
//����������
//======================================
void InitCountDown(void)
{

	//�f�o�C�X�̎擾
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();
	int nCntCountDown;

	g_nCountDownFrame = MAX_COUNTDOWN * 60;

	g_nCountDown = 0;
	g_nCountDownPoint = 0;
	g_bUse = true;
	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"Data\\TEXTURE\\number_pop.png",
		&g_pTextureCountDown);

	//�X�R�A�̏��̏�����

	g_posPcoreCount = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACECOUNT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCountDown,
		NULL);

	VERTEX_2D *pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^
	g_pVtxBuffCountDown->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntCountDown = 0; nCntCountDown < NUM_PLACECOUNT; nCntCountDown++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(((g_posPcoreCount.x + 120 * nCntCountDown) - 200), (g_posPcoreCount.y - 400), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(((g_posPcoreCount.x + 120 * nCntCountDown) + 200), (g_posPcoreCount.y - 400), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(((g_posPcoreCount.x + 120 * nCntCountDown) - 200), (g_posPcoreCount.y + 400), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(((g_posPcoreCount.x + 120 * nCntCountDown) + 200), (g_posPcoreCount.y + 400), 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}



	g_pVtxBuffCountDown->Unlock();
}

//======================================
//�I������
//======================================
void UninitCountDown(void)
{
	if (g_pTextureCountDown != NULL)
	{
		g_pTextureCountDown->Release();
		g_pTextureCountDown = NULL;
	}
	if (g_pVtxBuffCountDown != NULL)
	{
		g_pVtxBuffCountDown->Release();
		g_pVtxBuffCountDown = NULL;
	}
}
//======================================
//�X�V����
//======================================
void UpdateCountDown(void)
{
	TutorialSTATE Tutorial = GetTutorialState();
	if (Tutorial == TutorialSTATE_END)
	{
		int nCntCountDown;
		int aTexU[NUM_PLACECOUNT];
		VERTEX_2D *pVtx;
		g_nCountDownFrame--;
		g_nCountDown = (g_nCountDownFrame) / 60 + 1;

		aTexU[0] = g_nCountDown % 10 / 1;

		if (g_nCountDown <= 0)
		{
			//�T�E���h(SE)�̍Đ�
			CManager::GetSound()->Play(CSound::SOUND_SE_START);

			g_bUse = false;
		}

		g_pVtxBuffCountDown->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntCountDown = 0; nCntCountDown < NUM_PLACECOUNT; nCntCountDown++)
		{
			pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntCountDown]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntCountDown]) + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntCountDown]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntCountDown]) + 0.1f, 1.0f);

			pVtx += 4;
		}

		g_pVtxBuffCountDown->Unlock();
	}
}

//======================================
//�`�揈��
//======================================
void DrawCountDown(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntCountDown;

	//�f�o�C�X�̎擾
	CManager *pManager = GetManager();

	pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();
	pDevice->SetStreamSource(0, g_pVtxBuffCountDown, 0, sizeof(VERTEX_2D));


	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureCountDown);

	for (nCntCountDown = 0; nCntCountDown < NUM_PLACECOUNT; nCntCountDown++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCountDown * 4, 2);
	}

}
//======================================
//�ݒ菈��
//======================================
void SetCountDown(int nCountDown)
{
	int nCntCountDown;
	int aTexU[NUM_PLACECOUNT];
	VERTEX_2D *pVtx;
	g_nCountDown = nCountDown;

	aTexU[0] = g_nCountDown % 10 / 1;

	g_pVtxBuffCountDown->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntCountDown = 0; nCntCountDown < NUM_PLACECOUNT; nCntCountDown++)
	{
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntCountDown]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntCountDown]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntCountDown]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntCountDown]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffCountDown->Unlock();

}


bool GetCountDown()
{
	return g_bUse;
}