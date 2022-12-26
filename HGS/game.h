//=============================================================================
//
// game.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _GAME_H_			//���̃}�N����`������Ȃ�������
#define _GAME_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLOSSKEY (4)

//*****************************************************************************
// �񋓌^�錾
//*****************************************************************************
typedef enum
{
	TARGETBUTTON_NONE = 0,
	TARGETBUTTON_UP,
	TARGETBUTTON_DOWN,
	TARGETBUTTON_RIGHT,
	TARGETBUTTON_LEFT,
	TARGETBUTTON_MAX
}TARGETBUTTON;

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct
{
	TARGETBUTTON NowTargetButton; //�����{�^��
	int nPushCount; //��������
	int nPushLimitTime; //���̃{�^���Ɉڍs����܂ł̎���
	int nTotalLimitTime; //�Q�[���̐�������
	int nColorCount; //�F�̕ω�����
}PUSHSTATE;

//*****************************************************************************
// �O���錾
//*****************************************************************************
class C2DPolygon;
class CUI;
class CNumber_Manager;

//*****************************************************************************
// �񋓌^
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,						//�������Ă��Ȃ����
	GAMESTATE_NORMAL,						//�ʏ���
	GAMESTATE_PUSH,							//�A�ŏ��
	GAMESTATE_DOWN,							//�|�����
	GAMESTATE_END,							//�I�����
	GAMESTATE_MAX
}GAMESTATE;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame : public CObject
{
private:
	static const int TEXT_MAX = 17;
public:
	CGame();
	~CGame() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	CNumber_Manager* GetNumber_Manager() { return m_pNumber_Manager; }

private:
	C2DPolygon* m_pSea;
	CUI* m_pBg;
	C2DPolygon* m_pButton;
	C2DPolygon* m_pstone_bridge;
	C2DPolygon* m_pFlowObject;
	int m_nText[TEXT_MAX];
	CNumber_Manager* m_pNumber_Manager;



};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

D3DXVECTOR3 *GetPosWorld(void);
GAMESTATE GetGameState(void);
void SetGameState(GAMESTATE state);
PUSHSTATE*GetPushState();

#endif