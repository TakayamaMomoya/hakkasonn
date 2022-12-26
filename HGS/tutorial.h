#ifndef _Tutorial_H_
#define _Tutorial_H_

typedef enum
{
	TutorialSTATE_NONE = 0,
	TutorialSTATE_NORMAL,
	TutorialSTATE_START,
	TutorialSTATE_END,
	TutorialSTATE_MAX,
}TutorialSTATE;

//プロトタイプ宣言
void InitTutorial();
void UninitTutorial();
void UpdateTutorial();
void DrawTutorial();
void SetTutorialState(TutorialSTATE state, int nCounter);
TutorialSTATE GetTutorialState();

#endif // !_Tutorial_H_

