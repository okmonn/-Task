#include "Func.h"
#include <iostream>
using namespace func;

#define WINDOW_X 640
#define WINDOW_Y 480

// エントリーポイント
int main()
{
	func::SetWindowSize(WINDOW_X, WINDOW_Y);
	func::Init();

	unsigned int n = 0;
	func::LoadImg(n, "img/sample.png");

	static float a = 0;
	while (func::CheckMsg())
	{
		func::Set();
		if (CheckKey(DIK_RIGHT))
		{
			++a;
		}
		else if (TriggerKey(DIK_LEFT))
		{
			--a;
		}
		func::Draw(n, 0 + a, 0, 300, 300, false, false);
		func::Do();
	}

	return 0;
}