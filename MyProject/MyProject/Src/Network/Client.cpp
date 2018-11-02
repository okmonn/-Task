#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Client.h"
#include "Information.h"
#include <Windows.h>
#include <tchar.h>

// コンストラクタ
Client::Client()
{
	Init();
}

// デストラクタ
Client::~Client()
{
	Finish();
}

// 接続
int Client::Connect(void)
{
	auto hr = connect(sock, (sockaddr*)&addr, sizeof(addr));
	if (hr != 0)
	{
		OutputDebugString(_T("\nホストとの接続：失敗\n"));
		return hr;
	}
	
	FD_SET(sock, &readfds);

	return hr;
}

// 通信の準備
int Client::SetUp(void)
{
	addr.sin_family           = AF_INET;
	addr.sin_port             = htons(PORT);
	addr.sin_addr.S_un.S_addr = inet_addr(IP_NAME);

	auto hr = 0;

	FD_ZERO(&readfds);
	if (addr.sin_addr.S_un.S_addr == 0xffffffff)
	{
		hostent* host = nullptr;
		host = gethostbyname(IP_NAME);
		if (host == nullptr)
		{
			OutputDebugString(_T("\nホストがみつかりません\n"));
			return -1;
		}

		auto list = (unsigned int **)host->h_addr_list;
		while (*list != nullptr)
		{
			addr.sin_addr.S_un.S_addr = *(*list);
			hr = Connect();
			if (hr == 0)
			{
				break;
			}

			++list;
		}
	}
	else
	{
		hr = Connect();
	}

	return hr;
}

// 初期化
void Client::Init(void)
{
	StartUp();
	CreateSock();
	SetUp();
}

// 送信
int Client::Send(const char * data, int size)
{
	auto hr = sendto(sock, data, size, 0, (sockaddr*)&addr, sizeof(addr));
	if (hr != size)
	{
		OutputDebugString(_T("\n送信：失敗\n"));
		return -1;
	}

	return 0;
}

// 受信
int Client::Recv(std::vector<char> & data)
{
	data.clear();
	data.shrink_to_fit();

	memcpy(&fds, &readfds, sizeof(fd_set));

	char dummy[1024];
	memset(&dummy[0], 0, sizeof(dummy));

	if (select(0, &fds, nullptr, nullptr, &time) == 0
		|| select(0, &fds, nullptr, nullptr, &time) == -1)
	{
		OutputDebugString(_T("\n受信：失敗\n"));
		return -1;
	}

	if (FD_ISSET(sock, &fds))
	{
		while (true)
		{
			auto hr = recv(sock, dummy, sizeof(dummy), 0);
			if (hr == -1)
			{
				OutputDebugString(_T("\nサーバーが立ち上がっていません\n"));
				return -1;
			}
			else if (hr <= 0)
			{
				break;
			}

			for (int i = 0; i < hr; ++i)
			{
				data.push_back(dummy[i]);
			}
		}
	}

	return 0;
}
