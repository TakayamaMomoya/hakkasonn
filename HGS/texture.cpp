//=============================================================================
//
// texture.h
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "texture.h"
#include "manager.h"

int CTexture::m_nNowIndex = 0;
CTexture * CTexture::m_pTextuerClass = nullptr;
LPDIRECT3DTEXTURE9 CTexture::m_pTexture[TEXTURE_MAX] = {};

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTexture::CTexture()
{
	m_nNowIndex = 0;
	m_pTextuerClass = nullptr;
	ZeroMemory(m_pTexture, sizeof(m_pTexture));
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTexture::~CTexture()
{
}

//*****************************************************************************
// �e�N�X�`���̐���
//*****************************************************************************
CTexture *CTexture::Create()
{
	m_pTextuerClass = new CTexture;

	m_pTexture[0] = nullptr;
	m_nNowIndex++;

	return m_pTextuerClass;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CTexture::Uninit()
{
	for (int nCnt = 0;nCnt < m_nNowIndex; nCnt++)
	{
		if (m_pTexture[nCnt] != nullptr)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = nullptr;
		}
	}

	if (m_pTextuerClass != nullptr)
	{
		delete m_pTextuerClass;
		m_pTextuerClass = nullptr;
	}
}

//*****************************************************************************
// �e�N�X�`���̃��[�h
//*****************************************************************************
int CTexture::LoadTexture(char *address)
{
	CManager *pManager = GetManager();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pManager->GetDeviceManager();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pD3DDevice,
		address,
		&m_pTexture[m_nNowIndex]);

	int nIndex = m_nNowIndex;
	m_nNowIndex++;
	return nIndex;
}
