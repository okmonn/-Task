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
	LoadWave(w, "wave/sample2.wav");

	while (func::CheckMsg())
	{
		func::Set();
		Draw(n, 0, 0);
	
		PlayWave(w);
		func::Do();
	}

	return 0;
}