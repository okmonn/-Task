#include "_GameSrc/GameMane/GameMane.h"
#include "Network/Server.h"
#include <thread>

// エントリーポイント
int main()
{
	Server s;

	std::thread accept([](Server& s) {
		while ((GetAsyncKeyState(VK_ESCAPE) & 0x80) == false)
		{
			s.Accept();
		}
	}, std::ref(s));
	std::thread recv([](Server& s) {
		while ((GetAsyncKeyState(VK_ESCAPE) & 0x80) == false)
		{
			s.Recv();
		}
	}, std::ref(s));
	std::thread send([](Server& s) {
		while ((GetAsyncKeyState(VK_ESCAPE) & 0x80) == false)
		{
			s.Send();
		}
	}, std::ref(s));

	GameMane::Get().Run();

	accept.join();
	recv.join();
	send.join();

	return 0;
}