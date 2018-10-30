#include "Title.h"
#include "../../Func/Func.h"

int n = 0;
float x = 0.f;
float y = 10.f;
float z = -15.f;

// コンストラクタ
Title::Title()
{
	func::LoadPmd("Model/弱音ハク.pmd", n);
	func::Attach("swing2.vmd", n);
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
	func::ChangeWVP(x, y, z, 0, 10, 0);
	if (func::CheckKey(INPUT_RIGHT))
	{
		++x;
	}
	else if (func::CheckKey(INPUT_LEFT))
	{
		--x;
	}
	else if (func::CheckKey(INPUT_DOWN))
	{
		--y;
	}
	else if (func::CheckKey(INPUT_UP))
	{
		++y;
	}
	else if (func::CheckKey(INPUT_W))
	{
		++z;
	}
	else if (func::CheckKey(INPUT_S))
	{
		--z;
	}
	func::Animation(n, true);
}
