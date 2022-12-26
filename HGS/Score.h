//==================
//スコア処理
//==================
#ifndef _Score_H_
#define _Score_H_
#include "main.h"



//プロトタイプ宣言
void InitScore();
void UninitScore();
void UpdateScore();
void DrawScore();
void SetScore(int nScore);
void AddScore(int nValue);
int GetScore();

#endif // !_Score_H_
