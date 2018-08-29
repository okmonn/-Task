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
	func::LoadImg(n, "img/sample1.png");
	UINT w = 0;
	LoadWave(w, "wave/sample2.wav");

	float a = 0.f;
	float b = 0.f;

	while (func::CheckMsg() && CheckKey(INPUT_ESCAPE) != true)
	{
		func::Set();
		if (CheckKey(INPUT_RIGHT))
		{
			++a;
		}
		else if (CheckKey(INPUT_LEFT))
		{
			--a;
		}
		else if (CheckKey(INPUT_DOWN))
		{
			++b;
		}
		else if (CheckKey(INPUT_UP))
		{
			--b;
		}

		Draw(n, 0 + a, 0 + b);
	
		if (TriggerKey(INPUT_RETURN))
		{
			PlayWave(w);
		}
		else if (TriggerKey(INPUT_SPACE))
		{
			StopWave(w);
		}
		func::Do();
	}

	return 0;
}