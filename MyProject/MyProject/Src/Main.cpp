#include "Func/Func.h"

// エントリーポイント
int main()
{
	func::SetWindowSize();
	func::Start();
	Wave w("Synchronicity.wav");

	while (func::CheckMsg() && func::CheckKey(INPUT_ESCAPE) != true)
	{
		func::Set();
		if (func::CheckKey(INPUT_RETURN))
		{
			w.Play();
		}
		else if (func::CheckTriger(INPUT_SPACE))
		{
			w.Stop();
		}
		else if (func::CheckTriger(INPUT_LCON))
		{
			w.Reset();
		}
		func::Do();
	}

	func::End();

	return 0;
}