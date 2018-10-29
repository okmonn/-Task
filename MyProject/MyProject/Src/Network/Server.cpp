#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
#include <Windows.h>
#include <tchar.h>

// コンストラクタ
Server::Server() : 
	sock(INVALID_SOCKET)
{
	data = {};
	addr = {};
	fds = {};
	readfds = {};
	time = { 0, 1000 };

	client.clear();
	clientAddr.clear();
}

// デストラクタ
Server::~Server()
{
}

// 初期処理
int Server::Start(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\n起動時の初期処理：失敗\n"));
	}

	return hr;
}

// ソケットの生成
int Server::Create(void)
{
	char port[5] = {};
	printf("接続ポート番号：");
	scanf_s("%5s", port, sizeof(port));

	//アドレスの設定
	addr.sin_family           = AF_INET;
	addr.sin_port             = htons(std::atoi(port));
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	return 0;
}
