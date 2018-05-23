#include "Stage.h"

// コンストラクタ
Stage::Stage()
{
	//ステージサイズ
	rect = Rect(0, 0, 4000,0);
}

// デストラクタ
Stage::~Stage()
{
}

// 処理
void Stage::UpData(void)
{
}

// ステージサイズの取得
Rect & Stage::GetStageRange(void)
{
	return rect;
}
