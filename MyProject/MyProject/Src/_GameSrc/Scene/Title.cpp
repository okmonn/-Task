#include "Title.h"
#include "../../Func/Func.h"

int n = 0;

// コンストラクタ
Title::Title()
{
	func::LoadImg("avicii.png", n);
}

// デストラクタ
Title::~Title()
{
}

// 描画
void Title::Draw(void)
{
	func::DrawImg(n, 0, 0);
}

// 処理
void Title::UpData(void)
{
}
