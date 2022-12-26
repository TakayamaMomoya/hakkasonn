//=============================================================================
//
// number.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _NUMBER_H_			//���̃}�N����`������Ȃ�������
#define _NUMBER_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "2dpolygon.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CNumber
{
private:
	// �|���S���̕�
	static const float SIZE_NUMBER;
	static const int NUMBER_MAX = 8;
public:

	CNumber();
	~CNumber();

	HRESULT Init();
	void Uninit();
	void Draw();

	void SetNumber(D3DXVECTOR3 pos, int nDigit, float fSize = SIZE_NUMBER,int nNumerical = 0);
	void SetNumberPos(D3DXVECTOR3 pos);
	void AddNumberPos(D3DXVECTOR3 add);
	void SetNumerical(int nNumerical);

	D3DXVECTOR3 GetPos() { return m_Pos; }

	void SetTextNum(int nNum) { m_nTexIndex = nNum; }

private:
	C2DPolygon *m_pNum[NUMBER_MAX];
	int m_nDigit;
	int m_nNumerical;
	int m_nTexIndex;
	float m_fSize;
	D3DXVECTOR3 m_Pos;
};

#endif