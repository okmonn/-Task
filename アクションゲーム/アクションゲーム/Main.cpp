#include "Main.h"

// �G���g���[�|�C���g
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Game& game = Game::Instance();
	game.Init();
	game.Run();
	game.Destroy();

	return 0;
}