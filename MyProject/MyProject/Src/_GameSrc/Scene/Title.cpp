#include "Title.h"
#include "../../Func/Func.h"
#include "../MapEditor/MapEditor.h"

int n = 0;
float x = 0.f;
float y = 10.f;
float z = -15.f;
int a = 0;
MapEditor map;

// コンストラクタ
Title::Title()
{
	func::LoadPmd("Model/弱音ハク.pmd", n);
	func::Attach("ヤゴコロダンス.vmd", n);
	func::LoadImg("Rsc/Skeleton.png", a);
}

// デストラクタ
Title::~Title()
{
}

// 描画
void Title::Draw(void)
{
	func::DrawPmd(n);

	static int index = 0;
	static int flam = 0;
	func::DrawRectImg(a, 0, 0, 100, 118, 0 + (59 * index), 94/2, 59, 94/2);

	++flam;
	index = flam / 10 % 8;

	map.MapInfo({ 1280, 480 }, { 10, 10 });
	map.Draw();
}

// 処理
void Title::UpData(void)
{
	map.UpData();
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
