//=============================================================================
//
// number_manager.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "number_manager.h"
#include "number.h"
#include "texture.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CNumber_Manager::CNumber_Manager()
{
	
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CNumber_Manager::~CNumber_Manager()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CNumber_Manager::Init()
{
	m_nTextNum = CTexture::LoadTexture("data\\TEXTURE\\number_pop.png");
	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CNumber_Manager::Uninit()
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_NumberList.begin(); itr != m_NumberList.end();)
	{
		//�C�e���[�^�[���犢�I�̃|�C���^�̑��
		CNumber* pNumber = *itr;

		//���I�|�C���^�̉��
		if (pNumber != nullptr)
		{
			//�I������
			pNumber->Uninit();
			delete pNumber;
			pNumber = nullptr;
		}

		//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
		itr = m_NumberList.erase(itr);
	}
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CNumber_Manager::Update()
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_NumberList.begin(); itr != m_NumberList.end();)
	{
		//�C�e���[�^�[����i���o�[�̃|�C���^�̑��
		CNumber* pNumber = *itr;

		//�|�C���^�̉��
		if (pNumber == nullptr)
		{
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_NumberList.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//�X�V����
		pNumber->AddNumberPos(D3DXVECTOR3(-22.0f,0.0f,0.0f));

		//�I����������
		if (pNumber->GetPos().x < -300.0f)
		{
			//�I������
			pNumber->Uninit();
			delete pNumber;
			pNumber = nullptr;

			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_NumberList.erase(itr);

			//�ȉ��̏����𖳎�����
			continue;
		}

		//�C�e���[�^�[��i�߂�
		itr++;
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CNumber_Manager::Draw()
{
	//�C�e���[�^�[���[�v
	for (auto itr = m_NumberList.begin(); itr != m_NumberList.end(); itr++)
	{
		//�C�e���[�^�[����i���o�[�̃|�C���^�̑��
		CNumber* pNumber = *itr;

		//�|�C���^�̉��
		if (pNumber != nullptr)
		{
			//�`�揈��
			pNumber->Draw();
		}
	}
}

//�i���o�[�̃Z�b�g
void CNumber_Manager::SetNumber(D3DXVECTOR3 pos, int nNum)
{
	CNumber* pNumber = nullptr;

	pNumber = new CNumber;

	pNumber->SetTextNum(m_nTextNum);

	if (FAILED(pNumber->Init()))
	{
		return;
	}

	pNumber->SetNumber(pos,3,50.0f, nNum);

	

	//���X�g�Ɋ��I����ǉ�
	m_NumberList.push_back(pNumber);
}
