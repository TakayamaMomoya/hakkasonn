//=============================================================================
//
// number.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "number.h"
#include "main.h"
#include "texture.h"

// �|���S���̕�
const float CNumber::SIZE_NUMBER = 64.0f;
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CNumber::CNumber()
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CNumber::~CNumber()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CNumber::Init()
{
	m_nDigit = 0;
	m_nNumerical = 0;
	m_fSize = 0.0f;
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	ZeroMemory(m_pNum, sizeof(m_pNum));
	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CNumber::Uninit()
{
	for (int nCnt = 0; nCnt < NUMBER_MAX; nCnt++)
	{
		if (m_pNum[nCnt] == nullptr)
		{
			continue;
		}

		m_pNum[nCnt]->Uninit();

		delete m_pNum[nCnt];
		m_pNum[nCnt] = nullptr;
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CNumber::Draw()
{
	for (int nCnt = 0; nCnt < NUMBER_MAX; nCnt++)
	{
		if (m_pNum[nCnt] == nullptr)
		{
			continue;
		}

		m_pNum[nCnt]->Draw();
	}
}

//*****************************************************************************
// ���l�̐ݒ�
//*****************************************************************************
void CNumber::SetNumber(D3DXVECTOR3 pos, int nDigit, float fSize, int nNumerical)
{
	m_nDigit = nDigit;
	m_fSize = fSize;
	m_Pos = pos;
	for (int nCnt = 0; nCnt < nDigit; nCnt++)
	{
		if (m_pNum[nCnt] != nullptr)
		{
			continue;
		}
		m_pNum[nCnt] = new C2DPolygon;

		if (FAILED(m_pNum[nCnt]->Init()))
		{
			return;
		}

		m_pNum[nCnt]->SetPos(D3DXVECTOR3(pos.x - fSize * 0.5f * nCnt, pos.y,0.0f));
		m_pNum[nCnt]->SetDiagonalLine(fSize, fSize);
		m_pNum[nCnt]->SetTextIndex(m_nTexIndex);
		m_pNum[nCnt]->SetUVSize(D3DXVECTOR2(0.05f, 0.5f));
		m_pNum[nCnt]->SetPolygon();
	}

	SetNumerical(nNumerical);
}

//*****************************************************************************
// ���l�̕ύX
//*****************************************************************************
void CNumber::SetNumerical(int nNumerical)
{
	int nTemp = nNumerical;

	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
	{
		if (m_pNum[nCnt] == nullptr)
		{
			return;
		}

		int nPosTexU = nTemp % 10;
		nTemp /= 10;

		m_pNum[nCnt]->SetUV(D3DXVECTOR2(0.1f * nPosTexU + 0.05f, 0.5f));
		m_pNum[nCnt]->SetPolygon();
	}
}

void CNumber::SetNumberPos(D3DXVECTOR3 pos)
{
	m_Pos = pos;
	for (int nCnt = 0; nCnt < m_nDigit; nCnt++)
	{
		if (m_pNum[nCnt] == nullptr)
		{
			continue;
		}
		m_pNum[nCnt]->SetPos(D3DXVECTOR3(pos.x - m_fSize * 0.5f * nCnt, pos.y, 0.0f));
		m_pNum[nCnt]->SetPolygon();
	}
}

void CNumber::AddNumberPos(D3DXVECTOR3 add)
{
	m_Pos += add;
	SetNumberPos(m_Pos);
}