#include "Interface.h"
#include "Typedef.h"
#include "DxLib.h"


// コンストラクタ
Interface::Interface(std::weak_ptr<Player>pl) : pl(pl)
{
	//上部バー
	top = LoadGraph(_T("img/bar_top.png"));

	//下部バー
	bottom = LoadGraph(_T("img/bar_bottom.png"));

	//体力
	hp = LoadGraph("img/heart.png");
}

// デストラクタ
Interface::~Interface()
{
	DeleteGraph(top);
	DeleteGraph(bottom);
	DeleteGraph(hp);
}

// 描画
void Interface::Draw(void)
{
	//上部バー
	DrawGraph(0, 0, top, true);

	//下部バー
	DrawGraph(0, (WINDOW_Y - BAR_SIZE_Y), bottom, true);

	//体力
	for (int i = 0; i < pl.lock()->GetHp(); ++i)
	{
		DrawGraph(220 + (i * 32), (WINDOW_Y - 45), hp, true);
	}
}

// 処理
void Interface::UpData(void)
{
}
