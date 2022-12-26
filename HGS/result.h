//=============================================================================
//
// Result.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _RESULT_H_			//���̃}�N����`������Ȃ�������
#define _RESULT_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CUI;
class C2DPolygon;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CResult : public CObject
{
private:
	static const int TEXT_MAX = 17;
public:
	CResult();
	~CResult() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	C2DPolygon* m_pSea;
	CUI* m_pBg;
	C2DPolygon* m_pstone_bridge;
	C2DPolygon* m_pFlowObject;
	int m_nText[TEXT_MAX];
};

#endif