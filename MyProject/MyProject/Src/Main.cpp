#include "_GameSrc/GameMane/GameMane.h"
#include "Network/Client.h"

// エントリーポイント
int main()
{
	Client c;

	GameMane::Get().Run();

	return 0;
}