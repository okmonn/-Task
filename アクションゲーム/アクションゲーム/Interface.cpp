#include "Interface.h"
#include "Typedef.h"
#include "DxLib.h"

#define BAR_SIZE_Y  64


// コンストラクタ
Interface::Interface()
{
	//上部バー
	top = LoadGraph(_T("img/bar_top.png"));

	//下部バー
	bottom = LoadGraph(_T("img/bar_bottom.png"));
}

// デストラクタ
Interface::~Interface()
{
	DeleteGraph(top);
	DeleteGraph(bottom);
}

// 描画
void Interface::Draw(void)
{
	//上部バー
	DrawGraph(0, 0, top, false);

	//下部バー
	DrawGraph(0, (WINDOW_Y - BAR_SIZE_Y), bottom, false);
}

// 処理
void Interface::UpData(void)
{
}
