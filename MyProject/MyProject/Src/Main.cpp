#include "Func/Func.h"

// �G���g���[�|�C���g
int main()
{
	func::SetWindowSize();
	func::Start();

	int a = 0;

	func::LoadImg("avicii.png", a);

	while (func::CheckMsg())
	{
		func::Set();

		func::DrawImg(a, 0, 0);

		func::Do();
	}

	func::End();

	return 0;
}