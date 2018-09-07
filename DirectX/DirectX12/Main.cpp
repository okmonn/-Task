#include "Func.h"
#include <iostream>
using namespace func;

#define WINDOW_X 640
#define WINDOW_Y 480

// エントリーポイント
#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
#endif
{
	func::SetWindowSize(WINDOW_X, WINDOW_Y);
	func::Init();
	
	ChangeView(Vec3f(0, 10, -15), Vec3f(0, 10, 0), Vec3f(0, 1, 0));

	UINT q = 0;
	LoadPMD(q, "Model/初音ミク.pmd");

	UINT a = 0;
	LoadImg(a, "img/sample.png");
	UINT b = 0;
	LoadImg(b, "img/sample1.png");

	while (func::CheckMsg() && CheckKey(INPUT_ESCAPE) != true)
	{
		func::Set();
		
		//DrawPMD(q);

		Draw(a, 0, 0);

		func::Do();
	}

	return 0;
}