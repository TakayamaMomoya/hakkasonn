//=============================================================================
//
// 2DPolygon.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _2DPOLGON_H_			//���̃}�N����`������Ȃ�������
#define _2DPOLGON_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class C2DPolygon : public CObject
{
private:
	// ���_�t�H�[�}�b�g
	static const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	static const float FLASHING_AMOUNT;

public:
	C2DPolygon();//�R���X�g���N�^Flashing
	virtual ~C2DPolygon() override;//�f�X�g���N�^

	virtual HRESULT Init() override;//������
	virtual void Uninit() override;//�I������
	virtual void Update() override;//�X�V����
	virtual void Draw() override;//�`�揈��

	virtual bool IsUnused() { return true; }//�I����������
	virtual void SetUp(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move);//�Z�b�g�A�b�v

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	D3DXVECTOR3 GetPos() { return m_Pos; }
	void AddPos(D3DXVECTOR3 pos) { m_Pos += pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }
	D3DXVECTOR3 GetMove() { return m_Move; }
	void SetUV(D3DXVECTOR2 uv) { m_UV = uv; }
	void AddUVPos(D3DXVECTOR2 uvmove) { m_UV += uvmove; }
	void SetUVMove(D3DXVECTOR2 uvmove) { m_UVMove = uvmove; }
	void SetUVSize(D3DXVECTOR2 uvsize) { m_UVSize = uvsize; }
	void SetColor(D3DXCOLOR col) { m_Col = col; }
	void AddColor(D3DXCOLOR col) { m_Col += col; }
	D3DXCOLOR GetColor() { return m_Col; }
	D3DXVECTOR3 GetRot() { return m_Rot; }
	void SetFlashing(int nflashing) { m_nFlashingCnt = nflashing; }
	void SetFadeSpeed(float fspeed) { m_fFadeSpeed = fspeed; }
	void PolygonAppear(float fAppear);
	void SetAlphaLimitValue(float fAlphaMax, float fAlphaLeast) { m_fAlphaMax = fAlphaMax; m_fAlphaLeast = fAlphaLeast; }

	virtual void SetDiagonalLine(float fwidth, float fheight);//�\������|���S���̏c�����ݒ�
	void AddDiagonalLine(float faddwidth, float faddheight);//�\������|���S���̏c����Add
	float GetActualRadius() { return m_ActualRadius; }//���ۂ̔��a

	virtual void SetPolygon();//�X�V�����ȊO�ł̃|���S���̃Z�b�g
	void SetTextIndex(int nIndex) { m_nTextIndex = nIndex; }//���̃|���S�����g�p����e�N�X�`���ԍ��̃Z�b�g
	int GetTextIndex() { return m_nTextIndex; }//���̃|���S�����g�p����e�N�X�`���ԍ��̎擾
	virtual bool Collision(D3DXVECTOR3 pos,float radius);//�����蔻��

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_Pos;//�ʒu
	D3DXVECTOR3 m_Rot;//����
	D3DXVECTOR3 m_Move;//�ړ���
	D3DXVECTOR2 m_UVMove;//UV�̈ړ���
	D3DXVECTOR2 m_UV;//UV�̍��W
	D3DXVECTOR2 m_UVSize;//UV�̃T�C�Y

	D3DXCOLOR m_Col;//�F
	float m_fAlphaMax;
	float m_fAlphaLeast;
	float m_fWidth;//����
	float m_fHeight;//�c��
	//�Ίp���̒���
	float m_len;
	//�Ίp���̊p�x
	float m_angle;
	float m_ActualRadius;//���ۂ̔��a
	int m_nTextIndex;//�e�N�X�`���ԍ�
	int m_nFlashingCnt;//�_�ŃJ�E���g
	bool m_bFadeInOut;//�t�F�[�h���(true = in,false = out)
	float m_fFadeSpeed;//�_�ł̑���
	
};

#endif