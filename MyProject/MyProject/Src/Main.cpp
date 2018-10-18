#include "Union\Union.h"

// エントリーポイント
int main()
{
	Union::Get().SetWinSize(640, 480);
	Union::Get().Start();

	while (Union::Get().CheckMsg())
	{
		Union::Get().Set();
		Union::Get().Do();
	}

	Union::Get().End();

	return 0;
}