#include "Func/Func.h"
#include <iostream>
using namespace func;

#define WINDOW_X 640
#define WINDOW_Y 480

#define RAD(X) (X) * (3.14159264f / 180.0f)

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
	LoadPMD(q, "Material/model/初音ミク.pmd");
	UINT v = 0;
	LoadVMD(v, "Material/motion/pose.vmd");
	UINT a = 0;
	LoadImg(a, "Material/img/sample.png");
	UINT b = 0;
	LoadImg(b, "Material/img/sample2.png");

	//RotateBorn(q, v);

	while (func::CheckMsg() && CheckKey(INPUT_ESCAPE) != true)
	{
		func::Set();
		
		DrawPMD(q);

		//static float alpha = 1.0f;
		//SetAlpha(1.0f);
		//Draw(a, 0, 0);
		//SetAlpha(alpha);
		//Draw(b, 0, 0);
		//
		//alpha -= 0.005f;

		func::Do();
	}

	return 0;
}