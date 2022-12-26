#include "main.h"
#include "Tutorial.h"
#include "sound.h"
#include "input.h"
#include "Fade.h"
#include "domino.h"
#include "renderer.h"
#include "manager.h"

//�}�N����`
#define NUM_Tutorial (1) //�w�i�̐�

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTutorial = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;
LPDIRECT3DTEXTURE9 g_apTextureTutorial[NUM_Tutorial] = {}; //�e�N�X�`���ւ̃|�C���^

float g_aTexUT[NUM_Tutorial];


bool g_bDisptuTuorial = true;
float g_fAlpha;

TutorialSTATE g_TutorialState = TutorialSTATE_NONE;
int g_nCounterTutorial = 0;
//======================================
//����������
//======================================
void InitTutorial(void)
{
	//�f�o�C�X�̎擾
	g_bDisptuTuorial = true;
	g_fAlpha = 1.0f;

	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	//�e�N�X�`���ǂݍ���


	D3DXCreateTextureFromFile(pDevice, "Data\\TEXTURE\\tutorial.png", &g_apTextureTutorial[0]);



	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_Tutorial,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);
	VERTEX_2D * pVtx;
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTutorial = 0; nCntTutorial < NUM_Tutorial; nCntTutorial++)
	{
		g_aTexUT[nCntTutorial] = 0.0f;

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffTutorial->Unlock();

	g_TutorialState = TutorialSTATE_NORMAL;
	g_nCounterTutorial = 0;
}

//======================================
//�I������
//======================================
void UninitTutorial(void)
{
	int nCntTutorial;
	if (g_pTextureTutorial != NULL)
	{
		g_pTextureTutorial->Release();
		g_pTextureTutorial = NULL;
	}
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
	for (nCntTutorial = 0; nCntTutorial < NUM_Tutorial; nCntTutorial++)
	{

		if (g_apTextureTutorial[nCntTutorial] != NULL)
		{
			g_apTextureTutorial[nCntTutorial]->Release();
			g_apTextureTutorial[nCntTutorial] = NULL;
		}
	}
}

//======================================
//�X�V����
//======================================
void UpdateTutorial(void)
{
	//GetInput
	CInput *pInput = CInput::GetKey();

	if (pInput->Trigger(KEY_DECISION))
	{
		g_bDisptuTuorial = false;
	}
	//�\�����I�t�ɂ��ꂽ�Ƃ�
	if (g_bDisptuTuorial == false)
	{
		g_fAlpha -= 0.05f;
		
	}
	if (g_fAlpha <= 0.0f)
	{
		g_fAlpha = 0.0f;
		g_TutorialState = TutorialSTATE_END;
	}

	VERTEX_2D * pVtx;
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntTutorial = 0; nCntTutorial < NUM_Tutorial; nCntTutorial++)
	{
		g_aTexUT[nCntTutorial] = 0.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);

		pVtx += 4;
	}
	g_pVtxBuffTutorial->Unlock();
}

//======================================
//�`�揈��
//======================================
void DrawTutorial(void)
{
	int nCntTutorial;

	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
							   //�f�o�C�X�̎擾
	CManager *pManager = GetManager();

	 pDevice = nullptr;

	pDevice = pManager->GetDeviceManager();

	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTutorial = 0; nCntTutorial < NUM_Tutorial; nCntTutorial++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureTutorial[nCntTutorial]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTutorial * 4, 2);
	}

}
void SetTutorialState(TutorialSTATE state, int nCounter)
{
	g_TutorialState = state;
	g_nCounterTutorial = nCounter;
}

TutorialSTATE GetTutorialState()
{
	return g_TutorialState;
}