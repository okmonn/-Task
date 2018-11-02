#include "Sock.h"
#include <Windows.h>
#include <tchar.h>

#pragma comment(lib,"ws2_32.lib")

// コンストラクタ
Sock::Sock() : 
	sock(INVALID_SOCKET)
{
	data = {};
	addr = {};
	fds = {};
	time = { 10, 0 };
	readfds = {};
}

// デストラクタ
Sock::~Sock()
{
}

// スタートアップ
int Sock::StartUp(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\nWSAのスタートアップ：失敗\n"));
	}

	return hr;
}

// ソケットの生成
int Sock::CreateSock(void)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		OutputDebugString(_T("\nソケットの生成：失敗\n"));
		return -1;
	}

	return 0;
}

// 通信の終了
void Sock::Finish(void)
{
	if (sock != INVALID_SOCKET)
	{
		closesocket(sock);
	}

	WSACleanup();
}
