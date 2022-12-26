//=============================================================================
//
// 2DPolgon.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "2dpolygon.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

const float C2DPolygon::FLASHING_AMOUNT = 0.02f;
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
C2DPolygon::C2DPolygon()
{
	m_nTextIndex = 0;
	m_pVtxBuff = nullptr;//���_�o�b�t�@�ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_UV = D3DXVECTOR2(0.5f, 0.5f);
	m_UVMove = D3DXVECTOR2(0.0f, 0.0f);
	m_UVSize = D3DXVECTOR2(0.5f, 0.5f);
	m_Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_angle = 0.0f;
	m_len = 0.0f;
	m_ActualRadius = 0.0f;
	m_nFlashingCnt = 0;
	m_fAlphaMax = 1.0f;
	m_fAlphaLeast = 0.2f;
	m_bFadeInOut = true;
	m_fFadeSpeed = FLASHING_AMOUNT;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
C2DPolygon::~C2DPolygon()
{
}

//=============================================================================
// �|���S���̏�����
//=============================================================================
HRESULT C2DPolygon::Init()
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	// ���_�o�b�t�@�̐���
	pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �|���S���̏I��
//=============================================================================
void C2DPolygon::Uninit()
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================================
// �|���S���̍X�V
//=============================================================================
void C2DPolygon::Update()
{
	m_Pos += m_Move;
	m_UV += m_UVMove;

	if (m_nFlashingCnt > 0)
	{
		m_nFlashingCnt--;
		if (!m_bFadeInOut)
		{
			m_Col.a += m_fFadeSpeed;
			if (m_Col.a > m_fAlphaMax)
			{
				m_Col.a = m_fAlphaMax;
				m_bFadeInOut = true;
			}
		}
		else if (m_bFadeInOut)
		{
			m_Col.a -= m_fFadeSpeed;
			if (m_Col.a < m_fAlphaLeast)
			{
				m_Col.a = m_fAlphaLeast;
				m_bFadeInOut = false;
			}
		}

		if (m_nFlashingCnt <= 0)
		{
			m_Col.a = 1.0f;
		}
	}
	
	SetPolygon();
}

//=============================================================================
// �|���S���̕`��
//=============================================================================
void C2DPolygon::Draw()
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pD3DDevice->SetFVF(FVF_VERTEX_2D);

	LPDIRECT3DTEXTURE9 tex = CTexture::GetTexture(m_nTextIndex);

	//�e�N�X�`���̐ݒ�
	pD3DDevice->SetTexture(0, tex);

	//�|���S���̕`��
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);		//�v���~�e�B�u�i�|���S���j��
}

void C2DPolygon::PolygonAppear(float fAppear)
{
	m_Col.a += fAppear;
	if (m_Col.a > 1.0f)
	{
		m_Col.a = 1.0f;
	}
}

void C2DPolygon::SetDiagonalLine(float fwidth, float fheight)
{
	m_fWidth = fwidth;
	m_fHeight = fheight;

	//�Ίp���̒������Z�o����
	m_len = sqrtf(fwidth * fwidth + fheight * fheight) * 0.5f;
	//�Ίp���̊p�x���Z�o����
	m_angle = atan2f(fwidth, fheight);

	m_ActualRadius = sinf(m_Rot.z + (0.0f + D3DX_PI - m_angle)) * m_len;
}

void C2DPolygon::AddDiagonalLine(float faddwidth, float faddheight)
{
	m_fWidth += faddwidth;
	m_fHeight += faddheight;

	//�Ίp���̒������Z�o����
	m_len = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight) * 0.5f;
	//�Ίp���̊p�x���Z�o����
	m_angle = atan2f(m_fWidth, m_fHeight);

	m_ActualRadius = sinf(m_Rot.z + (0.0f + D3DX_PI - m_angle)) * m_len;
}

void C2DPolygon::SetPolygon()
{
	float len = m_len;
	float angle = m_angle;
	float rot = m_Rot.z;
	
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x + sinf(rot + (0.0f - D3DX_PI + angle)) * len, m_Pos.y + cosf(rot + (0.0f - D3DX_PI + angle)) * len, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x + sinf(rot + (0.0f + D3DX_PI - angle)) * len, m_Pos.y + cosf(rot + (0.0f + D3DX_PI - angle)) * len, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x + sinf(rot - angle) * len, m_Pos.y + cosf(rot - angle) * len, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + sinf(rot + angle) * len, m_Pos.y + cosf(rot + angle) * len, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_UV.x - m_UVSize.x, m_UV.y - m_UVSize.y);
	pVtx[1].tex = D3DXVECTOR2(m_UV.x + m_UVSize.x, m_UV.y - m_UVSize.y);
	pVtx[2].tex = D3DXVECTOR2(m_UV.x - m_UVSize.x, m_UV.y + m_UVSize.y);
	pVtx[3].tex = D3DXVECTOR2(m_UV.x + m_UVSize.x, m_UV.y + m_UVSize.y);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

void C2DPolygon::SetUp(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move)
{
	m_Pos = pos;
	m_Rot = rot;
	m_Move = move;
}

//*****************************************************************************
// �����蔻��
//*****************************************************************************
bool C2DPolygon::Collision(D3DXVECTOR3 pos, float radius)
{
	D3DXVECTOR3 mypos = GetPos();

	//�Q���̕��̂̔��a���m�̘a
	float fDiff = m_ActualRadius + radius;

	//�v�Z�p�ϐ�
	float fCalculationX, fCalculationZ;

	//X�̍���
	fCalculationX = pos.x - mypos.x;
	//Y�̍���
	fCalculationZ = pos.y - mypos.y;

	//���݂̂Q�_�̋���
	float fLength = sqrtf(fCalculationX * fCalculationX + fCalculationZ * fCalculationZ);

	//�Q���̕��̂̔��a���m�̘a��茻�݂̂Q�_�̋��������������ǂ���
	if (fDiff >= fLength)
	{
		//��������
        return true;
	}

	//�������ĂȂ�
	return false;

}