#include "_GameSrc/GameMane/GameMane.h"
#include "Network/Client.h"

// �G���g���[�|�C���g
int main()
{
	Client c;

	GameMane::Get().Run();

	return 0;
}