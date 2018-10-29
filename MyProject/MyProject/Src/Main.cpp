#include "_GameSrc/GameMane/GameMane.h"
#include "Network/Server.h"
#include "Network/Client.h"
#include <thread>

// エントリーポイント
int main()
{
	Server s;
	Client c;

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

	std::thread c_recv([](Client& s) {
		while ((GetAsyncKeyState(VK_ESCAPE) & 0x80) == false)
		{
			s.Recv();
		}
	}, std::ref(c));
	std::thread c_send([](Client& s) {
		while ((GetAsyncKeyState(VK_ESCAPE) & 0x80) == false)
		{
			s.Send();
		}
	}, std::ref(c));

	GameMane::Get().Run();

	c_recv.join();
	c_send.join();
	accept.join();
	recv.join();
	send.join();

	return 0;
}