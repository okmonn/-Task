#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Client.h"
#include <Windows.h>
#include <iostream>
#include <ws2tcpip.h>
#include <string>
#include <tchar.h>

#pragma comment(lib, "ws2_32.lib")

// コンストラクタ
Client::Client() : 
	sock(INVALID_SOCKET)
{
	data = {};
	addr = {};
	fds = {};
	readfds = {};

	list.clear();

	Init();
}

// デストラクタ
Client::~Client()
{
}

// 受信
void Client::Recv(void)
{
}

// 送信
void Client::Sent(void)
{
}

// 初期処理
int Client::Start(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\n初期化：失敗\n"));
	}

	return  hr;
}

// ソケットの生成
int Client::Create(void)
{
	//使用PC名前
	char name[256];

	//ホスト情報
	struct hostent *h = nullptr;

	auto hr = gethostname(name, _countof(name));
	if (hr != 0)
	{
		OutputDebugString(_T("\n使用しているPC名の取得：失敗\n"));
		return hr;
	}
	else
	{
		printf("このコンピュータの名前は%sです\n", name);
	}

	char hName[10];
	printf("サーバーPC名：");
	scanf_s("%s", hName, sizeof(hName));
	h = gethostbyname(hName);

	char port[5];
	printf("ポート番号：");
	scanf_s("%s", port, sizeof(port));
	//アドレス情報
	in_addr address = {};
	for (int i = 0; h->h_addr_list[i]; ++i)
	{
		memcpy(&address, h->h_addr_list[i], 4);
		list.push_back(address.S_un.S_addr);
	}
	
	//ソケットの設定
	addr.sin_family           = AF_INET;
	addr.sin_port             = htons(std::atoi(port));
	addr.sin_addr.S_un.S_addr = address.S_un.S_addr;

	//ソケットの生成
	sock = socket(addr.sin_family, SOCK_STREAM, 0);
	if (sock != INVALID_SOCKET)
	{
		OutputDebugString(_T("\nソケットの生成：失敗\n"));
	}

	return hr;
}

// サーバーとの接続
int Client::Connect(void)
{
	FD_ZERO(&readfds);
	auto hr = 0;
	for (UINT i = 0; i < list.size(); ++i)
	{
		addr.sin_addr.S_un.S_addr = list[i];

		hr = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
		if (hr != 0)
		{
			continue;
		}
		else
		{
			char str[INET_ADDRSTRLEN];
			printf("%sに接続しました\n", inet_ntop(addr.sin_family, &addr.sin_addr, str, sizeof(str)));
			FD_SET(sock, &readfds);
			break;
		}
	}

	if (hr != 0)
	{
		printf("サーバー接続：失敗 : %d\n", WSAGetLastError());
	}

	return hr;
}

// 初期化
void Client::Init(void)
{
	Start();
	Create();
	Connect();
}
