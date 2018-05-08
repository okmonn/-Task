#include "Ground.h"
#include "Typedef.h"
#include "DxLib.h"

// コンストラクタ
Ground::Ground(std::weak_ptr<Player>pl) : pl(pl)
{
	//地面座標
	y = 340;
}

// デストラクタ
Ground::~Ground()
{
}

// 描画
void Ground::Draw(void)
{
	DrawLine(0, y, WINDOW_X, y, GetColor(255, 0, 0), false);
}

// 処理
void Ground::UpData(void)
{
	if (pl.lock()->GetMode() == "Jump")
	{
		if (pl.lock()->GetPos().y >= y)
		{
			pl.lock()->SetPos(330.0f);
			pl.lock()->SetMode("Ground", pl.lock()->GetReverse());
		}
	}
}

// 地面座標の取得
int Ground::GetHeight(void)
{
	return y;
}
