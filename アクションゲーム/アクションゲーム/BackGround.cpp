#include "BackGround.h"
#include <tchar.h>

#define BACK_SIZE_X 576

// コンストラクタ
BackGround::BackGround()
{
	//背景
	back = LoadGraph(_T("img/splatterhouse.png"));
}

// デストラクタ
BackGround::~BackGround()
{
}

// 描画
void BackGround::Draw(void)
{
	//背景
	DrawGraph(0, 0, back, false);
	DrawTurnGraph(BACK_SIZE_X, 0, back, false);
}

// 処理
void BackGround::UpData(void)
{
}
