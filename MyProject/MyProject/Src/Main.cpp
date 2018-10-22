#include "Func/Func.h"

// エントリーポイント
int main()
{
	func::SetWindowSize();
	func::Start();

	int a = 0;
	int b = 0;

	func::LoadImg("avicii.png", a);
	func::LoadImg("avicii.png", b);

	while (func::CheckMsg() && func::CheckKey(INPUT_ESCAPE) != true)
	{
		func::Set();

		func::DrawImg(a, 0, 0);
		func::DrawFreelyImg(b, 320, 0, 640, 0, 320, 100, 640, 100);

		func::Do();
	}

	func::End();

	return 0;
}