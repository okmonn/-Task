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

	UINT n = 0;
	func::LoadImg(n, "img/avicii.png");
	UINT w = 0;
	LoadWave(w, "wave/sample3.wav");

	float a = 0.f;

	while (func::CheckMsg() && CheckKey(DIK_ESCAPE) != true)
	{
		func::Set();
		if (CheckKey(DIK_RIGHT))
		{
			++a;
		}
		else if (CheckKey(DIK_LEFT))
		{
			--a;
		}
		Draw(n, 0 + a, 0);
	
		if (TriggerKey(DIK_RETURN))
		{
			PlayWave(w);
		}
		else if (TriggerKey(DIK_SPACE))
		{
			StopWave(w);
		}
		func::Do();
	}

	return 0;
}