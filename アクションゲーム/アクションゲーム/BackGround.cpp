#include "BackGround.h"
#include "Typedef.h"
#include "DxLib.h"

#define BACK_SIZE_X 548
#define BAR_SIZE_Y  64

// コンストラクタ
BackGround::BackGround()
{
	//背景
	back = LoadGraph("img/splatterhouse.png");

	//上部バー
	top = LoadGraph("img/bar_top.png");

	//下部バー
	bottom = LoadGraph("img/bar_bottom.png");
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

	//上部バー
	DrawGraph(0, 0, top, false);

	//下部バー
	DrawGraph(0, WINDOW_Y - BAR_SIZE_Y, bottom, false);
}

// 処理
void BackGround::UpData(void)
{
}
