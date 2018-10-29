#include "_GameSrc/GameMane/GameMane.h"
#include "Network/Client.h"
#include <thread>

// エントリーポイント
int main()
{
	Client c;

	std::thread recv([](Client* c) {
		while (c->GetFlag() == false)
		{
			c->Recv();
		}
	}, &c);

	std::thread send([](Client* c) {
		while (true)
		{
			c->Send();
		}
	}, &c);

	GameMane::Get().Run();

	recv.detach();
	send.detach();
	return 0;
}