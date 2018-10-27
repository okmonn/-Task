#include "Title.h"
#include "../../Func/Func.h"

int n = 0;

// コンストラクタ
Title::Title()
{
	func::LoadPmd("Model/弱音ハク.pmd", n);
	func::Attach("charge.vmd", n);
}

// デストラクタ
Title::~Title()
{
}

// 描画
void Title::Draw(void)
{
	func::DrawPmd(n);
}

// 処理
void Title::UpData(void)
{
	func::Animation(n);
}
