#include "Func/Func.h"

// �G���g���[�|�C���g
int main()
{
	func::SetWindowSize();
	func::Start();

	while (func::CheckMsg() && func::CheckKey(INPUT_ESCAPE) != true)
	{
		func::Set();

		func::Do();
	}

	func::End();

	return 0;
}